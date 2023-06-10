#include <iostream>
#include "cube.h"

const char* TEST_Texture = "data/texture_test.jpg";

void Cube::update(float elapsedTime, const glm::mat4* parentModelMatrix) {
	
	ObjectInstance::update(elapsedTime, parentModelMatrix);
}

void Cube::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
	if (initialized && (shaderProgram != nullptr)) {
		glUseProgram(shaderProgram->program);

		glUniformMatrix4fv(shaderProgram->locations.PVMmatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix * viewMatrix * globalModelMatrix));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, geometry->texture);
		glBindVertexArray(geometry->vertexArrayObject);
		glDrawArrays(GL_TRIANGLES, 0, geometry->numTriangles*3);
		glBindVertexArray(0);
	}
	else {
		std::cerr << "Cube::draw(): Can't draw, cube not initialized properly!" << std::endl;
	}
}

Cube::Cube(ShaderProgram* shdrPrg) : ObjectInstance(shdrPrg), initialized(false)
{
	geometry = new ObjectGeometry;

	static constexpr float vertices[] = {
		//Position
		  -0.5f, -0.5f, -0.5f,  // 0
		  -0.5f, -0.5f,  0.5f,  // 1
		  -0.5f,  0.5f,  0.5f,  // 5  

		  -0.5f, -0.5f, -0.5f,  // 0
		  -0.5f,  0.5f,  0.5f,  // 5  
		  -0.5f,  0.5f, -0.5f,  // 4

		  -0.5f, -0.5f,  0.5f,  // 1
		   0.5f,  0.5f,  0.5f,  // 6
		  -0.5f,  0.5f,  0.5f,  // 5  

		  -0.5f, -0.5f,  0.5f,  // 1
		   0.5f, -0.5f,  0.5f,  // 2
		   0.5f,  0.5f,  0.5f,  // 6

		   0.5f, -0.5f,  0.5f,  // 2
		   0.5f, -0.5f, -0.5f,  // 3
		   0.5f,  0.5f, -0.5f,  // 7

		   0.5f, -0.5f,  0.5f,  // 2
		   0.5f,  0.5f, -0.5f,  // 7
		   0.5f,  0.5f,  0.5f,  // 6

		   0.5f, -0.5f, -0.5f,  // 3
		  -0.5f, -0.5f, -0.5f,  // 0
		  -0.5f,  0.5f, -0.5f,  // 4

		   0.5f, -0.5f, -0.5f,  // 3
		  -0.5f,  0.5f, -0.5f,  // 4
		   0.5f,  0.5f, -0.5f,  // 7

		  -0.5f,  0.5f,  0.5f,  // 5  
		   0.5f,  0.5f,  0.5f,  // 6
		   0.5f,  0.5f, -0.5f,  // 7

		  -0.5f,  0.5f,  0.5f,  // 5  
		   0.5f,  0.5f, -0.5f,  // 7
		  -0.5f,  0.5f, -0.5f,  // 4

		  -0.5f, -0.5f, -0.5f,  // 0
		   0.5f, -0.5f, -0.5f,  // 3
		   0.5f, -0.5f,  0.5f,  // 2

		  -0.5f, -0.5f, -0.5f,  // 0
		   0.5f, -0.5f,  0.5f,  // 2
		  -0.5f, -0.5f,  0.5f,  // 1

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
		glVertexAttribPointer(shaderProgram->locations.texture, 2, GL_FLOAT, GL_FALSE, 0, (void*)(12*3*3*2*sizeof(float)));

		glBindTexture(GL_TEXTURE_2D, geometry->texture);
		initialized = true;
	}
	else {
		std::cerr << "Cube::Cube(): shaderProgram struct not initialized!" << std::endl;
	}
}

Cube::~Cube() {
	glDeleteVertexArrays(1, &(geometry->vertexArrayObject));
	glDeleteBuffers(1, &(geometry->elementBufferObject));
	glDeleteBuffers(1, &(geometry->vertexBufferObject));

	delete geometry;
	geometry = nullptr;

	initialized = false;
}
