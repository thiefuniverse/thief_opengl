#ifndef __CAMERA_H_
#define __CAMERA_H_

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <vector>

class Camera {
public:
    using Vec = glm::vec3;
    using Mat4 = glm::mat4;

    constexpr static float YAW = -90.0f;
    constexpr static float PITCH = 0.0f;
    constexpr static float SPEED = 2.5f;
    constexpr static float SENSITIVITY = 0.1f;
    constexpr static float ZOOM = 45.0f;
    enum Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT };

    Vec position_;
    Vec front_;
    Vec up_;
    Vec right_;
    Vec worldUp_;

    float yaw_;
    float pitch_;

    float movementSpeed_;
    float mouseSensitivity_;
    float zoom_;

    Camera(Vec position = Vec(0.0f, 0.0f, 0.0f), Vec up = Vec(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH)
        : front_(Vec(0.0f, 0.0f, -1.0f)), movementSpeed_(SPEED), mouseSensitivity_(SENSITIVITY), zoom_(ZOOM) {
        position_ = position;
        worldUp_ = up;
        yaw_ = yaw;
        pitch_ = pitch;
        updateCameraVectors();
    }
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw = YAW, float pitch = PITCH)
        : front_(Vec(0.0f, 0.0f, -1.0f)), movementSpeed_(SPEED), mouseSensitivity_(SENSITIVITY), zoom_(ZOOM) {
        position_ = Vec(posX, posY, posZ);
        worldUp_ = Vec(upX, upY, upZ);
        yaw_ = yaw;
        pitch_ = pitch;
        updateCameraVectors();
    }
    Mat4 GetViewMatrix() {
        return glm::lookAt(position_, position_ + front_, up_);
    }
    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera
    // defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
        float velocity = movementSpeed_ * deltaTime;
        if (direction == FORWARD) position_ += front_ * velocity;
        if (direction == BACKWARD) position_ -= front_ * velocity;
        if (direction == LEFT) position_ -= right_ * velocity;
        if (direction == RIGHT) position_ += right_ * velocity;
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) {
        xoffset *= mouseSensitivity_;
        yoffset *= mouseSensitivity_;

        yaw_ += xoffset;
        pitch_ += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch) {
            if (pitch_ > 89.0f) pitch_ = 89.0f;
            if (pitch_ < -89.0f) pitch_ = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset) {
        zoom_ -= ( float )yoffset;
        if (zoom_ < 1.0f) zoom_ = 1.0f;
        if (zoom_ > 45.0f) zoom_ = 45.0f;
    }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors() {
        // calculate the new Front vector
        Vec front;
        front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
        front.y = sin(glm::radians(pitch_));
        front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
        front_ = glm::normalize(front);
        // also re-calculate the Right and Up vector
        right_ = glm::normalize(
            glm::cross(front_, worldUp_));  // normalize the vectors, because their length gets closer to 0 the more you
                                            // look up or down which results in slower movement.
        up_ = glm::normalize(glm::cross(right_, front_));
    }
};
#endif  // __CAMERA_H_
