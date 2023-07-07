#version 140

uniform mat4 PVM;     // Projection * View * Model --> model to clip coordinates
uniform float time;         // used for simulation of moving lights (such as sun)

in vec2 position;           // vertex position in world space
in vec2 texCoord;           // incoming texture coordinates

smooth out vec2 texCoord_v; // outgoing texture coordinates

void main() {

  gl_Position = vec4(position, 0.0, 1.0);
  texCoord_v = (position + 1)/2;
}