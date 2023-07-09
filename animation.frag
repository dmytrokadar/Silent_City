#version 140

uniform sampler2D texSampler;  // sampler for texture access
uniform vec3 cameraPos;
uniform float fogHeight;
uniform bool isFog;

smooth in vec2 texCoord_v;     // incoming fragment texture coordinates
out vec4 color_f;              // outgoing fragment color
out vec3 fragmentPos;

float fog = 1.0;
float fogBegin = 5.0;
float fogEnd = 15.0;
float exponent = 32.0;
float fogRange = fogEnd - fogBegin;

vec3 fogColor = vec3(0.7f);
void calculateFog(){
	float dist = length(fragmentPos - cameraPos);
	float fogDist = dist - fogBegin;
	fog = clamp(fogDist/fogRange, 0.0, 1.0);

//	if(fragmentPos.y > fogHeight){
//		fog = (0.0f);
//		return;
//	}
	//float dist = length(fragmentPos - cameraPos);
	//float fogDist = dist - fogBegin;
	//float fogDistExp = fragmentPos.y/fogHeight/2;
	//fog = exp(- fogDistExp);
	
	//fog *= clamp(fogDist/fogRange, 0.0, 1.0);
}

void main() {

  // fragment color is given only by the texture
  if(isFog){
	color_f = mix(texture(texSampler, texCoord_v), vec4(fogColor,1.0), fog);
  }
	color_f = texture(texSampler, texCoord_v);
}
