#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraMovement{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
};

const float YAW   = -90.f;
const float PITCH = 0.f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera{
    public:
        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 worldUp;
        
        float yaw;
        float pitch;
        
        float movementSpeed;
        float zoom;
        float mouseSensitivity;

        Camera(glm::vec3 position = glm::vec3(0.f, 0.f, 0.f), glm::vec3 up = glm::vec3(0.f, 1.f, 0.f), float yaw = YAW,float pitch = PITCH) :  front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM){
            this->position = position;
            this->worldUp = up;
            this->yaw = yaw;
            this->pitch = pitch;

            updateCameraVectors();
        }

        Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM)
        {
            this->position = glm::vec3(posX, posY, posZ);
            this->worldUp = glm::vec3(upX, upY, upZ);
            this->yaw = yaw;
            this->pitch = pitch;
            updateCameraVectors();
        }

        glm::mat4 getViewMatrix(){
            return glm::lookAt(this->position, this->position + this->front, this->up);
        }

        void processKeyboard(CameraMovement direction, float deltaTime)
        {
            float velocity = movementSpeed * deltaTime;
            if (direction == FORWARD)
                this->position += front * velocity;
            if (direction == BACKWARD)
                this->position -= front * velocity;
            if (direction == LEFT)
                this->position -= right * velocity;
            if (direction == RIGHT)
                this->position += right * velocity;
        }

        void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
        {
            xoffset *= mouseSensitivity;
            yoffset *= mouseSensitivity;

            this->yaw   += xoffset;
            this->pitch += yoffset;

            // make sure that when pitch is out of bounds, screen doesn't get flipped
            if (constrainPitch)
            {
                if (this->pitch > 89.0f)
                    this->pitch = 89.0f;
                if (this->pitch < -89.0f)
                    this->pitch = -89.0f;
            }

            // update Front, Right and Up Vectors using the updated Euler angles
            updateCameraVectors();
        }

        // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
        void processMouseScroll(float yoffset)
        {
            this->zoom -= (float)yoffset;
            if (this->zoom < 1.0f)
                this->zoom = 1.0f;
            if (this->zoom > 45.0f)
                this->zoom = 45.0f;
        }


    private:
        void updateCameraVectors(){
            glm::vec3 cameraFront;
            cameraFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            cameraFront.y = sin(glm::radians(pitch));
            cameraFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            this->front = glm::normalize(cameraFront);
            this->right = glm::normalize(glm::cross(this->front, this->worldUp));
            this->up = glm::normalize(glm::cross(this->right, this->front));
        }
};


#endif // CAMERA_H
