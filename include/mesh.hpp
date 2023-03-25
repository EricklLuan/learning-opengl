#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <vector>

#include "shader.hpp"

struct vertex {
	glm::vec3 position;
	glm::vec3 normals;
	glm::vec2 texCoords;
};

struct texture {
	unsigned int ID;
	std::string type;
};

class Mesh {
private:
	unsigned int VAO, VBO, EBO;

private:
	void setup();

public:
	std::vector<vertex>       vertices;
	std::vector<unsigned int> indices;
	std::vector<texture>      textures;
public:
	Mesh(std::vector<vertex>, std::vector<unsigned int>, std::vector<texture>);
	
	void draw(Shader &shader);
};