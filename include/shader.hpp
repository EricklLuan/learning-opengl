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
  Shader(const char* vertexPath, const char* fragmentPath, std::vector<std::string> locations_vars_names={});

  void use();

  void setFloat(const std::string &name, float value) const;
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setVec3(const std::string& name, glm::vec3 value);

  inline unsigned int getID() const { return ID; }
};