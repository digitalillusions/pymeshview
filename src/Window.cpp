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

}

Window::Window(int width, int height) {
    // Init default parameters
    initDefaultParameters();
    m_default_width = width;
    m_default_height = height;

    // Initialize the window
    initWindow();

}

Window::~Window() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

bool Window::shouldClose() {
    return glfwWindowShouldClose(m_window);
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
    m_default_width = 800;
    m_default_height = 600;
}

void Window::framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0,0,width,height);
    m_default_width = width;
    m_default_height = height;
}
