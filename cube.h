#pragma once

#include "object.h"

class Cube : public ObjectInstance
{
public:

	Cube(ShaderProgram* shdrPrg = nullptr, const glm::vec3 pos = glm::vec3(0.0f), std::string objName = "data/cubeTriangulated.obj");
	Cube(float t, ShaderProgram* shdrPrg = nullptr, const glm::vec3 pos = glm::vec3(0.0f), std::string objName = "data/cubeTriangulated.obj");
	~Cube();

	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3 light = glm::vec3(1.0f), const glm::vec3 lightPos = glm::vec3(1.0f), const glm::vec3 cameraPos = glm::vec3(1.0f), const glm::vec3 cameraDirection = glm::vec3(1.0f));
	void makeCurve();
	glm::vec3 countPositionOnCurve(float t);

private:
	glm::vec3 position;
	bool initialized;  ///< object has the shader with defined locations

	float tGlobal = 0;
	const int MAX_T = 4;
	std::vector<glm::vec3> knots;
	std::vector<glm::vec3> handles;
};