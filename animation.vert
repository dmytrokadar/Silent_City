#version 140

uniform mat4 PVM;     // Projection * View * Model --> model to clip coordinates
uniform int segment;
uniform mat4 model;

in vec3 position;           // vertex position in world space
in vec2 texCoord;           // incoming texture coordinates

smooth out vec2 texCoord_v; // outgoing texture coordinates
out vec3 fragmentPos;       // outgoing fragment position

float decay = 0.05;
int width = 5;
int height = 15;
int clipW = 1280;
int clipH = 720;

int globalW = width*clipW;
int globalH = height*clipH;

void main() {
  gl_Position = PVM * vec4(position, 1.0);   // outgoing vertex in clip coordinates

  int w = segment % width;
  int h = segment % height;

  w = (w==0) ? width : w;
  h = (h==0) ? height : h;

  w--;
  h--;

  w *= clipW;
  h *= clipH;

  //w = 5120; h = 2880;

  /*if(position.xy == vec2(-1, 1)){
	texCoord_v = vec2(w/globalW, h/globalH);
  }
  else if(position.xy == vec2(1, 1)){
	texCoord_v = vec2((w+clipW)/globalW, h/globalH);
  }
  else if(position.xy == vec2(1, -1)){
	texCoord_v = vec2((w+clipW)/globalW, (h+clipH)/globalH);
  }
  else if(position.xy == vec2(-1, -1)){
	texCoord_v = vec2(w/globalW, (h+clipH)/globalH);
  }*/

  vec2 tmp = (position.xy + 1)/2;
  texCoord_v = vec2((w+(tmp.x)*clipW)/globalW, (h+(tmp.y)*clipH)/globalH);
  fragmentPos = vec3(model * vec4(position, 1.0f));
  //texCoord_v = texCoord;
  //texCoord_v = (position.xy + 1)/2;
}
