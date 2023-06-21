#version 140
in vec3 position;
in vec2 texCoord;
in vec3 normals;

uniform mat4 PVM;

smooth out vec2 texCoord_v;

void main() {
	  //vTexCoord = aPosition * 0.5f + vec2(0.5f);
	  //gl_Position = vec4(aPosition, 0.0f, 1.0f);
	gl_Position = PVM * vec4(position, 1.0f);

	texCoord_v = texCoord;

}