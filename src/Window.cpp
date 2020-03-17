//
// Created by Stefan on 17.03.2020.
//

#include <meshview/Window.h>
#include <iostream>

Window::Window() {
    // Init default parameters
    initDefaultParameters();

    // Initialize window
    initWindow();

    // Init m_shader and camera object
    initCamera();
    initShader();
    initData();
}

Window::Window(int width, int height, bool visible) {
    // Init default parameters
    initDefaultParameters();
    m_default_width = width;
    m_default_height = height;
    m_visible = visible;

    // Initialize the window
    initWindow();

    // Init shader and camera object
    initCamera();
    initShader();
    initData();
}

void Window::initShader() {
    m_shader = std::make_unique<ShaderProgram>("shaders/lighting.vs", "shaders/lighting.fs");
}

void Window::initCamera() {
    m_camera = std::make_unique<Camera>();
    m_camera->lookAt(glm::vec3(0.0, 0.0, 5.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
}

void Window::initData(){
    // Add it to the data object
    m_data = std::make_shared<TetMeshBuffer>();
}

Window::~Window() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Window::run(int frames = 0) {
    if (frames){
        for (int i = 0; i < frames; ++i) {
            loop();
        }
    } else {
        while (!glfwWindowShouldClose(m_window)){
            loop();
        }
    }
}

void Window::loop() {
    m_preframe_callback(m_window);

    glClearColor(0.1, 0.1, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shader->use();
    m_shader->setMat4fv("model", glm::value_ptr(m_model));
    m_shader->setMat4fv("modelInv", glm::value_ptr(m_model_inv));
    m_shader->setMat4fv("view", glm::value_ptr(m_camera->getView()));
    m_shader->setMat4fv("projection", glm::value_ptr(m_camera->getProjection()));

    m_shader->setMat4fv("viewPos", glm::value_ptr(m_camera->getPos()));
    m_shader->setMat4fv("lightPos", glm::value_ptr(m_camera->getPos()));

    m_data->glDraw();

    m_camera->fpsCam(m_window);
    m_camera->alignCam(m_window, m_data->getBbox());

    convenienceCallback();
    glfwSwapBuffers(m_window);
    glfwPollEvents();

    m_postframe_callback(m_window);
}

void Window::initWindow() {
    // Initialize glfw
    if(!glfwInit()){
        std::cout << "Could not initialize glfw." << std::endl;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
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

    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);
}

void Window::initDefaultParameters() {
    m_visible = true;
    m_default_width = 800;
    m_default_height = 600;

    m_preframe_callback = [](GLFWwindow*){};
    m_postframe_callback = [](GLFWwindow*){};

    m_model = glm::mat4(1);
    m_model_inv = glm::mat4(1);
}

void Window::setPreFrameCallback(void (*callback)(GLFWwindow *)) {
    m_preframe_callback = callback;
}

void Window::setPostFrameCallback(void (*callback)(GLFWwindow *)) {
    m_postframe_callback = callback;
}

void Window::convenienceCallback() {
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(m_window, GLFW_TRUE);
    }
}

void Window::setData(std::shared_ptr<TetMeshBuffer> data) {
    m_data = data;
}