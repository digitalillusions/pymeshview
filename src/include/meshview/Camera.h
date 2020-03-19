//
// Created by sjeske on 3/12/20.
//

#ifndef LEARNOGL_CAMERA_H
#define LEARNOGL_CAMERA_H

#include <meshview/Common.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <utility>

namespace meshview {
    class Camera {
    public:
        Camera();
        Camera(glm::vec3 camPos, glm::vec3 camFront, glm::vec3 camUp);
        Camera(glm::vec3 camPos, float pitch, float yaw);

        void computeView();
        void computeAngles();
        void computeCamFront();

        glm::vec3 getPos();

        glm::mat4 getView();
        glm::mat4 getProjection();

        void fpsCam(GLFWwindow* window);
        void alignCam(GLFWwindow* window, std::pair<glm::vec3, glm::vec3> bbox);
        void alignCam(GLFWwindow* window, bbox_t bbox);

        void lookAt(glm::vec3 eye, glm::vec3 target, glm::vec3 up);
    private:
        void initDefaultParameters();
        void constrainAngles();
        void computeCoordinateSystem();

        glm::vec3 m_camPos, m_camFront, m_camUp;
        glm::vec3 m_unit_x, m_unit_y, m_unit_z;
        glm::mat4 m_view;
        glm::mat4 m_projection;

        float m_pitch, m_yaw;

        float m_xcursor_last, m_xcursor, m_ycursor_last, m_ycursor;
        float m_last_time;
        float m_cam_sensitivity;
        float m_move_sensitivity;
        float m_move_sprint_factor;
        int m_fps_startup_counter;
    };
}


#endif //LEARNOGL_CAMERA_H
