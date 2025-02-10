#include "screen.h"
#include "struct.h"
#include <iostream>

int main() {
    Screen screen;
    Camera camera;

    std::vector<vec3> points = {
        {-50, -50, -50}, { 50, -50, -50}, { 50,  50, -50}, {-50,  50, -50},
        {-50, -50,  50}, { 50, -50,  50}, { 50,  50,  50}, {-50,  50,  50}
    };

    std::vector<triangle> triangles = {
        {0, 1, 2, {255, 0, 0, 255}}, {0, 2, 3, {255, 0, 0, 255}},
        {4, 5, 6, {0, 255, 0, 255}}, {4, 6, 7, {0, 255, 0, 255}},
        {0, 4, 7, {0, 0, 255, 255}}, {0, 7, 3, {0, 0, 255, 255}},
        {1, 5, 6, {255, 255, 0, 255}}, {1, 6, 2, {255, 255, 0, 255}},
        {3, 2, 6, {255, 0, 255, 255}}, {3, 6, 7, {255, 0, 255, 255}},
        {0, 1, 5, {0, 255, 255, 255}}, {0, 5, 4, {0, 255, 255, 255}}
    };

    vec3 centroid = {0, 0, 0};
    for (const auto& p : points) {
        centroid.x += p.x;
        centroid.y += p.y;
        centroid.z += p.z;
    }
    centroid.x /= points.size();
    centroid.y /= points.size();
    centroid.z /= points.size();

    float angle = 0.0f;
    while (true) {
        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        if (keystate[SDL_SCANCODE_W]) camera.moveForward(10);
        if (keystate[SDL_SCANCODE_S]) camera.moveForward(-10);
        if (keystate[SDL_SCANCODE_A]) camera.moveRight(-10);
        if (keystate[SDL_SCANCODE_D]) camera.moveRight(10);
        if (keystate[SDL_SCANCODE_Q]) camera.moveUp(-10);
        if (keystate[SDL_SCANCODE_E]) camera.moveUp(10);
        if (keystate[SDL_SCANCODE_LEFT]) camera.rotateYaw(0.05f);
        if (keystate[SDL_SCANCODE_RIGHT]) camera.rotateYaw(-0.05f);

        angle += 0.01f;
        std::vector<vec3> rotatedPoints = points;  // Use rotatedPoints directly
        for (auto& p : rotatedPoints) {
            p.x -= centroid.x;
            p.y -= centroid.y;
            p.z -= centroid.z;
            screen.rotate(p, 0, angle, 0);
            p.x += centroid.x;
            p.y += centroid.y;
            p.z += centroid.z;
        }

        // Pass rotatedPoints directly to backfaceCullAndDraw
        screen.clear();
        screen.backfaceCullAndDraw(rotatedPoints, triangles, camera);
        screen.show();

        screen.input();
        SDL_Delay(30);
    }

    return 0;
}
