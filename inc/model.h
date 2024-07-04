#ifndef _MODEL_H_
#define _MODEL_H_

#include <iostream>
#include <string>

class Model {
private:
	unsigned int vertex_vbo;
	unsigned int color_vbo;
	unsigned int texture_vbo;

	unsigned int model_vao;

	size_t vertex_count;

	float *vertices;
	float *colors;
	float *texture_coords;

	unsigned int load_data_vbo(float *data, size_t size, int location, int dimension);

public:
	Model(size_t vertex_count, float *vertices, float *colors, float *texture_coords);
	~Model();
	void bind();
	void unbind();
	size_t num_verts();
};

#endif
