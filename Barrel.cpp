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

void Terrain::update(float elapsedTime, const glm::mat4* parentModelMatrix) {

	ObjectInstance::update(elapsedTime, parentModelMatrix);
}

void Terrain::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3 light, const glm::vec3 lightPos, const glm::vec3 cameraPos, const glm::vec3 cameraDirection, const bool isFog)
{
	if (initialized && (shaderProgram != nullptr)) {
		glUseProgram(shaderProgram->program);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, geometry->texture);
		glBindVertexArray(geometry->vertexArrayObject);

		//glBindVertexArray(0);

		//glm::mat4 model = glm::translate(globalModelMatrix, position);
		glUniformMatrix4fv(shaderProgram->locations.PVMmatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix * viewMatrix * globalModelMatrix));

		glUniformMatrix4fv(shaderProgram->locations.model, 1, GL_FALSE, glm::value_ptr(globalModelMatrix));
		//flashlight
		glUniform1f(shaderProgram->locations.flashlightAngle, glm::radians(20.0f));
		glUniform3f(shaderProgram->locations.flashlightColor, light.x, light.y, light.z);
		//glUniform3f(shaderProgram->locations.flashlightPos, lightPos.x, lightPos.y, lightPos.z);

		glUniform3f(shaderProgram->locations.ambientM, 0.1f, 0.1f, 0.1f);
		glUniform3f(shaderProgram->locations.diffuseM, 0.9f, 1.0f, 0.9f);
		glUniform3f(shaderProgram->locations.specularM, 0.5f, 0.5f, 0.5f);
		glUniform1f(shaderProgram->locations.shininessM, 10.0f);
		/*glUniform3f(shaderProgram->locations.light, light.x, light.y, light.z);
		glUniform3f(shaderProgram->locations.lightPos, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(shaderProgram->locations.cameraPos, cameraPos.x, cameraPos.y, cameraPos.z);
		glUniform3f(shaderProgram->locations.cameraDirection, cameraDirection.x, cameraDirection.y, cameraDirection.z);
		glUniform1i(shaderProgram->locations.isFog, isFog);*/
		CHECK_GL_ERROR();

		for (int i = 0; i < height*2 - 1; i++) {
			glDrawElements(GL_TRIANGLE_STRIP, width*4, GL_UNSIGNED_INT, (void*)(width * 4 * i * sizeof(unsigned int)));
		}
		//glActiveTexture(GL_TEXTURE0);
		/*glBindTexture(GL_TEXTURE_2D, geometry->texture);
		glBindVertexArray(geometry->vertexArrayObject);
		glDrawArrays(GL_TRIANGLES, 0, geometry->numTriangles * 3);*/
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	else {
		std::cerr << "Terrain::draw(): Can't draw, cube not initialized properly!" << std::endl;
	}
}

// generates from -height and -width to width and height vertexes
void Terrain::generateTerrain() {
	// vertices
	for (int i = -height; i < height; i++) {
		for (int j = -width; j < width; j++) {
			vertices.push_back(j);
			//if ((j < width && j > -width) && (i < height && i > -height))
				vertices.push_back(0);
			//else
				//vertices.push_back(perlin->Get(j, i));
			vertices.push_back(i);
		}
	}

	// indexes
	for (int i = 0; i < (height*2)-1; i++) {
		for (int j = 0; j < width*2; j++) {
			indexes.push_back(i*width*2 + j);
			indexes.push_back((i+1)*width*2 + j);
		}
	}
	
	// normals
	for (int i = -height; i < height; i++) {
		for (int j = -width; j < width; j++) {
			normals.push_back(0.0f);
			normals.push_back(1.0f);
			normals.push_back(0.0f);
		}
	}

	// textures
	float u = 0, v = 0, offset = 0.1;
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
	geometry->vertices.insert(geometry->vertices.end(), vertices.begin(), vertices.end());
	geometry->vertices.insert(geometry->vertices.end(), normals.begin(), normals.end());
	geometry->vertices.insert(geometry->vertices.end(), textures.begin(), textures.end());
	geometry->numTriangles = vertices.size() / 3;
}

Terrain::Terrain(ShaderProgram* shdrPrg, int h, int w, int hL, int wL) : ObjectInstance(shdrPrg)
{
	geometry = new ObjectGeometry;
	height = h;
	width = w;
	perlin = new Perlin(2, 0.01f, 10.0f, 1010);
	
	generateTerrain();

	geometry->texture = pgr::createTexture("data/leafy_grass_diff_4k.jpg");
	if (geometry->texture == 0) {
		std::cout << "Texture not loaded!" << std::endl;
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	/*geometry->elementBufferObject = 0;*/

	glGenVertexArrays(1, &geometry->vertexArrayObject);
	glBindVertexArray(geometry->vertexArrayObject);

	glGenBuffers(1, &geometry->vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBufferObject);

	glGenBuffers(1, &geometry->elementBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->elementBufferObject);

	glBufferData(GL_ARRAY_BUFFER, geometry->vertices.size() * sizeof(float), &(geometry->vertices[0]), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes.size() * sizeof(unsigned), &(indexes[0]), GL_STATIC_DRAW);
	if ((shaderProgram != nullptr) && shaderProgram->initialized && (shaderProgram->locations.position != -1) && (shaderProgram->locations.PVMmatrix != -1)) {
		glEnableVertexAttribArray(shaderProgram->locations.position);
		glVertexAttribPointer(shaderProgram->locations.position, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(shaderProgram->locations.normals);
		glVertexAttribPointer(shaderProgram->locations.normals, 3, GL_FLOAT, GL_FALSE, 0, (void*)(geometry->numTriangles * 3 * sizeof(float)));

		glEnableVertexAttribArray(shaderProgram->locations.texture);
		glVertexAttribPointer(shaderProgram->locations.texture, 2, GL_FLOAT, GL_FALSE, 0, (void*)(geometry->numTriangles * 3 * 2 * sizeof(float)));

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

#define ANIM_TEXTURE "data/animation2.png"

void Animation::update(float elapsedTime, const glm::mat4* parentModelMatrix) {

	ObjectInstance::update(elapsedTime, parentModelMatrix);
	if (play) {
		if (elapsedTime - lastTime > 0.1) {
			segment++;
			lastTime = elapsedTime;
			if (segment > 75)
				segment = 1;
		}
	}
}

void Animation::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3 light, const glm::vec3 lightPos, const glm::vec3 cameraPos, const glm::vec3 cameraDirection, const bool isFog)
{
	if (initialized && (shaderProgram != nullptr)) {
		glUseProgram(shaderProgram->program);
		glm::mat4 model = glm::rotate(globalModelMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, position);
		glUniformMatrix4fv(shaderProgram->locations.PVMmatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix * viewMatrix * model));
		glUniformMatrix4fv(shaderProgram->locations.model, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(shaderProgram->locations.cameraPos, cameraPos.x, cameraPos.y, cameraPos.z);
		glUniform1f(shaderProgram->locations.fogHeight, fogHeight);
		glUniform1i(shaderProgram->locations.isFog, isFog);

		glBindVertexArray(geometry->vertexArrayObject);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, geometry->texture);
		glUniform1i(shaderProgram->locations.segment, segment);
		glUniform1i(shaderProgram->locations.sampl, 5);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}
	else {
		std::cerr << "Animation::draw(): Can't draw, cube not initialized properly!" << std::endl;
	}
}

void Animation::loadShader() {
	GLuint shaders[] = {
		pgr::createShaderFromFile(GL_VERTEX_SHADER, "animation.vert"),
		pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "animation.frag"),
		0
	};

	animShaderProgram.program = pgr::createProgram(shaders);
	animShaderProgram.locations.position = glGetAttribLocation(animShaderProgram.program, "position");
	animShaderProgram.locations.normals = glGetAttribLocation(animShaderProgram.program, "normals");
	animShaderProgram.locations.texture = glGetAttribLocation(animShaderProgram.program, "texCoord");

	// other attributes and uniforms
	animShaderProgram.locations.PVMmatrix = glGetUniformLocation(animShaderProgram.program, "PVM");
	animShaderProgram.locations.model = glGetUniformLocation(animShaderProgram.program, "model");
	animShaderProgram.locations.sampl = glGetUniformLocation(animShaderProgram.program, "texSampler");
	animShaderProgram.locations.segment = glGetUniformLocation(animShaderProgram.program, "segment");

	animShaderProgram.locations.cameraPos = glGetUniformLocation(animShaderProgram.program, "cameraPos");
	animShaderProgram.locations.fogHeight = glGetUniformLocation(animShaderProgram.program, "fogHeight");
	animShaderProgram.locations.isFog = glGetUniformLocation(animShaderProgram.program, "isFog");

	assert(animShaderProgram.locations.PVMmatrix != -1);
	assert(animShaderProgram.locations.position != -1);
	// ...
	animShaderProgram.initialized = true;

	geometry = new ObjectGeometry;
}

Animation::Animation()
{
	loadShader();
	shaderProgram = &animShaderProgram;
	//ObjectInstance(animShaderProgram);
	glUseProgram(shaderProgram->program);

	position = glm::vec3(-9.8f, 1.7f, -18.6f);

	static const float tvScreen[] = {
		1.0f, 1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f
	};

	glActiveTexture(GL_TEXTURE5);
	geometry->texture = pgr::createTexture(ANIM_TEXTURE);
	if (geometry->texture == 0) {
		std::cout << "Texture not loaded!" << std::endl;
	}
	glBindTexture(GL_TEXTURE_2D, geometry->texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);

	geometry->numTriangles = 2;
	geometry->elementBufferObject = 0;

	glGenVertexArrays(1, &geometry->vertexArrayObject);
	glBindVertexArray(geometry->vertexArrayObject);

	glGenBuffers(1, &geometry->vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), (tvScreen), GL_STATIC_DRAW);

	glEnableVertexAttribArray(shaderProgram->locations.position);
	glVertexAttribPointer(shaderProgram->locations.position, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	initialized = true;
}

Animation::~Animation() {
	glDeleteVertexArrays(1, &(geometry->vertexArrayObject));
	glDeleteBuffers(1, &(geometry->elementBufferObject));
	glDeleteBuffers(1, &(geometry->vertexBufferObject));

	delete geometry;
	geometry = nullptr;

	initialized = false;
}