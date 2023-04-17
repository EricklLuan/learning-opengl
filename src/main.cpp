#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <filesystem>
#include <iostream>
#include <cmath>
#include <map>

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
float height = 0.1f;

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

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        height += 0.005;
        if (height > 1.0f) height = 1.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        height -= 0.005;
        if (height < 0.0f) height = 0.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.handleKeyboard(delta, C_FORWARD);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.handleKeyboard(delta, C_BACKWARD);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.handleKeyboard(delta, C_LEFT);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.handleKeyboard(delta, C_RIGHT);
}

static void glfwError(int id, const char* description) {
    std::cout << description << std::endl;
}

unsigned int quadVAO = 0;
unsigned int quadVBO = 0;
void renderQuad() {
    if (quadVAO == 0) {
        glm::vec3 vert1(-1.0f,  1.0f, 0.0f);
        glm::vec3 vert2(-1.0f, -1.0f, 0.0f);
        glm::vec3 vert3( 1.0f, -1.0f, 0.0f);
        glm::vec3 vert4( 1.0f,  1.0f, 0.0f);

        glm::vec2 uv1(0.0f, 1.0f);
        glm::vec2 uv2(0.0f, 0.0f);
        glm::vec2 uv3(1.0f, 0.0f);
        glm::vec2 uv4(1.0f, 1.0f);

        glm::vec3 nm(0.0f, 0.0f, 1.0f);

        glm::vec3 edge1 = vert2 - vert1;
        glm::vec3 edge2 = vert3 - vert1;
        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        glm::vec3 tangent1(
            f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x),
            f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y),
            f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z)
        );

        glm::vec3 bitangent1(
            f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x),
            f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y),
            f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z)
        );

        edge1 = vert3 - vert1;
        edge2 = vert4 - vert1;
        deltaUV1 = uv3 - uv1;
        deltaUV2 = uv4 - uv1;

        f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

        glm::vec3 tangent2(
            f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x),
            f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y),
            f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z)
        );

        glm::vec3 bitangent2(
            f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x),
            f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y),
            f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z)
        );

        float quadVertices[] = {
            // Positions                 // Normals          // TexCoords   //Tangent                            //Bitangent
            vert1.x, vert1.y, vert1.z,   nm.x, nm.y, nm.z,   uv1.x, uv1.y,   tangent1.x, tangent1.y, tangent1.z,   bitangent1.x, bitangent1.y, bitangent1.z,
            vert2.x, vert2.y, vert2.z,   nm.x, nm.y, nm.z,   uv2.x, uv2.y,   tangent1.x, tangent1.y, tangent1.z,   bitangent1.x, bitangent1.y, bitangent1.z,
            vert3.x, vert3.y, vert3.z,   nm.x, nm.y, nm.z,   uv3.x, uv3.y,   tangent1.x, tangent1.y, tangent1.z,   bitangent1.x, bitangent1.y, bitangent1.z,
                                                                                                                 
            vert1.x, vert1.y, vert1.z,   nm.x, nm.y, nm.z,   uv1.x, uv1.y,   tangent2.x, tangent2.y, tangent2.z,   bitangent2.x, bitangent2.y, bitangent2.z,
            vert3.x, vert3.y, vert3.z,   nm.x, nm.y, nm.z,   uv3.x, uv3.y,   tangent2.x, tangent2.y, tangent2.z,   bitangent2.x, bitangent2.y, bitangent2.z,
            vert4.x, vert4.y, vert4.z,   nm.x, nm.y, nm.z,   uv4.x, uv4.y,   tangent2.x, tangent2.y, tangent2.z,   bitangent2.x, bitangent2.y, bitangent2.z,

        };

        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);

        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));

        glBindVertexArray(0);
    }

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
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

    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, &framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";
    std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";

    glEnable(GL_DEPTH_TEST);

    Texture brick_wall = Texture("assets/parallax-mapping/bricks.jpg", GL_RGB, GL_RGB, GL_REPEAT);
    Texture brick_wall_norm = Texture("assets/parallax-mapping/bricks_normal.jpg", GL_RGB, GL_RGB, GL_REPEAT);
    Texture brick_wall_disp = Texture("assets/parallax-mapping/bricks_disp.jpg", GL_RGB, GL_RGB, GL_REPEAT);

    Shader parallaxmapShader = Shader(
        "shaders/parallax_mapping/parallax.vert",
        "shaders/parallax_mapping/parallax.frag",
        std::vector<const char*>{"aPosition", "aNormal", "aTexCoords", "aTangent", "aBitangent"}
    );

    parallaxmapShader.use();
    brick_wall.use(0);
    parallaxmapShader.setInt("diffuse_texture", 0);
    brick_wall_norm.use(1);
    parallaxmapShader.setInt("normal_map", 1);
    brick_wall_disp.use(2);
    parallaxmapShader.setInt("displacement_map", 2);

    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        float current_frame = (float)glfwGetTime();
        delta = current_frame - last_frame;
        last_frame = current_frame;

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), 800.0f / 600.0f, 0.1f, 100.0f);

        parallaxmapShader.use();
        parallaxmapShader.setMat4("model", model);
        parallaxmapShader.setMat4("view", view);
        parallaxmapShader.setMat4("projection", projection);
        parallaxmapShader.setMat4("inverseModel", glm::inverse(model));
        parallaxmapShader.setVec3("viewPos", camera.position);
        parallaxmapShader.setVec3("lightPos", { 0.5f, 1.0f, 0.3f });
        parallaxmapShader.setFloat("height_scale", height);

        renderQuad();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}
