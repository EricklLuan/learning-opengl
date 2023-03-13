#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.hpp"

static bool wireframe = false;

static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

static void process_input(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && !wireframe) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    wireframe = true;
  } else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE && wireframe) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    wireframe = false;
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
    return -1;
  }

  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, &framebuffer_size_callback);

  std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n";

  float verticies[] = {
    -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
     0.0f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
     0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f
  };

  unsigned int VBO;
  unsigned int VAO;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO); 
  glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
  glEnableVertexAttribArray(1);

  Shader ourShader = Shader(
    "../shaders/vertex.glsl",
    "../shaders/fragment.glsl",
    {"position", "color"}
  );

  while (!glfwWindowShouldClose(window)) {
    process_input(window);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
   
    ourShader.use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }
  
  glfwTerminate();
}
  