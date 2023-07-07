#include "Space.h"
#include "pgr.h"
#include <iostream>
#include <glm/gtx/quaternion.hpp>

const float radius = 3.0f;

void Sun::update(float elapsedTime, const glm::mat4* parentModelMatrix) {
	//TODO pereklasty z maina suda

	//if (sunMoving) {
	//	float sunX = sin(elapsedTime/1000.0f) * radius;
	//	float sunY = cos(elapsedTime/1000.0f) * radius;

	//	position.x = sunX;
	//	position.y = sunY;

	//	//sun->changePosition(sunPos);
	//}
	localModelMatrix = glm::translate(glm::mat4(1.0f), position);
	//globalModelMatrix = *parentModelMatrix * localModelMatrix;
	//glm::mat4 model = glm::translate(globalModelMatrix, position);
	ObjectInstance::update(elapsedTime, parentModelMatrix);
	/*for (Planet* child : planets) {
		if (child != nullptr)
			child->update(elapsedTime, &globalModelMatrix);
	}*/
}

void Sun::changePosition(const glm::vec3 pos) {
	position = pos;
}

void Sun::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3 cameraPos, const glm::vec3 cameraDirection, const bool isFog)
{
	if (initialized && (shaderProgram != nullptr)) {
		glUseProgram(shaderProgram->program);

		//glm::mat4 model = glm::translate(globalModelMatrix, position);

		glUniformMatrix4fv(shaderProgram->locations.PVMmatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix * viewMatrix * globalModelMatrix));

		glBindVertexArray(geometry->vertexArrayObject);
		glDrawArrays(GL_TRIANGLES, 0, geometry->numTriangles * 3);
		glBindVertexArray(0);

		for (ObjectInstance* child : children) {   //for (auto child : children) {
			if (child != nullptr)
				child->draw(viewMatrix, projectionMatrix, glm::vec3(1.0f), position, cameraPos, cameraDirection, isFog);
		}
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

		/*glEnableVertexAttribArray(shaderProgram->locations.normals);
		glVertexAttribPointer(shaderProgram->locations.normals, 3, GL_FLOAT, GL_FALSE, 0, (void*)(12 * 3 * 3 * sizeof(float)));

		glEnableVertexAttribArray(shaderProgram->locations.texture);
		glVertexAttribPointer(shaderProgram->locations.texture, 2, GL_FLOAT, GL_FALSE, 0, (void*)(12 * 3 * 3 * 2 * sizeof(float)));*/

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

void Planet::update(float elapsedTime, const glm::mat4* parentModelMatrix) {
	//if (sunMoving) {
	//float sunX = sin(elapsedTime) * radius;
	//float sunY = cos(elapsedTime) * radius;

	//position.x = sunX;
	//position.z = sunY;

		//sun->changePosition(sunPos);
	//}
	//localModelMatrix = glm::translate(glm::mat4(1.0f), position);


	//glm::mat4 tmpR = glm::rotate(glm::mat4(1.0f), glm::radians(1.5f), axis);
	//localModelMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(1.5f), axis) * localModelMatrix;
	//glm::mat4 RotationMatrix = glm::mat4_cast(rotationQuat);
	//localModelMatrix = glm::toMat4(rotationQuat) * localModelMatrix;
	//rotationQuat = glm::quat(glm::radians(1.5f), axis) * rotationQuat;
	rotationQuat = glm::angleAxis(glm::radians(angle), axis) * rotationQuat;
	glm::mat4 tmpM = glm::mat4_cast(rotationQuat);

	//ObjectInstance::update(elapsedTime, parentModelMatrix);
	if (parentModelMatrix != nullptr)
		globalModelMatrix = *parentModelMatrix * tmpM * localModelMatrix;
	else
		globalModelMatrix = tmpM * localModelMatrix;

	//globalModelMatrix = glm::translate(glm::mat4(1.0f), position) * globalModelMatrix;

	// update all children
	for (ObjectInstance* child : children) {
		if (child != nullptr) {
			//Planet* tmp = dynamic_cast<Planet*>(child);
			child->update(elapsedTime, &globalModelMatrix);
		}
	}
}

void Planet::changePosition(const glm::vec3 pos) {
	position = pos;
}

void Planet::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3 light, const glm::vec3 lightPos, const glm::vec3 cameraPos, const glm::vec3 cameraDirection, const bool isFog)
{
	if (initialized && (shaderProgram != nullptr)) {
		glUseProgram(shaderProgram->program);

		//glm::mat4 model = glm::translate(globalModelMatrix, position);

		glUniformMatrix4fv(shaderProgram->locations.PVMmatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix * viewMatrix * globalModelMatrix));

		glBindVertexArray(geometry->vertexArrayObject);
		glDrawArrays(GL_TRIANGLES, 0, geometry->numTriangles * 3);
		glBindVertexArray(0);

		for (ObjectInstance* child : children) {   //for (auto child : children) {
			if (child != nullptr)
				child->draw(viewMatrix, projectionMatrix, glm::vec3(1.0f), position, cameraPos, cameraDirection, isFog);
		}
	}
	else {
		std::cerr << "Sun::draw(): Can't draw, cube not initialized properly!" << std::endl;
	}
}

Planet::Planet(ShaderProgram* shdrPrg, const glm::vec3 pos, glm::vec3 axis, float radius, float angle) 
	: ObjectInstance(shdrPrg), initialized(false), radius(radius), angle(angle)
{
	geometry = new ObjectGeometry;
	position = pos;
	this->axis = normalize(axis);

	rotationQuat = glm::quat(glm::radians(angle), this->axis);
	//this->axis = glm::normalize(glm::cross(glm::vec3(1, 2, 3), glm::vec3(0, 1, 0)));
	glm::vec3 offset = glm::normalize(glm::cross(axis, glm::vec3(0, 1, 0)));
	localModelMatrix = glm::translate(glm::mat4(1.0f), radius * offset) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));

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

		/*glEnableVertexAttribArray(shaderProgram->locations.normals);
		glVertexAttribPointer(shaderProgram->locations.normals, 3, GL_FLOAT, GL_FALSE, 0, (void*)(12 * 3 * 3 * sizeof(float)));

		glEnableVertexAttribArray(shaderProgram->locations.texture);
		glVertexAttribPointer(shaderProgram->locations.texture, 2, GL_FLOAT, GL_FALSE, 0, (void*)(12 * 3 * 3 * 2 * sizeof(float)));*/

		//glBindTexture(GL_TEXTURE_2D, geometry->texture);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		initialized = true;
	}
	else {
		std::cerr << "Sun::Sun(): shaderProgram struct not initialized!" << std::endl;
	}
}

Planet::~Planet() {
	glDeleteVertexArrays(1, &(geometry->vertexArrayObject));
	glDeleteBuffers(1, &(geometry->elementBufferObject));
	glDeleteBuffers(1, &(geometry->vertexBufferObject));

	delete geometry;
	geometry = nullptr;

	initialized = false;
}