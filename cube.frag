#version 140

in vec2 vTexCoord;
uniform sampler2D tex;
uniform float splitX;

out vec4 fColor;

void main() {
  vec4 color = texture(tex, vTexCoord);
  if(vTexCoord.x > splitX) {
    float luma = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
    fColor = vec4(luma, luma, luma, 1.0f);
  } else {
    fColor = color;
  }
}