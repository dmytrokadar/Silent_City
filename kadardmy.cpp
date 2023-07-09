//----------------------------------------------------------------------------------------
/**
 * \file    skeleton.cpp : This file contains the 'main' function and callbacks.
			Program execution begins and ends there.
 * \author  Jaroslav Sloup, Petr Felkel
 * \date    2022/03/03
 * \brief   Prepared for the Computer graphics course on FEE and FIT CTU in Prague CZ
 */

 /**
  * \brief	\mainpage Documentation of the skeleton for the Computer graphics course on FEE and FIT CTU CZ
  *
  * This is the skeleton of an OpenGL application written in FreeGLUT and PGR framework.
  * It serves as a starter boiler-plate code for a semester project - a simple interactive application.
  * Write your code into the functions prepared. Add your data (models, textures, ...) and logic.
  *
  *
  * Comment your code using the [doxygen](https://www.doxygen.nl/index.html) documenting system style.
  * Create "doxygen" directory, make it current by "cd doxygen", prepare a configuration file with "doxygen -g" and edit the details.
  *
  * Start by renaming of this file from skeleton.cpp to <your_name>.cpp and the project to <your_name>.vcxproj
  *
  * In <your_name>.vcxproj:
  *   - rename skeleton in <RootNamespace>skeleton</RootNamespace> to <your_name>
  *   - replace skeleton.cpp in <ClCompile Include="skeleton.cpp" /> to <your_name>.cpp
  *
  * Start coding and documenting. Generate the documentation by the command "doxygen" in the "doxygen" directory.
  *
  */


  // TODO: tabulka klaves a jeji obsluha keyPressed/keyReleased a timer


#include <iostream>
#include "pgr.h"
#include "object.h"
#include "triangle.h"
#include "cube.h"
#include "Barrel.h"
#include "Space.h"
#include "ComplexMesh.h"
#include "CharactersDraw.h"
#include "singlemesh.h"
#include "Properties.h"
#include <AntTweakBar.h>
#include <ft2build.h>
#include <nlohmann/json.hpp>
#include FT_FREETYPE_H 


//constexpr int WINDOW_WIDTH = 800;
//constexpr int WINDOW_HEIGHT = 600;

// objects
ObjectList objects;
DefaultObject* transparent;
Sun* sun;
Car* car;
Terrain* terrain;
Animation* movie;
SingleMesh* sm;

// utilities
CharactersDraw* characterDraw;
Properties* properties;

// shared shader programs
ShaderProgram commonShaderProgram;
ShaderProgram bannerShaderProgram;
ShaderProgram bloodBannerShaderProgram;
ShaderProgram skyboxShaderProgram;
ShaderProgram sunShaderProgram;
ShaderProgram textShaderProgram;

GLuint texture;
//GLuint bloodTexture;


// -----------------------  OpenGL stuff ---------------------------------
bool keys[256];

bool ROTATION_FLAG = true;
bool SUN_MOTION_FLAG = false;
bool OVERALAY_FLAG = false;
float CAMERA_SPEED = 0.1f;
int ACCELERATION = 5;

float delayFrame;
float lastFrame;

ObjectGeometry* skyboxGeometry;
ObjectGeometry* bloodGeometry;
ObjectGeometry* bannerGeometry;

glm::vec3 cameraPosGlobal = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraLook = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 direction;
glm::vec3 curveDirection;

glm::vec3 objPos = glm::vec3(1.0f);
float objSc = 1.0f;

std::vector<glm::vec3> knots;
std::vector<glm::vec3> handles;

glm::vec3 sunPos = glm::vec3(4.0f, 5.0f, 0.0f);

float pitch = 0, yaw = -90.0f;
float tGlobal = 0;
//float lastMouseX = WINDOW_WIDTH / 2, lastMouseY = WINDOW_HEIGHT / 2;

glm::mat4 viewMatrix = glm::mat4(1.0f);
glm::mat4 projectionMatrix = glm::mat4(1.0f);

//std::string skyboxFaces[] = {
//	"data/Skybox/posx.jpg",
//	"data/Skybox/negx.jpg",
//	"data/Skybox/posy.jpg",
//	"data/Skybox/negy.jpg",
//	"data/Skybox/posz.jpg",
//	"data/Skybox/negz.jpg"
//};

TwBar* menuBar;

struct StateInfo {
	// window size
	int windowWidth;
	int windowHeight;

	//world borders(collision)
	int worldBorderX;
	int worldBorderY;
	int worldBorderYUp;
	int worldBorderZ;

	//toggles
	bool freeCamera;
	bool staticCam1;
	bool staticCam2;
	bool cameraCurve;
	bool fog;
	bool blood;
	bool banner;
	bool text;
	bool drag;
	bool dragCamera;
	bool carCameraFlag;
	bool menu;

	//light toggles
	bool isFlashlight;
	bool isDirLight;
	bool isPointLight;
	
	glm::vec2 mouseLastPos;

	int fixedCameraPos;
	float sensetivity;
	float sensetivityDrag;
	float cameraSpeed;

	float cameraAcc;
	float transparency;
	float fogHeight;
} stateInfo;

void loadDefaultShader() {
	// initializing default shader
	GLuint shaders[] = {
		/*pgr::createShaderFromSource(GL_VERTEX_SHADER, vertexShaderSrc),
		pgr::createShaderFromSource(GL_FRAGMENT_SHADER, fragmentShaderSrc),*/
		pgr::createShaderFromFile(GL_VERTEX_SHADER, "perPixel.vert"),
		pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "perPixel.frag"),

		0
	};


	commonShaderProgram.program = pgr::createProgram(shaders);
	commonShaderProgram.locations.position = glGetAttribLocation(commonShaderProgram.program, "position");
	commonShaderProgram.locations.normals = glGetAttribLocation(commonShaderProgram.program, "normals");
	commonShaderProgram.locations.texture = glGetAttribLocation(commonShaderProgram.program, "texCoord");

	// other attributes and uniforms
	commonShaderProgram.locations.PVMmatrix = glGetUniformLocation(commonShaderProgram.program, "PVM");
	commonShaderProgram.locations.model = glGetUniformLocation(commonShaderProgram.program, "model");
	commonShaderProgram.locations.sampl = glGetUniformLocation(commonShaderProgram.program, "sampl");
	commonShaderProgram.locations.alphaChannel = glGetUniformLocation(commonShaderProgram.program, "alphaChannel");

	//flashlight uniforms
	commonShaderProgram.locations.flashlightAngle = glGetUniformLocation(commonShaderProgram.program, "flashlightAngle");
	//commonShaderProgram.locations.flashlightPos = glGetUniformLocation(commonShaderProgram.program, "flashlightPos");
	commonShaderProgram.locations.flashlightColor = glGetUniformLocation(commonShaderProgram.program, "flashlightColor");

	//light uniforms
	commonShaderProgram.locations.light = glGetUniformLocation(commonShaderProgram.program, "light");
	commonShaderProgram.locations.lightPos = glGetUniformLocation(commonShaderProgram.program, "lightPos");
	commonShaderProgram.locations.dirLightVec = glGetUniformLocation(commonShaderProgram.program, "dirLightVec");
	CHECK_GL_ERROR();

	commonShaderProgram.locations.cameraDirection = glGetUniformLocation(commonShaderProgram.program, "cameraDirection");
	commonShaderProgram.locations.isFog = glGetUniformLocation(commonShaderProgram.program, "isFog");
	commonShaderProgram.locations.cameraPos = glGetUniformLocation(commonShaderProgram.program, "cameraPos");
	commonShaderProgram.locations.fogHeight = glGetUniformLocation(commonShaderProgram.program, "fogHeight");

	commonShaderProgram.locations.isFlashlight = glGetUniformLocation(commonShaderProgram.program, "isFlashlight");
	commonShaderProgram.locations.isDirLight = glGetUniformLocation(commonShaderProgram.program, "isDirLight");
	commonShaderProgram.locations.isPointLight = glGetUniformLocation(commonShaderProgram.program, "isPointLight");

	commonShaderProgram.locations.ambientM = glGetUniformLocation(commonShaderProgram.program, "material.ambient");
	commonShaderProgram.locations.diffuseM = glGetUniformLocation(commonShaderProgram.program, "material.diffuse");
	commonShaderProgram.locations.specularM = glGetUniformLocation(commonShaderProgram.program, "material.specular");
	commonShaderProgram.locations.shininessM = glGetUniformLocation(commonShaderProgram.program, "material.shininess");
	
	commonShaderProgram.locations.ambientL = glGetUniformLocation(commonShaderProgram.program, "light_s.ambient");
	commonShaderProgram.locations.diffuseL = glGetUniformLocation(commonShaderProgram.program, "light_s.diffuse");
	commonShaderProgram.locations.specularL = glGetUniformLocation(commonShaderProgram.program, "light_s.specular");
	
	commonShaderProgram.locations.constant = glGetUniformLocation(commonShaderProgram.program, "light_s.constant");
	commonShaderProgram.locations.linear = glGetUniformLocation(commonShaderProgram.program, "light_s.linear");
	commonShaderProgram.locations.quadratic = glGetUniformLocation(commonShaderProgram.program, "light_s.quadratic");


	assert(commonShaderProgram.locations.PVMmatrix != -1);
	assert(commonShaderProgram.locations.position != -1);
	//assert(commonShaderProgram.locations.isFog != -1);
	// ...
	commonShaderProgram.initialized = true;
}

void loadTextShader() {
	// initializing default shader
	GLuint shaders[] = {
		pgr::createShaderFromFile(GL_VERTEX_SHADER, "text.vert"),
		pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "text.frag"),

		0
	};


	textShaderProgram.program = pgr::createProgram(shaders);
	textShaderProgram.locations.position = glGetAttribLocation(textShaderProgram.program, "position");
	//textShaderProgram.locations.texture = glGetAttribLocation(textShaderProgram.program, "texCoord");

	// other attributes and uniforms
	textShaderProgram.locations.projection = glGetUniformLocation(textShaderProgram.program, "projection");
	textShaderProgram.locations.sampl = glGetUniformLocation(textShaderProgram.program, "sampl");
	textShaderProgram.locations.textColor = glGetUniformLocation(textShaderProgram.program, "textColor");

	assert(textShaderProgram.locations.projection != -1);
	assert(textShaderProgram.locations.position != -1);
	// ...
	textShaderProgram.initialized = true;
}

void loadBannerTexture() {
	// texture setup
	glActiveTexture(GL_TEXTURE4);
	bannerGeometry->texture = pgr::createTexture(properties->getBannerI());
	if (bannerGeometry->texture == 0) {
		std::cout << "Texture not loaded!" << std::endl;
	}
	CHECK_GL_ERROR();
	glBindTexture(GL_TEXTURE_2D, bannerGeometry->texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
}

void loadBannerShader() {
	// initializing default shader
	GLuint shaders[] = {
		/*pgr::createShaderFromSource(GL_VERTEX_SHADER, vertexShaderSrc),
		pgr::createShaderFromSource(GL_FRAGMENT_SHADER, fragmentShaderSrc),*/
		pgr::createShaderFromFile(GL_VERTEX_SHADER, "banner.vert"),
		pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "banner.frag"),

		0
	};

	static const float screenRectangle[] = {
		1.0f, 1.0f,
		-1.0f,  1.0f,
		1.0f, -1.0f,
		-1.0f, -1.0f
	};

	bannerShaderProgram.program = pgr::createProgram(shaders);
	bannerShaderProgram.locations.position = glGetAttribLocation(bannerShaderProgram.program, "position");
	bannerShaderProgram.locations.normals = glGetAttribLocation(bannerShaderProgram.program, "normals");
	bannerShaderProgram.locations.texture = glGetAttribLocation(bannerShaderProgram.program, "texCoord");

	// other attributes and uniforms
	bannerShaderProgram.locations.model = glGetUniformLocation(bannerShaderProgram.program, "model");
	bannerShaderProgram.locations.trans = glGetUniformLocation(bannerShaderProgram.program, "trans");
	bannerShaderProgram.locations.time = glGetUniformLocation(bannerShaderProgram.program, "time");
	bannerShaderProgram.locations.sampl = glGetUniformLocation(bannerShaderProgram.program, "texSampler");

	//assert(bannerShaderProgram.locations.PVMmatrix != -1);
	assert(bannerShaderProgram.locations.position != -1);
	// ...
	bannerShaderProgram.initialized = true;

	bannerGeometry = new ObjectGeometry;

	loadBannerTexture();

	// buffers 
	glGenBuffers(1, &bannerGeometry->vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, bannerGeometry->vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenRectangle), screenRectangle, GL_STATIC_DRAW);

	glGenVertexArrays(1, &bannerGeometry->vertexArrayObject);
	glBindVertexArray(bannerGeometry->vertexArrayObject);
	glEnableVertexAttribArray(bloodBannerShaderProgram.locations.position);
	glVertexAttribPointer(bloodBannerShaderProgram.locations.position, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
	CHECK_GL_ERROR();
}

void loadBloodTexture() {
	glActiveTexture(GL_TEXTURE4);
	bloodGeometry->texture = pgr::createTexture(properties->getBloodI());
	if (bloodGeometry->texture == 0) {
		std::cout << "Texture not loaded!" << std::endl;
	}
	CHECK_GL_ERROR();
	glBindTexture(GL_TEXTURE_2D, bloodGeometry->texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
}

void loadBloodBannerShader() {
	// initializing default shader
	GLuint shaders[] = {
		/*pgr::createShaderFromSource(GL_VERTEX_SHADER, vertexShaderSrc),
		pgr::createShaderFromSource(GL_FRAGMENT_SHADER, fragmentShaderSrc),*/
		pgr::createShaderFromFile(GL_VERTEX_SHADER, "bannerBlood.vert"),
		pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "bannerBlood.frag"),

		0
	};

	static const float screenRectangle[] = {
		1.0f, 1.0f,
		-1.0f,  1.0f,
		1.0f, -1.0f,
		-1.0f, -1.0f
	};

	bloodBannerShaderProgram.program = pgr::createProgram(shaders);
	bloodBannerShaderProgram.locations.position = glGetAttribLocation(bloodBannerShaderProgram.program, "position");
	bloodBannerShaderProgram.locations.normals = glGetAttribLocation(bloodBannerShaderProgram.program, "normals");
	bloodBannerShaderProgram.locations.texture = glGetAttribLocation(bloodBannerShaderProgram.program, "texCoord");

	// other attributes and uniforms
	bloodBannerShaderProgram.locations.model = glGetUniformLocation(bloodBannerShaderProgram.program, "model");
	bloodBannerShaderProgram.locations.sampl = glGetUniformLocation(bloodBannerShaderProgram.program, "texSampler");

	//assert(bannerShaderProgram.locations.PVMmatrix != -1);
	assert(bloodBannerShaderProgram.locations.position != -1);
	// ...
	bloodBannerShaderProgram.initialized = true;

	bloodGeometry = new ObjectGeometry;

	// texture setup
	loadBloodTexture();

	//glGenTextures(1, &bloodGeometry->texture);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, bloodGeometry->texture);

	// buffers 
	glGenBuffers(1, &bloodGeometry->vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, bloodGeometry->vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenRectangle), screenRectangle, GL_STATIC_DRAW);

	glGenVertexArrays(1, &bloodGeometry->vertexArrayObject);
	glBindVertexArray(bloodGeometry->vertexArrayObject);
	glEnableVertexAttribArray(bloodBannerShaderProgram.locations.position);
	glVertexAttribPointer(bloodBannerShaderProgram.locations.position, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
	CHECK_GL_ERROR();
}

void loadSunShader() {
	// initializing default shader
	GLuint shaders[] = {
		/*pgr::createShaderFromSource(GL_VERTEX_SHADER, vertexShaderSrc),
		pgr::createShaderFromSource(GL_FRAGMENT_SHADER, fragmentShaderSrc),*/
		pgr::createShaderFromFile(GL_VERTEX_SHADER, "cube.vert"),
		pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "cube.frag"),

		0
	};


	sunShaderProgram.program = pgr::createProgram(shaders);
	sunShaderProgram.locations.position = glGetAttribLocation(sunShaderProgram.program, "position");
	sunShaderProgram.locations.normals = glGetAttribLocation(sunShaderProgram.program, "normals");
	sunShaderProgram.locations.texture = glGetAttribLocation(sunShaderProgram.program, "texCoord");

	// other attributes and uniforms
	sunShaderProgram.locations.PVMmatrix = glGetUniformLocation(sunShaderProgram.program, "PVM");

	assert(sunShaderProgram.locations.PVMmatrix != -1);
	assert(sunShaderProgram.locations.position != -1);
	// ...
	sunShaderProgram.initialized = true;
}

void loadSkyboxTexture() {
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxGeometry->texture);

	for (int i = 0; i < 6; i++) {
		if (!pgr::loadTexImage2D(properties->skyboxFaces[i], GL_TEXTURE_CUBE_MAP_POSITIVE_X + i))
			std::cout << "Failed loading " << properties->skyboxFaces[i] << std::endl;
	}

	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void loadSkybox() {

	GLuint skyboxShaders[] = {
	  pgr::createShaderFromFile(GL_VERTEX_SHADER, "skyboxShader.vert"),
	  pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "skyboxShader.frag"),

	  0
	};

	skyboxShaderProgram.program = pgr::createProgram(skyboxShaders);
	skyboxShaderProgram.locations.position = glGetAttribLocation(skyboxShaderProgram.program, "position");
	skyboxShaderProgram.locations.texture = glGetAttribLocation(skyboxShaderProgram.program, "texCoord");

	// other attributes and uniforms
	skyboxShaderProgram.locations.PVMmatrix = glGetUniformLocation(skyboxShaderProgram.program, "PVM");
	skyboxShaderProgram.locations.sampl = glGetUniformLocation(skyboxShaderProgram.program, "cubeMap");

	assert(skyboxShaderProgram.locations.PVMmatrix != -1);
	assert(skyboxShaderProgram.locations.position != -1);

	skyboxShaderProgram.initialized = true;

	static const float screenRectangle[] = {
		-1.0f, -1.0f,
		1.0f, -1.0f,
		-1.0f, 1.0f,
		1.0f, 1.0f
	};

	glUseProgram(skyboxShaderProgram.program);
	skyboxGeometry = new ObjectGeometry;
	glGenBuffers(1, &skyboxGeometry->vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxGeometry->vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenRectangle), screenRectangle, GL_DYNAMIC_DRAW);

	//glActiveTexture(GL_TEXTURE2);
	glGenTextures(1, &skyboxGeometry->texture);

	glGenVertexArrays(1, &skyboxGeometry->vertexArrayObject);
	glBindVertexArray(skyboxGeometry->vertexArrayObject);
	glEnableVertexAttribArray(skyboxShaderProgram.locations.position);
	glVertexAttribPointer(skyboxShaderProgram.locations.position, 2, GL_FLOAT, GL_FALSE, 0, 0);
	loadSkyboxTexture();
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

/**
 * \brief Load and compile shader programs. Get attribute locations.
 */
void loadShaderPrograms()
{
	/*std::string vertexShaderSrc =
		"#version 140\n"
		"in vec3 position;\n"
		"in vec2 texCoord;\n"
		"uniform mat4 PVM;\n"
		"smooth out vec2 texCoord_v;\n"
		"void main() {\n"
		"  gl_Position = PVM * vec4(position, 1.0f);\n"
		"  texCoord_v = texCoord;\n"
		"}\n"
		;*/

	//fragmentColor = vec4(0.5f, 1.0f, 1.0f, 1.0f);
	/*std::string fragmentShaderSrc =
		"#version 140\n"
		"smooth in vec3 position_v;\n"
		"smooth in vec2 texCoord_v;\n"
		"out vec4 fragmentColor;"
		"uniform sampler2D sampl;"
		"void main() {\n"
		"  fragmentColor = vec4(0.5f, 1.0f, 1.0f, 1.0f);\n"
		"}\n"
		;*/

	loadDefaultShader();
	loadTextShader();
	loadSkybox();
	loadSunShader();
	loadBloodBannerShader();
	loadBannerShader();
}

/**
 * \brief Delete all shader program objects.
 */
void cleanupShaderPrograms(void) {

	pgr::deleteProgramAndShaders(commonShaderProgram.program);
	pgr::deleteProgramAndShaders(skyboxShaderProgram.program);
	pgr::deleteProgramAndShaders(sunShaderProgram.program);
}

void drawSkybox() {


	glBindVertexArray(skyboxGeometry->vertexArrayObject);
	glUseProgram(skyboxShaderProgram.program);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxGeometry->texture);
	glBindVertexArray(skyboxGeometry->vertexArrayObject);

	glUniformMatrix4fv(skyboxShaderProgram.locations.PVMmatrix, 1, GL_FALSE, glm::value_ptr(glm::inverse(projectionMatrix * viewMatrix * glm::mat4(1.0f))));
	glUniform1i(skyboxShaderProgram.locations.sampl, 2);
	//glUniformMatrix4fv(skyboxShaderProgram.locations.PVMmatrix, 1, GL_FALSE, glm::value_ptr(invPV));
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);
	glUseProgram(0);
}

void drawBanner() {
	glDepthFunc(GL_NOTEQUAL);
	glUseProgram(bannerShaderProgram.program);
	glBindVertexArray(bannerGeometry->vertexArrayObject);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, bannerGeometry->texture);
	//glBindVertexArray(bloodGeometry->vertexArrayObject);

	static glm::mat3 rot = glm::mat3(1);

	float fact = (glm::sin(glutGet(GLUT_ELAPSED_TIME) * 0.003) + 1) / 2;
	fact = glm::mix(1.0, 10.0, fact);

	rot = glm::mat3(glm::rotate(glm::mat4(1), glm::radians(3.0f), glm::vec3(0, 0, 1))) * rot;
	glm::mat3 sc = glm::scale(glm::mat4(1), glm::vec3(fact, fact, 1.0f));

	glUniformMatrix3fv(bannerShaderProgram.locations.trans, 1, GL_FALSE, glm::value_ptr(rot * sc));

	//glUniform1f(bannerShaderProgram.locations.time, 0.001f * static_cast<float>(glutGet(GLUT_ELAPSED_TIME)));
	//glUniformMatrix4fv(skyboxShaderProgram.locations.PVMmatrix, 1, GL_FALSE, glm::value_ptr(invPV));
	//glUniformMatrix4fv(bannerShaderProgram.locations.PVMmatrix, 1, GL_FALSE, glm::value_ptr(glm::inverse(projectionMatrix * viewMatrix * glm::mat4(1.0f))));
	//glUniform1f(bannerShaderProgram.locations.time, 0.001f * static_cast<float>(glutGet(GLUT_ELAPSED_TIME)));
	glUniform1i(bannerShaderProgram.locations.sampl, 4);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


	glBindVertexArray(0);
	glUseProgram(0);
	glDepthFunc(GL_LESS);
}

void drawBloodBanner() {
	glDepthFunc(GL_NOTEQUAL);
	glUseProgram(bloodBannerShaderProgram.program);
	glBindVertexArray(bloodGeometry->vertexArrayObject);
	glActiveTexture(GL_TEXTURE4);
	CHECK_GL_ERROR();
	glBindTexture(GL_TEXTURE_2D, bloodGeometry->texture);
	CHECK_GL_ERROR();
	//glBindVertexArray(bloodGeometry->vertexArrayObject);
	CHECK_GL_ERROR();

	//glUniformMatrix4fv(bannerShaderProgram.locations.PVMmatrix, 1, GL_FALSE, glm::value_ptr(glm::inverse(projectionMatrix * viewMatrix * glm::mat4(1.0f))));
	glUniform1i(bloodBannerShaderProgram.locations.sampl, 4);
	//glUniformMatrix4fv(skyboxShaderProgram.locations.PVMmatrix, 1, GL_FALSE, glm::value_ptr(invPV));
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);
	glUseProgram(0);
	glDepthFunc(GL_LESS);
}

void makeCurve() {
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

// count direction to face while moving on curve
void countDirectionOnCurve(float t, int segment) {
	glm::vec3 resultDir;
	//resultPos = (b1 * handles[segment * 2]) + (b2 * knots[segment]) + (b3 * knots[segment+1]) + (b4 * handles[(segment * 2) + 1]);

	float b1 = -3*pow(t, 2) + 4 * t - 1;
	float b2 = 3 * 3 * pow(t, 2) - 5 * 2 * t;
	float b3 = -3 * -3 * pow(t, 2) + 4 * 2 * t + 1;
	float b4 = 3 * pow(t, 2) - 2*t;

	resultDir = (b1 * handles[segment * 2]) + (b2 * knots[segment]) + (b3 * knots[segment + 1]) + (b4 * handles[(segment * 2) + 1]);
	curveDirection = resultDir;

	//return resultDir;
}

// count position with given t value on curve segment
glm::vec3 countPositionOnCurve(float t) {
	glm::vec3 resultPos;

	int segment = floor(t);
	if (segment >= MAX_T) {
		t -= 4;
		tGlobal = t;
		segment = 0;
	}

	t -= segment;

	float b1 = -pow(t, 3)  + 2 * pow(t, 2) - t;
	float b2 = 3*pow(t, 3) - 5 * pow(t, 2) + 2;
	float b3 = -3*pow(t, 3)+ 4 * pow(t, 2) + t;
	float b4 = pow(t, 3)   - pow(t, 2);

	//std::cout << "t: " << t << std::endl;
	//std::cout << "segment: " << segment << std::endl;
	//std::cout << segment * 2 << " " << segment * 2 + 1 << std::endl;

	resultPos = (b1 * handles[segment * 2]) + (b2 * knots[segment]) + (b3 * knots[segment+1]) + (b4 * handles[(segment * 2) + 1]);

	//resultPos = (float)pow(1 - t, 3) * knots[segment] + 3 * (float)pow(1 - t, 2) * t * handles[segment * 2] + 3 * (1 - t) * (float)pow(t, 2) * handles[(segment * 2) + 1] + (float)pow(t, 3) * knots[segment + 1];
	countDirectionOnCurve(t, segment);

	return resultPos * 10.0f;
}

/**
 * \brief Draw all scene objects.
 */
void drawScene(void)
{
	float time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	delayFrame = time - lastFrame;
	lastFrame = time;

	CAMERA_SPEED = stateInfo.cameraAcc * delayFrame;
	const float radius = 10.0f;

	if (SUN_MOTION_FLAG) {
		float sunX = sin(time) * radius;
		float sunY = cos(time) * radius;

		sunPos.x = sunX;
		sunPos.y = sunY;

		sun->changePosition(sunPos);
	}

	//if (ROTATION_FLAG) {
	//	float camX = sin(time) * radius;
	//	float camZ = cos(time) * radius;

	//	//std::cout << glutGet(GLUT_ELAPSED_TIME) << std::endl;

	//	cameraPosGlobal = glm::vec3(camX / 2, 3.0, camZ / 2);
	//	cameraLook = glm::vec3(0.0f) - cameraPosGlobal;
	//	cameraLook = normalize(cameraLook);
	//	viewMatrix = glm::lookAt(cameraPosGlobal, cameraLook, glm::vec3(0.0, 1.0, 0.0));
	//}
	if (!stateInfo.freeCamera) {
		if (stateInfo.staticCam1) {
			cameraPosGlobal = glm::vec3(2.0, 3.0, 2.0);
			//static glm::vec3 look ;
			cameraLook = glm::vec3(0.0f);
			//cameraLook = normalize(cameraLook);
			viewMatrix = glm::lookAt(cameraPosGlobal, cameraLook, glm::vec3(0.0, 1.0, 0.0));
			cameraPos = cameraPosGlobal;
		}
		else {
			cameraPosGlobal = glm::vec3(1.0, 5.0, 4.0);
			cameraLook = glm::vec3(1.5f);
			//cameraLook = normalize(cameraLook);
			viewMatrix = glm::lookAt(cameraPosGlobal, cameraLook, glm::vec3(0.0, 1.0, 0.0));
			cameraPos = cameraPosGlobal;
		}
	}
	else if (stateInfo.carCameraFlag) {
		cameraPosGlobal = car->getCarPos();
		cameraPosGlobal.y += 2;
		cameraLook = car->getCarLook();
		//cameraLook = normalize(cameraLook);
		viewMatrix = glm::lookAt(cameraPosGlobal, cameraLook+cameraPosGlobal, glm::vec3(0.0, 1.0, 0.0));
		cameraPos = cameraPosGlobal;
	} 
	else if (stateInfo.cameraCurve) {
		cameraPosGlobal = countPositionOnCurve(tGlobal);
		cameraLook = curveDirection;

		viewMatrix = glm::lookAt(cameraPosGlobal, cameraLook + cameraPosGlobal, glm::vec3(0.0, 1.0, 0.0));
		tGlobal += 0.01f;
	} else {
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraLook = glm::normalize(direction);
		viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraLook, cameraUp);
		cameraPosGlobal = cameraPos;
	}
	glUseProgram(commonShaderProgram.program);

	// setup uniforms
	glUniform3f(commonShaderProgram.locations.light, 1.0f, 1.0f, 1.0f);
	CHECK_GL_ERROR();
	glUniform3f(commonShaderProgram.locations.lightPos, sunPos.x, sunPos.y, sunPos.z);
	CHECK_GL_ERROR();
	glUniform3f(commonShaderProgram.locations.cameraPos, cameraPosGlobal.x, cameraPosGlobal.y, cameraPosGlobal.z);
	CHECK_GL_ERROR();
	glUniform3f(commonShaderProgram.locations.cameraDirection, cameraLook.x, cameraLook.y, cameraLook.z);
	CHECK_GL_ERROR();
	glUniform1i(commonShaderProgram.locations.isFog, stateInfo.fog);

	glUniform1i(commonShaderProgram.locations.isFlashlight, stateInfo.isFlashlight);
	glUniform1i(commonShaderProgram.locations.isDirLight, stateInfo.isDirLight);
	glUniform1i(commonShaderProgram.locations.isPointLight, stateInfo.isPointLight);

	glUniform1f(commonShaderProgram.locations.fogHeight, stateInfo.fogHeight);
	movie->fogHeight = stateInfo.fogHeight;
	glUniform1f(commonShaderProgram.locations.alphaChannel, 1);
	float dirX = sin(time) * radius;
	float dirY = cos(time) * radius;
	glUniform3f(commonShaderProgram.locations.dirLightVec, -dirX, -dirY, 0.0f);
	CHECK_GL_ERROR();

	//setup materials
	glUniform3f(commonShaderProgram.locations.ambientM, 0.1f, 0.1f, 0.1f);
	glUniform3f(commonShaderProgram.locations.diffuseM, 1.0f, 1.0f, 1.0f);
	glUniform3f(commonShaderProgram.locations.specularM, 1.0f, 1.0f, 1.0f);
	glUniform1f(commonShaderProgram.locations.shininessM, 32.0f);

	//setup light
	glUniform3f(commonShaderProgram.locations.ambientL, 1.0f, 1.0f, 1.0f);
	glUniform3f(commonShaderProgram.locations.diffuseL, 0.5f, 0.5f, 0.5f);
	glUniform3f(commonShaderProgram.locations.specularL, 1.0f, 1.0f, 1.0f);
	glUniform1f(commonShaderProgram.locations.constant, 1.0f);
	glUniform1f(commonShaderProgram.locations.linear, 0.1f);
	glUniform1f(commonShaderProgram.locations.quadratic, 0.005f);

	//glUseProgram(0);
	projectionMatrix = glm::perspective(glm::radians(60.0f), float(glutGet(GLUT_WINDOW_WIDTH)) / float(glutGet(GLUT_WINDOW_HEIGHT)), 0.1f, 100.0f);
	//TODO testing
	//((ComplexMesh*)objects[objects.size() - 1])->chPosition(objPos);
	//((ComplexMesh*)objects[objects.size() - 1])->chScale(glm::vec3(objSc));
	for (ObjectInstance* object : objects) {   // for (auto object : objects) {
		if (object != nullptr)
			object->draw(viewMatrix, projectionMatrix, glm::vec3(1.0f, 1.0f, 1.0f), sunPos, cameraPosGlobal, cameraLook, stateInfo.fog);
	}
	terrain->draw(viewMatrix, projectionMatrix, glm::vec3(1.0f, 1.0f, 1.0f), sunPos, cameraPosGlobal, cameraLook, stateInfo.fog);
	
	glStencilFunc(GL_ALWAYS, 2, -1);
	sun->draw(viewMatrix, projectionMatrix, cameraPosGlobal, cameraLook, stateInfo.fog);
	glStencilFunc(GL_ALWAYS, 1, -1);
	car->draw(viewMatrix, projectionMatrix, glm::vec3(1.0f, 1.0f, 1.0f), sunPos, cameraPosGlobal, cameraLook, stateInfo.fog);
	glStencilFunc(GL_ALWAYS, 3, -1);
	movie->draw(viewMatrix, projectionMatrix, glm::vec3(1.0f, 1.0f, 1.0f), sunPos, cameraPosGlobal, cameraLook, stateInfo.fog);
	glStencilFunc(GL_ALWAYS, 0, -1);

	/*glUniform1i(commonShaderProgram.locations.sampl, 0);
	glActiveTexture(GL_TEXTURE0);*/
	if (!stateInfo.fog) {
		drawSkybox();
	}
	if(stateInfo.text) {
		characterDraw->draw("test Text", 100.0f, 100.0f, 1.0f, glm::vec3(1.0f));
	}
	glUseProgram(commonShaderProgram.program);
	glUniform1f(commonShaderProgram.locations.alphaChannel, stateInfo.transparency);
	// place for transparent objects
	glUniform3f(commonShaderProgram.locations.ambientM, 0.1f, 0.1f, 0.1f);
	glUniform3f(commonShaderProgram.locations.diffuseM, 1.0f, 1.0f, 1.0f);
	glUniform3f(commonShaderProgram.locations.specularM, 1.0f, 1.0f, 1.0f);
	glUniform1f(commonShaderProgram.locations.shininessM, 64.0f);
	transparent->draw(viewMatrix, projectionMatrix, glm::vec3(1.0f, 1.0f, 1.0f), sunPos, cameraPosGlobal, cameraLook, stateInfo.fog);
	glUniform1f(commonShaderProgram.locations.alphaChannel, 1);
	glDisable(GL_STENCIL_TEST);
	if(stateInfo.blood)
		drawBloodBanner();
	if(stateInfo.banner)
		drawBanner();
	glEnable(GL_STENCIL_TEST);
}

// Clicking on object in scene and doing actions if object has one
void pick(int x, int y) {
	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//glClearStencil(0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//glUseProgram(commonShaderProgram.program);

	//glEnable(GL_STENCIL_TEST);
	//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	//glStencilFunc(GL_ALWAYS, 1, -1);

	unsigned char id;
	glReadPixels(x, y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &id);
	//std::cout << x << " " << y << " " << (int)id << std::endl;

	switch (id) {
		case 1:
			stateInfo.carCameraFlag = true;
			//std::cout << "click" << std::endl;
			break;
		case 2:
			SUN_MOTION_FLAG = !SUN_MOTION_FLAG;
			break;
		case 3:
			movie->changePlayState();
			break;
	}

}


// -----------------------  Window callbacks ---------------------------------

/**
 * \brief Draw the window contents.
 */
void displayCb() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// draw the window contents (scene objects)
	drawScene();

	glDisable(GL_BLEND);

	TwDraw();
	glutSwapBuffers();
}

/**
 * \brief Window was reshaped.
 * \param newWidth New window width
 * \param newHeight New window height
 */
void reshapeCb(int newWidth, int newHeight) {
	// TODO: Take new window size and update the application state,
	// window and projection.

	// glViewport(...);
	glViewport(0, 0, newWidth, newHeight);
	stateInfo.windowWidth = newWidth;
	stateInfo.windowHeight = newHeight;
	TwWindowSize(stateInfo.windowWidth, stateInfo.windowHeight);
};

// -----------------------  Keyboard ---------------------------------

/**
 * \brief Handle the key pressed event.
 * Called whenever a key on the keyboard was pressed. The key is given by the "keyPressed"
 * parameter, which is an ASCII character. It's often a good idea to have the escape key (ASCII value 27)
 * to call glutLeaveMainLoop() to exit the program.
 * \param keyPressed ASCII code of the key
 * \param mouseX mouse (cursor) X position
 * \param mouseY mouse (cursor) Y position
 */
void keyboardCb(unsigned char keyPressed, int mouseX, int mouseY) {
	//CAMERA_SPEED = delayFrame / 5.0f;
	if (TwEventKeyboardGLUT(keyPressed, mouseX, mouseY))
		return;

	switch (keyPressed)
	{
		case 27://esc
			glutLeaveMainLoop();
			exit(EXIT_SUCCESS);
			break;
		case 'c':
			std::cout << "C pressed" << std::endl;
			stateInfo.freeCamera = !stateInfo.freeCamera;
			stateInfo.cameraCurve = false;
			break;
		case 'l':
			std::cout << "Sun stopped" << std::endl;
			SUN_MOTION_FLAG = !SUN_MOTION_FLAG;
			break;

		case 'f':
			stateInfo.fog = !stateInfo.fog;
			//glUniform1i(commonShaderProgram.locations.isFog, stateInfo.fog);
			break;
		case 'm':
			stateInfo.menu = !stateInfo.menu;
			if (stateInfo.menu)
				TwDefine(" Menu iconified=false");
			else
				TwDefine(" Menu iconified=true");
			//std::cout << "menu " << stateInfo.menu << std::endl;
			break;
		case 'z':
			stateInfo.drag = !stateInfo.drag;
			//std::cout << "drag " << stateInfo.menu << std::endl;
			break;
		default:
			break;
	}
	keys[keyPressed] = true;
	if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
		std::cout << "shift pressed" << std::endl;
		CAMERA_SPEED *= ACCELERATION;
	}
}

// Called whenever a key on the keyboard was released. The key is given by
// the "keyReleased" parameter, which is in ASCII. 
/**
 * \brief Handle the key released event.
 * \param keyReleased ASCII code of the released key
 * \param mouseX mouse (cursor) X position
 * \param mouseY mouse (cursor) Y position
 */
void keyboardUpCb(unsigned char keyReleased, int mouseX, int mouseY) {
	keys[keyReleased] = false;

	switch (keyReleased)
	{
		default:
			break;
	}
}

//
/**
 * \brief Handle the non-ASCII key pressed event (such as arrows or F1).
 *  The special keyboard callback is triggered when keyboard function (Fx) or directional
 *  keys are pressed.
 * \param specKeyPressed int value of a predefined glut constant such as GLUT_KEY_UP
 * \param mouseX mouse (cursor) X position
 * \param mouseY mouse (cursor) Y position
 */
void specialKeyboardCb(int specKeyPressed, int mouseX, int mouseY) {
	if (TwEventSpecialGLUT(specKeyPressed, mouseX, mouseY))
		return;

	if (specKeyPressed == GLUT_KEY_F1) {
		stateInfo.menu = !stateInfo.menu;
		if (stateInfo.menu)
			TwDefine(" Menu iconified=false");
		else
			TwDefine(" Menu iconified=true");
	}
}

void specialKeyboardUpCb(int specKeyReleased, int mouseX, int mouseY) {
} // key released

// -----------------------  Mouse ---------------------------------
// three events - mouse click, mouse drag, and mouse move with no button pressed

// 
/**
 * \brief React to mouse button press and release (mouse click).
 * When the user presses and releases mouse buttons in the window, each press
 * and each release generates a mouse callback (including release after dragging).
 *
 * \param buttonPressed button code (GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, or GLUT_RIGHT_BUTTON)
 * \param buttonState GLUT_DOWN when pressed, GLUT_UP when released
 * \param mouseX mouse (cursor) X position
 * \param mouseY mouse (cursor) Y position
 */
void mouseCb(int buttonPressed, int buttonState, int mouseX, int mouseY) {
	if (TwEventMouseButtonGLUT(buttonPressed, buttonState, mouseX, mouseY))
		return;

	if (buttonPressed == GLUT_RIGHT_BUTTON) {
		if (buttonState == GLUT_DOWN) {
			stateInfo.dragCamera = true;
			stateInfo.mouseLastPos = glm::vec2(mouseX, mouseY);
		}
		else {
			stateInfo.dragCamera = false;
		}
	}

	if (buttonPressed == GLUT_LEFT_BUTTON && buttonState == GLUT_DOWN) {
		pick(mouseX, stateInfo.windowHeight - mouseY);
	}
}

/**
 * \brief Handle mouse dragging (mouse move with any button pressed).
 *        This event follows the glutMouseFunc(mouseCb) event.
 * \param mouseX mouse (cursor) X position
 * \param mouseY mouse (cursor) Y position
 */
void mouseMotionCb(int mouseX, int mouseY) {
	if (TwEventMouseMotionGLUT(mouseX, mouseY))
		return;

	if (stateInfo.drag && stateInfo.dragCamera) {
		float offsetX = mouseX - stateInfo.mouseLastPos.x;
		float offsetY = mouseY - stateInfo.mouseLastPos.y;

		offsetX *= stateInfo.sensetivityDrag;
		offsetY *= stateInfo.sensetivityDrag;

		yaw += offsetX;
		pitch -= offsetY;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}
}

/**
 * \brief Handle mouse movement over the window (with no button pressed).
 * \param mouseX mouse (cursor) X position
 * \param mouseY mouse (cursor) Y position
 */
void passiveMouseMotionCb(int mouseX, int mouseY) {
	if (TwEventMouseMotionGLUT(mouseX, mouseY))
		return;

	// mouse hovering over window

	// create display event to redraw window contents if needed (and not handled in the timer callback)
	// glutPostRedisplay();

	if (stateInfo.freeCamera && !stateInfo.drag) {
		//inspitation https://learnopengl.com/Getting-started/Camera
		/*std::cout << "Mouse x: " << mouseX << std::endl;
		std::cout << "Mouse y: " << mouseY << std::endl;*/

		float offsetX = mouseX - stateInfo.windowWidth / 2;
		float offsetY = mouseY - stateInfo.windowHeight / 2;

		offsetX *= stateInfo.sensetivity;
		offsetY *= stateInfo.sensetivity;

		yaw += offsetX;
		pitch -= offsetY;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glutWarpPointer(stateInfo.windowWidth / 2, stateInfo.windowHeight / 2);
	}
}

// -----------------------  Timer ---------------------------------

/**
 * \brief Callback responsible for the scene update.
 */
void timerCb(int)
{
#ifndef SKELETON // @task_1_0
	const glm::mat4 sceneRootMatrix = glm::mat4(1.0f);

	float elapsedTime = 0.001f * static_cast<float>(glutGet(GLUT_ELAPSED_TIME)); // milliseconds => seconds

	// update the application state
	for (ObjectInstance* object : objects) {   // for (auto object : objects) {
		if (object != nullptr)
			object->update(elapsedTime, &sceneRootMatrix);
	}

	if(SUN_MOTION_FLAG)
		sun->update(elapsedTime, &sceneRootMatrix);
	car->update(elapsedTime, &sceneRootMatrix);
	movie->update(elapsedTime, &sceneRootMatrix);
#endif // task_1_0

	glm::vec3 direction = CAMERA_SPEED * cameraLook;

	//inspiration https://learnopengl.com/Getting-started/Camera
	if (keys['w']) {
		//std::cout << "w pressed" << std::endl;
		cameraPos += direction;
		std::cout << cameraPos.x << " " << cameraPos.y << " " << cameraPos.z << std::endl;
		if(cameraPos.x > stateInfo.worldBorderX || cameraPos.x < -stateInfo.worldBorderX ||
			cameraPos.z > stateInfo.worldBorderZ || cameraPos.z < -stateInfo.worldBorderZ ||
			cameraPos.y < stateInfo.worldBorderY || cameraPos.y > stateInfo.worldBorderYUp)
			cameraPos -= direction;
	}

	if (keys['s']) {
		//std::cout << "s pressed" << std::endl;
		cameraPos -= direction;
		if (cameraPos.x > stateInfo.worldBorderX || cameraPos.x < -stateInfo.worldBorderX ||
			cameraPos.z > stateInfo.worldBorderZ || cameraPos.z < -stateInfo.worldBorderZ ||
			cameraPos.y < stateInfo.worldBorderY || cameraPos.y > stateInfo.worldBorderYUp)
		cameraPos += direction;

	}

	direction = CAMERA_SPEED * glm::normalize(glm::cross(cameraLook, cameraUp));
	if (keys['d']) {
		//std::cout << "d pressed" << std::endl;
		cameraPos += direction;
		if (cameraPos.x > stateInfo.worldBorderX || cameraPos.x < -stateInfo.worldBorderX ||
			cameraPos.z > stateInfo.worldBorderZ || cameraPos.z < -stateInfo.worldBorderZ ||
			cameraPos.y < stateInfo.worldBorderY || cameraPos.y > stateInfo.worldBorderYUp)
		cameraPos -= direction;
	}

	if (keys['a']) {
			//std::cout << "a pressed" << std::endl;
			cameraPos -= direction;
		if (cameraPos.x > stateInfo.worldBorderX || cameraPos.x < -stateInfo.worldBorderX ||
			cameraPos.z > stateInfo.worldBorderZ || cameraPos.z < -stateInfo.worldBorderZ ||
			cameraPos.y < stateInfo.worldBorderY || cameraPos.y > stateInfo.worldBorderYUp)
			cameraPos += direction;
	}
	

	// and plan a new event
	glutTimerFunc(16, timerCb, 0);

	// create display event
	glutPostRedisplay();
}

void TW_CALL bloodCB(void *p) {
	stateInfo.blood = !stateInfo.blood;
}

void TW_CALL reloadPropertiesCB(void *p) {
	properties->reloadProperties();
	//reshapeCb(properties->getWinW(), properties->getWinH());
	loadSkyboxTexture();
	loadBannerTexture();
	loadBloodTexture();
	car->updateCurve(properties->getKnotsBezier(), properties->getHandlesBezier());
	knots = properties->getKnotsCatmull();
	handles = properties->getHandlesCatmull();
}

void TW_CALL stCam1(void* p) {
	stateInfo.staticCam1 = true;
	stateInfo.staticCam2 = false;
	stateInfo.freeCamera = false;
	stateInfo.cameraCurve = false;
	//stateInfo.carCameraFlag = false;
}

void TW_CALL stCam2(void* p) {
	stateInfo.staticCam2 = true;
	stateInfo.staticCam1 = false;
	stateInfo.freeCamera = false;
	stateInfo.cameraCurve = false;
	//stateInfo.carCameraFlag = false;
}

void TW_CALL curveCam(void* p) {
	stateInfo.cameraCurve = true;
	stateInfo.staticCam1 = false;
	stateInfo.staticCam2 = false;
	stateInfo.carCameraFlag = false;
	stateInfo.freeCamera = true;
	//stateInfo.carCameraFlag = false;
}

//TwStructMember Vector3fMembers[] = {
//	{ "x", TW_TYPE_FLOAT, offsetof(Vector3f, x), "" },
//	{ "y", TW_TYPE_FLOAT, offsetof(Vector3f, y), "" },
//	{ "z", TW_TYPE_FLOAT, offsetof(Vector3f, z), "" }
//};
//
//auto TW_TYPE_OGLDEV_VECTOR3F = TwDefineStruct("Vector3f", Vector3fMembers, 3, sizeof(Vector3f), NULL, NULL);

// initiate AntTweaksBar menu and elements inside menu
void initMenu() {
	menuBar = TwNewBar("Menu");
	TwDefine("Menu size='240 420' color='20 20 20' fontsize=3 iconified=true");
	//TwAddButton(menuBar, "BloodHUD", bloodCB, NULL, " label='Blood HUD' ");
	TwAddVarRW(menuBar, "BloodHUD", TW_TYPE_BOOLCPP, &stateInfo.blood, " label='Blood HUD' ");
	TwAddVarRW(menuBar, "Banner", TW_TYPE_BOOLCPP, &stateInfo.banner, " label='Banner' ");
	TwAddVarRW(menuBar, "Text", TW_TYPE_BOOLCPP, &stateInfo.text, " label='Text' ");
	TwAddVarRW(menuBar, "Fog", TW_TYPE_BOOLCPP, &stateInfo.fog, " label='Fog' ");
	TwAddVarRW(menuBar, "SunMotion", TW_TYPE_BOOLCPP, &SUN_MOTION_FLAG, " label='Sun Motion' ");
	TwAddButton(menuBar, "staticCam1", stCam1, NULL, " label='Static Camera 1' ");
	TwAddButton(menuBar, "staticCam2", stCam2, NULL, " label='Static Camera 2' ");
	TwAddButton(menuBar, "cameraCurve", curveCam, NULL, " label='Camera Curve' ");
	TwAddVarRW(menuBar, "CarCamera", TW_TYPE_BOOLCPP, &stateInfo.carCameraFlag, " label='Car Camera' ");
	TwAddVarRW(menuBar, "FogHeight", TW_TYPE_FLOAT, &stateInfo.fogHeight, " label='Fog Height' step=0.10 min=0.001");
	TwAddVarRW(menuBar, "CameraAcc", TW_TYPE_FLOAT, &stateInfo.cameraAcc, " label='CameraAcceleration' step=0.10 min=1 max=20");
	TwAddVarRW(menuBar, "Transp", TW_TYPE_FLOAT, &stateInfo.transparency, " label='Transparency' step=0.1 min=0.001 max=1");
	TwAddVarRW(menuBar, "Pos1", TW_TYPE_FLOAT, &objPos.x, " label='Position X' step=0.5 min=-1000 max=1000");
	TwAddVarRW(menuBar, "Pos2", TW_TYPE_FLOAT, &objPos.y, " label='Position Y' step=0.5 min=-1000 max=1000");
	TwAddVarRW(menuBar, "Pos3", TW_TYPE_FLOAT, &objPos.z, " label='Position Z' step=0.5 min=-1000 max=1000");
	TwAddVarRW(menuBar, "Scale", TW_TYPE_FLOAT, &objSc, " label='Scale' step=0.01 min=-100 max=100");
	
	//light toggle
	//TwAddSeparator(menuBar, NULL, " label='Light Toggles' ");
	TwAddVarRW(menuBar, "Flashlight", TW_TYPE_BOOLCPP, &stateInfo.isFlashlight, " label='Flashlight' group='Light Toggles' ");
	TwAddVarRW(menuBar, "DirLight", TW_TYPE_BOOLCPP, &stateInfo.isDirLight, " label='Directional Light' group='Light Toggles' ");
	TwAddVarRW(menuBar, "PointLight", TW_TYPE_BOOLCPP, &stateInfo.isPointLight, " label='Point Light(sun)' group='Light Toggles' ");
	
	TwAddButton(menuBar, "Reload", reloadPropertiesCB, NULL, " label='Reload Settings' ");
}


// -----------------------  Application ---------------------------------

/**
 * \brief Initialize application data and OpenGL stuff.
 */
void initApplication() {
	// init OpenGL
	// - all programs (shaders), buffers, textures, ...
	

	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	loadShaderPrograms();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	//glPixelStore(GL_UNPACK_ALIGNMENT, 1);
	//glUniform1i(commonShaderProgram.locations.sampl, 0);

	//objects.push_back(new Triangle(&commonShaderProgram));
	//objects.push_back(new Cube(&commonShaderProgram));
	/*for (float i = 0; i < 3.9; i += 0.1) {
		objects.push_back(new Cube(i, &commonShaderProgram, glm::vec3(0.0f, 2.5f, 2.5f), "data/cubeTriangulated.obj"));
	}*/

	//objects.push_back(new Cube(&commonShaderProgram, glm::vec3(0.0f, 2.5f, 0.0f)));
	//objects.push_back(new Cube(&commonShaderProgram, glm::vec3(0.0f, 2.5f, 2.5f)));
	//objects.push_back(new Cube(&commonShaderProgram, glm::vec3(0.0f, 2.5f, -2.5f)));
	//objects.push_back(new Cube(&commonShaderProgram, glm::vec3(0.0f, 3.5f, 0.0f)));
	//objects.push_back(new Cube(&commonShaderProgram, glm::vec3(0.0f, 3.5f, 2.5f)));
	//objects.push_back(new Cube(&commonShaderProgram, glm::vec3(0.0f, 3.5f, -2.5f)));
	//objects.push_back(new Cube(&commonShaderProgram, glm::vec3(0.0f, 4.5f, 0.0f)));
	//objects.push_back(new Cube(&commonShaderProgram, glm::vec3(0.0f, 4.5f, 2.5f)));
	//objects.push_back(new Cube(&commonShaderProgram, glm::vec3(0.0f, 4.5f, -2.5f)));
	//objects.push_back(new Cube(&commonShaderProgram, glm::vec3(-2.5f, 0.0f, 0.0f)));
	//objects.push_back(new Cube(&commonShaderProgram, glm::vec3(-2.5f, -2.5f, 0.0f)));
	//objects.push_back(new Cube(&commonShaderProgram, glm::vec3(0.0f, -2.5f, 0.0f)));
	objects.push_back(new ComplexMesh(&commonShaderProgram, glm::vec3(0.0f, 6.5f, 0.0f), "data/car.obj"));
	objects.push_back(new ComplexMesh(&commonShaderProgram, glm::vec3(500.0f, 1.0f, 500.0f), "data/house_textured/house_textured.obj", glm::vec3(0.01f)));
	objects.push_back(new ComplexMesh(&commonShaderProgram, glm::vec3(50.0f, 1.0f, 94.0f), "data/gasCylinders/GasCylinders.obj", glm::vec3(0.1f)));
	objects.push_back(new ComplexMesh(&commonShaderProgram, glm::vec3(163.5f, 1.0f, 141.5f), "data/BlastFurnace02/BlastFurnace02.obj", glm::vec3(0.05f)));
	objects.push_back(new ComplexMesh(&commonShaderProgram, glm::vec3(-4.5f, 0.001f, 4.0f), "data/old_wooden_church/old_wooden_church.obj", glm::vec3(2.0f)));
	objects.push_back(new ComplexMesh(&commonShaderProgram, glm::vec3(220.0f, 0.5f, -150.0f), "data/Gas tower without ad/GasTowerWithoutText.obj", glm::vec3(0.1f)));
	objects.push_back(new ComplexMesh(&commonShaderProgram, glm::vec3(89.0f, -0.5f, 17.5f), "data/GrainStorage02/GrainStorage02.obj", glm::vec3(0.32f)));
	objects.push_back(new ComplexMesh(&commonShaderProgram, glm::vec3(1.0f, 0.0f, -27.0f), "data/HorusiceTrainStation/HorusiceTrainStation.obj", glm::vec3(1.0f)));

	transparent = new DefaultObject(&commonShaderProgram, glm::vec3(2.0f, 0.0f, 3.0f), "data/transparent.obj", "data/transparentMat.png");
	//transparent->textureName = "data/transparentMat.png";
	std::cout << "Transparent Object rendered" << std::endl;
	sun = new Sun(&sunShaderProgram, sunPos);
	//sun->addChildren(new Cube(&commonShaderProgram, glm::vec3(3.0f, 3.0f, 3.0f)));
	std::cout << "Sun rendered" << std::endl;
	Planet* pl = new Planet(&commonShaderProgram, glm::vec3(0.0f), glm::vec3(5,1,2), 3.0f, 1.5f);
	pl->addChildren(new Planet(&commonShaderProgram, glm::vec3(0.0f), glm::vec3(2,0, 10), 2.0f, 2.0f));
	sun->addChildren(pl);
	std::cout << "Planets Rendered" << std::endl;
	car = new Car(&commonShaderProgram, glm::vec3(4.0f, 1.5f, 2.0f));
	std::cout << "Car Rendered" << std::endl;
	car->updateCurve(properties->getKnotsBezier(), properties->getHandlesBezier());
	std::cout << "c" << std::endl;
	terrain = new Terrain(&commonShaderProgram, stateInfo.worldBorderZ, stateInfo.worldBorderX);
	std::cout << "Terrain Generated" << std::endl;
	// objects.push_back(new SingleMesh(&commonShaderProgram));
	//(objects[0])->loadObjFromFile("data/cubeTriangulated.obj");

	movie = new Animation();
	//sm = new SingleMesh();
	// init your Application
	// - setup the initial application state
	characterDraw = new CharactersDraw(&textShaderProgram);

	initMenu();
}

/**
 * \brief Delete all OpenGL objects and application data.
 */
void finalizeApplication(void) {

	// cleanUpObjects();

	// delete buffers
	// cleanupModels();

	// delete shaders
	cleanupShaderPrograms();
}

/**
 * \brief Entry point of the application.
 * \param argc number of command line arguments
 * \param argv array with argument strings
 * \return 0 if OK, <> elsewhere
 */
int main(int argc, char** argv) {

	//init parameters
	stateInfo.fog = false;
	stateInfo.blood = false;
	stateInfo.banner = false;
	stateInfo.menu = false;
	stateInfo.text = false;
	stateInfo.drag = false;
	stateInfo.dragCamera = false;
	stateInfo.freeCamera = false;
	stateInfo.carCameraFlag = false;
	stateInfo.cameraCurve = false;

	stateInfo.staticCam1 = true;
	stateInfo.staticCam2 = false;

	stateInfo.isFlashlight = true;
	stateInfo.isDirLight = false;
	stateInfo.isPointLight = true;

	stateInfo.transparency = 0.3;
	stateInfo.cameraAcc = 2.5f;

	properties = new Properties();
	stateInfo.windowHeight = properties->getWinH();
	stateInfo.windowWidth = properties->getWinW();

	knots = properties->getKnotsCatmull();
	handles = properties->getHandlesCatmull();

	stateInfo.worldBorderX = 50;
	stateInfo.worldBorderY = 0;
	stateInfo.worldBorderYUp = 500;
	stateInfo.worldBorderZ = 50;

	stateInfo.sensetivity = 0.1f;
	stateInfo.sensetivityDrag = 0.01f;
	stateInfo.fogHeight = 3.0f;


	// initialize the GLUT library (windowing system)
	glutInit(&argc, argv);

	//glutInitContextVersion(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitContextVersion(4, 4);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextFlags(GLUT_DEBUG);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);

	//   initial window size + title
	glutInitWindowSize(stateInfo.windowWidth, stateInfo.windowHeight);
	glutCreateWindow(WINDOW_TITLE);

	// initialize pgr-framework (GL, DevIl, etc.)
	//if (!pgr::initialize(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR))
	if (!pgr::initialize(4,4, pgr::DEBUG_LOW))
		pgr::dieWithError("pgr init failed, required OpenGL not supported?");
	TwInit(TW_OPENGL_CORE, NULL);
	TwWindowSize(stateInfo.windowWidth, stateInfo.windowHeight);

	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);

	//CHECK_GL_ERROR();
	// callbacks - use only those you need
	glutDisplayFunc(displayCb);
	glutReshapeFunc(reshapeCb);

	glutKeyboardFunc(keyboardCb);
	glutKeyboardUpFunc(keyboardUpCb);
	glutSpecialFunc(specialKeyboardCb);     // key pressed
	glutSpecialUpFunc(specialKeyboardUpCb); // key released

	glutMouseFunc(mouseCb);
	glutMotionFunc(mouseMotionCb);
	glutPassiveMotionFunc(passiveMouseMotionCb);
	glutIgnoreKeyRepeat(true);
	TwGLUTModifiersFunc(glutGetModifiers);
	//texture = pgr::createTexture(TEST_Texture);


#ifndef SKELETON // @task_1_0
		glutTimerFunc(33, timerCb, 0);
#else
		// glutTimerFunc(33, timerCb, 0);
#endif // task_1_0



	// init your stuff - shaders & program, buffers, locations, state of the application
	initApplication();

	// handle window close by the user
	glutCloseFunc(finalizeApplication);

	// Infinite loop handling the events
	glutMainLoop();

	// code after glutLeaveMainLoop()
	// cleanup

	return EXIT_SUCCESS;
}
