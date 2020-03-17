//
// Created by Stefan on 17.03.2020.
//

#ifndef PYMESHVIEWER_WINDOW_H
#define PYMESHVIEWER_WINDOW_H

#include "Common.h"

class Window {
public:
    Window();
    Window(int width, int height);
    ~Window();

    bool shouldClose();

private:
    int m_default_width, m_default_height;
    GLFWwindow * m_window;

    void initDefaultParameters();
    void initWindow();
    void framebufferSizeCallback(GLFWwindow*, int, int);
};


#endif //PYMESHVIEWER_WINDOW_H
