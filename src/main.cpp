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
bool blinn = true;

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

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
        blinn = !blinn;
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

    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, &framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";
    std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
    
    unsigned int depthFramebuffer;
    glGenFramebuffers(1, &depthFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, depthFramebuffer);

    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    unsigned int depthCubeMap;
    glGenTextures(1, &depthCubeMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);

    for (unsigned int face = 0; face < 6; face++) {
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_DEPTH_COMPONENT, 
            SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 
            NULL
        );
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    float planeVertices[] = {
        // positions            // normals         // texcoords
         10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
        -10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,

         10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
        -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
         10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
    };

    float cubeVertices[] = {
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

    float quadVertices[] = {
        // positions        // texture Coords
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };

    unsigned int quadVBO;
    glGenBuffers(1, &quadVBO);

    unsigned int quadVAO;
    glGenVertexArrays(1, &quadVAO);

    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindVertexArray(0);

    unsigned int planeVBO;
    glGenBuffers(1, &planeVBO);

    unsigned int planeVAO;
    glGenVertexArrays(1, &planeVAO);

    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    glBindVertexArray(0);

    unsigned int cubeVBO;
    glGenBuffers(1, &cubeVBO);

    unsigned int cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    glBindVertexArray(0);

    Texture plank("assets/plank.png", GL_RGB, GL_RGB, GL_REPEAT);

    Shader depthShader = {
        "shaders/advanced_lighting/point_shadows/depth.vert",
        "shaders/advanced_lighting/point_shadows/depth.frag",
        std::vector<const char*>{"aPosition"},
        "shaders/advanced_lighting/point_shadows/depth.geom"
    };

    Shader shadowShader = {
        "shaders/advanced_lighting/point_shadows/point_shadow.vert",
        "shaders/advanced_lighting/point_shadows/point_shadow.frag",
        std::vector<const char*>{"aPosition", "aNormal", "aTexCoords"},
    };

    glEnable(GL_DEPTH_TEST);

    glm::vec3 lightPos = {0.0f, 0.0f, 0.0f};

    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        float current_frame = (float)glfwGetTime();
        delta = current_frame - last_frame;
        last_frame = current_frame;

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthFramebuffer);
            glClear(GL_DEPTH_BUFFER_BIT);
            
            float aspect = static_cast<float>(SHADOW_WIDTH) / static_cast<float>(SHADOW_HEIGHT);
            float near   = 0.1f;
            float far    = 100.0f;

            glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far);
            
            std::vector<glm::mat4> shadowTransforms;
            shadowTransforms.push_back(
                shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f))
            );

            shadowTransforms.push_back(
                shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f))
            );

            shadowTransforms.push_back(
                shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f))
            );

            shadowTransforms.push_back(
                shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f))
            );

            shadowTransforms.push_back(
                shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f))
            );

            shadowTransforms.push_back(
                shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))
            );

            depthShader.use();
            depthShader.setFloat("far", far);
            depthShader.setVec3("lightPos", lightPos);

            for (unsigned int i = 0; i < 6; i++) { 
                depthShader.setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]); 
            }

            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::scale(model, glm::vec3(5.0f));
                depthShader.setMat4("model", model);
                glDisable(GL_CULL_FACE); 
                depthShader.setInt("reverse_normals", 1);
                
                glBindVertexArray(cubeVAO);
                glDrawArrays(GL_TRIANGLES, 0, 36);

                depthShader.setInt("reverse_normals", 0);
                glEnable(GL_CULL_FACE);
                
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(4.0f, -3.5f, 0.0));
                model = glm::scale(model, glm::vec3(0.5f));
                depthShader.setMat4("model", model);
                
                glBindVertexArray(cubeVAO);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(2.0f, 3.0f, 1.0));
                model = glm::scale(model, glm::vec3(0.75f));
                depthShader.setMat4("model", model);
                
                glBindVertexArray(cubeVAO);
                glDrawArrays(GL_TRIANGLES, 0, 36);

                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(-3.0f, -1.0f, 0.0));
                model = glm::scale(model, glm::vec3(0.5f));
                depthShader.setMat4("model", model);
                
                glBindVertexArray(cubeVAO);
                glDrawArrays(GL_TRIANGLES, 0, 36);

                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(-1.5f, 1.0f, 1.5));
                model = glm::scale(model, glm::vec3(0.5f));
                depthShader.setMat4("model", model);
                
                glBindVertexArray(cubeVAO);
                glDrawArrays(GL_TRIANGLES, 0, 36);

                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(-1.5f, 2.0f, -3.0));
                model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
                model = glm::scale(model, glm::vec3(0.75f));
                depthShader.setMat4("model", model);
                
                glBindVertexArray(cubeVAO);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glViewport(0, 0, 800, 600);
        glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shadowShader.use();
        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), 800.0f / 600.0f, 0.1f, 100.0f);
        glm::mat4 view = camera.getViewMatrix();

        shadowShader.setMat4("projection", projection);
        shadowShader.setMat4("view", view);

        shadowShader.setVec3("lightPos", lightPos);
        shadowShader.setVec3("viewPos", camera.position);
        shadowShader.setFloat("far", far);

        plank.use(0);
        shadowShader.setInt("diffuse_texture", 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
        shadowShader.setInt("shadowMap", 1);

        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::scale(model, glm::vec3(5.0f));
            shadowShader.setMat4("model", model);
            shadowShader.setMat4("inverseModel", glm::inverse(model));
            glDisable(GL_CULL_FACE);

            glBindVertexArray(cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            glEnable(GL_CULL_FACE);

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(4.0f, -3.5f, 0.0));
            model = glm::scale(model, glm::vec3(0.5f));
            shadowShader.setMat4("model", model);
            shadowShader.setMat4("inverseModel", glm::inverse(model));

            glBindVertexArray(cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(2.0f, 3.0f, 1.0));
            model = glm::scale(model, glm::vec3(0.75f));
            shadowShader.setMat4("model", model);
            shadowShader.setMat4("inverseModel", glm::inverse(model));

            glBindVertexArray(cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-3.0f, -1.0f, 0.0));
            model = glm::scale(model, glm::vec3(0.5f));
            shadowShader.setMat4("model", model);
            shadowShader.setMat4("inverseModel", glm::inverse(model));

            glBindVertexArray(cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-1.5f, 1.0f, 1.5));
            model = glm::scale(model, glm::vec3(0.5f));
            shadowShader.setMat4("model", model);
            shadowShader.setMat4("inverseModel", glm::inverse(model));

            glBindVertexArray(cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-1.5f, 2.0f, -3.0));
            model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
            model = glm::scale(model, glm::vec3(0.75f));
            shadowShader.setMat4("model", model);
            shadowShader.setMat4("inverseModel", glm::inverse(model));

            glBindVertexArray(cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}
