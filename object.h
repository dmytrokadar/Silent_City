#pragma once

#include "pgr.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>  

/**
 * \brief Shader program related stuff (id, locations, ...).
 */
typedef struct _ShaderProgram {
	/// identifier for the shader program
	GLuint program;

	bool initialized;

	/**
	  * \brief Indices of the vertex shader inputs (locations)
	  */
	struct {
		// vertex attributes locations
		GLint position;
		GLint texture;
		GLint normals;
		GLint color;
		// uniforms locations
		GLint PVMmatrix;
		GLint model;
		GLint time;
		GLint projection;
		GLint sampl;
		GLint segment;
		// transformation matrix for banner
		GLint trans;
		GLint alphaChannel;
		//flashlight uniforms
		GLint flashlightColor;
		GLint flashlightPos;
		GLint flashlightAngle;
		//light unitforms
		GLint light;
		GLint lightPos;
		GLint dirLightVec;
		GLint dirLightColor;
		GLint textColor;
		GLint cameraPos;
		GLint cameraDirection;
		GLint isFog;
		GLint fogHeight;
		//lights toggle
		GLint isFlashlight;
		GLint isDirLight;
		GLint isPointLight;
		//material uniform
		GLint ambientM;
		GLint diffuseM;
		GLint specularM;
		GLint shininessM;
		//light uniform
		GLint ambientL;
		GLint diffuseL;
		GLint specularL;

		GLint constant;
		GLint linear;
		GLint quadratic;


	} locations;

	// ...

	_ShaderProgram() : program(0), initialized(false) {
		locations.position = -1;
		locations.PVMmatrix = -1;
	}

} ShaderProgram;

/**
 * \brief Geometry of an object (vertices, triangles).
 */
typedef struct _ObjectGeometry {
	GLuint        vertexBufferObject;   ///< identifier for the vertex buffer object
	GLuint        elementBufferObject;  ///< identifier for the element buffer object
	GLuint        vertexArrayObject;    ///< identifier for the vertex array object
	unsigned int  numTriangles;         ///< number of triangles in the mesh
	unsigned int  numVertices;         ///< number of vertices in the mesh
	GLuint        texture;				///< texture id
	std::vector<float> vertices;		///< vertices of the object
	std::vector<unsigned int> indexes;	///< indexes of the object

	glm::vec3 ambient = glm::vec3(0.2f); ///< ambient of the object
	glm::vec3 diffuse = glm::vec3(1.0f); ///< diffuse of the object
	glm::vec3 specular = glm::vec3(0.8f); ///< specular of the object
	float shininess = 32.0f;              ///< shininess of the object

	// ...
} ObjectGeometry;

class ObjectInstance;
/**
 * \brief Linear representation of the scene objects.  The objects themselves may represent the subtrees.
 */
typedef std::vector<ObjectInstance*> ObjectList;  

class ObjectInstance {

protected:

	ObjectGeometry* geometry;
	glm::mat4		localModelMatrix;
	glm::mat4		globalModelMatrix;

	// dynamic objects
	// glm::vec3 direction;
	// float     speed;
	// ...

	ShaderProgram* shaderProgram;

	ObjectList children;

public:

	/**
	 * \brief ObjectInstance constructor. Takes a pointer to the shader and must create object resources (VBO and VAO)
	 * \param shdrPrg pointer to the shader program for rendering objects data
	 */
	ObjectInstance(ShaderProgram* shdrPrg = nullptr) : geometry(nullptr), shaderProgram(shdrPrg) {}
	~ObjectInstance() {}
  
	virtual void addChildren(ObjectInstance *oi) {
		children.push_back(oi);
	}

	/**
	* \brief Recalculates the global matrix and updates all children.
	*   Derived classes should also call this method (using ObjectInstance::update()).
	* \param elapsedTime time value in seconds, such as 0.001*glutGet(GLUT_ELAPSED_TIME) (conversion milliseconds => seconds)
	* \param parentModelMatrix parent transformation in the scene-graph subtree
	*/
	virtual void update(const float elapsedTime, const glm::mat4* parentModelMatrix) {
		// update model matrix - localModelMatrix - of the instance 
		// ...

		// if we have parent, multiply parent's matrix with ours
		if (parentModelMatrix != nullptr)
			globalModelMatrix = *parentModelMatrix * localModelMatrix;
		else
			globalModelMatrix = localModelMatrix;

		// update all children
		for (ObjectInstance* child : children) {
			if (child != nullptr)
				child->update(elapsedTime, &globalModelMatrix);
		}
	}

	/**
	 * \brief Draw instance geometry and calls the draw() on child nodes.
	 */
	virtual void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
		// draw instance geometry using globalModelMatrix
		// ...

		// process all children
		for (ObjectInstance* child : children) {   //for (auto child : children) {
			if (child != nullptr)
				child->draw(viewMatrix, projectionMatrix);
		}
	}

	virtual void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3 light, const glm::vec3 lightPos, const glm::vec3 cameraPos, const glm::vec3 cameraDirection, const bool isFog) {
		// draw instance geometry using globalModelMatrix
		// ...

		// process all children
		for (ObjectInstance* child : children) {   //for (auto child : children) {
			if (child != nullptr)
				child->draw(viewMatrix, projectionMatrix, light, lightPos, cameraPos, cameraDirection, isFog);
		}
	}

	virtual void loadObjFromFileAssimp(const aiMesh* mesh, const aiMaterial* material, const std::string& file)
	{
		std::vector<float> vertices;
		std::vector<float> normals;
		std::vector<float> tex;

		vertices.reserve(mesh->mNumVertices * 3);
		for (int vi = 0; vi < mesh->mNumVertices; ++vi)
		{
			auto v = mesh->mVertices[vi];
			vertices.push_back(v.x);
			vertices.push_back(v.y);
			vertices.push_back(v.z);
		}

		if (mesh->mNormals)
		{
			normals.reserve(mesh->mNumVertices * 3);
			for (int ni = 0; ni < mesh->mNumVertices; ++ni)
			{
				auto n = mesh->mNormals[ni];
				normals.push_back(n.x);
				normals.push_back(n.y);
				normals.push_back(n.z);
			}
		}

		if (mesh->mTextureCoords[0])
		{
			tex.reserve(mesh->mNumVertices * 2);
			for (int ti = 0; ti < mesh->mNumVertices; ++ti)
			{
				auto t = mesh->mTextureCoords[0][ti];
				tex.push_back(t.x);
				tex.push_back(t.y);
			}
		}

		geometry->indexes.resize(mesh->mNumFaces * 3);
		for (int i = 0; i < mesh->mNumFaces; ++i)
		{
			memcpy(geometry->indexes.data() + i * 3, mesh->mFaces[i].mIndices, 3 * sizeof(unsigned int));
		}

		geometry->numTriangles = mesh->mNumFaces;
		geometry->numVertices = mesh->mNumVertices;

		int off = 0;
		geometry->vertices.resize(vertices.size() + normals.size() + tex.size());
		memcpy(geometry->vertices.data() + off, vertices.data(), vertices.size() * sizeof(float));
		off += vertices.size();
		memcpy(geometry->vertices.data() + off, normals.data(), normals.size() * sizeof(float));
		off += normals.size();
		memcpy(geometry->vertices.data() + off, tex.data(), tex.size() * sizeof(float));

		if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString texPath;
			aiReturn texFound = material->GetTexture(aiTextureType_DIFFUSE, 0, &texPath);

			size_t found = file.find_last_of("/\\");
			std::string textureName = file.substr(0, found + 1) + texPath.data;
			geometry->texture = pgr::createTexture(textureName);
		}
		else {
			//geometry->texture = pgr::createTexture();
		}

		aiColor4D color;

		//glm::vec3 ambient(0.0f);
		//glm::vec3 diffuse(0.0f);
		//glm::vec3 specular(0.0f);

		if (aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &color) == AI_SUCCESS)
		{
			geometry->ambient.r = color.r;
			geometry->ambient.g = color.g;
			geometry->ambient.b = color.b;
		}
		if (aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &color) == AI_SUCCESS)
		{
			geometry->diffuse.r = color.r;
			geometry->diffuse.g = color.g;
			geometry->diffuse.b = color.b;
		}
		if (aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &color) == AI_SUCCESS)
		{
			geometry->specular.r = color.r;
			geometry->specular.g = color.g;
			geometry->specular.b = color.b;
		}

		ai_real shine = 1.0f, strength = 1.0f;
		uint32_t max;
		max = 1;
		if (aiGetMaterialFloatArray(material, AI_MATKEY_SHININESS, &shine, &max) != AI_SUCCESS)
			shine = 1.0f;
		max = 1;
		if (aiGetMaterialFloatArray(material, AI_MATKEY_SHININESS_STRENGTH, &strength, &max) != AI_SUCCESS)
			strength = 1.0f;

		geometry->shininess = shine * strength;
	}

	virtual void loadObjFromFileAssimp(std::string path) {
		Assimp::Importer importer;

		importer.SetPropertyInteger(AI_CONFIG_PP_PTV_NORMALIZE, 1);
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_TransformUVCoords);

		assert(scene != nullptr);
		assert(scene->mNumMeshes > 0);

		int numVert = 0;
		int numFace = 0;

		for (int i = 0; i < 1; ++i)
		{
			numVert += scene->mMeshes[i]->mNumVertices;
			numFace += scene->mMeshes[i]->mNumFaces;
		}

		//geometry->vertices.reserve(numVert);
		//geometry->vertices.resize(numVert);

		std::vector<glm::vec3> vert;
		std::vector<glm::vec3> norm;
		std::vector<glm::vec2> tex;

		//vert.resize(numVert);
		//norm.resize(numVert);
		//tex.resize(numVert);
		vert.reserve(numVert);
		norm.reserve(numVert);
		tex.reserve(numVert);

		geometry->indexes.resize(numFace * 3);
		geometry->numTriangles = numFace;
		geometry->numVertices = numVert;

		size_t offset = 0;
		for (int mi = 0; mi < 1; ++mi)
		{
			auto m = scene->mMeshes[mi];

			for (int i = 0; i < m->mNumVertices; ++i)
			{
				auto v = m->mVertices[i];
				vert.emplace_back(v.x, v.y, v.z);
			}

			if (m->mNormals)
			{
				for (int i = 0; i < m->mNumVertices; ++i)
				{
					auto vn = m->mNormals[i];
					norm.emplace_back(vn.x, vn.y, vn.z);
				}
			}

			if (m->mTextureCoords[0])
			{
				for (int i = 0; i < m->mNumVertices; ++i)
				{
					auto vt = m->mTextureCoords[0][i];
					tex.emplace_back(vt.x, vt.y);
				}
			}

			for (int i = 0; i < m->mNumFaces; ++i)
			{
				auto f = m->mFaces[i];
				memcpy(geometry->indexes.data() + offset + i * 3, m->mFaces[i].mIndices, 3 * sizeof(unsigned int));
			}
			offset += m->mNumFaces * 3;
		}

		std::string texName;
		auto mat = scene->mMaterials[scene->mMeshes[0]->mMaterialIndex];
		if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString texPath;
			aiReturn texFound = mat->GetTexture(aiTextureType_DIFFUSE, 0, &texPath); // AI_SUCCESS

			size_t found = path.find_last_of("/\\");
			texName = path.substr(0, found + 1) + texPath.data;

			geometry->texture = pgr::createTexture(texName);
		}
		else
		{
			geometry->texture = pgr::createTexture("data/texture_test_small.jpg");
		}

		geometry->vertices.resize(vert.size() * 3 + norm.size() * 3 + tex.size() * 2);
		memcpy(geometry->vertices.data(), vert.data(), vert.size() * sizeof(float) * 3);
		int off = vert.size() * 3;
		memcpy(geometry->vertices.data() + off, norm.data(), norm.size() * sizeof(float) * 3);
		off += norm.size() * 3;
		memcpy(geometry->vertices.data() + off, tex.data(), tex.size() * sizeof(float) * 2);

	}

	virtual void loadObjFromFile(std::string path) {
		std::ifstream readObj(path);
		std::string str, lineFirstWord, lineNumbers;

		std::string vertexSign = "v";
		std::string textureSign = "vt";
		std::string normalSign = "vn";
		std::string faceSign = "f";

		std::vector<glm::vec3> vertexesSoup;
		std::vector<glm::vec2> texturesSoup;
		std::vector<glm::vec3> normalsSoup;
		
		std::vector<glm::vec3> vertexes;
		std::vector<glm::vec2> textures;
		std::vector<glm::vec3> normals;

		int facesNum = 0;

		if (readObj.fail()) {
			std::cout << "Can't open file " << path << std::endl;
			return;
		}

		// reading file, extracting values
		while (getline(readObj, str))
		{
			std::istringstream numbers(str);

			//lineFirstWord = str.substr(0, str.find(" "));
			numbers >> lineFirstWord;

			if (lineFirstWord == vertexSign) {
				glm::vec3 vert;
				numbers >> vert.x >> vert.y >> vert.z;
				vertexesSoup.push_back(vert);
			}
			if (lineFirstWord == textureSign) {
				glm::vec2 texture;
				numbers >> texture.x >> texture.y;
				texturesSoup.push_back(texture);
			}
			if (lineFirstWord == normalSign) {
				glm::vec3 normal;
				numbers >> normal.x >> normal.y >> normal.z;
				normalsSoup.push_back(normal);
			}
			if (lineFirstWord == faceSign) {
				/*std::vector<int> faceVertexes;
				std::vector<int> faceTextures;
				std::vector<int> faceNormals;*/

				facesNum++;
				
				while (!numbers.eof()) {
					std::string v1;
					int v, t, n;
					char trash;

					numbers >> v1;

					std::stringstream ss(v1);

					ss >> v >> trash >> t >> trash >> n;

					//std::cout << v << " " << t << " " << n << std::endl;
					vertexes.push_back(vertexesSoup[v - 1]);
					textures.push_back(texturesSoup[t - 1]);
					normals.push_back(normalsSoup[n - 1]);
				}
			}
		}
		
		// printing coordinates to check if it was parsed
		//int count = 0;
		//for (glm::vec3 vert : vertexes) {
		//	std::cout << vert.x << " " << vert.y << " " << vert.z << std::endl;
		//	count++;
		//	if (count == 3) {
		//		count = 0;
		//		std::cout << std::endl;
		//	}
		//}

		//count = 0;
		//for (glm::vec2 vert : textures) {
		//	std::cout << vert.x << " " << vert.y << " " << std::endl;
		//	count++;
		//	if (count == 3) {
		//		count = 0;
		//		std::cout << std::endl;
		//	}
		//}

		//count = 0;
		//for (glm::vec3 vert : vertexes) {
		//	std::cout << vert.x << " " << vert.y << " " << vert.z << std::endl;
		//	count++;
		//	if (count == 3) {
		//		count = 0;
		//		std::cout << std::endl;
		//	}
		//}

		readObj.close();

		// adding values to object matrix
		for (glm::vec3 vert : vertexes) {
			geometry->vertices.push_back(vert.x);
			geometry->vertices.push_back(vert.y);
			geometry->vertices.push_back(vert.z);
			//std::cout << vert.x << ", " << vert.y << ", " << vert.z << ", " << std::endl;
		}
		/*for (float num : geometry->vertices) {
			std::cout << num << std::endl;
		}*/
		for (glm::vec3 normal : normals) {
			geometry->vertices.push_back(normal.x);
			geometry->vertices.push_back(normal.y);
			geometry->vertices.push_back(normal.z);
			//std::cout << normal.x << ", " << normal.y << ", " << normal.z << ", " << std::endl;
		}
		for (glm::vec2 tex : textures) {
			geometry->vertices.push_back(tex.x);
			geometry->vertices.push_back(tex.y);
			//std::cout << tex.x << ", " << tex.y << ", " << std::endl;
		}


		geometry->numTriangles = facesNum;
	}

};

class Skybox : ObjectInstance
{
public:
	Skybox(ShaderProgram* shdrPrg = nullptr) {

	}
	~Skybox() {}

private:

};