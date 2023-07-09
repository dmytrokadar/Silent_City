#pragma once

#include "object.h"
#include "ComplexMesh.h"

class DefaultObject : public ObjectInstance
{
public:
	std::string textureName = "data/texture_test_small.jpg";

	DefaultObject(ShaderProgram* shdrPrg = nullptr, const glm::vec3 pos = glm::vec3(0.0f), std::string objName = "data/cubeTriangulated.obj", std::string objTexture = "data/texture_test_small.jpg");
	DefaultObject(float t, ShaderProgram* shdrPrg = nullptr, const glm::vec3 pos = glm::vec3(0.0f), std::string objName = "data/cubeTriangulated.obj");
	~DefaultObject();

	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3 light = glm::vec3(1.0f), const glm::vec3 lightPos = glm::vec3(1.0f), const glm::vec3 cameraPos = glm::vec3(1.0f), const glm::vec3 cameraDirection = glm::vec3(1.0f), const bool isFog = false);
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
const int MAX_T = 4;

class Car : public ComplexMesh
{
public:

	Car(ShaderProgram* shdrPrg = nullptr, const glm::vec3 pos = glm::vec3(0.0f), std::string objName = "data/cubeTriangulated.obj");
	//Car(float t, ShaderProgram* shdrPrg = nullptr, const glm::vec3 pos = glm::vec3(0.0f), std::string objName = "data/cubeTriangulated.obj");
	~Car();

	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3 light = glm::vec3(1.0f), const glm::vec3 lightPos = glm::vec3(1.0f), const glm::vec3 cameraPos = glm::vec3(1.0f), const glm::vec3 cameraDirection = glm::vec3(1.0f), const bool isFog = false);
	void makeCurve();
	void updateCurve(std::vector<glm::vec3> k, std::vector<glm::vec3> h) {
		knots = k;
		handles = h;
	}
	glm::vec3 countPositionOnCurve(float t);
	glm::vec3 countDirectionOnCurve(float t, int segment);
	glm::vec3 getCarPos();
	glm::vec3 getCarLook();

private:
	//glm::vec3 position;
	//glm::vec3 direction;
	bool initialized;  ///< object has the shader with defined locations

	float tGlobal = 0;
	std::vector<glm::vec3> knots;
	std::vector<glm::vec3> handles;
};