#pragma once

#include "object.h"

class Planet : public ObjectInstance
{
public:

	Planet(ShaderProgram* shdrPrg = nullptr, const glm::vec3 pos = glm::vec3(0.0f), glm::vec3 axis = glm::vec3(1,2,3), float radius = 3.0f, float angle = 1.5f);
	~Planet();

	void update(float elapsedTime, const glm::mat4* parentModelMatrix);
	void changePosition(const glm::vec3 pos);
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3 light, const glm::vec3 lightPos, const glm::vec3 cameraPos, const glm::vec3 cameraDirection, const bool isFog);

private:
	glm::vec3 position;
	bool initialized;  ///< object has the shader with defined locations

	glm::vec3 axis;
	float radius;
	float angle;
	glm::quat rotationQuat;
};

class Sun : public ObjectInstance
{
public:

	Sun(ShaderProgram* shdrPrg = nullptr, const glm::vec3 pos = glm::vec3(0.0f));
	~Sun();

	void update(float elapsedTime, const glm::mat4* parentModelMatrix);
	void changePosition(const glm::vec3 pos);
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3 cameraPos, const glm::vec3 cameraDirection, const bool isFog);
	//void addPlanet(Planet* p);
private:
	glm::vec3 position;
	//std::vector<Planet*> planets;
	bool initialized;  ///< object has the shader with defined locations
};

