#pragma once

#include <glad/glad.h>
#include <vector>

class Texture {
private:
	unsigned int ID;
	GLenum type;
public:
	Texture(const char* path, GLenum ctype, GLenum rtype, GLenum wrap);
	Texture(std::vector<const char*> paths, GLenum ctype, GLenum wrap);

	void use(int number);
};