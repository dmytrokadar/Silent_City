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
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3 cameraPos, const glm::vec3 cameraDirection, const bool isFog);

private:
	glm::vec3 position;
	bool initialized;  ///< object has the shader with defined locations
};

class Terrain : public ObjectInstance
{
public:

	Terrain(ShaderProgram* shdrPrg = nullptr, int height = 1, int width = 1);
	~Terrain();

	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3 light, const glm::vec3 lightPos, const glm::vec3 cameraPos, const glm::vec3 cameraDirection, const bool isFog) override;
	void generateTerrain();

private:
	bool initialized;  ///< object has the shader with defined locations
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> textures;
	std::vector<unsigned> indexes;
	int height;
	int width;
};