#include "screen.h"
#include "struct.h"
#include "gameobject.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <random> // For random number generation

int main() {
    Screen screen;
    Camera camera;

    std::vector<GameObject> triangles;

    // --- Random Number Generation Setup ---
    std::random_device rd;  // Obtain a seed from the operating system
    std::mt19937 gen(rd()); // Standard Mersenne Twister engine
    std::uniform_real_distribution<> disPos(-200.0, 200.0); // Distribution for position
    std::uniform_real_distribution<> disRotSpeed(-1.0, 1.0);  // Distribution for rotation speed
    std::uniform_int_distribution<> disColor(0, 255);       // Distribution for color components

    using namespace std::chrono;
    high_resolution_clock::time_point lastFrameTime = high_resolution_clock::now();

    while (true) {
        high_resolution_clock::time_point currentFrameTime = high_resolution_clock::now();
        duration<float> deltaTime = duration_cast<duration<float>>(currentFrameTime - lastFrameTime);
        lastFrameTime = currentFrameTime;
        float dt = deltaTime.count();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                std::cout << "Number of triangles: " << triangles.size() << std::endl; // Output count
                return 0;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_SPACE) {
                    // --- Create a New Triangle ---
                    GameObject newTriangle;

                    // Random position
                    newTriangle.position.x = disPos(gen);
                    newTriangle.position.y = disPos(gen);
                    newTriangle.position.z = disPos(gen);

                    // Random rotation speeds
                    newTriangle.rotationSpeed.x = disRotSpeed(gen);
                    newTriangle.rotationSpeed.y = disRotSpeed(gen);
                    newTriangle.rotationSpeed.z = disRotSpeed(gen);

                    // Single triangle mesh
                    newTriangle.mesh.points = {
                        { 0.0f,  50.0f, 0.0f},
                        {50.0f, -50.0f, 0.0f},
                        {-50.0f, -50.0f, 0.0f}
                    };
                    newTriangle.mesh.triangles.push_back({0, 1, 2, {
                        static_cast<Uint8>(disColor(gen)), // Random red
                        static_cast<Uint8>(disColor(gen)), // Random green
                        static_cast<Uint8>(disColor(gen)), // Random blue
                        255                                // Alpha
                    }});

                    triangles.push_back(newTriangle);
                }
                //Basic camera controll
                const Uint8* keystate = SDL_GetKeyboardState(NULL);
                if (keystate[SDL_SCANCODE_W]) camera.moveForward(100.0f * dt); // Multiply by dt
                if (keystate[SDL_SCANCODE_S]) camera.moveForward(-100.0f * dt);
                if (keystate[SDL_SCANCODE_A]) camera.moveRight(-100.0f * dt);
                if (keystate[SDL_SCANCODE_D]) camera.moveRight(100.0f * dt);
                if (keystate[SDL_SCANCODE_Q]) camera.moveUp(-100.0f * dt);
                if (keystate[SDL_SCANCODE_E]) camera.moveUp(100.0f * dt);
                if (keystate[SDL_SCANCODE_LEFT]) camera.rotateYaw(1.0f * dt);  // Adjust rotation speed
                if (keystate[SDL_SCANCODE_RIGHT]) camera.rotateYaw(-1.0f * dt);
                if (keystate[SDL_SCANCODE_UP]) camera.rotatePitch(1.0f * dt);
                if (keystate[SDL_SCANCODE_DOWN]) camera.rotatePitch(-1.0f * dt);
            }
        }

        // --- Update Triangle Rotations ---
        for (auto& tri : triangles) {
            tri.rotation.x += tri.rotationSpeed.x * dt;
            tri.rotation.y += tri.rotationSpeed.y * dt;
            tri.rotation.z += tri.rotationSpeed.z * dt;
        }

        // --- Rendering ---
        screen.clear();
        for (const auto& tri : triangles) {
            screen.renderObject(tri, camera);
        }
        screen.show();
    }
    return 0;
}
