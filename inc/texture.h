#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <cstdint>

class Texture {
private:
	unsigned int texture;

	unsigned int get_texture();

public:
	//TODO: Figure out why size_t broke my compiler
	Texture(uint32_t width, uint32_t height, unsigned char *buffer);
};

#endif
