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
        // Convert 3D point to camera space (including pitch rotation)
        vec3 relative = {
            point.x - position.x,
            point.y - position.y,
            point.z - position.z
        };

        // Apply yaw rotation (around Y-axis)
        vec3 camSpaceYaw = rotateVector(relative, -yaw);

        // Apply pitch rotation (around X-axis) -  Rotate around the X-axis *after* yaw
        float cosPitch = std::cos(-pitch); // Use -pitch for correct direction
        float sinPitch = std::sin(-pitch);
        vec3 camSpace = {
            camSpaceYaw.x,
            camSpaceYaw.y * cosPitch - camSpaceYaw.z * sinPitch,
            camSpaceYaw.y * sinPitch + camSpaceYaw.z * cosPitch
        };

        // Clipping (Near and Far Planes)
        if (camSpace.z < nearPlane || camSpace.z > farPlane) {
            return {0, 0, -1}; // Indicate the point is clipped
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
        // Clamp pitch to prevent looking straight up/down (optional)
        pitch = std::max(-static_cast<float>(M_PI) / 2.0f + 0.01f, std::min(static_cast<float>(M_PI) / 2.0f - 0.01f, pitch));
    }

    void moveForward(float amount) {
        float cosYaw = std::cos(yaw);
        float sinYaw = std::sin(yaw);
        position.x += amount * sinYaw;
        position.z += amount * cosYaw;
    }

    void moveRight(float amount) {
        // Correct strafing:  Use perpendicular vector to forward
        float cosYaw = std::cos(yaw);
        float sinYaw = std::sin(yaw);
        position.x += amount * cosYaw;  // +cos(yaw) for right
        position.z -= amount * sinYaw;  // -sin(yaw) for right
    }

    void moveUp(float amount) {
        position.y += amount;
    }
};

#endif
