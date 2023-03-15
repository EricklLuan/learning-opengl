#pragma once

#include <glad/glad.h>

#include <string>
#include <vector>

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

  inline unsigned int getID() const { return ID; }
};