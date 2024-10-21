#include <iostream>
#include <string>
#include <cstdint>
#include "glad/glad.h"
#include "texture.h"

Texture::Texture(uint32_t width, uint32_t height, uint8_t *buffer) {
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	//Wrap around
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Making it pixelated instead of smooth
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);

	glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int Texture::get_texture() {
	return texture;
}
