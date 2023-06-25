#version 140

//smooth in vec3 position_v;
smooth in vec2 texCoord_v;
in vec3 fragmentPos;
in vec3 normals_v;

out vec4 fragmentColor;
uniform sampler2D sampl;
uniform float flashlightAngle;
uniform vec3 flashlightColor;
uniform vec3 light;
uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform vec3 cameraDirection;
uniform bool isFog;

uniform struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
} material;

float ambientS = 0.1;
float specularS = 0.5;
float fog = 1.0;
float fogBegin = 5.0;
float fogEnd = 15.0;
float fogRange = fogEnd - fogBegin;

vec3 flashlightPos;
vec3 fogColor = vec3(0.7f);

void calculateFog(){
	float dist = length(fragmentPos - cameraPos);
	float fogDist = dist - fogBegin;
	fog = clamp(fogDist/fogRange, 0.0, 1.0);
}

void main() {
	flashlightPos = cameraPos;
	vec3 flashlightDirection = normalize(cameraDirection);
	vec3 normal = normalize(normals_v);

	vec3 L = normalize(flashlightPos - fragmentPos);
	vec3 R = reflect(-L, normal);
	vec3 V = normalize(cameraPos - fragmentPos);
	//calculate flashlight
	float spec = pow(max(dot(V, R), 0.0), 32.0);
	float diff = max(dot(normal, L), 0.0);

	vec3 ambient = ambientS*flashlightColor;
	vec3 specular = specularS * spec * flashlightColor;
	vec3 difuse = diff * flashlightColor;

	vec3 flLightColor = (ambient + difuse + specular);
	if(abs(acos(dot(flashlightDirection, -L))) > flashlightAngle)
		flLightColor = vec3(0.0f);
	//float tmp = max(0.0, dot(-flashlightDirection, cameraDirection));
	//if(tmp > 0.90){
		//flLightColor *= tmp;
	//} else{
		//flLightColor = vec3(0.0f);
	//}

	//calculate light
	vec3 lightDirection = normalize(lightPos - fragmentPos);

	spec = pow(max(dot(normalize(cameraPos - fragmentPos), reflect(-lightDirection, normal)), 0.0), 32);
	ambient = ambientS*light;
	specular = specularS * spec * light;

	diff = max(dot(normal, lightDirection), 0.0);
	// TODO pererobyty tak shob bulo vec3 a potim dodavalos 1.0 v kinci
	difuse = diff * light;
	calculateFog();

	vec3 lightColor = (ambient + difuse + specular) + flLightColor;
	if(isFog == 1){
		fragmentColor = texture(sampl, texCoord_v) * vec4(lightColor, 1.0);
		fragmentColor = mix(fragmentColor, vec4(fogColor,1.0), fog);
	}
	else
		fragmentColor = vec4(0.5f, 1.0f, 1.0f, 1.0f);
}