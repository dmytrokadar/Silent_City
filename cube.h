#pragma once

#include "object.h"
#include "ComplexMesh.h"

class DefaultObject : public ObjectInstance
{
public:
	std::string textureName = "data/texture_test_small.jpg";

	// create object(load from file with own loader)
	DefaultObject(ShaderProgram* shdrPrg = nullptr, const glm::vec3 pos = glm::vec3(0.0f), std::string objName = "data/cubeTriangulated.obj", std::string objTexture = "data/texture_test_small.jpg");
	// depricated func for creating object(load from file with own loader)
	DefaultObject(float t, ShaderProgram* shdrPrg = nullptr, const glm::vec3 pos = glm::vec3(0.0f), std::string objName = "data/cubeTriangulated.obj");
	~DefaultObject();

	// update object and children
	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	// draw object and children
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3 light = glm::vec3(1.0f), const glm::vec3 lightPos = glm::vec3(1.0f), const glm::vec3 cameraPos = glm::vec3(1.0f), const glm::vec3 cameraDirection = glm::vec3(1.0f), const bool isFog = false);
	// init curve
	void makeCurve();
	// count position on curve
	glm::vec3 countPositionOnCurve(float t);
	void chPosition(glm::vec3 pos) {
		position = pos;
	}
private:
	glm::vec3 position;  ///< object position
	bool initialized;  ///< object has the shader with defined locations

	float tGlobal = 0;   ///< global t for counting object position and direction on curve
	const int MAX_T = 4; ///< maximum value for t
	std::vector<glm::vec3> knots; ///< knots for curve
	std::vector<glm::vec3> handles; ///< handles for curve
};
const int MAX_T = 4;

class Car : public ComplexMesh
{
public:
	// create car(load from file)
	Car(ShaderProgram* shdrPrg = nullptr, const glm::vec3 pos = glm::vec3(0.0f), std::string objName = "data/cubeTriangulated.obj");
	//Car(float t, ShaderProgram* shdrPrg = nullptr, const glm::vec3 pos = glm::vec3(0.0f), std::string objName = "data/cubeTriangulated.obj");
	~Car();

	// update car position and direction based on curve
	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	// draw car
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3 light = glm::vec3(1.0f), const glm::vec3 lightPos = glm::vec3(1.0f), const glm::vec3 cameraPos = glm::vec3(1.0f), const glm::vec3 cameraDirection = glm::vec3(1.0f), const bool isFog = false);
	// init Bezier curve
	void makeCurve();
	// update curve 
	void updateCurve(std::vector<glm::vec3> k, std::vector<glm::vec3> h) {
		knots = k;
		handles = h;
	}
	// count position on Bezier curve
	glm::vec3 countPositionOnCurve(float t);
	// count direction on Bezier curve
	glm::vec3 countDirectionOnCurve(float t, int segment);
	glm::vec3 getCarPos();
	glm::vec3 getCarLook();

private:
	//glm::vec3 position;
	//glm::vec3 direction;
	bool initialized;  ///< object has the shader with defined locations

	float tGlobal = 0; ///< global t for counting object position and direction on curve
	std::vector<glm::vec3> knots;   ///< knots for curve
	std::vector<glm::vec3> handles;  ///< handles for curve
};