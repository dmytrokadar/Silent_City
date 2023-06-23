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
#include "CharactersDraw.h"
#include "singlemesh.h"
//#include <AntTweakBar.h>
#include <ft2build.h>
#include FT_FREETYPE_H 


constexpr int WINDOW_WIDTH = 500;
constexpr int WINDOW_HEIGHT = 500;
constexpr char WINDOW_TITLE[] = "PGR: Application Skeleton";

// objects
ObjectList objects;
Sun * sun;

// utilities
CharactersDraw* characterDraw;

// shared shader programs
ShaderProgram commonShaderProgram;
ShaderProgram bannerShaderProgram;
ShaderProgram skyboxShaderProgram;
ShaderProgram sunShaderProgram;
ShaderProgram textShaderProgram;

GLuint texture;
GLuint bloodTexture;


// -----------------------  OpenGL stuff ---------------------------------
bool keys[256];

bool ROTATION_FLAG = true;
bool SUN_MOTION_FLAG = true;
bool OVERALAY_FLAG = false;
float CAMERA_SPEED = 0.1f;

float delayFrame;
float lastFrame;

ObjectGeometry* skyboxGeometry;
ObjectGeometry* bloodGeometry;

glm::vec3 cameraPosGlobal = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraLook = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 direction;

glm::vec3 sunPos = glm::vec3(4.0f, 5.0f, 0.0f);

float pitch = 0, yaw = -90.0f;
float lastMouseX = WINDOW_WIDTH / 2, lastMouseY = WINDOW_HEIGHT / 2;
const float sensetivity = 0.1f;

glm::mat4 viewMatrix = glm::mat4(1.0f);
glm::mat4 projectionMatrix = glm::mat4(1.0f);

//TwBar* menuBar;

std::string skyboxFaces[] = {
	"data/Skybox/posx.jpg",
	"data/Skybox/negx.jpg",
	"data/Skybox/posy.jpg",
	"data/Skybox/negy.jpg",
	"data/Skybox/posz.jpg",
	"data/Skybox/negz.jpg"
};

struct StateInfo {
	int windowWidth;
	int windowHeight;

	bool freeCamera;
	int fixedCameraPos;
	float sensetivity;

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
	commonShaderProgram.locations.light = glGetUniformLocation(commonShaderProgram.program, "light");
	commonShaderProgram.locations.lightPos = glGetUniformLocation(commonShaderProgram.program, "lightPos");
	commonShaderProgram.locations.cameraPos = glGetUniformLocation(commonShaderProgram.program, "cameraPos");
	commonShaderProgram.locations.isFog = glGetUniformLocation(commonShaderProgram.program, "isFog");

	assert(commonShaderProgram.locations.PVMmatrix != -1);
	assert(commonShaderProgram.locations.position != -1);
	// ...
	commonShaderProgram.initialized = true;
}

void loadTextShader() {
	// initializing default shader
	GLuint shaders[] = {
		/*pgr::createShaderFromSource(GL_VERTEX_SHADER, vertexShaderSrc),
		pgr::createShaderFromSource(GL_FRAGMENT_SHADER, fragmentShaderSrc),*/
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
		-1.0f,  0.15f, 0.0f, 0.0f, 1.0f,
		  -1.0f, -0.15f, 0.0f, 0.0f, 0.0f,
		   1.0f,  0.15f, 0.0f, 3.0f, 1.0f,
		   1.0f, -0.15f, 0.0f, 3.0f, 0.0f
	};

	bannerShaderProgram.program = pgr::createProgram(shaders);
	bannerShaderProgram.locations.position = glGetAttribLocation(bannerShaderProgram.program, "position");
	bannerShaderProgram.locations.normals = glGetAttribLocation(bannerShaderProgram.program, "normals");
	bannerShaderProgram.locations.texture = glGetAttribLocation(bannerShaderProgram.program, "texCoord");

	// other attributes and uniforms
	bannerShaderProgram.locations.PVMmatrix = glGetUniformLocation(bannerShaderProgram.program, "PVM");
	bannerShaderProgram.locations.model = glGetUniformLocation(bannerShaderProgram.program, "model");
	bannerShaderProgram.locations.sampl = glGetUniformLocation(bannerShaderProgram.program, "texSampler");
	bannerShaderProgram.locations.light = glGetUniformLocation(bannerShaderProgram.program, "light");
	bannerShaderProgram.locations.lightPos = glGetUniformLocation(bannerShaderProgram.program, "lightPos");
	bannerShaderProgram.locations.cameraPos = glGetUniformLocation(bannerShaderProgram.program, "cameraPos");

	assert(bannerShaderProgram.locations.PVMmatrix != -1);
	assert(bannerShaderProgram.locations.position != -1);
	// ...
	bannerShaderProgram.initialized = true;

	// texture setup
	glActiveTexture(GL_TEXTURE3);
	bloodTexture = pgr::createTexture("data/PainHud.png");
	if (bloodTexture == 0) {
		std::cout << "Texture not loaded!" << std::endl;
	}
	CHECK_GL_ERROR();
	glBindTexture(GL_TEXTURE_2D, bloodTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);

	glGenTextures(1, &bloodGeometry->texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, bloodGeometry->texture);

	// buffers 
	glGenBuffers(1, &bloodGeometry->vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, bloodGeometry->vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenRectangle), screenRectangle, GL_DYNAMIC_DRAW);

	glGenVertexArrays(1, &bloodGeometry->vertexArrayObject);
	glBindVertexArray(bloodGeometry->vertexArrayObject);
	glEnableVertexAttribArray(bannerShaderProgram.locations.position);
	glVertexAttribPointer(bannerShaderProgram.locations.position, 2, GL_FLOAT, GL_FALSE, 0, 0);

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

	glActiveTexture(GL_TEXTURE2);
	glGenTextures(1, &skyboxGeometry->texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxGeometry->texture);

	glGenBuffers(1, &skyboxGeometry->vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxGeometry->vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenRectangle), screenRectangle, GL_DYNAMIC_DRAW);

	glGenVertexArrays(1, &skyboxGeometry->vertexArrayObject);
	glBindVertexArray(skyboxGeometry->vertexArrayObject);
	glEnableVertexAttribArray(skyboxShaderProgram.locations.position);
	glVertexAttribPointer(skyboxShaderProgram.locations.position, 2, GL_FLOAT, GL_FALSE, 0, 0);

	for (int i = 0; i < 6; i++) {
		if(!pgr::loadTexImage2D(skyboxFaces[i], GL_TEXTURE_CUBE_MAP_POSITIVE_X+i))
			std::cout << "Failed loading " << skyboxFaces[i] << std::endl;
	}

  	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	
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
	//loadBannerShader();
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
	glBindVertexArray(bloodGeometry->vertexArrayObject);
	glUseProgram(bannerShaderProgram.program);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, bloodGeometry->texture);
	glBindVertexArray(bloodGeometry->vertexArrayObject);
	CHECK_GL_ERROR();

	glUniformMatrix4fv(bannerShaderProgram.locations.PVMmatrix, 1, GL_FALSE, glm::value_ptr(glm::inverse(projectionMatrix * viewMatrix * glm::mat4(1.0f))));
	glUniform1i(bannerShaderProgram.locations.sampl, 2);
	//glUniformMatrix4fv(skyboxShaderProgram.locations.PVMmatrix, 1, GL_FALSE, glm::value_ptr(invPV));
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);
	glUseProgram(0);
}

/**
 * \brief Draw all scene objects.
 */
void drawScene(void)
{
	float time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	delayFrame = time - lastFrame;
	lastFrame = time;

	CAMERA_SPEED = 2.5f * delayFrame;
	const float radius = 10.0f;

	if (SUN_MOTION_FLAG) {
		float sunX = sin(time) * radius;
		float sunY = cos(time) * radius;

		sunPos.x = sunX;
		sunPos.y = sunY;

		sun->changePosition(sunPos);
	}


	if (ROTATION_FLAG) {
		float camX = sin(time) * radius;
		float camZ = cos(time) * radius;

		//std::cout << glutGet(GLUT_ELAPSED_TIME) << std::endl;

		cameraPosGlobal = glm::vec3(camX / 2, 3.0, camZ / 2);
		viewMatrix = glm::lookAt(glm::vec3(camX / 2, 3.0, camZ / 2), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	}
	else {
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraLook = glm::normalize(direction);

		viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraLook, cameraUp);
		cameraPosGlobal = cameraPos;
	}
	projectionMatrix = glm::perspective(glm::radians(60.0f), float(glutGet(GLUT_WINDOW_WIDTH)) / float(glutGet(GLUT_WINDOW_HEIGHT)), 0.1f, 100.0f);
	for (ObjectInstance* object : objects) {   // for (auto object : objects) {
		if (object != nullptr)
			object->draw(viewMatrix, projectionMatrix, glm::vec3(1.0f, 1.0f, 1.0f), sunPos, cameraPosGlobal);
	}
	sun->draw(viewMatrix, projectionMatrix);

	glUniform1i(commonShaderProgram.locations.sampl, 0);
	glActiveTexture(GL_TEXTURE0);

	drawSkybox();
	characterDraw->draw("test Text", 100.0f, 100.0f, 1.0f, glm::vec3(1.0f));
	//drawBanner();
}


// -----------------------  Window callbacks ---------------------------------

/**
 * \brief Draw the window contents.
 */
void displayCb() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// draw the window contents (scene objects)
	drawScene();

	//TwDraw();
	glDisable(GL_BLEND);
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


	switch (keyPressed)
	{
		case 27://esc
			glutLeaveMainLoop();
			exit(EXIT_SUCCESS);
			break;
		case 'c'://c
			std::cout << "C pressed" << std::endl;
			ROTATION_FLAG = !ROTATION_FLAG;
			break;
		case 'l':
			std::cout << "Sun stopped" << std::endl;
			SUN_MOTION_FLAG = !SUN_MOTION_FLAG;
			break;
		default:
			break;
	}
	keys[keyPressed] = true;
	
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
}

/**
 * \brief Handle mouse dragging (mouse move with any button pressed).
 *        This event follows the glutMouseFunc(mouseCb) event.
 * \param mouseX mouse (cursor) X position
 * \param mouseY mouse (cursor) Y position
 */
void mouseMotionCb(int mouseX, int mouseY) {

}

/**
 * \brief Handle mouse movement over the window (with no button pressed).
 * \param mouseX mouse (cursor) X position
 * \param mouseY mouse (cursor) Y position
 */
void passiveMouseMotionCb(int mouseX, int mouseY) {

	// mouse hovering over window

	// create display event to redraw window contents if needed (and not handled in the timer callback)
	// glutPostRedisplay();

	if (!ROTATION_FLAG) {
		//inspitation https://learnopengl.com/Getting-started/Camera
		/*std::cout << "Mouse x: " << mouseX << std::endl;
		std::cout << "Mouse y: " << mouseY << std::endl;*/

		float offsetX = mouseX - WINDOW_WIDTH / 2;
		float offsetY = mouseY - WINDOW_HEIGHT / 2;

		offsetX *= sensetivity;
		offsetY *= sensetivity;

		yaw += offsetX;
		pitch -= offsetY;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glutWarpPointer(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
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
#endif // task_1_0

	//inspiration https://learnopengl.com/Getting-started/Camera
	if (keys['w']) {
		//std::cout << "w pressed" << std::endl;
		cameraPos += CAMERA_SPEED * cameraLook;
	}

	if (keys['s']) {
		//std::cout << "s pressed" << std::endl;
		cameraPos -= CAMERA_SPEED * cameraLook;
	}

	if (keys['d']) {
		//std::cout << "d pressed" << std::endl;
		cameraPos += CAMERA_SPEED * glm::normalize(glm::cross(cameraLook, cameraUp));
	}

	if (keys['a']) {
			//std::cout << "a pressed" << std::endl;
			cameraPos -= CAMERA_SPEED * glm::normalize(glm::cross(cameraLook, cameraUp));
	}

	// and plan a new event
	glutTimerFunc(33, timerCb, 0);

	// create display event
	glutPostRedisplay();
}


// -----------------------  Application ---------------------------------

/**
 * \brief Initialize application data and OpenGL stuff.
 */
void initApplication() {
	// init OpenGL
	// - all programs (shaders), buffers, textures, ...

	loadShaderPrograms();
	glEnable(GL_DEPTH_TEST);
	//glPixelStore(GL_UNPACK_ALIGNMENT, 1);
	//glUniform1i(commonShaderProgram.locations.sampl, 0);

	//objects.push_back(new Triangle(&commonShaderProgram));
	//objects.push_back(new Cube(&commonShaderProgram));
	for (float i = 0; i < 3.9; i += 0.1) {
		objects.push_back(new Cube(&commonShaderProgram, glm::vec3(0.0f, 2.5f, 2.5f), "data/cubeTriangulated.obj", i));
	}

	/*objects.push_back(new Cube(&commonShaderProgram, glm::vec3(0.0f, 2.5f, 0.0f)));
	objects.push_back(new Cube(&commonShaderProgram, glm::vec3(0.0f, 2.5f, 2.5f)));
	objects.push_back(new Cube(&commonShaderProgram, glm::vec3(0.0f, 2.5f, -2.5f)));
	objects.push_back(new Cube(&commonShaderProgram, glm::vec3(-2.5f, 0.0f, 0.0f)));
	objects.push_back(new Cube(&commonShaderProgram, glm::vec3(-2.5f, -2.5f, 0.0f)));
	objects.push_back(new Cube(&commonShaderProgram, glm::vec3(0.0f, -2.5f, 0.0f)));*/
	sun = new Sun(&sunShaderProgram, sunPos);
	// objects.push_back(new SingleMesh(&commonShaderProgram));
	//(objects[0])->loadObjFromFile("data/cubeTriangulated.obj");

	// init your Application
	// - setup the initial application state
	characterDraw = new CharactersDraw(&textShaderProgram);
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

	// initialize the GLUT library (windowing system)
	glutInit(&argc, argv);

	glutInitContextVersion(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	// for each window
	{
		//   initial window size + title
		glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
		glutCreateWindow(WINDOW_TITLE);

		/*TwInit(TW_OPENGL, NULL);
		TwWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);*/
		//menuBar = TwNewBar("Menu");
		//TwDefine("Menu size='240 420' color='20 20 20' fontsize=3");

		//CHECK_GL_ERROR();
		// callbacks - use only those you need
		glutDisplayFunc(displayCb);
		glutReshapeFunc(reshapeCb);
		glutKeyboardFunc(keyboardCb);
		 glutKeyboardUpFunc(keyboardUpCb);
		// glutSpecialFunc(specialKeyboardCb);     // key pressed
		 glutSpecialUpFunc(specialKeyboardUpCb); // key released
		 glutMouseFunc(mouseCb);
		 glutMotionFunc(mouseMotionCb);
		 glutPassiveMotionFunc(passiveMouseMotionCb);
		 glutIgnoreKeyRepeat(true);
		 //texture = pgr::createTexture(TEST_Texture);
		 

#ifndef SKELETON // @task_1_0
		glutTimerFunc(33, timerCb, 0);
#else
		// glutTimerFunc(33, timerCb, 0);
#endif // task_1_0

	}
	// end for each window 

	// initialize pgr-framework (GL, DevIl, etc.)
	if (!pgr::initialize(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR))
		pgr::dieWithError("pgr init failed, required OpenGL not supported?");

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
