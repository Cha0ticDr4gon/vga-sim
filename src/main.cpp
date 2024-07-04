#include <iostream>
#include <glad/glad.h>
#include <cstring>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "model.h"
#include "mnist.h"
#include "texture.h"

float vertices[] = {
	-1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f, 0.0f,
	-1.0f, -1.0f, 0.0f,
	 1.0f,  1.0f, 0.0f,
	-1.0f, -1.0f, 0.0f,
	 1.0f, -1.0f, 0.0f
};

float colors[] = {
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 0.0f
};

float texture_coords[] = {
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f
};

size_t num_verts = 6;

//Simple test for putting multiple 
//TODO: Create image class so copying stuff around is nicer
unsigned char *generate_tiled_image(MnistImageSet& image_set, int width, int height) {
	//These are in pixels
	size_t buffer_width = width * image_set.get_width();
	size_t buffer_height = height * image_set.get_height();
	size_t buffer_size = buffer_width * buffer_height;

	//Multiply by 3 because pixels * RGB
	unsigned char *buffer = new unsigned char[buffer_size * 3];

	//Hopefully this works pretty well
	std::cout << "Loading image:" << std::endl;
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			int image_index = x + y * width;
			unsigned char *image = image_set.get_image(image_index);
			std::cout << static_cast<int>(image_set.get_label(image_index)) << " ";
			for(int i = 0; i < image_set.get_height(); i++) {
				unsigned char *buffer_pos = buffer + ((i + y * image_set.get_height()) * buffer_width * 3) + (x * image_set.get_width() * 3);
				unsigned char *image_pos = image + (i * image_set.get_width() * 3);
				std::memcpy(buffer_pos, image_pos, image_set.get_width() * 3);
			}
		}
		std::cout << std::endl;
	}

	return buffer;
}

void process_input(GLFWwindow *window) {
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

int main(int argc, char *argv[]) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	size_t side_len = 28 * 3 * 10;
	GLFWwindow *window = glfwCreateWindow(side_len, side_len, "MNIST Display", NULL, NULL);
	if(window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, side_len, side_len);

	//Model loading
	Model model(num_verts, vertices, colors, texture_coords);

	//Shaders
	Shader shader("shaders/basic_vertex_shader.glsl", "shaders/basic_fragment_shader.glsl");

	//MNIST
	MnistImageSet image_set("res/t10k-images.idx3-ubyte", "res/t10k-labels.idx1-ubyte");

	//Create 10x10 MNIST texture
	size_t texture_width = image_set.get_width() * 10;
	size_t texture_height = image_set.get_height() * 10;
	unsigned char *texture_data = generate_tiled_image(image_set, 10, 10);
	Texture texture(texture_width, texture_height, texture_data);

	//Moving this out here for now
	shader.use_program();
	//TODO: Do this better with a subclass
	glUniform1i(glGetUniformLocation(shader.get_program(), "texture"), 0);

	//Main draw/event loop
	while(!glfwWindowShouldClose(window)) {
		process_input(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		model.bind();

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		//TODO: Make draw method for model
		glDrawArrays(GL_TRIANGLES, 0, model.num_verts());

		model.unbind();

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}
