//
// Created by Stefan on 17.03.2020.
//

#include <meshview/Window.h>
#include <iostream>
#include <stb/stb_image_write.h>

meshview::Window::Window() {
    // Init default parameters
    initDefaultParameters();

    // Initialize window
    initWindow();

    // Init m_shader and camera object
    initCamera();
    initShader();
    initData();
}

meshview::Window::Window(int width, int height, bool visible) {
    // Init default parameters
    initDefaultParameters();
    m_default_width = width;
    m_default_height = height;
    delete m_data_image;
    m_data_image = new unsigned char[m_default_width*m_default_height*4];
    m_visible = visible;

    // Initialize the window
    initWindow();

    // Init shader and camera object
    initCamera();
    initShader();
    initData();
}

void meshview::Window::initShader() {
    m_shader = std::unique_ptr<meshview::ShaderProgram>(new meshview::ShaderProgram());
    // m_shader = std::make_unique<meshview::ShaderProgram>("shaders/lighting.vs", "shaders/lighting.fs");
}

void meshview::Window::initCamera() {
    m_camera = std::unique_ptr<meshview::Camera>(new meshview::Camera());
    m_camera->lookAt(glm::vec3(0.0, 0.0, 5.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
}

void meshview::Window::initData(){
    // Add it to the data object
    m_data = std::make_shared<meshview::TetMeshBuffer>();
}

meshview::Window::~Window() {
    delete m_data_image;
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void meshview::Window::run(int frames) {
    // Init last time before rendering starts
    initTimes();
    if (frames){
        for (int i = 0; i < frames; ++i) {
            loop();
        }
    } else {
        if (!m_visible){
            std::cout << "Cannot run without gui in endless loop. Please specify a number of frames to run for." << std::endl;
            return;
        }
        while (!glfwWindowShouldClose(m_window)){
            loop();
        }
    }
}

void meshview::Window::highlight(vec3 axis, float radius, std::string prefix_path){
    m_camera->initRotation(vec3FromArray(axis), radius);
    initTimes();
    while (!glfwWindowShouldClose(m_window)){
        loop();
        saveFrame(prefix_path);
    }

}

void meshview::Window::loop() {
    // Get time before rendering starts
    m_preframe_callback(m_window);

    glClearColor(0.1, 0.1, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shader->use();
    m_shader->setMat4fv("model", glm::value_ptr(m_data->getModel()));
    m_shader->setMat4fv("modelInv", glm::value_ptr(m_data->getModelInv()));
    m_shader->setMat4fv("view", glm::value_ptr(m_camera->getView()));
    m_shader->setMat4fv("projection", glm::value_ptr(m_camera->getProjection()));

    m_shader->setMat4fv("viewPos", glm::value_ptr(m_camera->getPos()));
    m_shader->setMat4fv("lightPos", glm::value_ptr(m_camera->getPos()));

    m_data->glDraw();

    m_camera->fpsCam(m_window);
    m_camera->alignCam(m_window, m_data->getBbox());
    m_camera->toggleRotation(m_window);

    convenienceCallback();
    glfwSwapBuffers(m_window);
    glfwPollEvents();

    m_postframe_callback(m_window);
}

void meshview::Window::initWindow() {
    // Initialize glfw
    if(!glfwInit()){
        std::cout << "Could not initialize glfw." << std::endl;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    if (!m_visible){
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    }

    // Create a window and make its context the current context
    m_window = glfwCreateWindow(m_default_width, m_default_height, "Mesh Viewer", NULL, NULL);
    if (!m_window){
        std::cout << "Could not create a window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(m_window);

    // Load glad
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Could not initialize GLAD." << std::endl;
        glfwTerminate();
    }

    glViewport(0, 0, m_default_width, m_default_height);
    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow * window, int width, int height){
        glViewport(0,0,width, height);
    });

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    if (m_visible){
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

void meshview::Window::initDefaultParameters() {
    m_visible = true;
    m_default_width = 800;
    m_default_height = 600;
    m_data_image = new unsigned char[m_default_width*m_default_height*4];

    m_preframe_callback = [](GLFWwindow*){};
    m_postframe_callback = [](GLFWwindow*){};

    m_model = glm::mat4(1);
    m_model_inv = glm::mat4(1);

    m_frame_counter = 0;
    m_last_time = 0;
    m_time_last_frame = 0;
    m_fps = 30;
}

void meshview::Window::setPreFrameCallback(void (*callback)(GLFWwindow *)) {
    m_preframe_callback = callback;
}

void meshview::Window::setPostFrameCallback(void (*callback)(GLFWwindow *)) {
    m_postframe_callback = callback;
}

void meshview::Window::convenienceCallback() {
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(m_window, GLFW_TRUE);
    }
}

void meshview::Window::setData(std::shared_ptr<meshview::TetMeshBuffer> data) {
    m_data = data;
}

void meshview::Window::saveFrame(std::string prefix_path) {
    if (!drawFrameNow()){
        return;
    }
    int width, height;
    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, m_default_width, m_default_height, GL_RGBA, GL_UNSIGNED_BYTE, m_data_image);
    std::stringstream out;
    out << prefix_path << "/" << "output-frame-" << m_frame_counter << ".png";
    stbi_flip_vertically_on_write(true);
    int success = stbi_write_png(out.str().c_str(), m_default_width, m_default_height, 4, (void*) m_data_image, m_default_width*4);
    if (!success){
        std::cout << "Error while opening file and writing png. Exiting..." << std::endl;
        glfwSetWindowShouldClose(m_window, GLFW_TRUE);
    }
    m_frame_counter++;
}

bool meshview::Window::drawFrameNow() {
    float frame_time = (float)glfwGetTime() - m_time_last_frame;
    if (frame_time < 1./m_fps) {
        return false;
    } else {
        m_time_last_frame = (float)glfwGetTime();
        return true;
    }
}

void meshview::Window::initTimes() {
    m_last_time = (float)glfwGetTime();
    m_time_last_frame = (float)glfwGetTime();
}

