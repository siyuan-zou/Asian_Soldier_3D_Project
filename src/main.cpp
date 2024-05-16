// OpenGL Loader - GLAD should be included before GLFW
// GLAD: https://github.com/Dav1dde/glad
// GLAD files generated from https://glad.dav1d.de/
#include "../third_party/src/glad/include/glad/glad.hpp"

// Include GLFW
#include <GLFW/glfw3.h>

// Update GLFW variables for some system
#ifndef GLFW_TRUE
#define GLFW_TRUE 1
#define GLFW_FALSE 0
#endif

#include <iostream>
#include <vector>

// #include "io/keyboard.h"
// #include "io/mouse.h"
#include "control/joystick.hpp"
#include "control/Camera.h"
#include "control/Light.h"
// #include "control/camera.hpp"
// #include "io/camera.h"

#include "load_obj/OBJLoader.h"
#include "load_obj/Model.h"
// #include "load_obj/Texture.h"
#include "load_obj/Shader.h"
// #include "load_obj/Material.h"
#include "load_obj/Vertex.h"
#include "load_obj/Mesh.h"

#include "helper_opengl.hpp"
#include "Environment.hpp"
// #include "load_model/model.hpp"

/*
Global Variables
*/

// Window dimensions
int framebufferWidth = 800;
int framebufferHeight = 600;

// Delta time
float dt;
float curTime;
float lastTime;

// Mouse Input
double lastMouseX;
double lastMouseY;
double mouseX;
double mouseY;
double mouseOffsetX;
double mouseOffsetY;
bool firstMouse;

// Camera
Camera camera;

// Matrices
glm::mat4 ViewMatrix;
glm::vec3 camPosition;
glm::vec3 worldUp;
glm::vec3 camFront;

glm::mat4 ProjectionMatrix;
float fov;
float nearPlane;
float farPlane;

// Shaders
std::vector<Shader *> shaders;

// Textures
std::vector<Texture *> textures;

// Materials
std::vector<Material *> materials;

// Models
std::vector<Model *> models;

// Lights
std::vector<PointLight *> pointLights;

GLFWwindow *create_window_using_glfw(int width, int height);
void glfw_error_callback(int error, const char *description);

void initMatrices();
void initShaders();
void initTextures();
void initMaterials();

void initUniforms();

void updateDt();
void updateKeyboardInput(GLFWwindow *window);
void updateMouseInput(GLFWwindow *window);

int main(int, char *argv[])
{
	// Debug message (name of the program)
	std::cout << "Run " << argv[0] << std::endl;

	// Create the window using GLFW
	GLFWwindow *window = create_window_using_glfw(framebufferWidth, framebufferHeight);

	// ************************************************* //
	//             1 - Setup/Load the shaders            //
	// ************************************************* //
	// GLuint shader = opengl_load_shader("shaders/vertex_core.glsl", "shaders/fragment_core.glsl");

	// ************************************************* //
	//           2 - Prepare and send data to GPU        //
	// ************************************************* //

	std::cout << "*** Setup Data ***" << std::endl;

	initMatrices();
	initShaders();
	initUniforms();

	/*
	Background
	*/

	// glEnable(GL_DEPTH_TEST);

	Environment env;
	env.init();

	std::vector<Vertex> mesh = loadOBJ("assets/horse.obj");
	Mesh horse(mesh.data(), mesh.size(), NULL, 0, glm::vec3(1.f, 0.f, 0.f),
			   glm::vec3(0.f),
			   glm::vec3(0.f),
			   glm::vec3(1.f));

	horse.move(glm::vec3(0.f, 0.f, 0.f));
	horse.setOrigin(glm::vec3(0.f, 0.f, 0.f));

	// ******************************** //
	// Display loop
	// ******************************** //

	std::cout
		<< "\nStart display loop ..." << std::endl;
	while (!glfwWindowShouldClose(window)) // loop as long as the window is not closed
	{
		updateDt();
		glfwPollEvents(); // poll GLFW events
		updateKeyboardInput(window);
		updateMouseInput(window);

		camera.updateInput(dt, -1, mouseOffsetX, mouseOffsetY);

		// ************************************************* //
		//           3 - Displaying Data                     //
		// ************************************************* //
		env.render(shader);
		horse.render(shader);

		// End of frame
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

/*
function to process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
*/

GLFWwindow *create_window_using_glfw(int width, int height)
{
	// Set GLFW callback to catch and display error
	glfwSetErrorCallback(glfw_error_callback);

	// Initialize GLFW
	int const glfw_init_value = glfwInit();
	if (glfw_init_value != GLFW_TRUE)
	{
		std::string s = "\nError: Failed to Initialize GLFW.\n";
		abort();
	}

	// Set GLFW parameter before creating the window
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);			   // change to GLFW_OPENGL_ES_API for WebGL
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Use only modern OpenGL
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);				   // Expect OpenGL 3.3 or greater
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE); // Required for MacOS
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);  // Allow possible debug

	glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);   // Take focus when created
	glfwWindowHint(GLFW_SAMPLES, 8);		   // Multisampling
	glfwWindowHint(GLFW_FLOATING, GLFW_FALSE); // Windows is not always on top

#ifdef __APPLE__
	glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE); // To avoid HiDPI issues with pixel size on Mac
#endif

	// Actual creation of the window
	GLFWwindow *window = glfwCreateWindow(width, height, "GLFW Window", nullptr, nullptr);

	// Check that the window is created
	if (window == nullptr)
	{
		std::cerr << "Failed to create GLFW Window" << std::endl;
		abort();
	}
	std::cout << "\nSuccessfully created a window of size " << width << " x " << height << std::endl;

	// Set an OpenGL context for this window
	glfwMakeContextCurrent(window);

	// Initialize GLAD to get access to OpenGL functions
	//   - while GLFW is used to create a window,
	//      another initialization is needed to get access to the correct OpenGL version.
	//      We use the library GLAD to get the correct OpenGL functions.
	const int glad_init_value = gladLoadGL();
	if (glad_init_value == 0)
	{
		std::cout << "Failed to Init GLAD" << std::endl;
		abort();
	}

	// Display debug information on command line
	std::cout << "\nDebug information on OpenGL version" << std::endl;
	std::cout << "\t[VENDOR]      : " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "\t[RENDERDER]   : " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "\t[VERSION]     : " << glGetString(GL_VERSION) << std::endl;
	std::cout << "\t[GLSL VERSION]: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	return window;
}

void glfw_error_callback(int error, const char *description)
{
	std::cerr << "Received GLFW error" << std::endl;
	std::cerr << "\t Error " << " (" << error << ")" << std::endl;
	std::cerr << "\t Description - " << description << std::endl;
}
void initMatrices()
{
	ViewMatrix = glm::mat4(1.f);
	ViewMatrix = glm::lookAt(camPosition, camPosition + camFront, worldUp);

	ProjectionMatrix = glm::mat4(1.f);
	ProjectionMatrix = glm::perspective(
		glm::radians(fov),
		static_cast<float>(framebufferWidth) / framebufferHeight,
		nearPlane,
		farPlane);
}

void initShaders()
{
	shaders.push_back(new Shader("shaders/vertex_core.glsl", "shaders/fragment_core.glsl"));
}

void initTextures()
{
	// textures.push_back(new Texture("images/wood.jpg", GL_TEXTURE_2D));
	// textures.push_back(new Texture("images/container.jpg", GL_TEXTURE_2D));
}

void initMaterials()
{
	// materials.push_back(new Material(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), 0, 1));
}

void initUniforms()
{
	shaders[0]->setMat4fv(ViewMatrix, "ViewMatrix");
	shaders[0]->setMat4fv(ProjectionMatrix, "ProjectionMatrix");
	// shaders[0]->setVec3f(camPosition, "cameraPos");
	for (size_t i = 0; i < pointLights.size(); i++)
	{
		pointLights[i]->sendToShader(*shaders[0], i);
	}
}

void updateDt()
{
	curTime = static_cast<float>(glfwGetTime());
	dt = curTime - lastTime;
	lastTime = curTime;
}
void updateKeyboardInput(GLFWwindow *window);
{
	// Program
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	// Camera
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.move(dt, FORWARD);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.move(dt, BACKWARD);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.move(dt, LEFT);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.move(dt, RIGHT);
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	{
		camPosition.y -= 0.05f;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		camPosition.y += 0.05f;
	}
}
void updateMouseInput(GLFWwindow *window);
{
	glfwGetCursorPos(window, &mouseX, &mouseY);

	if (firstMouse)
	{
		lastMouseX = mouseX;
		lastMouseY = mouseY;
		firstMouse = false;
	}

	// Calc offset
	mouseOffsetX = mouseX - lastMouseX;
	mouseOffsetY = lastMouseY - mouseY;

	// Set last X and Y
	lastMouseX = mouseX;
	lastMouseY = mouseY;

	// Move light
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
	{
		pointLights[0]->setPosition(camera.getPosition());
	}
}
