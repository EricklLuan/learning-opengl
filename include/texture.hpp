#pragma once

#include <glad/glad.h>

class Texture {
private:
	unsigned int ID;
public:
	Texture(const char* path, GLenum type, GLenum wrap);

	void use(int number);
};