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
#include "singlemesh.h"


constexpr int WINDOW_WIDTH = 500;
constexpr int WINDOW_HEIGHT = 500;
constexpr char WINDOW_TITLE[] = "PGR: Application Skeleton";

ObjectList objects;

// shared shader programs
ShaderProgram commonShaderProgram;

GLuint texture;


// -----------------------  OpenGL stuff ---------------------------------
bool keys[256];

bool ROTATION_FLAG = true;
float CAMERA_SPEED = 0.1f;

float delayFrame;
float lastFrame;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraLook = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 direction;

float pitch = 0, yaw = -90.0f;
float lastMouseX = WINDOW_WIDTH / 2, lastMouseY = WINDOW_HEIGHT / 2;
const float sensetivity = 0.1f;

glm::mat4 viewMatrix = glm::mat4(1.0f);
glm::mat4 projectionMatrix = glm::mat4(1.0f);

struct StateInfo {
	int windowWidth;
	int windowHeight;

	bool freeCamera;
	int fixedCameraPos;
	float sensetivity;

} stateInfo;

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

	GLuint shaders[] = {
	  /*pgr::createShaderFromSource(GL_VERTEX_SHADER, vertexShaderSrc),
	  pgr::createShaderFromSource(GL_FRAGMENT_SHADER, fragmentShaderSrc),*/
	  pgr::createShaderFromFile(GL_VERTEX_SHADER, "default.vert"),
	  pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "default.frag"),

	  0
	};

	commonShaderProgram.program = pgr::createProgram(shaders);
	commonShaderProgram.locations.position = glGetAttribLocation(commonShaderProgram.program, "position");

	// other attributes and uniforms
	commonShaderProgram.locations.PVMmatrix = glGetUniformLocation(commonShaderProgram.program, "PVM");
	commonShaderProgram.locations.sampl = glGetUniformLocation(commonShaderProgram.program, "sampl");

	assert(commonShaderProgram.locations.PVMmatrix != -1);
	assert(commonShaderProgram.locations.position != -1);
	// ...

	commonShaderProgram.initialized = true;
}

/**
 * \brief Delete all shader program objects.
 */
void cleanupShaderPrograms(void) {

	pgr::deleteProgramAndShaders(commonShaderProgram.program);
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

	if (ROTATION_FLAG) {
		const float radius = 10.0f;
		float camX = sin(time) * radius;
		float camZ = cos(time) * radius;

		//std::cout << glutGet(GLUT_ELAPSED_TIME) << std::endl;

		viewMatrix = glm::lookAt(glm::vec3(camX / 2, 3.0, camZ / 2), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	}
	else {
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraLook = glm::normalize(direction);

		viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraLook, cameraUp);
	}
	projectionMatrix = glm::perspective(glm::radians(60.0f), float(glutGet(GLUT_WINDOW_WIDTH)) / float(glutGet(GLUT_WINDOW_HEIGHT)), 0.1f, 100.0f);
	for (ObjectInstance* object : objects) {   // for (auto object : objects) {
		if (object != nullptr)
			object->draw(viewMatrix, projectionMatrix);
	}

	glUniform1i(commonShaderProgram.locations.sampl, 0);
	glActiveTexture(GL_TEXTURE0);

}


// -----------------------  Window callbacks ---------------------------------

/**
 * \brief Draw the window contents.
 */
void displayCb() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// draw the window contents (scene objects)
	drawScene();

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

	//glUniform1i(commonShaderProgram.locations.sampl, 0);

	//objects.push_back(new Triangle(&commonShaderProgram));
	objects.push_back(new Cube(&commonShaderProgram));
	// objects.push_back(new SingleMesh(&commonShaderProgram));

	// init your Application
	// - setup the initial application state
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
