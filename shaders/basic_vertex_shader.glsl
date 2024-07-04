#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texture_coord;

out vec3 pass_color;
out vec2 pass_texture_coord;

void main(void) {
	gl_Position = vec4(position.x, position.y, position.z, 1.0);
	pass_color = color;
	pass_texture_coord = texture_coord;
}
