#ifndef CUBE_H
#define CUBE_H

#include <cmath>
#include "struct.h"

struct Camera {
    vec3 position{0, 0, -200};
    vec3 direction{0, 0, 1};
    float fov = 90.0f;
    float nearPlane = 10.0f; // Add near plane distance.  IMPORTANT!
    float farPlane = 1000.0f;
    float yaw = 0.0f;

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
        vec3 camSpace = rotateVector(relative, -yaw);

        // --- Clipping (Near and Far Planes) ---
        if (camSpace.z < nearPlane || camSpace.z > farPlane) {
            return {0, 0, -1}; // Indicate the point is clipped
        }

        float aspectRatio = static_cast<float>(screenWidth) / screenHeight;
        float scale = 1.0f / std::tan(fov * 0.5f * M_PI / 180.0f);

        // No need for division by zero check now, as clipping handles it
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
    // No changes to the project function for now, as clipping is handled there
    void moveForward(float amount) {
          float cosYaw = std::cos(yaw);
          float sinYaw = std::sin(yaw);
          position.x += amount * sinYaw; // Move along the camera's forward direction (sin for x)
          position.z += amount * cosYaw; // Move along the camera's forward direction (cos for z)
      }

      void moveRight(float amount) {
          float cosYaw = std::cos(yaw);
          float sinYaw = std::sin(yaw);
          position.x += amount * cosYaw; // Move along the camera's right direction (cos for x)
          position.z -= amount * sinYaw; // Move along the camera's right direction (-sin for z)
      }

      void moveUp(float amount) {
          position.y += amount;
      }
};

#endif
