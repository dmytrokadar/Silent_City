﻿#version 140

uniform mat4 PVM;     // Projection * View * Model --> model to clip coordinates
uniform float time;         // used for simulation of moving lights (such as sun)

in vec2 position;           // vertex position in world space
in vec2 texCoord;           // incoming texture coordinates

smooth out vec2 texCoord_v; // outgoing texture coordinates

float decay = 0.05;

void main() {

  // vertex position after the projection (gl_Position is predefined output variable)
  gl_Position = vec4(position, 0.0, 1.0);   // outgoing vertex in clip coordinates

  //float localTime = time * decay;
  // localTime = 0;

  //vec2 offset = vec2((floor(localTime) - localTime) * 4 + 1.0, 0.0);

  // outputs entering the fragment shader
  //texCoord_v =  texCoord;
  texCoord_v = (position + 1)/2;
}