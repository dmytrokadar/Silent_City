#pragma once

#include "object.h"
#include "perlin.h"

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

class Terrain : public ObjectInstance
{
public:

	Terrain(ShaderProgram* shdrPrg = nullptr, int height = 1, int width = 1, int hL=200, int wL=200);
	~Terrain();

	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3 light, const glm::vec3 lightPos, const glm::vec3 cameraPos, const glm::vec3 cameraDirection, const bool isFog) override;
	// proceduraly generating terrain
	void generateTerrain();

private:
	Perlin* perlin;
	bool initialized;  ///< object has the shader with defined locations
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> textures;
	std::vector<unsigned> indexes;
	int height;
	int width;
	int heightL;
	int widthL;
};

class Animation : public ObjectInstance
{
public:

	Animation();
	~Animation();

	float fogHeight = 4.0;

	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3 light, const glm::vec3 lightPos, const glm::vec3 cameraPos, const glm::vec3 cameraDirection, const bool isFog) override;
	// loading shader inside class
	void loadShader();
	// animation can be paused
	void changePlayState() {play = !play;}
private:
	bool initialized;  ///< object has the shader with defined locations
	bool play = true;
	float vertices;
	int segment = 1;
	float lastTime = 0;
	std::vector<float> normals;
	std::vector<float> textures;

	glm::vec3 position;

	ShaderProgram animShaderProgram;
};