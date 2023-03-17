#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cmath>

#include "shader.hpp"
#include "texture.hpp"

glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront    = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp       = glm::vec3(0.0f, 1.0f, 0.0f);

static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

static void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    const float SPEED = 0.05f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPosition += SPEED * cameraFront;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPosition -= SPEED * cameraFront;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * SPEED;
    }
    
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * SPEED;
    }
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

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";
    std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";

    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    unsigned int VBO;
    unsigned int VAO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    Shader ourShader = Shader(
        "shaders/vertex.glsl",
        "shaders/fragment.glsl",
        {"position", "textureCoord"}
    );

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Texture texture1 = Texture("assets/container.jpg", GL_RGB);
    Texture texture2 = Texture("assets/awesomeface.png", GL_RGBA);

    ourShader.use();
    ourShader.setInt("boxTexture", 0);
    ourShader.setInt("smileFaceTexture", 1);

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        ourShader.use();
        texture1.use(0);
        texture2.use(1);

        glBindVertexArray(VAO);

        for (int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(20.0f * i), glm::vec3(1.0f, 0.3f, 0.5f));
            
            glm::mat4 view = glm::mat4(1.0f);
            view = glm::lookAt(
                cameraPosition,
                cameraPosition + cameraFront,
                cameraUp
            );

            glm::mat4 projection = glm::mat4(1.0f);
            projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

            glUniformMatrix4fv(glGetUniformLocation(ourShader.getID(), "model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(glGetUniformLocation(ourShader.getID(), "view"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(ourShader.getID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindVertexArray(0);
        glUseProgram(0);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
  
    glfwTerminate();
}
  