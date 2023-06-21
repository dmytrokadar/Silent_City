#version 140

//smooth in vec3 position_v;
smooth in vec2 texCoord_v;

out vec4 fragmentColor;
uniform sampler2D sampl;
uniform vec4 light;

void main() {
	fragmentColor = texture(sampl, texCoord_v) * light;
	//fragmentColor = vec4(0.5f, 1.0f, 1.0f, 1.0f);
}