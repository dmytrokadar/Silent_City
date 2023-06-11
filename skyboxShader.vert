#version 140

in vec2 position;
in vec3 texCoords;

uniform mat4 PVM;

smooth out vec3 texCoord_v;


void main() {
	gl_Position = vec4(position, 1.0f, 1.0f);
	vec4 worldCoord = PVM * gl_Position;

	texCoord_v = worldCoord.xyz / worldCoord.w;
}