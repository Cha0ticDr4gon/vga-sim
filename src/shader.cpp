#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "glad/glad.h"
#include "shader.h"

static std::map<GLuint, std::string> shader_types = {
	{GL_VERTEX_SHADER, "VERTEX_SHADER"},
	{GL_FRAGMENT_SHADER, "FRAGMENT_SHADER"}
};

Shader::Shader(const std::string& vertex_file, const std::string& fragment_file) {
	vertex_shader = create_vertex_shader(vertex_file);
	fragment_shader = create_fragment_shader(fragment_file);
	shader_program = create_shader_program(vertex_shader, fragment_shader);

	//For now, deleting these here
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

Shader::~Shader() {
	glDeleteProgram(shader_program);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

void Shader::use_program() {
	glUseProgram(shader_program);
}

std::string *Shader::read_file(const std::string& file_name) {
	//Surprised there isn't a better way to do this (to my knowledge)
	std::ifstream file_input(file_name);
	std::string line;
	std::stringstream text_output;
	
	//TODO: Replace with error handling stuff
	while(std::getline(file_input, line)) text_output << line << std::endl;
	text_output << "\0";

	file_input.close();

	return new std::string(text_output.str());
}

unsigned int Shader::create_shader(const std::string& file_name, GLuint shader_type) {
	//Read and compile shader source
	std::string *shader_source = read_file(file_name);
	unsigned int shader = glCreateShader(shader_type);

	const char *shader_code = shader_source->c_str();

	//std::cout << shader_code << std::endl;

	glShaderSource(shader, 1, &shader_code, nullptr);
	glCompileShader(shader);

	delete shader_source;

	//Make sure we know if it failed
	int success;
	char info_log[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(shader, 512, nullptr, info_log);
		std::cout << "ERROR::SHADER::" << shader_types[shader_type] << "::COMPILATION_FAILED:" << std::endl;
		std::cout << info_log << std::endl;
		return 0;
	}

	return shader;
}

unsigned int Shader::create_vertex_shader(const std::string& file_name) {
	return create_shader(file_name, GL_VERTEX_SHADER);
}

unsigned int Shader::create_fragment_shader(const std::string& file_name) {
	return create_shader(file_name, GL_FRAGMENT_SHADER);
}

unsigned int Shader::create_shader_program(unsigned int vertex_shader, unsigned int fragment_shader) {
	//Link shader stages to shader program
	unsigned int program = glCreateProgram();

	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

	//Make sure we know if it failed
	int success;
	char info_log[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(program, 512, nullptr, info_log);
		std::cout << "ERROR::SHADER::SHADER_PROGRAM::LINKING_FAILED:\n" << std::endl;
		std::cout << info_log << std::endl;
		return 0;
	}

	return program;
}
