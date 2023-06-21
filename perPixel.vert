#version 140

in vec3 position;
in vec2 texCoord;
in vec3 normals;

uniform mat4 PVM;
uniform mat4 model;

smooth out vec2 texCoord_v;
out vec3 fragmentPos;
out vec3 normals_v;

void main() {
	gl_Position = PVM * vec4(position, 1.0f);

	texCoord_v = texCoord;
	fragmentPos = vec3(model * vec4(position, 1.0f));
	normals_v = mat3(transpose(inverse(model))) * normals;
}