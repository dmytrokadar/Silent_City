#pragma once

#include "object.h"
class ComplexMesh : public ObjectInstance
{
public:
	// load mesh from file
	ComplexMesh(ShaderProgram* shader, const glm::vec3& pos, const std::string& filename, const glm::vec3 sc = glm::vec3(1.0f), const float rot = glm::radians(0.0f));
	// load mesh from mesh file
	ComplexMesh(ShaderProgram* shader, const aiMesh* mesh, const aiMaterial* material, const std::string& filename, const glm::vec3& pos, const glm::vec3 sc = glm::vec3(1.0f), const float rot = glm::radians(0.0f));
	virtual ~ComplexMesh();

	//update mesh
	void update(float elapsedTime, const glm::mat4* parentModelMatrix, glm::vec3 pos);
	// update mesh with position and direction
	void update(glm::vec3 pos, glm::vec3 dir);
	//void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	// draw mesh
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3 light = glm::vec3(1.0f), const glm::vec3 lightPos = glm::vec3(1.0f), const glm::vec3 cameraPos = glm::vec3(1.0f), const glm::vec3 cameraDirection = glm::vec3(1.0f), const bool isFog = false) override;
	// change position of the mesh
	void chPosition(glm::vec3 pos) {
		position = pos;
		for (auto subm : submeshes)
		{
			subm->chPosition(pos);
		}
	}
	
	// change scale of the mesh
	void chScale(glm::vec3 sc) {
		scale = sc;
		for (auto subm : submeshes)
		{
			subm->chScale(scale);
		}
	}
protected:
	bool initialized;

	glm::vec3 scale; // scale of the object
	float rotation; // object rotation angle on Y-axis
	std::vector<ComplexMesh*> submeshes; // submeshes of the mesh
	glm::vec3 position; // position of the object
	glm::vec3 direction; // direction of the object
	
};

