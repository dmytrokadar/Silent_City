#include <iostream>
#include "Barrel.h"

void Barrel::update(float elapsedTime, const glm::mat4* parentModelMatrix) {

	ObjectInstance::update(elapsedTime, parentModelMatrix);
}

void Barrel::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
	if (initialized && (shaderProgram != nullptr)) {
		glUseProgram(shaderProgram->program);

		glUniformMatrix4fv(shaderProgram->locations.PVMmatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix * viewMatrix * globalModelMatrix));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, geometry->texture);
		glBindVertexArray(geometry->vertexArrayObject);
		glDrawArrays(GL_TRIANGLES, 0, geometry->numTriangles * 3);
		glBindVertexArray(0);
	}
	else {
		std::cerr << "Cube::draw(): Can't draw, cube not initialized properly!" << std::endl;
	}
}

Barrel::Barrel(ShaderProgram* shdrPrg) : ObjectInstance(shdrPrg), initialized(false)
{
	geometry = new ObjectGeometry;

	static constexpr float vertices[] = {
		//Position
		  -0.5f, -0.5f, -0.5f,
		  -0.5f, -0.5f,  0.5f,
		  -0.5f,  0.5f,  0.5f,

		  -0.5f, -0.5f, -0.5f,
		  -0.5f,  0.5f,  0.5f,
		  -0.5f,  0.5f, -0.5f,

		  -0.5f, -0.5f,  0.5f,
		   0.5f,  0.5f,  0.5f,
		  -0.5f,  0.5f,  0.5f,

		  -0.5f, -0.5f,  0.5f,
		   0.5f, -0.5f,  0.5f,
		   0.5f,  0.5f,  0.5f,

		   0.5f, -0.5f,  0.5f,
		   0.5f, -0.5f, -0.5f,
		   0.5f,  0.5f, -0.5f,

		   0.5f, -0.5f,  0.5f,
		   0.5f,  0.5f, -0.5f,
		   0.5f,  0.5f,  0.5f,

		   0.5f, -0.5f, -0.5f,
		  -0.5f, -0.5f, -0.5f,
		  -0.5f,  0.5f, -0.5f,

		   0.5f, -0.5f, -0.5f,
		  -0.5f,  0.5f, -0.5f,
		   0.5f,  0.5f, -0.5f,

		  -0.5f,  0.5f,  0.5f,
		   0.5f,  0.5f,  0.5f,
		   0.5f,  0.5f, -0.5f,

		  -0.5f,  0.5f,  0.5f,
		   0.5f,  0.5f, -0.5f,
		  -0.5f,  0.5f, -0.5f,

		  -0.5f, -0.5f, -0.5f,
		   0.5f, -0.5f, -0.5f,
		   0.5f, -0.5f,  0.5f,

		  -0.5f, -0.5f, -0.5f,
		   0.5f, -0.5f,  0.5f,
		  -0.5f, -0.5f,  0.5f,

		  // Normals
		  -1.0f,  0.0f,  0.0f,
		  -1.0f,  0.0f,  0.0f,
		  -1.0f,  0.0f,  0.0f,

		  -1.0f,  0.0f,  0.0f,
		  -1.0f,  0.0f,  0.0f,
		  -1.0f,  0.0f,  0.0f,

		   0.0f,  0.0f,  1.0f,
		   0.0f,  0.0f,  1.0f,
		   0.0f,  0.0f,  1.0f,

		   0.0f,  0.0f,  1.0f,
		   0.0f,  0.0f,  1.0f,
		   0.0f,  0.0f,  1.0f,

		   1.0f,  0.0f,  0.0f,
		   1.0f,  0.0f,  0.0f,
		   1.0f,  0.0f,  0.0f,

		   1.0f,  0.0f,  0.0f,
		   1.0f,  0.0f,  0.0f,
		   1.0f,  0.0f,  0.0f,

		   0.0f,  0.0f, -1.0f,
		   0.0f,  0.0f, -1.0f,
		   0.0f,  0.0f, -1.0f,

		   0.0f,  0.0f, -1.0f,
		   0.0f,  0.0f, -1.0f,
		   0.0f,  0.0f, -1.0f,

		   0.0f,  1.0f,  0.0f,
		   0.0f,  1.0f,  0.0f,
		   0.0f,  1.0f,  0.0f,

		   0.0f,  1.0f,  0.0f,
		   0.0f,  1.0f,  0.0f,
		   0.0f,  1.0f,  0.0f,

		   0.0f, -1.0f,  0.0f,
		   0.0f, -1.0f,  0.0f,
		   0.0f, -1.0f,  0.0f,

		   0.0f, -1.0f,  0.0f,
		   0.0f, -1.0f,  0.0f,
		   0.0f, -1.0f,  0.0f,

		   // Texture
		   0.0f, 0.0f,
		   1.0f, 0.0f,
		   1.0f, 1.0f,

		   0.0f, 0.0f,
		   1.0f, 1.0f,
		   0.0f, 1.0f,

		   0.0f, 0.0f,
		   1.0f, 1.0f,
		   0.0f, 1.0f,

		   0.0f, 0.0f,
		   1.0f, 0.0f,
		   1.0f, 1.0f,

		   0.0f, 0.0f,
		   1.0f, 0.0f,
		   1.0f, 1.0f,

		   0.0f, 0.0f,
		   1.0f, 1.0f,
		   0.0f, 1.0f,

		   0.0f, 0.0f,
		   1.0f, 0.0f,
		   1.0f, 1.0f,

		   0.0f, 0.0f,
		   1.0f, 1.0f,
		   0.0f, 1.0f,

		   0.0f, 0.0f,
		   1.0f, 0.0f,
		   1.0f, 1.0f,

		   0.0f, 0.0f,
		   1.0f, 1.0f,
		   0.0f, 1.0f,

		   0.0f, 0.0f,
		   1.0f, 0.0f,
		   1.0f, 1.0f,

		   0.0f, 0.0f,
		   1.0f, 1.0f,
		   0.0f, 1.0f,
	};

	geometry->texture = pgr::createTexture("data/texture_test_small.jpg");
	if (geometry->texture == 0) {
		std::cout << "Texture not loaded!" << std::endl;
	}

	geometry->numTriangles = 12;
	geometry->elementBufferObject = 0;

	glGenVertexArrays(1, &geometry->vertexArrayObject);
	glBindVertexArray(geometry->vertexArrayObject);

	glGenBuffers(1, &geometry->vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	if ((shaderProgram != nullptr) && shaderProgram->initialized && (shaderProgram->locations.position != -1) && (shaderProgram->locations.PVMmatrix != -1)) {
		glEnableVertexAttribArray(shaderProgram->locations.position);
		glVertexAttribPointer(shaderProgram->locations.position, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(shaderProgram->locations.texture);
		glVertexAttribPointer(shaderProgram->locations.texture, 2, GL_FLOAT, GL_FALSE, 0, (void*)(12 * 3 * 3 * 2 * sizeof(float)));

		glBindTexture(GL_TEXTURE_2D, geometry->texture);
		initialized = true;
	}
	else {
		std::cerr << "Cube::Cube(): shaderProgram struct not initialized!" << std::endl;
	}
}

Barrel::~Barrel() {
	glDeleteVertexArrays(1, &(geometry->vertexArrayObject));
	glDeleteBuffers(1, &(geometry->elementBufferObject));
	glDeleteBuffers(1, &(geometry->vertexBufferObject));

	delete geometry;
	geometry = nullptr;

	initialized = false;
}

void Sun::update(float elapsedTime, const glm::mat4* parentModelMatrix) {

	ObjectInstance::update(elapsedTime, parentModelMatrix);
}

void Sun::changePosition(const glm::vec3 pos) {
	position = pos;
}

void Sun::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
	if (initialized && (shaderProgram != nullptr)) {
		glUseProgram(shaderProgram->program);

		glm::mat4 model = glm::translate(globalModelMatrix, position);

		glUniformMatrix4fv(shaderProgram->locations.PVMmatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix * viewMatrix * model));

		glBindVertexArray(geometry->vertexArrayObject);
		glDrawArrays(GL_TRIANGLES, 0, geometry->numTriangles * 3);
		glBindVertexArray(0);
	}
	else {
		std::cerr << "Sun::draw(): Can't draw, cube not initialized properly!" << std::endl;
	}
}

Sun::Sun(ShaderProgram* shdrPrg, const glm::vec3 pos) : ObjectInstance(shdrPrg), initialized(false)
{
	geometry = new ObjectGeometry;

	position = pos;

	static constexpr float vertices[] = {
		//Position
		  -0.5f, -0.5f, -0.5f,
		  -0.5f, -0.5f,  0.5f,
		  -0.5f,  0.5f,  0.5f,

		  -0.5f, -0.5f, -0.5f,
		  -0.5f,  0.5f,  0.5f,
		  -0.5f,  0.5f, -0.5f,

		  -0.5f, -0.5f,  0.5f,
		   0.5f,  0.5f,  0.5f,
		  -0.5f,  0.5f,  0.5f,

		  -0.5f, -0.5f,  0.5f,
		   0.5f, -0.5f,  0.5f,
		   0.5f,  0.5f,  0.5f,

		   0.5f, -0.5f,  0.5f,
		   0.5f, -0.5f, -0.5f,
		   0.5f,  0.5f, -0.5f,

		   0.5f, -0.5f,  0.5f,
		   0.5f,  0.5f, -0.5f,
		   0.5f,  0.5f,  0.5f,

		   0.5f, -0.5f, -0.5f,
		  -0.5f, -0.5f, -0.5f,
		  -0.5f,  0.5f, -0.5f,

		   0.5f, -0.5f, -0.5f,
		  -0.5f,  0.5f, -0.5f,
		   0.5f,  0.5f, -0.5f,

		  -0.5f,  0.5f,  0.5f,
		   0.5f,  0.5f,  0.5f,
		   0.5f,  0.5f, -0.5f,

		  -0.5f,  0.5f,  0.5f,
		   0.5f,  0.5f, -0.5f,
		  -0.5f,  0.5f, -0.5f,

		  -0.5f, -0.5f, -0.5f,
		   0.5f, -0.5f, -0.5f,
		   0.5f, -0.5f,  0.5f,

		  -0.5f, -0.5f, -0.5f,
		   0.5f, -0.5f,  0.5f,
		  -0.5f, -0.5f,  0.5f,

		  // Normals
		  -1.0f,  0.0f,  0.0f,
		  -1.0f,  0.0f,  0.0f,
		  -1.0f,  0.0f,  0.0f,

		  -1.0f,  0.0f,  0.0f,
		  -1.0f,  0.0f,  0.0f,
		  -1.0f,  0.0f,  0.0f,

		   0.0f,  0.0f,  1.0f,
		   0.0f,  0.0f,  1.0f,
		   0.0f,  0.0f,  1.0f,

		   0.0f,  0.0f,  1.0f,
		   0.0f,  0.0f,  1.0f,
		   0.0f,  0.0f,  1.0f,

		   1.0f,  0.0f,  0.0f,
		   1.0f,  0.0f,  0.0f,
		   1.0f,  0.0f,  0.0f,

		   1.0f,  0.0f,  0.0f,
		   1.0f,  0.0f,  0.0f,
		   1.0f,  0.0f,  0.0f,

		   0.0f,  0.0f, -1.0f,
		   0.0f,  0.0f, -1.0f,
		   0.0f,  0.0f, -1.0f,

		   0.0f,  0.0f, -1.0f,
		   0.0f,  0.0f, -1.0f,
		   0.0f,  0.0f, -1.0f,

		   0.0f,  1.0f,  0.0f,
		   0.0f,  1.0f,  0.0f,
		   0.0f,  1.0f,  0.0f,

		   0.0f,  1.0f,  0.0f,
		   0.0f,  1.0f,  0.0f,
		   0.0f,  1.0f,  0.0f,

		   0.0f, -1.0f,  0.0f,
		   0.0f, -1.0f,  0.0f,
		   0.0f, -1.0f,  0.0f,

		   0.0f, -1.0f,  0.0f,
		   0.0f, -1.0f,  0.0f,
		   0.0f, -1.0f,  0.0f,

		   // Texture
		   0.0f, 0.0f,
		   1.0f, 0.0f,
		   1.0f, 1.0f,

		   0.0f, 0.0f,
		   1.0f, 1.0f,
		   0.0f, 1.0f,

		   0.0f, 0.0f,
		   1.0f, 1.0f,
		   0.0f, 1.0f,

		   0.0f, 0.0f,
		   1.0f, 0.0f,
		   1.0f, 1.0f,

		   0.0f, 0.0f,
		   1.0f, 0.0f,
		   1.0f, 1.0f,

		   0.0f, 0.0f,
		   1.0f, 1.0f,
		   0.0f, 1.0f,

		   0.0f, 0.0f,
		   1.0f, 0.0f,
		   1.0f, 1.0f,

		   0.0f, 0.0f,
		   1.0f, 1.0f,
		   0.0f, 1.0f,

		   0.0f, 0.0f,
		   1.0f, 0.0f,
		   1.0f, 1.0f,

		   0.0f, 0.0f,
		   1.0f, 1.0f,
		   0.0f, 1.0f,

		   0.0f, 0.0f,
		   1.0f, 0.0f,
		   1.0f, 1.0f,

		   0.0f, 0.0f,
		   1.0f, 1.0f,
		   0.0f, 1.0f,
	};

	//loadObjFromFile("data/cubeTriangulated.obj");

	geometry->numTriangles = 12;
	std::cout << geometry->numTriangles << std::endl;
	geometry->elementBufferObject = 0;

	glGenVertexArrays(1, &geometry->vertexArrayObject);
	glBindVertexArray(geometry->vertexArrayObject);

	glGenBuffers(1, &geometry->vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	if ((shaderProgram != nullptr) && shaderProgram->initialized && (shaderProgram->locations.position != -1) && (shaderProgram->locations.PVMmatrix != -1)) {
		glEnableVertexAttribArray(shaderProgram->locations.position);
		glVertexAttribPointer(shaderProgram->locations.position, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(shaderProgram->locations.normals);
		glVertexAttribPointer(shaderProgram->locations.normals, 3, GL_FLOAT, GL_FALSE, 0, (void*)(12 * 3 * 3 * sizeof(float)));

		glEnableVertexAttribArray(shaderProgram->locations.texture);
		glVertexAttribPointer(shaderProgram->locations.texture, 2, GL_FLOAT, GL_FALSE, 0, (void*)(12 * 3 * 3 * 2 * sizeof(float)));

		//glBindTexture(GL_TEXTURE_2D, geometry->texture);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		initialized = true;
	}
	else {
		std::cerr << "Sun::Sun(): shaderProgram struct not initialized!" << std::endl;
	}
}

Sun::~Sun() {
	glDeleteVertexArrays(1, &(geometry->vertexArrayObject));
	glDeleteBuffers(1, &(geometry->elementBufferObject));
	glDeleteBuffers(1, &(geometry->vertexBufferObject));

	delete geometry;
	geometry = nullptr;

	initialized = false;
}

void Terrain::update(float elapsedTime, const glm::mat4* parentModelMatrix) {

	ObjectInstance::update(elapsedTime, parentModelMatrix);
}

void Terrain::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
	if (initialized && (shaderProgram != nullptr)) {
		glUseProgram(shaderProgram->program);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, geometry->texture);
		glBindVertexArray(geometry->vertexArrayObject);
		glDrawArrays(GL_TRIANGLES, 0, geometry->numTriangles * 3);
		glBindVertexArray(0);

		//glm::mat4 model = glm::translate(globalModelMatrix, position);
		glUniformMatrix4fv(shaderProgram->locations.PVMmatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix * viewMatrix * globalModelMatrix));

		glUniformMatrix4fv(shaderProgram->locations.model, 1, GL_FALSE, glm::value_ptr(globalModelMatrix));
		//flashlight
		glUniform1f(shaderProgram->locations.flashlightAngle, glm::radians(20.0f));
		//TODO change to another color than light
		//glUniform3f(shaderProgram->locations.flashlightColor, light.x, light.y, light.z);
		////glUniform3f(shaderProgram->locations.flashlightPos, lightPos.x, lightPos.y, lightPos.z);

		//glUniform3f(shaderProgram->locations.light, light.x, light.y, light.z);
		//glUniform3f(shaderProgram->locations.lightPos, lightPos.x, lightPos.y, lightPos.z);
		//glUniform3f(shaderProgram->locations.cameraPos, cameraPos.x, cameraPos.y, cameraPos.z);
		//glUniform3f(shaderProgram->locations.cameraDirection, cameraDirection.x, cameraDirection.y, cameraDirection.z);
		//glUniform1i(shaderProgram->locations.isFog, 1);
		//CHECK_GL_ERROR();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, geometry->texture);
		glBindVertexArray(geometry->vertexArrayObject);
		glDrawArrays(GL_TRIANGLES, 0, geometry->numTriangles * 3);
		glBindVertexArray(0);
	}
	else {
		std::cerr << "Cube::draw(): Can't draw, cube not initialized properly!" << std::endl;
	}
}

// generates from -height and -width to width and height vertexes
void Terrain::generateTerrain() {
	// vertices
	for (int i = -height; i < height; i++) {
		for (int j = -width; j < width; j++) {
			vertices.push_back(j);
			vertices.push_back(0);
			vertices.push_back(i);
		}
	}

	// indexes
	for (int i = -height; i < height-1; i++) {
		for (int j = -width; j < width; j++) {
			indexes.push_back(i*width + j);
			indexes.push_back((i+1)*width + j);
		}
	}
	
	// normals
	for (int i = -height; i < height; i++) {
		for (int j = -width; j < width; j++) {
			normals.push_back(j);
			normals.push_back(1.0f);
			normals.push_back(i);
		}
	}

	// textures
	float u = 0, v = 0, offset = 0.01;
	for (int i = -height; i < height; i++) {
		if (v > 511.0) {
			v = 0;
		}
		for (int j = -width; j < width; j++) {
			if (u > 511.0)
				u = 0;
			textures.push_back(u);
			textures.push_back(v);
			u += offset;
		}
		u = 0;
		v += offset;
	}

	//geometry->vertices = vertices;
	geometry->vertices.insert(vertices.end(), normals.begin(), normals.end());
	geometry->vertices.insert(geometry->vertices.end(), textures.begin(), textures.end());
	geometry->numTriangles = textures.size() / 2;
}

Terrain::Terrain(ShaderProgram* shdrPrg, int h, int w) : ObjectInstance(shdrPrg)
{
	geometry = new ObjectGeometry;
	height = h;
	width = w;
	
	generateTerrain();

	geometry->texture = pgr::createTexture("data/wall.jpg");
	if (geometry->texture == 0) {
		std::cout << "Texture not loaded!" << std::endl;
	}

	/*geometry->elementBufferObject = 0;*/

	glGenVertexArrays(1, &geometry->vertexArrayObject);
	glBindVertexArray(geometry->vertexArrayObject);

	glGenBuffers(1, &geometry->vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBufferObject);

	glGenBuffers(1, &geometry->elementBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->elementBufferObject);

	glBufferData(GL_ARRAY_BUFFER, geometry->vertices.size() * sizeof(float), &(vertices[0]), GL_STATIC_DRAW);

	if ((shaderProgram != nullptr) && shaderProgram->initialized && (shaderProgram->locations.position != -1) && (shaderProgram->locations.PVMmatrix != -1)) {
		glEnableVertexAttribArray(shaderProgram->locations.position);
		glVertexAttribPointer(shaderProgram->locations.position, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(shaderProgram->locations.normals);
		glVertexAttribPointer(shaderProgram->locations.normals, 3, GL_FLOAT, GL_FALSE, 0, (void*)(geometry->numTriangles * 3 * 3 * sizeof(float)));

		glEnableVertexAttribArray(shaderProgram->locations.texture);
		glVertexAttribPointer(shaderProgram->locations.texture, 2, GL_FLOAT, GL_FALSE, 0, (void*)(geometry->numTriangles * 3 * 3 * 2 * sizeof(float)));

		glBindTexture(GL_TEXTURE_2D, geometry->texture);
		initialized = true;
	}
	else {
		std::cerr << "Terrain::Terrain(): shaderProgram struct not initialized!" << std::endl;
	}
}

Terrain::~Terrain() {
	glDeleteVertexArrays(1, &(geometry->vertexArrayObject));
	glDeleteBuffers(1, &(geometry->elementBufferObject));
	glDeleteBuffers(1, &(geometry->vertexBufferObject));

	delete geometry;
	geometry = nullptr;

	initialized = false;
}
