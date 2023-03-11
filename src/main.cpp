#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cmath>

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
    -0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f
  };

  unsigned int VBO;
  unsigned int VAO;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO); 
  glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  const char* vertexShaderSrc = 
  "#version 140\n"
  "attribute vec3 position;\n"
  "void main(){\n"
  " gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
  "}\0";

  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
  glCompileShader(vertexShader);  

  int sucess;
  char log[512];

  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &sucess);
  if (!sucess) {
    glGetShaderInfoLog(vertexShader, 512, NULL, log);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED:\n" << log << "\n";
  }

  const char* fragmentShaderSrc = 
  "#version 140\n"
  "out vec4 fragColor;\n"
  "uniform vec4 ourColor;\n"
  "void main(){\n"
  " fragColor = ourColor;\n"
  "}\0";

  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &sucess);
  if (!sucess) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, log);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED:\n" << log << "\n";
  }

  unsigned int shaderProgram = glCreateProgram();

  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);

  glBindAttribLocation(shaderProgram, 0, "position");
  glLinkProgram(shaderProgram);
  
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &sucess);
  if (!sucess) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, log);
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);


  while (!glfwWindowShouldClose(window)) {
    process_input(window);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
   
    float time = glfwGetTime();
    float green = (std::sin(time) / 2.0f) + 0.5f;
    int fragColorLocation = glGetUniformLocation(shaderProgram, "ourColor");

    glUseProgram(shaderProgram);
    glUniform4f(fragColorLocation, 0.0f, green, 0.0f, 1.0f);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }
  
  glfwTerminate();
}
  