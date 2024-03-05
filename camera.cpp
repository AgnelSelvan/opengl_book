#define GLM_ENABLE_EXPERIMENTAL
#include "camera.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/string_cast.hpp"
#include <iostream>

Camera::Camera(){
    mEye = glm::vec3(0., 0., 0.);
    mViewDirection = glm::vec3(0., 0., -1.);
    mUpVector = glm::vec3(0.,1.,0.);
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(mEye, mEye + mViewDirection, mUpVector);
}

void Camera::mouseLook(int mouseX, int mouseY){
    glm::vec2 currentMouse = glm::vec2(mouseX, mouseY);
    glm::vec2 deltaMouse = mOldMousePosition-currentMouse;
    std::cout << "Mouse position X:" << deltaMouse.x << " Y: " << deltaMouse.y << " mViewDirection: " << glm::to_string(mViewDirection) << " mUpVector: "<< glm::to_string(mUpVector) << std::endl;
    mViewDirection = glm::rotate(mViewDirection, glm::radians((float)mouseX), mUpVector);
    mOldMousePosition = currentMouse;
}

void Camera::moveForward(float speed){
    mEye -= (mViewDirection * speed);
}

void Camera::moveBackward(float speed){
    mEye += (mViewDirection * speed);
}

void Camera::moveRight(float speed){
    glm::vec3 vector = glm::cross(mViewDirection, mUpVector);
    mEye += vector * speed;
}

void Camera::moveLeft(float speed){
    glm::vec3 vector = glm::cross(mViewDirection, mUpVector);
    mEye -= vector * speed;
}
