//
// Created by sjeske on 3/12/20.
//

#include <meshview/Camera.h>
#include <iostream>

meshview::Camera::Camera(){
    initDefaultParameters();

    computeView();
    computeAngles();
}

meshview::Camera::Camera(const glm::vec3 camPos, const glm::vec3 camFront, const glm::vec3 camUp) {
    initDefaultParameters();

    m_camPos = camPos;
    m_camFront = glm::normalize(camFront);
    m_camUp = glm::normalize(camUp);

    computeCoordinateSystem();
    computeAngles();
}

meshview::Camera::Camera(const glm::vec3 camPos, const float pitch, const float yaw) {
    initDefaultParameters();

    m_camPos = camPos;
    m_pitch = pitch; constrainAngles();
    m_yaw = yaw;

    computeCamFront();
}

void meshview::Camera::initDefaultParameters() {
    m_fps_startup_counter = 2;
    m_cam_sensitivity = 0.05f;
    m_move_sensitivity = 10.f;
    m_last_time = 0.0f;
    m_delta_time = 0.0f;

    m_unit_x = glm::vec3(1.0, 0.0, 0.0);
    m_unit_y = glm::vec3(0.0, 1.0, 0.0);
    m_unit_z = glm::vec3(0.0, 0.0, 1.0);

    m_projection = glm::perspective(45.f, 800.f/600.f, 0.01f, 200.f);

    m_camPos = glm::vec3(0.0, 0.0, 3.0);
    m_camFront = glm::vec3(0.0, 0.0, -1.0);
    m_camUp = glm::vec3(0.0, 1.0, 0.0);

    m_rotate_toggle = false;
    m_rotate = false;
    m_rotate_x = glm::vec3(1.0, 0.0, 0.0);
    m_rotate_y = glm::vec3(0.0, 1.0, 0.0);
    m_rotate_z = glm::vec3(0.0, 0.0, 1.0);

    m_rotate_yaw = 0.;
    m_rotate_pitch = 0.;
    m_rotate_radius = 1.;
}

void meshview::Camera::computeView() {
    // m_view = glm::lookAt(m_camPos, m_camPos + m_camFront, m_camUp);

    auto zaxis =  -m_camFront;
    glm::vec3 up = m_camUp;
    glm::vec3 xaxis = glm::normalize(glm::cross(up, zaxis));
    glm::vec3 yaxis = glm::normalize(glm::cross(zaxis, xaxis));

    glm::mat4 trans(1);
    trans[3][0] = -m_camPos.x;
    trans[3][1] = -m_camPos.y;
    trans[3][2] = -m_camPos.z;

    glm::mat4 rot(1.0);
    rot[0][0] = xaxis.x;
    rot[1][0] = xaxis.y;
    rot[2][0] = xaxis.z;
    rot[0][1] = yaxis.x;
    rot[1][1] = yaxis.y;
    rot[2][1] = yaxis.z;
    rot[0][2] = zaxis.x;
    rot[1][2] = zaxis.y;
    rot[2][2] = zaxis.z;

    m_view = rot*trans;

}

void meshview::Camera::computeAngles() {
    float y_component = glm::dot(m_camFront, m_unit_y);
    m_pitch = glm::degrees(asin(y_component));
    constrainAngles();
    float x_component = glm::dot(m_camFront, m_unit_x);
    m_yaw = glm::degrees(acos(x_component/cos(glm::radians(m_pitch))));
    // std::cout << "Pitch:" << m_pitch << ", Yaw: " << m_yaw << std::endl;
}

void meshview::Camera::computeCamFront() {
    glm::vec3 direction;

    direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    direction.y = sin(glm::radians(m_pitch));
    direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

    // Compute direction as components of the unit vectors
    direction = direction.x * m_unit_x + direction.y * m_unit_y + direction.z * m_unit_z;
    m_camFront = glm::normalize(direction);
}

void meshview::Camera::lookAt(const glm::vec3 eye, const glm::vec3 target, const glm::vec3 up) {
    m_camPos = eye;
    m_camFront = glm::normalize(target - eye);
    m_camUp = glm::normalize(up);

    computeCoordinateSystem();
    computeAngles();
    computeView();
}

void meshview::Camera::fpsCam(GLFWwindow *window) {
    // Body movement
    auto time = (float)glfwGetTime();
    double xCursorPos, yCursorPos;
    glfwGetCursorPos(window, &xCursorPos, &yCursorPos);
    if (m_fps_startup_counter){
        m_last_time = time;
        m_xcursor_last = (float)xCursorPos;
        m_ycursor_last = (float)yCursorPos;
        m_fps_startup_counter--;
    }
    m_delta_time = time - m_last_time;
    m_last_time = time;
    if (!m_rotate){
        float deltaMove = m_move_sensitivity*m_delta_time;
        glm::vec3 delta(0.0);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
            delta += m_camFront;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
            delta -= m_camFront;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
            delta += glm::normalize(glm::cross(m_camFront, m_camUp));
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
            delta -= glm::normalize(glm::cross(m_camFront, m_camUp));
        }
        // delta.y = 0.0; // Uncomment for true fps m_camera
        m_camPos += delta*deltaMove;

        // Mouse movement
        m_xcursor = (float)xCursorPos;
        m_ycursor = (float)yCursorPos;
        float yawDelta =  m_cam_sensitivity*(m_xcursor - m_xcursor_last);
        float pitchDelta = -m_cam_sensitivity*(m_ycursor - m_ycursor_last); // Negative because y axis is negative
        m_xcursor_last = (float)xCursorPos;
        m_ycursor_last = (float)yCursorPos;
        m_pitch += pitchDelta;
        m_yaw += yawDelta;
        constrainAngles();
        computeCamFront();
    }
}

void meshview::Camera::constrainAngles() {
    if (m_pitch > 89.){
        m_pitch = 89;
    }
    else if (m_pitch < -89.){
        m_pitch = -89;
    }
}

glm::mat4 meshview::Camera::getView() {
    computeView();
    return m_view;
}

glm::vec3 meshview::Camera::getPos() {
    return m_camPos;
}

glm::mat4 meshview::Camera::getProjection() {
    return m_projection;
}

void meshview::Camera::alignCam(GLFWwindow *window, bbox_t bbox) {
    std::pair<glm::vec3, glm::vec3> box = std::make_pair(vec3FromArray(bbox.first), vec3FromArray(bbox.second));
    alignCam(window, box);
}

void meshview::Camera::alignCam(GLFWwindow *window, std::pair<glm::vec3, glm::vec3> bbox) {
    // TODO: hardcoded field of view in degrees as specified in the perspective projection matrix also move the computations to precomputations
    float fov = 45.;
    float tan_half_fov = tanf(fov/2.f);

    // Compute midpoint and extents of the bounding box
    glm::vec3 mid, delta;
    mid = (bbox.first + bbox.second)/2.f;
    delta = (bbox.second - bbox.first);

    auto distances = delta/(2.f*tan_half_fov) + 2.f;

    // pressing 0 will align with the z axis and show the xy plane
    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_0) == GLFW_PRESS){
        m_camPos = mid + glm::vec3(0., 0., std::fmax(distances[0], distances[1]));
        lookAt(m_camPos, mid, glm::vec3(0.0, 1.0, 0.0));
    }
    // pressing 1 will align with the -z axis and show the xy plane
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS){
        m_camPos = mid - glm::vec3(0., 0., std::fmax(distances[0], distances[1]));
        lookAt(m_camPos, mid, glm::vec3(0.0, 1.0, 0.0));
    }
    // pressing 2 will align with the y axis and show the xz plane
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS){
        m_camPos = mid + glm::vec3(0., std::fmax(distances[0], distances[2]), 0.);
        lookAt(m_camPos, mid, glm::vec3(0.0, 0.0, 1.0));
    }
    // pressing 3 will align with the -y axis and show the xz plane
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS){
        m_camPos = mid - glm::vec3(0., std::fmax(distances[0], distances[2]), 0.);
        lookAt(m_camPos, mid, glm::vec3(0.0, 0.0, 1.0));
    }
    // pressing 4 will align with the x axis and show the yz plane
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS){
        m_camPos = mid + glm::vec3(std::fmax(distances[1], distances[2]), 0., 0.);
        lookAt(m_camPos, mid, glm::vec3(0.0, 1.0, 0.0));
    }
    // pressing 5 will align with the x axis and show the yz plane
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS){
        m_camPos = mid - glm::vec3(std::fmax(distances[1], distances[2]), 0., 0.);
        lookAt(m_camPos, mid, glm::vec3(0.0, 1.0, 0.0));
    }

}

void meshview::Camera::computeCoordinateSystem() {
    // To account for different UP directions, the current coordinate system is computed
    m_unit_y = glm::normalize(m_camUp);
    glm::vec3 z_temp = glm::normalize(m_camFront);
    m_unit_x = glm::normalize(glm::cross(m_unit_y, z_temp));
    m_unit_z = glm::normalize(glm::cross(m_unit_x, m_unit_y));

    // auto zaxis =  -m_camFront;
    // glm::vec3 up = m_camUp;
    // glm::vec3 xaxis = glm::normalize(glm::cross(up, zaxis));
    // glm::vec3 yaxis = glm::normalize(glm::cross(zaxis, xaxis));

}

void meshview::Camera::toggleRotation(GLFWwindow * window) {
    bool toggle_rotation_now = glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS;
    if (toggle_rotation_now && !m_rotate_toggle){
        m_rotate = !m_rotate;
        if (m_rotate){
            initRotation(glm::vec3(0.0, 0.0, 1.0), 4.0);
        }
    }
    m_rotate_toggle = glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS;
    
    if (m_rotate){
        rotate(window);
    }
}

void meshview::Camera::enableRotation() {
    m_rotate = true;
}

void meshview::Camera::disableRotation() {
    m_rotate = false;
}

void meshview::Camera::initRotation(glm::vec3 axis, float radius) {
    m_rotate_y = glm::normalize(axis);
    m_rotate_x = glm::normalize(glm::cross(m_rotate_y, glm::vec3(1.0, 1.0, 1.0)));
    m_rotate_z = glm::normalize(glm::cross(m_rotate_x, m_rotate_y));

    m_rotate_radius = radius;
    m_rotate_yaw = 0.0;
    m_rotate_pitch = 0.0;

    // Automatically enable rotation when rotation is initialized
    enableRotation();
}

void meshview::Camera::rotate(GLFWwindow * window) {
    glm::vec3 angles(1.);
    angles[0] = m_rotate_radius * cosf(glm::radians(m_rotate_yaw)) * cosf(glm::radians(m_rotate_pitch));
    angles[1] = m_rotate_radius * sinf(glm::radians(m_rotate_pitch));
    angles[2] = m_rotate_radius * sinf(glm::radians(m_rotate_yaw)) * cosf(glm::radians(m_rotate_pitch));

    glm::vec3 new_pos = angles[0] * m_rotate_x + angles[1] * m_rotate_y + angles[2] * m_rotate_z;

    // std::cout << "New Pos:" << meshview::printArray(meshview::arrayFromVec3<float>(new_pos)) << std::endl;
    lookAt(new_pos, glm::vec3(0.), m_rotate_y);

    m_rotate_yaw += m_delta_time * 80.f;
    m_rotate_pitch += m_delta_time * 5.f;
    if (m_rotate_pitch > 89.){
        m_rotate = false;
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}


