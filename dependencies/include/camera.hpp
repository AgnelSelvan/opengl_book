#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "glm/glm.hpp"

class Camera{
    public:
        Camera();

        glm::mat4 getViewMatrix() const;

        void mouseLook(int mouseX, int mouseY);
        void moveForward(float speed);
        void moveBackward(float speed);
        void moveRight(float speed);
        void moveLeft(float speed);

    private:
        glm::vec3 mEye;
        glm::vec3 mViewDirection;
        glm::vec3 mUpVector;
        
        glm::vec2 mOldMousePosition;
};

#endif