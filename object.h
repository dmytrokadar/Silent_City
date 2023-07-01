#pragma once

#include "pgr.h"
#include <fstream>
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
		GLint projection;
		GLint sampl;
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
		//material uniform
		GLint ambient;
		GLint diffuse;
		GLint specular;
		GLint shininess;

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
	GLuint        texture;				///< texture id
	std::vector<float> vertices;
	std::vector<unsigned int> indexes;

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

	virtual void loadObjFromFileAssimp(std::string path) {
		Assimp::Importer importer;

		importer.SetPropertyInteger(AI_CONFIG_PP_PTV_NORMALIZE, 1);
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_TransformUVCoords | 0);


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
		}
		/*for (float num : geometry->vertices) {
			std::cout << num << std::endl;
		}*/
		for (glm::vec3 normal : normals) {
			geometry->vertices.push_back(normal.x);
			geometry->vertices.push_back(normal.y);
			geometry->vertices.push_back(normal.z);
		}
		/*for (float num : geometry->vertices) {
			std::cout << num << std::endl;
		}*/
		for (glm::vec2 tex : textures) {
			geometry->vertices.push_back(tex.x);
			geometry->vertices.push_back(tex.y);
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