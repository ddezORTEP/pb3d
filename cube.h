#ifndef CUBE_H
#define CUBE_H

#include <cmath>
#include "struct.h"

struct Camera {
    vec3 position{0, 0, -200};
    vec3 direction{0, 0, 1};
    float fov = 90.0f;
    float nearPlane = 10.0f;
    float farPlane = 1000.0f;
    float yaw = 0.0f;
    float pitch = 0.0f;

    vec3 rotateVector(const vec3& v, float angle) const {
        float cosAngle = std::cos(angle);
        float sinAngle = std::sin(angle);
        return {
            v.x * cosAngle - v.z * sinAngle,
            v.y,
            v.x * sinAngle + v.z * cosAngle
        };
    }

    vec3 project(const vec3& point, int screenWidth, int screenHeight) const {
        vec3 relative = {
            point.x - position.x,
            point.y - position.y,
            point.z - position.z
        };

        vec3 camSpaceYaw = rotateVector(relative, -yaw);
        float cosPitch = std::cos(-pitch);
        float sinPitch = std::sin(-pitch);
        vec3 camSpace = {
            camSpaceYaw.x,
            camSpaceYaw.y * cosPitch - camSpaceYaw.z * sinPitch,
            camSpaceYaw.y * sinPitch + camSpaceYaw.z * cosPitch
        };

        if (camSpace.z < nearPlane || camSpace.z > farPlane) {
            return {0, 0, -1};
        }

        float aspectRatio = static_cast<float>(screenWidth) / screenHeight;
        float scale = 1.0f / std::tan(fov * 0.5f * M_PI / 180.0f);

        float projX = (scale * camSpace.x) / (aspectRatio * camSpace.z);
        float projY = (scale * camSpace.y) / camSpace.z;

        return {
            (projX + 1) * 0.5f * screenWidth,
            (1 - projY) * 0.5f * screenHeight,
            camSpace.z
        };
    }

    void rotateYaw(float angle) {
        yaw += angle;
    }

    void rotatePitch(float angle) {
        pitch += angle;
        // Clamp pitch, ensuring consistent types (all floats)
        pitch = std::max(-static_cast<float>(M_PI) / 2.0f + 0.01f, std::min(static_cast<float>(M_PI) / 2.0f - 0.01f, pitch));
    }

    void moveForward(float amount) {
        float cosYaw = std::cos(yaw);
        float sinYaw = std::sin(yaw);
        position.x += amount * sinYaw;
        position.z += amount * cosYaw;
    }

    void moveRight(float amount) {
        float cosYaw = std::cos(yaw);
        float sinYaw = std::sin(yaw);
        position.x += amount * cosYaw;
        position.z -= amount * sinYaw;
    }

    void moveUp(float amount) {
        position.y += amount;
    }
};

#endif
