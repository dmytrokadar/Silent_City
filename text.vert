#version 140

in vec3 position;
in vec2 texCoord;

uniform mat4 projection;

smooth out vec2 texCoord_v;


void main() {
	gl_Position = projection * vec4(position.xy, 0.0f, 1.0f);

	texCoord_v = texCoord;
}