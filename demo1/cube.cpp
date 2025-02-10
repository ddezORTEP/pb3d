#include "screen.h"
#include "struct.h"
#include "gameobject.h"
#include <iostream>
#include <vector>
#include <chrono>

int main() {
    Screen screen;
    Camera camera;
    camera.position = {0, 150, -200};
    camera.rotatePitch(0.5f);

    const int landscapeWidth = 32;
    const int landscapeDepth = 64;
    const float cellSize = 20.0f;
    const float scrollSpeed = 50.0f;
    const float landscapeLength = landscapeDepth * cellSize;

    // --- Two Landscape Objects ---
    GameObject landscape1;
    GameObject landscape2;

    // Function to initialize a landscape mesh
    auto initLandscape = [&](GameObject& landscape) {
        landscape.mesh.points.resize(landscapeWidth * landscapeDepth);
        for (int z = 0; z < landscapeDepth - 1; ++z) {
            for (int x = 0; x < landscapeWidth - 1; ++x) {
                int topLeft = z * landscapeWidth + x;
                int topRight = topLeft + 1;
                int bottomLeft = (z + 1) * landscapeWidth + x;
                int bottomRight = bottomLeft + 1;

                triangle tri1 = {topLeft, topRight, bottomLeft, {0, 128, 255, 255}};
                triangle tri2 = {topRight, bottomRight, bottomLeft, {0, 128, 255, 255}};
                landscape.mesh.triangles.push_back(tri1);
                landscape.mesh.triangles.push_back(tri2);
            }
        }
    };

    // Initialize both landscapes
    initLandscape(landscape1);
    initLandscape(landscape2);

    // Initial positions (landscape2 is placed after landscape1)
    landscape1.position.z = 0;          // Correct Z positioning
    landscape2.position.z = landscapeLength;

    // --- Timing ---
    using namespace std::chrono;
    high_resolution_clock::time_point lastFrameTime = high_resolution_clock::now();

    // --- Game Loop ---
    while (true) {
        high_resolution_clock::time_point currentFrameTime = high_resolution_clock::now();
        duration<float> deltaTime = duration_cast<duration<float>>(currentFrameTime - lastFrameTime);
        lastFrameTime = currentFrameTime;
        float dt = deltaTime.count();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return 0;
            }
        }

        // --- Update Landscape Positions (Scrolling) ---
        landscape1.position.z -= scrollSpeed * dt;
        landscape2.position.z -= scrollSpeed * dt;

        // --- Check and Reset Landscape Positions ---
        if (landscape1.position.z < -landscapeLength) {
            landscape1.position.z = landscape2.position.z + landscapeLength;
        }
        if (landscape2.position.z < -landscapeLength) {
            landscape2.position.z = landscape1.position.z + landscapeLength;
        }

      // --- Update Landscape Heights (Apply heightmap) ---
        auto updateLandscapeHeights = [&](GameObject& landscape) {
            for (int z = 0; z < landscapeDepth; ++z) {
                for (int x = 0; x < landscapeWidth; ++x) {
                    int index = z * landscapeWidth + x;
                    float worldX = (x - landscapeWidth / 2.0f) * cellSize;
                    float worldZ = (z * cellSize) + landscape.position.z; // Use object's position

                    float height = 20.0f * (std::sin(worldX * 0.1f) + std::cos(worldZ * 0.1f));
                    landscape.mesh.points[index] = {worldX, height, worldZ - landscape.position.z}; //Relative to landscape
                }
            }
        };

        updateLandscapeHeights(landscape1);
        updateLandscapeHeights(landscape2);

        // --- Rendering ---
        screen.clear();
        screen.renderObject(landscape1, camera);
        screen.renderObject(landscape2, camera);
        screen.show();
    }
    return 0;
}
