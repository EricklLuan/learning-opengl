#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <assimp/Importer.hpp>

#include <iostream>
#include <cmath>

#include "mesh.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "camera.hpp"

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

    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    unsigned int VBO;
    unsigned int VAO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int lightVAO;

    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    Shader cubeShader = Shader(
        "shaders/c_vertex.glsl",
        "shaders/c_fragment.glsl"
    );

    Shader lightShader = Shader(
        "shaders/l_vertex.glsl",
        "shaders/l_fragment.glsl"
    );

    Texture container = Texture("assets/container2.png", GL_RGBA);
    Texture containerSpecular = Texture("assets/container2_specular.png", GL_RGBA);

    glm::vec3 lightPos = {1.5f, 1.0f, 1.0f};

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

    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f,  0.2f,  2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3(0.0f,  0.0f, -3.0f)
    };

    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        float current_frame = (float)glfwGetTime();
        delta = current_frame - last_frame;
        last_frame = current_frame;

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        cubeShader.use();
        glBindVertexArray(VAO);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(camera.zoom), 800.0f / 600.0f, 0.1f, 100.0f);

        container.use(0);
        containerSpecular.use(1);
        
        cubeShader.setInt("texture_diffuse1", 0);
        cubeShader.setInt("texture_specular1", 1);
        cubeShader.setFloat("material.shininess", 64.0f);

        cubeShader.setVec3("light.diffuse", { 1.0f, 1.0f, 1.0f });
        cubeShader.setVec3("light.ambient", { 0.0f, 0.0f, 0.0f });
        cubeShader.setVec3("light.specular", { 1.0f, 1.0f, 1.0f });

        cubeShader.setFloat("light.constant", 1.0f);
        cubeShader.setFloat("light.linear", 0.09f);
        cubeShader.setFloat("light.quadratic", 0.032f); 
        cubeShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
        cubeShader.setFloat("light.outerCutOff", glm::cos(glm::radians(15.5f)));

        cubeShader.setVec3("dirLight.direction", { -0.2f, -1.0f, -0.3f });
        cubeShader.setVec3("dirLight.ambient", { 0.05f, 0.05f, 0.05f });
        cubeShader.setVec3("dirLight.diffuse", { 0.4f, 0.4f, 0.4f });
        cubeShader.setVec3("dirLight.specular", { 0.5f, 0.5f, 0.5f });
        // point light 1
        cubeShader.setVec3("pointLight[0].position", pointLightPositions[0]);
        cubeShader.setVec3("pointLight[0].ambient", 0.05f, 0.05f, 0.05f);
        cubeShader.setVec3("pointLight[0].diffuse", 0.8f, 0.8f, 0.8f);
        cubeShader.setVec3("pointLight[0].specular", 1.0f, 1.0f, 1.0f);
        cubeShader.setFloat("pointLight[0].constant", 1.0f);
        cubeShader.setFloat("pointLight[0].linear", 0.09f);
        cubeShader.setFloat("pointLight[0].quadratic", 0.032f);
        // point light 2
        cubeShader.setVec3("pointLight[1].position", pointLightPositions[1]);
        cubeShader.setVec3("pointLight[1].ambient", 0.05f, 0.05f, 0.05f);
        cubeShader.setVec3("pointLight[1].diffuse", 0.8f, 0.8f, 0.8f);
        cubeShader.setVec3("pointLight[1].specular", 1.0f, 1.0f, 1.0f);
        cubeShader.setFloat("pointLight[1].constant", 1.0f);
        cubeShader.setFloat("pointLight[1].linear", 0.09f);
        cubeShader.setFloat("pointLight[1].quadratic", 0.032f);
        // point light 3
        cubeShader.setVec3("pointLight[2].position", pointLightPositions[2]);
        cubeShader.setVec3("pointLight[2].ambient", 0.05f, 0.05f, 0.05f);
        cubeShader.setVec3("pointLight[2].diffuse", 0.8f, 0.8f, 0.8f);
        cubeShader.setVec3("pointLight[2].specular", 1.0f, 1.0f, 1.0f);
        cubeShader.setFloat("pointLight[2].constant", 1.0f);
        cubeShader.setFloat("pointLight[2].linear", 0.09f);
        cubeShader.setFloat("pointLight[2].quadratic", 0.032f);
        // point light 4
        cubeShader.setVec3("pointLight[3].position", pointLightPositions[3]);
        cubeShader.setVec3("pointLight[3].ambient", 0.05f, 0.05f, 0.05f);
        cubeShader.setVec3("pointLight[3].diffuse", 0.8f, 0.8f, 0.8f);
        cubeShader.setVec3("pointLight[3].specular", 1.0f, 1.0f, 1.0f);
        cubeShader.setFloat("pointLight[3].constant", 1.0f);
        cubeShader.setFloat("pointLight[3].linear", 0.09f);
        cubeShader.setFloat("pointLight[3].quadratic", 0.032f);
        
        cubeShader.setVec3("light.position", camera.position);
        cubeShader.setVec3("light.direction", camera.FRONT);
        cubeShader.setVec3("viewPos", camera.position);
        
        glUniformMatrix4fv(glGetUniformLocation(cubeShader.getID(), "view"), 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));
        glUniformMatrix4fv(glGetUniformLocation(cubeShader.getID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        
        for (int i = 0; i < 10; i++) {

            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, glm::radians(20.0f * i), { 1.0f, 0.3f, 0.5f });
            
            glUniformMatrix4fv(glGetUniformLocation(cubeShader.getID(), "model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(glGetUniformLocation(cubeShader.getID(), "inverseModel"), 1, GL_FALSE, glm::value_ptr(glm::inverse(model)));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindVertexArray(0);
        glUseProgram(0);

        lightShader.use();
        glBindVertexArray(lightVAO);
        glUniformMatrix4fv(glGetUniformLocation(lightShader.getID(), "view"), 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));
        glUniformMatrix4fv(glGetUniformLocation(lightShader.getID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        for (int i = 0; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));

            glUniformMatrix4fv(glGetUniformLocation(lightShader.getID(), "model"), 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindVertexArray(0);
        glUseProgram(0);

        glfwPollEvents();
        glfwSwapBuffers(window);

    }
  
    glfwTerminate();
}
  