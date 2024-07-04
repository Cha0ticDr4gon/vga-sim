#include <iostream>
#include <string>
#include "glad/glad.h"
#include "model.h"

//TODO: Look into EBOs for some stuff

Model::Model(size_t vertex_count, float *vertices, float *colors, float *texture_coords) {
	this->vertex_count = vertex_count;
	this->vertices = vertices;
	this->colors = colors;
	this->texture_coords = texture_coords;

	//Create VAO so OpenGL is happy
	glGenVertexArrays(1, &model_vao);
	glBindVertexArray(model_vao);

	//Load data into separate buffers for now (might do interleving later if it's faster)
	vertex_vbo = load_data_vbo(vertices, vertex_count * 3, 0, 3);
	color_vbo = load_data_vbo(colors, vertex_count * 3, 1, 3);
	texture_vbo = load_data_vbo(texture_coords, vertex_count * 2, 2, 2);

	//Unbind the vao for cleanup
	glBindVertexArray(0);
}

Model::~Model() {
	//delete[] vertices;
	//delete[] colors;
	//delete[] texture_coords;

	//Clean all OpenGL related buffers
	glDeleteVertexArrays(1, &model_vao);
	glDeleteBuffers(1, &vertex_vbo);
	glDeleteBuffers(1, &color_vbo);
	glDeleteBuffers(1, &texture_vbo);
}

unsigned int Model::load_data_vbo(float *data, size_t size, int location, int dimension) {
	unsigned int vbo;
	glGenBuffers(1, &vbo);

	//Put data in VBO
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), data, GL_STATIC_DRAW);

	glVertexAttribPointer(location, dimension, GL_FLOAT, GL_FALSE, dimension * sizeof(float), nullptr);
	//glEnableVertexAttribArray(location);

	//Unbind VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return vbo;
}

void Model::bind() {
	glBindVertexArray(model_vao);
}

void Model::unbind() {
	glBindVertexArray(0);
}

size_t Model::num_verts() {
	return vertex_count;
}
