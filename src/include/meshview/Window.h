//
// Created by Stefan on 17.03.2020.
//

#ifndef PYMESHVIEWER_WINDOW_H
#define PYMESHVIEWER_WINDOW_H

#include <meshview/Common.h>
#include <meshview/ShaderProgram.h>
#include <meshview/Camera.h>
#include <meshview/TetMeshBuffer.h>

#include <memory>

namespace meshview {
    class Window {
    public:
        Window();
        Window(int width, int height, bool visible=true);
        ~Window();

        void setPreFrameCallback(void(*callback)(GLFWwindow*));
        void setPostFrameCallback(void(*callback)(GLFWwindow*));

        void setData(std::shared_ptr<TetMeshBuffer> data);

        void run(int frames = 0);

        void highlight(vec3 axis, float radius, int fps=24);

    private:
        bool m_visible;
        int m_default_width, m_default_height;
        GLFWwindow * m_window;
        std::unique_ptr<ShaderProgram> m_shader;
        std::unique_ptr<Camera> m_camera;
        std::shared_ptr<TetMeshBuffer> m_data;

        glm::mat4 m_model, m_model_inv;

        int m_frame_counter;
        float m_last_time;
        float m_time_since_last_drawn_frame;
        int m_fps;

        void (*m_preframe_callback)(GLFWwindow*);
        void (*m_postframe_callback)(GLFWwindow*);

        void initDefaultParameters();
        void initShader();
        void initCamera();
        void initData();
        void initWindow();

        void saveFrame();

        void loop();

        void convenienceCallback();
    };
}


#endif //PYMESHVIEWER_WINDOW_H
