#include "texture.hpp"

#define STB_IMAGE_IMPLEMENTATION 
#include <stb_image.h>

#include <iostream>

Texture::Texture(const char* path, GLenum type) {

	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cout << "Failed to load texture\n";
	}

	stbi_image_free(data);
}

void Texture::use(int number) {
	glActiveTexture(GL_TEXTURE0 + number);
	glBindTexture(GL_TEXTURE_2D, ID);
}
