#include "shader.hpp"

#include <glad/glad.h>

#include <sstream>
#include <fstream>
#include <iostream>

void Shader::handleError(unsigned int shaderID, const char* error_text) const {
  int success;
  char log[512];

  glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shaderID, 512, NULL, log);
    std::cout << error_text << log << "\n";
  }
}

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath) {
  std::string vertexSRC;
  std::string fragmentSRC;
  std::string geometrySRC;


  std::ifstream vertexFile;
  std::ifstream fragmentFile;
  std::ifstream geometryFile;

  vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  if (strcmp(geometryPath, (const char*)"") != 0) 
      geometryFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    vertexFile.open(vertexPath);
    fragmentFile.open(fragmentPath);
    if (strcmp(geometryPath, (const char*)"") != 0)
        geometryFile.open(geometryPath);

    std::stringstream vertex, fragment, geometry;

    vertex << vertexFile.rdbuf();
    fragment << fragmentFile.rdbuf();
    if (strcmp(geometryPath, (const char*)"") != 0)
        geometry << geometryFile.rdbuf();

    vertexSRC = vertex.str();
    fragmentSRC = fragment.str();
    geometrySRC = geometry.str();
  } catch (std::ifstream::failure e) {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n";
  }

  const char* vCode = vertexSRC.c_str();
  const char* fCode = fragmentSRC.c_str();
  const char* gCode = geometrySRC.c_str();

  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vCode, NULL);
  glCompileShader(vertexShader);
  handleError(vertexShader, "ERROR::SHADER::VERTEX::COMPILATION_FAILED: ");

  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fCode, NULL);
  glCompileShader(fragmentShader);
  handleError(fragmentShader, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: ");

  unsigned int geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
  if (strcmp(geometryPath, (const char*)"") != 0) {
    glShaderSource(geometryShader, 1, &gCode, NULL);
    glCompileShader(geometryShader);
    handleError(geometryShader, "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED: ");
  }

  ID = glCreateProgram();
  glAttachShader(ID, vertexShader);
  glAttachShader(ID, fragmentShader);
  if (strcmp(geometryPath, (const char*)"") != 0)
      glAttachShader(ID, geometryShader);

  glBindAttribLocation(ID, 0, "position");
  glBindAttribLocation(ID, 1, "normals");
  glBindAttribLocation(ID, 2, "texCoords");

  glLinkProgram(ID);

  int success;
  char log[512];
  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(ID, 512, NULL, log);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED: " << log << "\n";
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glDeleteShader(geometryShader);
}

void Shader::use() {
    glUseProgram(ID);
}

void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec3(const std::string& name, glm::vec3 value) {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z);
}

void Shader::setVec3(const std::string& name, float v1, float v2, float v3)
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3);
}

void Shader::setMat4(const std::string& name, glm::mat4 matrix) {

    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}
