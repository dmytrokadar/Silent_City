#pragma once

#include "object.h"

class Barrel : public ObjectInstance
{
public:

	Barrel(ShaderProgram* shdrPrg = nullptr);
	~Barrel();

	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) override;

private:

	bool initialized;  ///< object has the shader with defined locations
};

class Sun : public ObjectInstance
{
public:

	Sun(ShaderProgram* shdrPrg = nullptr, const glm::vec3 pos = glm::vec3(0.0f));
	~Sun();

	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	void changePosition(const glm::vec3 pos);
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) override;

private:
	glm::vec3 position;
	bool initialized;  ///< object has the shader with defined locations
};