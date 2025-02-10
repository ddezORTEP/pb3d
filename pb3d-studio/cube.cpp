#include "screen.h"
#include "struct.h"
#include "gameobject.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
#include <filesystem>

bool loadModel(const std::string& filename, GameObject& obj) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file: " << filename << std::endl;
        return false;
    }

    obj.mesh.points.clear();
    obj.mesh.triangles.clear();

    std::string line;
    int vertexCount = 0;
    int triangleCount = 0;
    int currentVertex = 0; // Track the current vertex index

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string keyword;
        ss >> keyword;

        if (keyword == "VERTICES") {
            ss >> vertexCount;
            obj.mesh.points.resize(vertexCount); // Resize *once*
        } else if (keyword == "TRIANGLES") {
            ss >> triangleCount;
            obj.mesh.triangles.reserve(triangleCount); // Reserve is correct here
        } else if (currentVertex < vertexCount && keyword.rfind("#", 0) != 0 && keyword != "TRIANGLES") {
            // Read vertex data directly into the vector at the correct index
            float x, y, z;
            if (ss >> x >> y >> z) {
                obj.mesh.points[currentVertex] = {x, y, z};
                currentVertex++; // Increment only on success
            } else {
                std::cerr << "Error: Invalid vertex data in line: " << line << std::endl;
                file.close();
                return false;
            }
        } else if (!keyword.empty() && keyword.rfind("#", 0) != 0) {
            // Read triangle data
            int v1, v2, v3;
            Uint8 r, g, b, a;
            if (ss >> v1 >> v2 >> v3 >> r >> g >> b >> a) {
                if (v1 < 0 || v1 >= obj.mesh.points.size() ||
                    v2 < 0 || v2 >= obj.mesh.points.size() ||
                    v3 < 0 || v3 >= obj.mesh.points.size())
                {
                    std::cerr << "Error: Invalid vertex indices in line: " << line << std::endl;
                    file.close();
                    return false;
                }
                obj.mesh.triangles.push_back({v1, v2, v3, {r, g, b, a}});
            } else {
                std::cerr << "Error: Invalid triangle data in line: " << line << std::endl;
                file.close();
                return false;
            }
        }
    }

    file.close();

    if (obj.mesh.points.size() == 0 || obj.mesh.triangles.size() == 0) {
        std::cerr << "Warning: No vertex or triangle data read from " << filename << std::endl;
        return false;
    }
    //Check if all points have been written to
    if (currentVertex != vertexCount)
    {
        std::cerr << "Warning: Number of written vertices doesn't match expected count " << filename << std::endl;
        return false;
    }

    std::cout << "Loaded model: " << filename << " (Vertices: " << obj.mesh.points.size()
              << ", Triangles: " << obj.mesh.triangles.size() << ")" << std::endl;
    return true;
}

int main(int argc, char* argv[]) {
    Screen screen;
    Camera camera;
    camera.position = {0, 0, -200};

    std::string modelFilename = "model.model";
    if (argc > 1) {
        modelFilename = argv[1];
    }

//    if (!std::filesystem::exists(modelFilename)) {
//        std::cerr << "Error: Model file not found: " << modelFilename << std::endl;
//        return 1;
//    }

    GameObject model;
    if (!loadModel(modelFilename, model)) {
        return 1;
    }

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
                return 0;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_r && (event.key.keysym.mod & KMOD_CTRL)) {
                    if (loadModel(modelFilename, model)) {
                        std::cout << "Model reloaded." << std::endl;
                    }
                }
            }
        }

        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        if (keystate[SDL_SCANCODE_LEFT]) model.rotation.y += 1.0f * dt;
        if (keystate[SDL_SCANCODE_RIGHT]) model.rotation.y -= 1.0f * dt;
        if (keystate[SDL_SCANCODE_UP]) model.rotation.x += 1.0f * dt;
        if (keystate[SDL_SCANCODE_DOWN]) model.rotation.x -= 1.0f * dt;

        screen.clear();
        screen.renderObject(model, camera);
        screen.show();
    }

    return 0;
}
