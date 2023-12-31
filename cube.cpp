#include <iostream>
#include "cube.h"

const char* TEST_Texture = "data/texture_test_small.jpg";

void DefaultObject::update(float elapsedTime, const glm::mat4* parentModelMatrix) {
	
	ObjectInstance::update(elapsedTime, parentModelMatrix);
}

void DefaultObject::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3 light, const glm::vec3 lightPos, const glm::vec3 cameraPos, const glm::vec3 cameraDirection, const bool isFog)
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

		glUniform3f(shaderProgram->locations.ambientM, 0.1f, 0.1f, 0.1f);
		glUniform3f(shaderProgram->locations.diffuseM, 1.0f, 1.0f, 1.0f);
		glUniform3f(shaderProgram->locations.specularM, 0.5f, 0.5f, 0.5f);
		glUniform1f(shaderProgram->locations.shininessM, 32.0f);
		//glUniform3f(shaderProgram->locations.flashlightPos, lightPos.x, lightPos.y, lightPos.z);

		/*glUniform3f(shaderProgram->locations.light, light.x, light.y, light.z);
		CHECK_GL_ERROR();
		glUniform3f(shaderProgram->locations.lightPos, lightPos.x, lightPos.y, lightPos.z);
		CHECK_GL_ERROR();
		glUniform3f(shaderProgram->locations.cameraPos, cameraPos.x, cameraPos.y, cameraPos.z);
		CHECK_GL_ERROR();
		glUniform3f(shaderProgram->locations.cameraDirection, cameraDirection.x, cameraDirection.y, cameraDirection.z);
		CHECK_GL_ERROR();
		glUniform1i(shaderProgram->locations.isFog, isFog);
		CHECK_GL_ERROR();*/

		glActiveTexture(GL_TEXTURE0);
		CHECK_GL_ERROR();
		glBindTexture(GL_TEXTURE_2D, geometry->texture);
		CHECK_GL_ERROR();
		glBindVertexArray(geometry->vertexArrayObject);
		CHECK_GL_ERROR();
		glDrawArrays(GL_TRIANGLES, 0, geometry->numTriangles*3);
		CHECK_GL_ERROR();
		glBindVertexArray(0);

		//for (ObjectInstance* child : children) {   //for (auto child : children) {
		//	if (child != nullptr)
		//		child->draw(viewMatrix, projectionMatrix, light, lightPos, cameraPos, cameraDirection, isFog);
		//}
	}
	else {
		std::cerr << "Cube::draw(): Can't draw, cube not initialized properly!" << std::endl;
	}
}

DefaultObject::DefaultObject(ShaderProgram* shdrPrg, const glm::vec3 pos, std::string objName, std::string objTexture) : ObjectInstance(shdrPrg), initialized(false), textureName(objTexture)
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

	geometry->texture = pgr::createTexture(textureName);
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
	glBufferData(GL_ARRAY_BUFFER, geometry->vertices.size() * sizeof(float), &(geometry->vertices[0]), GL_STATIC_DRAW);
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

DefaultObject::DefaultObject(float t, ShaderProgram* shdrPrg, const glm::vec3 pos, std::string objName) : ObjectInstance(shdrPrg), initialized(false)
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
	glBufferData(GL_ARRAY_BUFFER, geometry->vertices.size() * sizeof(float), &(geometry->vertices[0]), GL_STATIC_DRAW);

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

void DefaultObject::makeCurve() {
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

glm::vec3 DefaultObject::countPositionOnCurve(float t) {
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

DefaultObject::~DefaultObject() {
	glDeleteVertexArrays(1, &(geometry->vertexArrayObject));
	glDeleteBuffers(1, &(geometry->elementBufferObject));
	glDeleteBuffers(1, &(geometry->vertexBufferObject));

	delete geometry;
	geometry = nullptr;

	initialized = false;
}


void Car::update(float elapsedTime, const glm::mat4* parentModelMatrix) {
	position = countPositionOnCurve(tGlobal);
	ComplexMesh::update(position, -direction);
	//glm::mat4 model = glm::inverse(glm::lookAt(position, position + direction, glm::vec3(0.0f, 1.0f, 0.0f)));
	tGlobal += 0.01f;
	ComplexMesh::update(elapsedTime, parentModelMatrix, position);
}

void Car::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3 light, const glm::vec3 lightPos, const glm::vec3 cameraPos, const glm::vec3 cameraDirection, const bool isFog)
{
	/*position = countPositionOnCurve(tGlobal);
	tGlobal += 0.05f;*/

	//glUniform3f(shaderProgram->locations.ambientM, 0.1f, 0.1f, 0.1f);
	//glUniform3f(shaderProgram->locations.diffuseM, 1.0f, 1.0f, 1.0f);
	//glUniform3f(shaderProgram->locations.specularM, 0.8f, 0.8f, 0.8f);
	//glUniform1f(shaderProgram->locations.shininessM, 40.0f);

	//ComplexMesh::draw(viewMatrix, projectionMatrix, light, lightPos, cameraPos, cameraDirection, isFog);
	//ComplexMesh::draw(viewMatrix, projectionMatrix, light, lightPos, cameraPos, cameraDirection, isFog);	
	if (initialized && shaderProgram != nullptr) {
		glUseProgram(shaderProgram->program);
		glm::mat4 model = glm::scale(globalModelMatrix, scale);
		model = glm::inverse(glm::lookAt(position, position + direction, glm::vec3(0, 1, 0))) * model;

		//model = glm::rotate(model, rotation, glm::vec3(0.0f, 1.0f, 0.0f));

		//model = glm::translate(model, position);
		glUniformMatrix4fv(shaderProgram->locations.PVMmatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix * viewMatrix * model));
		glUniformMatrix4fv(shaderProgram->locations.model, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(shaderProgram->locations.flashlightAngle, glm::radians(20.0f));
		glUniform3f(shaderProgram->locations.flashlightColor, light.x, light.y, light.z);
		glUniform3f(shaderProgram->locations.ambientM, 0.1f, 0.1f, 0.1f);
		glUniform3f(shaderProgram->locations.diffuseM, 1.0f, 1.0f, 1.0f);
		glUniform3f(shaderProgram->locations.specularM, 0.7, 0.7f, 0.7f);
		glUniform1f(shaderProgram->locations.shininessM, 32.0f);

		glActiveTexture(GL_TEXTURE0);
		glUniform1i(shaderProgram->locations.sampl, 0);
		glBindTexture(GL_TEXTURE_2D, geometry->texture);
		glBindVertexArray(geometry->vertexArrayObject);
		glDrawElements(GL_TRIANGLES, geometry->indexes.size(), GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
	}
	else {
		std::cerr << "ComplexMesh::draw(): Can't draw, cube not initialized properly!" << std::endl;
	}

	for (auto subm : submeshes)
	{
		subm->draw(viewMatrix, projectionMatrix, light, lightPos, cameraPos, cameraDirection, isFog);
	}

	for (auto child : children)
	{
		if (child)
		{
			child->draw(viewMatrix, projectionMatrix);
		}
	}
}

Car::Car(ShaderProgram* shdrPrg, const glm::vec3 pos, std::string objName) : ComplexMesh(shdrPrg, glm::vec3(0.0f), "data/car.obj", glm::vec3(1.0f), 180.0f), initialized(false)
{
	//geometry = new ObjectGeometry;

	position = pos;

	makeCurve();
	position = countPositionOnCurve(tGlobal);
	tGlobal += 0.05f;

	initialized = true;
	//loadObjFromFile(objName);

	//geometry->texture = pgr::createTexture(TEST_Texture);
	//if (geometry->texture == 0) {
	//	std::cout << "Texture not loaded!" << std::endl;
	//}

	////geometry->numTriangles = geometry->vertices.size();
	//std::cout << geometry->numTriangles << std::endl;
	//geometry->elementBufferObject = 0;

	//glGenVertexArrays(1, &geometry->vertexArrayObject);
	//glBindVertexArray(geometry->vertexArrayObject);

	//glGenBuffers(1, &geometry->vertexBufferObject);
	//glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBufferObject);

	//glBufferData(GL_ARRAY_BUFFER, geometry->vertices.size() * sizeof(float), &(geometry->vertices[0]), GL_STATIC_DRAW);

	//if ((shaderProgram != nullptr) && shaderProgram->initialized && (shaderProgram->locations.position != -1) && (shaderProgram->locations.PVMmatrix != -1)) {
	//	glEnableVertexAttribArray(shaderProgram->locations.position);
	//	glVertexAttribPointer(shaderProgram->locations.position, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//	glEnableVertexAttribArray(shaderProgram->locations.normals);
	//	glVertexAttribPointer(shaderProgram->locations.normals, 3, GL_FLOAT, GL_FALSE, 0, (void*)(geometry->numTriangles * 3 * 3 * sizeof(float)));

	//	glEnableVertexAttribArray(shaderProgram->locations.texture);
	//	glVertexAttribPointer(shaderProgram->locations.texture, 2, GL_FLOAT, GL_FALSE, 0, (void*)(geometry->numTriangles * 3 * 3 * 2 * sizeof(float)));

	//	glBindTexture(GL_TEXTURE_2D, geometry->texture);
	//	initialized = true;
	//}
	//else {
	//	std::cerr << "Cube::Cube(): shaderProgram struct not initialized!" << std::endl;
	//}
}

void Car::makeCurve() {
	// every segment has two knots and handlers, but second knot of every segment
	// is first knot of next segment, so we can store them only once
	// segnent 0
	knots.push_back(glm::vec3(-1.0000, 0.0000, 0.0000));
	handles.push_back(glm::vec3(-0.3260, 0.0000, 0.2139));
	handles.push_back(glm::vec3(0.1340, 0.0000, -0.5000));
	//knots.push_back(glm::vec3(1.0000, 0.0000, 0.0000));
	// segnent 1
	knots.push_back(glm::vec3(1.0000, 0.0000, 0.0000));
	handles.push_back(glm::vec3(1.6440, 0.0000, 0.3718));
	handles.push_back(glm::vec3(2.4522, 0.0000, -0.8467));
	//knots.push_back(glm::vec3(1.3612, -0.8741, 0.0000));
	//segnent  2
	knots.push_back(glm::vec3(1.3612, 0.0000, -0.8741));
	handles.push_back(glm::vec3(0.4038, 0.0000, -0.8982));
	handles.push_back(glm::vec3(0.7235, 0.0000, -1.4217));
	//knots.push_back(glm::vec3(-0.3155, -1.0881, 0.0000));
	// segnent  3
	knots.push_back(glm::vec3(-0.3155, 0.0000, -1.0881));
	handles.push_back(glm::vec3(-0.9639, 0.0000, -0.8799));
	handles.push_back(glm::vec3(-1.4255, 0.0000, -0.1350));
	// last knot is first knot
	knots.push_back(glm::vec3(-1.0000, 0.0000, 0.0000));
	//knots.push_back(glm::vec3(-1.0000, 0.0000, 0.0000));


}

// count position with given t value on curve segment
glm::vec3 Car::countPositionOnCurve(float t) {
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

	//std::cout << "t: " << t << std::endl;
	//std::cout << "segment: " << segment << std::endl;
	//std::cout << segment * 2 << " " << segment * 2 + 1 << std::endl;

	//resultPos = (b1 * handles[segment * 2]) + (b2 * knots[segment]) + (b3 * knots[segment+1]) + (b4 * handles[(segment * 2) + 1]);

	resultPos = (float)pow(1 - t, 3) * knots[segment] + 3 * (float)pow(1 - t, 2) * t * handles[segment * 2] + 3 * (1 - t) * (float)pow(t, 2) * handles[(segment * 2) + 1] + (float)pow(t, 3) * knots[segment + 1];
	countDirectionOnCurve(t, segment);

	return resultPos * 10.0f;
}

// count direction to face while moving on curve
glm::vec3 Car::countDirectionOnCurve(float t, int segment) {
	glm::vec3 resultDir;
	//resultPos = (b1 * handles[segment * 2]) + (b2 * knots[segment]) + (b3 * knots[segment+1]) + (b4 * handles[(segment * 2) + 1]);

	resultDir = 3*(float)pow(1 - t, 2) * (handles[segment * 2] - knots[segment]) + 6 * (1 - t) * t * (handles[(segment * 2) + 1] - handles[segment * 2]) + 3 * (float)pow(t, 2) * (knots[segment + 1] - handles[(segment * 2) + 1]);
	direction = resultDir;

	return resultDir * 10.0f;
}

glm::vec3 Car::getCarPos() {
	return position;
}

glm::vec3 Car::getCarLook() {
	return direction;
}

Car::~Car() {
	glDeleteVertexArrays(1, &(geometry->vertexArrayObject));
	glDeleteBuffers(1, &(geometry->elementBufferObject));
	glDeleteBuffers(1, &(geometry->vertexBufferObject));

	delete geometry;
	geometry = nullptr;

	initialized = false;
}
