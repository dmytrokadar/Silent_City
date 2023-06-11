#version 140

//smooth in vec3 position_v;
smooth in vec3 texCoord_v;

out vec4 fragmentColor;
uniform samplerCube cubeMap;

void main() {
	fragmentColor = texture(cubeMap, texCoord_v);
	//fragmentColor = vec4(0.5f, 1.0f, 1.0f, 1.0f);
}