#ifndef _SHADER_H_
#define _SHADER_H_

#include <string>
#include "glad/glad.h"

class Shader {
private:
	unsigned int vertex_shader;
	unsigned int fragment_shader;
	unsigned int shader_program;

	std::string *read_file(const std::string& file_name);
	unsigned int create_shader(const std::string& file_name, GLuint shader_type);
	unsigned int create_vertex_shader(const std::string& file_name);
	unsigned int create_fragment_shader(const std::string& file_name);
	unsigned int create_shader_program(unsigned int vertex_shader, unsigned int fragment_shader);

public:
	Shader(const std::string& vertex_file, const std::string& fragment_file);
	~Shader();
	void use_program();
	//TODO: Remove and do the right way
	unsigned int get_program() {
		return shader_program;
	}
};

#endif
