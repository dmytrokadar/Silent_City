#version 140
in vec2 aPosition;
out vec2 vTexCoord;

void main() {
	  vTexCoord = aPosition * 0.5f + vec2(0.5f);
	  gl_Position = vec4(aPosition, 0.0f, 1.0f);
}