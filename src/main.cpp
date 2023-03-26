#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <filesystem>
#include <iostream>
#include <cmath>

#include <stb_image.h>

#include "shader.hpp"
#include "texture.hpp"
#include "camera.hpp"
#include "model.hpp"

float delta = 0.0f;
float last_frame = 0.0f;

float lastX = 400;
float lastY = 300;
bool firstMouse = true;

Camera camera = Camera({ 0.0f, 0.0f, 3.0f }, { 0.0f, 1.0f, 0.0f });

static void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
    camera.handleScroll(static_cast<float>(yOffset));
}

static void mouse_callback(GLFWwindow* window, double xPosIn, double yPosIn) {

    float xPos = static_cast<float>(xPosIn);
    float yPos = static_cast<float>(yPosIn);

    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    
    float Xoffset = xPos - lastX;
    float Yoffset = lastY - yPos;
    lastX = xPos;
    lastY = yPos;

    camera.handleMouse(Xoffset, Yoffset);
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

static void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.handleKeyboard(delta, C_FORWARD);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.handleKeyboard(delta, C_BACKWARD);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.handleKeyboard(delta, C_LEFT);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.handleKeyboard(delta, C_RIGHT);
}

static void glfwError(int id, const char* description) {
    std::cout << description << std::endl;
}

int main(int, char**) {
    glfwSetErrorCallback(&glfwError);
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(800, 600, "Learnig Opengl", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, &framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";
    std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";

    stbi_set_flip_vertically_on_load(true);

    Shader backpackShader(
        "shaders/model_loadvs.glsl",
        "shaders/model_loadfs.glsl"
    );

    Model backpack = Model("models/backpack/backpack.obj");

    // fazer o shader para illuminação basico ambient e difuse
    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        float current_frame = (float)glfwGetTime();
        delta = current_frame - last_frame;
        last_frame = current_frame;

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        backpackShader.use();

        glm::mat4 model;
        glm::mat4 projection;
        glm::mat4 view;

        projection = glm::perspective(glm::radians(camera.zoom), 800.0f / 600.0f, 0.1f, 100.0f);
        view       = camera.getViewMatrix();
        model      = glm::mat4(1.0f);

        model = glm::translate(model, { 0.0f, 0.0f, 0.0f });
        model = glm::scale(model, { 1.0f, 1.0f, 1.0f });

        backpackShader.setMat4("model", model);
        backpackShader.setMat4("projection", projection);
        backpackShader.setMat4("view", view);
        backpackShader.setMat4("inverseModel", glm::inverse(model));

        backpackShader.setVec3("viewPos", camera.position);
        backpackShader.setVec3("light.ambient", { 0.2f, 0.2f, 0.2f });
        backpackShader.setVec3("light.diffuse", { 0.5f, 0.5f, 0.5f });
        backpackShader.setVec3("light.specular", { 1.0f, 1.0f, 1.0f });
        backpackShader.setVec3("light.position", { std::cos(glfwGetTime()) * 5.0f, 1.0f, std::sin(glfwGetTime()) * 5.0f });

        backpack.draw(backpackShader);

        projection = glm::perspective(glm::radians(camera.zoom), 800.0f / 600.0f, 0.1f, 100.0f);
        view = camera.getViewMatrix();
        model = glm::mat4(1.0f);

        model = glm::translate(model, { std::cos(glfwGetTime()) * 5.0f, 1.0f, std::sin(glfwGetTime()) * 5.0f });
        model = glm::scale(model, { 0.2f, 0.2f, 0.2f });

        backpackShader.setMat4("model", model);
        backpackShader.setMat4("projection", projection);
        backpackShader.setMat4("view", view);

        backpackShader.setVec3("light.ambient", { 0.2f, 0.2f, 0.2f });

        backpack.draw(backpackShader);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }
  
    glfwTerminate();
}
  