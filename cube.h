#pragma once

#include "object.h"

class Cube : public ObjectInstance
{
public:

	Cube(ShaderProgram* shdrPrg = nullptr);
	~Cube();

	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) override;

private:

	bool initialized;  ///< object has the shader with defined locations
};