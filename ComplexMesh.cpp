#include "ComplexMesh.h"

void ComplexMesh::update(float elapsedTime, const glm::mat4* parentModelMatrix, glm::vec3 pos)
{
	//position = pos;
	//for (auto subm : submeshes)
	//{
	//	subm->update(elapsedTime, parentModelMatrix, position);
	//}
	ObjectInstance::update(elapsedTime, parentModelMatrix);
}

void ComplexMesh::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3 light, const glm::vec3 lightPos, const glm::vec3 cameraPos, const glm::vec3 cameraDirection, const bool isFog)
{
	if (initialized && shaderProgram != nullptr) {
		glUseProgram(shaderProgram->program);
		glm::mat4 model = glm::scale(globalModelMatrix, scale);
		model = glm::rotate(model, rotation, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, position);
		glUniformMatrix4fv(shaderProgram->locations.PVMmatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix * viewMatrix * model));
		glUniformMatrix4fv(shaderProgram->locations.model, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(shaderProgram->locations.flashlightAngle, glm::radians(20.0f));
		glUniform3f(shaderProgram->locations.flashlightColor, light.x, light.y, light.z);
		glUniform3f(shaderProgram->locations.ambientM, 0.1f, 0.1f, 0.1f);
		glUniform3f(shaderProgram->locations.diffuseM, 1.0f, 1.0f, 1.0f);
		glUniform3f(shaderProgram->locations.specularM, 0.5f, 0.5f, 0.5f);
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

ComplexMesh::ComplexMesh(ShaderProgram* shader, const glm::vec3& pos, const std::string& filename, const glm::vec3 sc, const float rot)
	: ObjectInstance(shader), position(pos), scale(sc)
{

	Assimp::Importer importer;

	importer.SetPropertyInteger(AI_CONFIG_PP_PTV_NORMALIZE, 1);

	const aiScene* scene = importer.ReadFile(filename.c_str(),
		aiProcess_Triangulate |
		//aiProcess_PreTransformVertices |
		aiProcess_GenSmoothNormals |
		aiProcess_JoinIdenticalVertices |
		aiProcess_TransformUVCoords |
		0);

	assert(scene != nullptr);
	assert(scene->mNumMeshes > 0);
	//if (scene == nullptr) 
	//{
	//	std::cerr << importer.GetErrorString() << std::endl;
	//}

	//if (scene->mNumMeshes < 1) 
	//{
	//	std::cerr << "No meshes in scene " << filename << std::endl;
	//}

	auto m = scene->mMeshes[0];
	const aiMaterial* material = scene->mMaterials[m->mMaterialIndex];

	//loadObjFromFileAssimp(m, material, filename);
	//ComplexMesh::ComplexMesh(shader, m, material, filename, pos);
	geometry = new ObjectGeometry();
	loadObjFromFileAssimp(m, material, filename);
	glGenVertexArrays(1, &geometry->vertexArrayObject);
	glBindVertexArray(geometry->vertexArrayObject);

	glGenBuffers(1, &geometry->vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBufferObject);

	glGenBuffers(1, &geometry->elementBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->elementBufferObject);

	glBufferData(GL_ARRAY_BUFFER, geometry->vertices.size() * sizeof(float), geometry->vertices.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, geometry->indexes.size() * sizeof(unsigned int), geometry->indexes.data(), GL_STATIC_DRAW);

	if ((shader != nullptr) && shader->initialized && (shader->locations.position != -1) && (shader->locations.PVMmatrix != -1))
	{
		glEnableVertexAttribArray(shaderProgram->locations.position);
		glVertexAttribPointer(shaderProgram->locations.position, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(shaderProgram->locations.normals);
		glVertexAttribPointer(shaderProgram->locations.normals, 3, GL_FLOAT, GL_FALSE, 0, (void*)(geometry->numVertices * 3 * sizeof(float)));

		glEnableVertexAttribArray(shaderProgram->locations.texture);
		glVertexAttribPointer(shaderProgram->locations.texture, 2, GL_FLOAT, GL_FALSE, 0, (void*)(geometry->numVertices * 3 * 2 * sizeof(float)));
	}
	else
	{
		std::cerr << "Shader not inilialized" << std::endl;
	}

	glBindVertexArray(0);
	//localModelMatrix = glm::scale(glm::mat4(1.0f), scale);

	for (int mi = 1; mi < scene->mNumMeshes; ++mi)
	{
		m = scene->mMeshes[mi];
		material = scene->mMaterials[m->mMaterialIndex];

		submeshes.push_back(new ComplexMesh(shader, m, material, filename, pos, scale, rot));
	}

	initialized = true;
}

ComplexMesh::ComplexMesh(ShaderProgram* shader, const aiMesh* mesh, const aiMaterial* material, const std::string& filename, const glm::vec3& pos, const glm::vec3 sc, const float rot)
	: ObjectInstance(shader), position(pos), scale(sc), rotation(rot)
{
	geometry = new ObjectGeometry();

	loadObjFromFileAssimp(mesh, material, filename);

	glGenVertexArrays(1, &geometry->vertexArrayObject);
	glBindVertexArray(geometry->vertexArrayObject);

	glGenBuffers(1, &geometry->vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBufferObject);

	glGenBuffers(1, &geometry->elementBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->elementBufferObject);

	glBufferData(GL_ARRAY_BUFFER, geometry->vertices.size() * sizeof(float), &(geometry->vertices[0]), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, geometry->indexes.size() * sizeof(unsigned int), &(geometry->indexes[0]), GL_STATIC_DRAW);

	if ((shader != nullptr) && shader->initialized && (shader->locations.position != -1) && (shader->locations.PVMmatrix != -1))
	{
		glEnableVertexAttribArray(shaderProgram->locations.position);
		glVertexAttribPointer(shaderProgram->locations.position, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(shaderProgram->locations.normals);
		glVertexAttribPointer(shaderProgram->locations.normals, 3, GL_FLOAT, GL_FALSE, 0, (void*)(geometry->numVertices * 3 * sizeof(float)));

		glEnableVertexAttribArray(shaderProgram->locations.texture);
		glVertexAttribPointer(shaderProgram->locations.texture, 2, GL_FLOAT, GL_FALSE, 0, (void*)(geometry->numVertices * 3 * 2 * sizeof(float)));
	}
	else
	{
		std::cerr << "Shader not inilialized" << std::endl;
	}

	glBindVertexArray(0);
	//localModelMatrix = glm::scale(glm::mat4(1.0f), scale);

	initialized = true;
}

ComplexMesh::~ComplexMesh()
{
}
