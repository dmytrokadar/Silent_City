#include <iostream>
#include "cube.h"

const char* TEST_Texture = "data/texture_test_small.jpg";

void Cube::update(float elapsedTime, const glm::mat4* parentModelMatrix) {
	
	ObjectInstance::update(elapsedTime, parentModelMatrix);
}

void Cube::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3 light, const glm::vec3 lightPos, const glm::vec3 cameraPos, const glm::vec3 cameraDirection, const bool isFog)
{
	/*position = countPositionOnCurve(tGlobal);
	tGlobal += 0.05f;*/


	if (initialized && (shaderProgram != nullptr)) {
		CHECK_GL_ERROR();
		glUseProgram(shaderProgram->program);

		glm::mat4 model = glm::translate(globalModelMatrix, position);
		glUniformMatrix4fv(shaderProgram->locations.PVMmatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix * viewMatrix * model));
		CHECK_GL_ERROR();

		glUniformMatrix4fv(shaderProgram->locations.model, 1, GL_FALSE, glm::value_ptr(model));
		CHECK_GL_ERROR();
		//flashlight
		glUniform1f(shaderProgram->locations.flashlightAngle, glm::radians(20.0f));
		CHECK_GL_ERROR();
		//TODO change to another color than light
		glUniform3f(shaderProgram->locations.flashlightColor, light.x, light.y, light.z);
		CHECK_GL_ERROR();
		//glUniform3f(shaderProgram->locations.flashlightPos, lightPos.x, lightPos.y, lightPos.z);

		glUniform3f(shaderProgram->locations.light, light.x, light.y, light.z);
		CHECK_GL_ERROR();
		glUniform3f(shaderProgram->locations.lightPos, lightPos.x, lightPos.y, lightPos.z);
		CHECK_GL_ERROR();
		glUniform3f(shaderProgram->locations.cameraPos, cameraPos.x, cameraPos.y, cameraPos.z);
		CHECK_GL_ERROR();
		glUniform3f(shaderProgram->locations.cameraDirection, cameraDirection.x, cameraDirection.y, cameraDirection.z);
		CHECK_GL_ERROR();
		glUniform1i(shaderProgram->locations.isFog, isFog);
		CHECK_GL_ERROR();

		glActiveTexture(GL_TEXTURE0);
		CHECK_GL_ERROR();
		glBindTexture(GL_TEXTURE_2D, geometry->texture);
		CHECK_GL_ERROR();
		glBindVertexArray(geometry->vertexArrayObject);
		CHECK_GL_ERROR();
		glDrawArrays(GL_TRIANGLES, 0, geometry->numTriangles*3);
		CHECK_GL_ERROR();
		glBindVertexArray(0);
	}
	else {
		std::cerr << "Cube::draw(): Can't draw, cube not initialized properly!" << std::endl;
	}
}

Cube::Cube(ShaderProgram* shdrPrg, const glm::vec3 pos, std::string objName) : ObjectInstance(shdrPrg), initialized(false)
{
	geometry = new ObjectGeometry;

	position = pos;

	makeCurve();

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

	loadObjFromFile(objName);

	geometry->texture = pgr::createTexture(TEST_Texture);
	if (geometry->texture == 0) {
		std::cout << "Texture not loaded!" << std::endl;
	}

	//geometry->numTriangles = geometry->vertices.size();
	std::cout << geometry->numTriangles << std::endl;
	geometry->elementBufferObject = 0;

	glGenVertexArrays(1, &geometry->vertexArrayObject);
	glBindVertexArray(geometry->vertexArrayObject);

	glGenBuffers(1, &geometry->vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBufferObject);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->elementBufferObject);
	//&(geometry->vertices[0])
	glBufferData(GL_ARRAY_BUFFER, geometry->vertices.size() * sizeof(float), (vertices), GL_STATIC_DRAW);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, geometry->indexes.size() * sizeof(unsigned int), &(geometry->indexes[0]), GL_STATIC_DRAW);

	if ((shaderProgram != nullptr) && shaderProgram->initialized && (shaderProgram->locations.position != -1) && (shaderProgram->locations.PVMmatrix != -1)) {
		glEnableVertexAttribArray(shaderProgram->locations.position);
		glVertexAttribPointer(shaderProgram->locations.position, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		
		glEnableVertexAttribArray(shaderProgram->locations.normals);
		glVertexAttribPointer(shaderProgram->locations.normals, 3, GL_FLOAT, GL_FALSE, 0, (void*)(geometry->numTriangles * 3 * 3 * sizeof(float)));

		glEnableVertexAttribArray(shaderProgram->locations.texture);
		glVertexAttribPointer(shaderProgram->locations.texture, 2, GL_FLOAT, GL_FALSE, 0, (void*)(geometry->numTriangles *3*3*2*sizeof(float)));

		glBindTexture(GL_TEXTURE_2D, geometry->texture);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		initialized = true;
	}
	else {
		std::cerr << "Cube::Cube(): shaderProgram struct not initialized!" << std::endl;
	}
}

Cube::Cube(float t, ShaderProgram* shdrPrg, const glm::vec3 pos, std::string objName) : ObjectInstance(shdrPrg), initialized(false)
{
	geometry = new ObjectGeometry;

	position = pos;
	tGlobal = t;

	makeCurve();
	position = countPositionOnCurve(tGlobal);
	tGlobal += 0.05f;

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

	loadObjFromFile(objName);

	geometry->texture = pgr::createTexture(TEST_Texture);
	if (geometry->texture == 0) {
		std::cout << "Texture not loaded!" << std::endl;
	}

	//geometry->numTriangles = geometry->vertices.size();
	std::cout << geometry->numTriangles << std::endl;
	geometry->elementBufferObject = 0;

	glGenVertexArrays(1, &geometry->vertexArrayObject);
	glBindVertexArray(geometry->vertexArrayObject);

	glGenBuffers(1, &geometry->vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBufferObject);

	//&(geometry->vertices[0])
	glBufferData(GL_ARRAY_BUFFER, geometry->vertices.size() * sizeof(float), (vertices), GL_STATIC_DRAW);

	if ((shaderProgram != nullptr) && shaderProgram->initialized && (shaderProgram->locations.position != -1) && (shaderProgram->locations.PVMmatrix != -1)) {
		glEnableVertexAttribArray(shaderProgram->locations.position);
		glVertexAttribPointer(shaderProgram->locations.position, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(shaderProgram->locations.normals);
		glVertexAttribPointer(shaderProgram->locations.normals, 3, GL_FLOAT, GL_FALSE, 0, (void*)(geometry->numTriangles * 3 * 3 * sizeof(float)));

		glEnableVertexAttribArray(shaderProgram->locations.texture);
		glVertexAttribPointer(shaderProgram->locations.texture, 2, GL_FLOAT, GL_FALSE, 0, (void*)(geometry->numTriangles * 3 * 3 * 2 * sizeof(float)));

		glBindTexture(GL_TEXTURE_2D, geometry->texture);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		initialized = true;
	}
	else {
		std::cerr << "Cube::Cube(): shaderProgram struct not initialized!" << std::endl;
	}
}

void Cube::makeCurve() {
	// every segment has two knots and handlers, but second knot of every segment
	// is first knot of next segment, so we can store them only once
	// segnent 0
	knots.push_back(glm::vec3(-1.0000, 0.0000, 0.0000));
	handles.push_back(glm::vec3(-0.3260, 0.2139, 0.0000));
	handles.push_back(glm::vec3(0.1340, -0.5000, 0.0000));
	//knots.push_back(glm::vec3(1.0000, 0.0000, 0.0000));
	// segnent 1
	knots.push_back(glm::vec3(1.0000, 0.0000, 0.0000));
	handles.push_back(glm::vec3(1.6440, 0.3718, 0.0000));
	handles.push_back(glm::vec3(2.4522, -0.8467, 0.0000));
	//knots.push_back(glm::vec3(1.3612, -0.8741, 0.0000));
	//segnent  2
	knots.push_back(glm::vec3(1.3612, -0.8741, 0.0000));  
	handles.push_back(glm::vec3(0.4038, -0.8982, 0.0000));
	handles.push_back(glm::vec3(0.7235, -1.4217, 0.0000));
	//knots.push_back(glm::vec3(-0.3155, -1.0881, 0.0000));
	// segnent  3
	knots.push_back(glm::vec3(-0.3155, -1.0881, 0.0000));   
	handles.push_back(glm::vec3(-0.9639, -0.8799, 0.0000));
	handles.push_back(glm::vec3(-1.4255, -0.1350, 0.0000));
	// last knot is first knot
	knots.push_back(glm::vec3(-1.0000, 0.0000, 0.0000));
	//knots.push_back(glm::vec3(-1.0000, 0.0000, 0.0000));


}

glm::vec3 Cube::countPositionOnCurve(float t) {
	glm::vec3 resultPos;

	int segment = floor(t);
	if (segment >= MAX_T) {
		t -= 4;
		tGlobal = t;
		segment = 0;
	}

	t -= segment;

	/*float b1 = -pow(t, 3)  + 2 * pow(t, 2) - t;
	float b2 = 3*pow(t, 3) - 5 * pow(t, 2) + 2;
	float b3 = -3*pow(t, 3)+ 4 * pow(t, 2) + t;
	float b4 = pow(t, 3)   - pow(t, 2);*/

	std::cout << "t: " << t << std::endl;
	std::cout << "segment: " << segment << std::endl;
	std::cout << segment * 2 << " " << segment * 2 + 1 << std::endl;

	//resultPos = (b1 * handles[segment * 2]) + (b2 * knots[segment]) + (b3 * knots[segment+1]) + (b4 * handles[(segment * 2) + 1]);

	resultPos = (float)pow(1-t, 3)* knots[segment] + 3*(float)pow(1-t,2)*t*handles[segment*2] + 3*(1-t)*(float)pow(t,2)*handles[(segment * 2)+1] + (float)pow(t, 3)*knots[segment + 1];


	return resultPos*10.0f;
}

Cube::~Cube() {
	glDeleteVertexArrays(1, &(geometry->vertexArrayObject));
	glDeleteBuffers(1, &(geometry->elementBufferObject));
	glDeleteBuffers(1, &(geometry->vertexBufferObject));

	delete geometry;
	geometry = nullptr;

	initialized = false;
}
