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
bool hdr = true;

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
        std::cout << "Exposure: " << height << "\n";
        height += 0.05;
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        std::cout << "Exposure: " << height << "\n";
        height -= 0.05;
        if (height < 0.0f) height = 0.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
        hdr = !hdr;
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

unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube() {
    if (cubeVAO == 0) {
        float vertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
             // bottom face
             -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
              1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
              1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
              1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
             -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
             -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
             // top face
             -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
              1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
              1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
              1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
             -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
             -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
        };


        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);

        glBindVertexArray(cubeVAO);

        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

        glBindVertexArray(0);
    }

    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

unsigned int frameVAO = 0;
unsigned int frameVBO = 0;
void renderFrame() {
    if (frameVAO == 0) {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };

        glGenVertexArrays(1, &frameVAO);
        glGenBuffers(1, &frameVBO);

        glBindVertexArray(frameVAO);

        glBindBuffer(GL_ARRAY_BUFFER, frameVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
        
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glBindVertexArray(0);
    }

    glBindVertexArray(frameVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
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

    unsigned int pingpongFBO[2];
    unsigned int pingpongBuffers[2];
    glGenFramebuffers(2, pingpongFBO);
    glGenTextures(2, pingpongBuffers);

    for (unsigned int i = 0; i < 2; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
        glBindTexture(GL_TEXTURE_2D, pingpongBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 800, 600, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongBuffers[i], 0);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    unsigned int color_buffers[2];
    glGenTextures(2, color_buffers);
    for (unsigned int i = 0; i < 2; i++) {
        glBindTexture(GL_TEXTURE_2D, color_buffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 800, 600, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, color_buffers[i], 0);
    }

    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);

    unsigned int depth_rbo;
    glGenRenderbuffers(1, &depth_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, depth_rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 800, 600);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "The framebuffer is incomplete\n";
        return 0;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    std::vector<glm::vec3> lightPositions;
    lightPositions.push_back(glm::vec3(10.0f, 2.0f, 15.5f)); // back light
    lightPositions.push_back(glm::vec3(-12.0f, -5.0f, 15.5f)); // back light
    // colors
    std::vector<glm::vec3> lightColors;
    lightColors.push_back(glm::vec3(50.0f, 50.0f, 50.0f));
    lightColors.push_back(glm::vec3(100.0f, 50.0f, 50.0f));

    Shader HDR(
        "shaders/bloom/hdr.vert",
        "shaders/bloom/hdr.frag",
        std::vector<const char*>{"aPosition", "aTexCoords"}
    );

    Shader shader(
        "shaders/bloom/normal.vert",
        "shaders/bloom/normal.frag",
        std::vector<const char*>{"aPosition", "aNormal", "aTexCoords"}
    );

    Shader bloom(
        "shaders/bloom/bloom.vert",
        "shaders/bloom/bloom.frag",
        std::vector<const char*>{"aPosition", "aTexCoords"}
    );

    Shader light(
        "shaders/bloom/light.vert",
        "shaders/bloom/light.frag",
        std::vector<const char*>{"aPosition"}
    );


    Texture wood("assets/plank.png", GL_RGB, GL_RGB, GL_REPEAT);

    shader.use();
    shader.setInt("diffuse_texture", 0);

    HDR.use();
    HDR.setInt("scene", 1);

    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        float current_frame = (float)glfwGetTime();
        delta = current_frame - last_frame;
        last_frame = current_frame;

        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), 800.0f / 600.0f, 0.1f, 100.0f);
            glm::mat4 view = camera.getViewMatrix();
            
            for (unsigned int j = 0; j < 4; j++) { // cubes
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(2.0f * j, 0.0f, 5.0f * j));
                shader.use();
                wood.use(0);
                
                for (unsigned int i = 0; i < lightPositions.size(); i++) {
                    shader.setVec3("lights[" + std::to_string(i) + "].position", lightPositions[i]);
                    shader.setVec3("lights[" + std::to_string(i) + "].color", lightColors[i]);
                }

                shader.setMat4("projection", projection);
                shader.setMat4("view", view);
                shader.setMat4("model", model);
                shader.setMat4("inverseModel", glm::inverse(model));

                renderCube();

            }

            { // lights
                glm::mat4 model = glm::mat4(1.0f);
                light.use();
                wood.use(0);

                for (unsigned int i = 0; i < lightPositions.size(); i++) {
                    model = glm::translate(model, lightPositions[i]);
                    model = glm::scale(model, glm::vec3(0.3f));
                    light.setVec3("lightColor", lightColors[i]);
                    light.setMat4("projection", projection);
                    light.setMat4("view", view);
                    light.setMat4("model", model);

                    renderCube();
                }

            }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        bool horizontal = true, first_iteration = true;
        bloom.use();
        for (unsigned int i = 0; i < 10; i++) {
            glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
            bloom.setBool("horizontal", horizontal);
            glBindTexture(GL_TEXTURE_2D, first_iteration ? color_buffers[1] : pingpongBuffers[!horizontal]);
            renderFrame();
            horizontal = !horizontal;
            if (first_iteration) {
                first_iteration = false;
            }
        }


        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        HDR.use();
        HDR.setFloat("exposure", height);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, color_buffers[0]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, pingpongBuffers[!horizontal]);

        renderFrame();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}
