#ifndef SCREEN_H
#define SCREEN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h> // Include for SDL_Vertex and related functions
#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream> // For error output
#include "struct.h"  // Include your structure definitions
#include "cube.h"    // Include cube.h for the Camera struct

class Screen {
    SDL_Window* window;
    SDL_Renderer* renderer;
    //SDL_Texture* depthTexture = nullptr; // Depth texture for Z-buffering - Commented out for wireframe

public:
    Screen() {
        SDL_Init(SDL_INIT_VIDEO);
        window = SDL_CreateWindow("3D Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

        // Create the depth texture - Commented for wireframe
        /*depthTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 640, 480);
        if (!depthTexture) {
            std::cerr << "Failed to create depth texture: " << SDL_GetError() << std::endl;
            // Consider handling the error more gracefully (e.g., throwing an exception)
        }
        SDL_SetTextureBlendMode(depthTexture, SDL_BLENDMODE_NONE); // No blending for the depth texture*/
    }

    ~Screen() {
        /*if (depthTexture) {
            SDL_DestroyTexture(depthTexture);
        }*/
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void clear() {
        // Clear the color buffer (main screen)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Clear the depth texture to the farthest depth (1.0, represented as white) - commented out for wireframe
        /*SDL_SetRenderTarget(renderer, depthTexture);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White = far
        SDL_RenderClear(renderer);
        SDL_SetRenderTarget(renderer, nullptr); // Reset render target to the main window*/
    }

    void show() {
        SDL_RenderPresent(renderer);
    }

    void setDrawColor(const color& c) {
        SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
    }

    void setPixel(int x, int y, const color& c) {
        // Not directly used with SDL_RenderGeometry, but kept for completeness
        setDrawColor(c); // Set the color before drawing the point
        SDL_RenderDrawPoint(renderer, x, y);
    }
     vec3 rotateVector(const vec3& v, float angle) const {
        float cosAngle = std::cos(angle);
        float sinAngle = std::sin(angle);
        return {
            v.x * cosAngle - v.z * sinAngle,
            v.y,
            v.x * sinAngle + v.z * cosAngle
        };
    }

    vec3 transformToCameraSpace(const vec3& point, const Camera& camera) const {
        vec3 relative = {
            point.x - camera.position.x,
            point.y - camera.position.y,
            point.z - camera.position.z
        };

        return rotateVector(relative, -camera.yaw);
    }

  void backfaceCullAndDraw(const std::vector<vec3>& points, const std::vector<triangle>& triangles, const Camera& camera) {
    for (const auto& tri : triangles) {
        // Project *each* vertex of the triangle *inside* the loop
        vec3 p1 = camera.project(points[tri.p1], 640, 480);
        vec3 p2 = camera.project(points[tri.p2], 640, 480);
        vec3 p3 = camera.project(points[tri.p3], 640, 480);

        // Check if *all* vertices are valid (not clipped)
        if (p1.z != -1 && p2.z != -1 && p3.z != -1) {
            setDrawColor(tri.col); // Set the triangle's color
            SDL_RenderDrawLineF(renderer, p1.x, p1.y, p2.x, p2.y);
            SDL_RenderDrawLineF(renderer, p2.x, p2.y, p3.x, p3.y);
            SDL_RenderDrawLineF(renderer, p3.x, p3.y, p1.x, p1.y);
        }
    }
}
    void rotate(vec3& point, float angleX, float angleY, float angleZ) {
        // Rotate around X-axis
        float cosX = std::cos(angleX);
        float sinX = std::sin(angleX);
        float y = point.y * cosX - point.z * sinX;
        float z = point.y * sinX + point.z * cosX;
        point.y = y;
        point.z = z;

        // Rotate around Y-axis
        float cosY = std::cos(angleY);
        float sinY = std::sin(angleY);
        float x = point.x * cosY + point.z * sinY;
        z = -point.x * sinY + point.z * cosY;
        point.x = x;
        point.z = z;

        // Rotate around Z-axis
        float cosZ = std::cos(angleZ);
        float sinZ = std::sin(angleZ);
        x = point.x * cosZ - point.y * sinZ;
        y = point.x * sinZ + point.y * cosZ;
        point.x = x;
        point.y = y;
    }
    void input() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                exit(0);
            }
        }
    }
};

#endif
