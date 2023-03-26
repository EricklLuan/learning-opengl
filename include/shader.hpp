#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
private:
  unsigned int ID;
private:
  void handleError(unsigned int shaderID, const char* error_text) const;

public:
  Shader(const char* vertexPath, const char* fragmentPath);

  void use();

  void setFloat(const std::string &name, float value) const;
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setVec3(const std::string& name, glm::vec3 value);
  void setVec3(const std::string& name, float v1, float v2, float v3);
  void setMat4(const std::string& name, glm::mat4 matrix);

  inline unsigned int getID() const { return ID; }
};