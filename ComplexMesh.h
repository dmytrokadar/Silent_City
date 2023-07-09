#pragma once

#include "object.h"
class ComplexMesh : public ObjectInstance
{
public:
	ComplexMesh(ShaderProgram* shader, const glm::vec3& pos, const std::string& filename, const glm::vec3 sc = glm::vec3(1.0f), const float rot = glm::radians(0.0f));
	ComplexMesh(ShaderProgram* shader, const aiMesh* mesh, const aiMaterial* material, const std::string& filename, const glm::vec3& pos, const glm::vec3 sc = glm::vec3(1.0f), const float rot = glm::radians(0.0f));
	virtual ~ComplexMesh();

	void update(float elapsedTime, const glm::mat4* parentModelMatrix, glm::vec3 pos);
	//void update(float elapsedTime, const glm::mat4* parentModelMatrix) override;
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3 light = glm::vec3(1.0f), const glm::vec3 lightPos = glm::vec3(1.0f), const glm::vec3 cameraPos = glm::vec3(1.0f), const glm::vec3 cameraDirection = glm::vec3(1.0f), const bool isFog = false) override;
	void chPosition(glm::vec3 pos) {
		position = pos;
		for (auto subm : submeshes)
		{
			subm->chPosition(pos);
		}
	}
	
	void chScale(glm::vec3 sc) {
		scale = sc;
		for (auto subm : submeshes)
		{
			subm->chScale(scale);
		}
	}
private:
	bool initialized;

	glm::vec3 scale;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 shininess;
	float rotation;
	std::vector<ComplexMesh*> submeshes;
protected:
	glm::vec3 position;
	glm::vec3 direction;
	
};

