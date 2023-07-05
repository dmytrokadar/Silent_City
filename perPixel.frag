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

uniform float alphaChannel;

uniform vec3 dirLightVec;
vec3 dirLightColor = vec3(1.0, 1.0, 1.0);

uniform struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
} material;

struct Light {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
}light_s;

//float ambientS = 0.1;
//float specularS = 0.5;
float fog = 1.0;
float fogBegin = 5.0;
float fogEnd = 15.0;
float exponent = 32.0;
float fogRange = fogEnd - fogBegin;

vec3 flashlightPos;
vec3 fogColor = vec3(0.7f);

void calculateFog(){
	float dist = length(fragmentPos - cameraPos);
	float fogDist = dist - fogBegin;
	fog = clamp(fogDist/fogRange, 0.0, 1.0);
}

void calculateLight(){
	flashlightPos = cameraPos;
	vec3 flashlightDirection = normalize(cameraDirection);
	vec3 normal = normalize(normals_v);

	vec3 L = normalize(flashlightPos - fragmentPos);
	vec3 R = reflect(-L, normal);
	vec3 V = normalize(cameraPos - fragmentPos);
}

void main() {
	light_s.ambient = vec3(1.0f, 1.0f, 1.0f);
	light_s.diffuse = vec3(0.5f, 0.5f, 0.5f);
	light_s.specular = vec3(1.0f, 1.0f, 1.0f);

	flashlightPos = cameraPos;
	vec3 flashlightDirection = normalize(cameraDirection);
	vec3 normal = normalize(normals_v);

	vec3 L = normalize(flashlightPos - fragmentPos);
	vec3 R = reflect(-L, normal);
	vec3 V = normalize(cameraPos - fragmentPos);
	//calculate flashlight
	float spec = pow(max(dot(V, R), 0.0), material.shininess);
	float diff = max(dot(normal, L), 0.0);

	vec3 ambient = material.ambient*flashlightColor * light_s.ambient;
	vec3 specular = material.specular * spec * flashlightColor * light_s.specular;
	vec3 difuse = (diff*material.diffuse) * flashlightColor * light_s.diffuse;

	vec3 flLightColor = (ambient + difuse + specular);
	float dotViewLight = dot(flashlightDirection, -L);
	if(abs(acos(dotViewLight))> flashlightAngle)
		flLightColor = vec3(0.0f);
	else
		flLightColor *= pow(max(0.0, dotViewLight), exponent); 
	//float tmp = max(0.0, dot(-flashlightDirection, cameraDirection));
	//if(tmp > 0.90){
		//flLightColor *= tmp;
	//} else{
		//flLightColor = vec3(0.0f);
	//}

	//calculate directional light
	vec3 dirLightDir = normalize(dirLightVec);

	spec = pow(max(dot(V, reflect(-dirLightDir, normal)), 0.0), material.shininess);
	ambient = material.ambient*dirLightColor * light_s.ambient;
	specular = material.specular * spec * dirLightColor * light_s.specular;

	diff = max(dot(normal, dirLightDir), 0.0);
	// TODO pererobyty tak shob bulo vec3 a potim dodavalos 1.0 v kinci
	difuse = (diff*material.diffuse) * dirLightColor * light_s.diffuse;
	//calculateFog();

	vec3 lightColorDir = (ambient + difuse + specular);

	//calculate light
	vec3 lightDirection = normalize(lightPos - fragmentPos);

	spec = pow(max(dot(normalize(cameraPos - fragmentPos), reflect(-lightDirection, normal)), 0.0), material.shininess);
	ambient = material.ambient * light * light_s.ambient;
	specular = material.specular * spec * light * light_s.specular;

	diff = max(dot(normal, lightDirection), 0.0);
	difuse = (diff*material.diffuse) * light * light_s.diffuse;
	calculateFog();

	vec3 lightColor = (ambient + difuse + specular) + flLightColor + lightColorDir;
	fragmentColor = texture(sampl, texCoord_v) * vec4(lightColor, 1.0);
	if(isFog){
		fragmentColor = mix(fragmentColor, vec4(fogColor,1.0), fog);
	}

	fragmentColor.w = alphaChannel;
	//else
		//fragmentColor = vec4(0.5f, 1.0f, 1.0f, 1.0f);
}