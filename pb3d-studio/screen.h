#ifndef SCREEN_H
#define SCREEN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
// Removed: #include <SDL2/SDL_ttf.h>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>
// Removed: #include <string>
#include "struct.h"
#include "cube.h"
#include "gameobject.h"

class Screen {
    SDL_Window* window;
    SDL_Renderer* renderer;
    // Removed: TTF_Font* font = nullptr;

public:
    Screen() {
        SDL_Init(SDL_INIT_VIDEO);
        window = SDL_CreateWindow("3D Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    }

    ~Screen() {
        // Removed: TTF_CloseFont(font); and TTF_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void clear() {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    }

    void show() {
        SDL_RenderPresent(renderer);
    }

    void setDrawColor(const color& c) {
        SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
    }

    void setPixel(int x, int y, const color& c) {
        setDrawColor(c);
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

    void renderObject(const GameObject& obj, const Camera& camera) {
       std::vector<vec3> transformedPoints = obj.mesh.points;
        for (auto& p : transformedPoints) {
            p.x *= obj.scale.x;
            p.y *= obj.scale.y;
            p.z *= obj.scale.z;
            rotate(p, obj.rotation.x, obj.rotation.y, obj.rotation.z);
            p.x += obj.position.x;
            p.y += obj.position.y;
            p.z += obj.position.z;
        }

        for (const auto& tri : obj.mesh.triangles) {
            vec3 p1 = camera.project(transformedPoints[tri.p1], 640, 480);
            vec3 p2 = camera.project(transformedPoints[tri.p2], 640, 480);
            vec3 p3 = camera.project(transformedPoints[tri.p3], 640, 480);

            if (p1.z != -1 && p2.z != -1 && p3.z != -1) {
                setDrawColor(tri.col);
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
