// cube.cpp
#include "screen.h"
#include "struct.h"
#include "gameobject.h" // Include the new gameobject.h
#include <iostream>
#include <vector>
#include <chrono>  // For timing


int main() {
    Screen screen;
    Camera camera;

    // --- Game Objects ---
    std::vector<GameObject> gameObjects;

    // Create a cube object.
    GameObject cube;
    cube.mesh.points = {  //Now using gameobject.h
        {-50, -50, -50}, { 50, -50, -50}, { 50,  50, -50}, {-50,  50, -50},
        {-50, -50,  50}, { 50, -50,  50}, { 50,  50,  50}, {-50,  50,  50}
    };
    cube.mesh.triangles = { //Now using gameobject.h
        {0, 1, 2, {255, 0, 0, 255}}, {0, 2, 3, {255, 0, 0, 255}},
        {4, 5, 6, {0, 255, 0, 255}}, {4, 6, 7, {0, 255, 0, 255}},
        {0, 4, 7, {0, 0, 255, 255}}, {0, 7, 3, {0, 0, 255, 255}},
        {1, 5, 6, {255, 255, 0, 255}}, {1, 6, 2, {255, 255, 0, 255}},
        {3, 2, 6, {255, 0, 255, 255}}, {3, 6, 7, {255, 0, 255, 255}},
        {0, 1, 5, {0, 255, 255, 255}}, {0, 5, 4, {0, 255, 255, 255}}
    };
    cube.position = {0, 0, 0}; // Initial position
    gameObjects.push_back(cube);

    // Create a second cube (to demonstrate multiple objects)
    GameObject cube2;
    cube2.mesh.points = cube.mesh.points; // Reuse the same mesh data
    cube2.mesh.triangles = cube.mesh.triangles;
    cube2.position = {150, 0, 50};       // Different position
    cube2.scale = {0.5f, 0.5f, 0.5f};     // Smaller scale
     for (auto& tri : cube2.mesh.triangles) { //Change color
        tri.col = {255, 255, 255, 255}; // White color
    }
    gameObjects.push_back(cube2);

      // Create a third cube (to demonstrate multiple objects)
    GameObject cube3;
    cube3.mesh.points = cube.mesh.points; // Reuse the same mesh data
    cube3.mesh.triangles = cube.mesh.triangles;
    cube3.position = {-150, 0, 50};       // Different position
    cube3.scale = {0.5f, 0.5f, 0.5f};     // Smaller scale
     for (auto& tri : cube3.mesh.triangles) { //Change color
        tri.col = {0, 0, 0, 255}; // Black color
    }
    gameObjects.push_back(cube3);


    // --- Timing ---
    using namespace std::chrono;
    high_resolution_clock::time_point lastFrameTime = high_resolution_clock::now();

    // --- Game Loop ---
    while (true) {
        // Calculate delta time
        high_resolution_clock::time_point currentFrameTime = high_resolution_clock::now();
        duration<float> deltaTime = duration_cast<duration<float>>(currentFrameTime - lastFrameTime);
        lastFrameTime = currentFrameTime;
        float dt = deltaTime.count();

        // --- Input Handling ---
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return 0; // Exit the game
            }
        }

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

        // --- Game Logic (Example: Rotating the first cube) ---
        gameObjects[0].rotation.y += 0.5f * dt;  // Rotate the first cube
          // --- Game Logic (Example: Rotating the second cube) ---
        gameObjects[1].rotation.x += 0.5f * dt;  // Rotate the second cube
          // --- Game Logic (Example: Rotating the third cube) ---
        gameObjects[2].rotation.z += 0.5f * dt;  // Rotate the third cube
        // --- Rendering ---
        screen.clear();
        for (const auto& obj : gameObjects) {
           screen.renderObject(obj, camera); // Use new renderObject Function.
        }

        screen.show();

      //  SDL_Delay(30); // No longer needed with delta time, we are using waits
    }

    return 0;
}
