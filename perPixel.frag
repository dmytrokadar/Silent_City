#version 140

//smooth in vec3 position_v;
smooth in vec2 texCoord_v;
in vec3 fragmentPos;
in vec3 normals_v;

out vec4 fragmentColor;
uniform sampler2D sampl;
uniform vec3 light;
uniform vec3 lightPos;
uniform vec3 cameraPos;

uniform struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
} material;

float ambientS = 0.1;
float specularS = 0.5;

void main() {

	vec3 lightDirection = normalize(lightPos - fragmentPos);
	vec3 normal = normalize(normals_v);
	vec3 ambient = ambientS*light;

	float spec = pow(max(dot(normalize(cameraPos - fragmentPos), reflect(-lightDirection, normal)), 0.0), 32);
	vec3 specular = specularS * spec * light;

	float diff = max(dot(normal, lightDirection), 0.0);
	// TODO pererobyty tak shob bulo vec3 a potim dodavalos 1.0 v kinci
	vec3 difuse = diff * light;
	fragmentColor = texture(sampl, texCoord_v) * vec4((ambient + difuse + specular), 1.0);
	//fragmentColor = vec4(0.5f, 1.0f, 1.0f, 1.0f);
}