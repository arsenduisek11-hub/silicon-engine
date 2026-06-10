#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../Input/Input.hpp"
#include "../Graphics/Graphics.hpp"

namespace Silicon {
    class Scene {
    public:
        virtual ~Scene() = default;
        virtual void onStart(Renderer& renderer) {}
        virtual void onUpdate(float dt) {}
        virtual void onDraw(Renderer& renderer) {}
    };

    class Engine {
    private:
        GLFWwindow* window;
        int width, height;
        float deltaTime = 0.0f;
        float lastFrame = 0.0f;

        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
            if (action == GLFW_PRESS) Input::setKey(key, true);
            else if (action == GLFW_RELEASE) Input::setKey(key, false);
        }

    public:
        Engine(int w, int h, const char* title) : width(w), height(h) {
            glfwInit();
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            window = glfwCreateWindow(width, height, title, NULL, NULL);
            glfwMakeContextCurrent(window);
            glfwSwapInterval(1); // VSync
            glfwSetKeyCallback(window, keyCallback);

            gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
            
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }

        ~Engine() { glfwTerminate(); }

        template<typename T>
        void run() {
            Shader shader("shaders/vertex.glsl", "shaders/fragment.glsl");
            glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);
            shader.use();
            shader.setMat4("projection", projection);

            Renderer renderer(&shader);
            T scene;
            scene.onStart(renderer);

            while (!glfwWindowShouldClose(window)) {
                float currentFrame = glfwGetTime();
                deltaTime = currentFrame - lastFrame;
                lastFrame = currentFrame;

                glfwPollEvents();

                scene.onUpdate(deltaTime);

                glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                scene.onDraw(renderer);

                glfwSwapBuffers(window);
            }
        }
    };
}
