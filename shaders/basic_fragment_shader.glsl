#version 330 core

in vec3 pass_color;
in vec2 pass_texture_coord;

out vec4 out_color;

uniform sampler2D texture1;

void main(void) {
	//out_color = vec4(pass_color, 1.0);
	out_color = texture(texture1, pass_texture_coord);
}
