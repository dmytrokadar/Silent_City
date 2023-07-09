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

	// generate and initialize terrain
	Terrain(ShaderProgram* shdrPrg = nullptr, int height = 1, int width = 1, int hL=200, int wL=200);
	// delete terrain
	~Terrain();

	//update terrain
	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	//draw terrain
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3 light, const glm::vec3 lightPos, const glm::vec3 cameraPos, const glm::vec3 cameraDirection, const bool isFog) override;
	// proceduraly generate terrain
	void generateTerrain();

private:
	Perlin* perlin;   
	bool initialized;  ///< object has the shader with defined locations
	std::vector<float> vertices; ///< terrain vertice coordinates
	std::vector<float> normals; ///< terrain normal coordinates
	std::vector<float> textures; ///< terrain texture coordinates
	std::vector<unsigned> indexes; ///< terrain indexes
	int height;                   ///< flat terrain z maximum
	int width;					   ///< flat terrain x maximum
	int heightL;					///< flat terrain z maximum
	int widthL;					 ///< terrain x maximum
};

class Animation : public ObjectInstance
{
public:

	Animation();
	~Animation();

	float fogHeight = 4.0; ///< fog height for counting fog level

	// change current frame
	void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	// draw panel with animation
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3 light, const glm::vec3 lightPos, const glm::vec3 cameraPos, const glm::vec3 cameraDirection, const bool isFog) override;
	// loading shader inside class
	void loadShader();
	// animation can be paused
	void changePlayState() {play = !play;}
	void chPosition(glm::vec3 pos) {
		position = pos;
	}
private:
	bool initialized;  ///< object has the shader with defined locations
	bool play = true;  ///< is animation playing
	//float vertices;	   
	int segment = 1;   ///< segment(frame) that us currently playing
	float lastTime = 0;  ///< last timme to count when to change frame
	//std::vector<float> normals;     
	//std::vector<float> textures;

	glm::vec3 position; ///< object position in world space

	ShaderProgram animShaderProgram; ///< shader program
};