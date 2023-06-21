#version 140

//smooth in vec3 position_v;
smooth in vec2 texCoord_v;

out vec4 fragmentColor;
uniform sampler2D sampl;
uniform vec3 textColor;

void main() {
	vec4 sampledTex = vec4(1.0f);
	sampledTex.w = texture(sampl, texCoord_v).r;
	fragmentColor = vec4(textColor, 1.0f) * sampledTex;
	//fragmentColor = vec4(0.5f, 1.0f, 1.0f, 1.0f);
}