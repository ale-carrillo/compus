#include <iostream>
#include <cmath>

#include<stdlib.h>

#include<time.h>
#include<chrono>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"
#include "modelAnim.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void animacion();

// Window dimensions
const GLuint WIDTH = 1920, HEIGHT = 1080;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 6.0f, 15.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
float range = 0.0f;
float rot = 0.0f;
float movCamera = 0.0f;


// Animaciones

// Rotación de planetas
float rotPlaneta = 0.0f;

// Rotación de marcianito
float rotMarcianoCamara = 0.0f;

// Planeta Tierra
float rotTierraX = 0.0f;
float rotTierraY = 0.0f;
float rotTierraZ = 0.0f;
float escTierra = 1.0f;

// Planeta Marte
float rotMarteX = 0.0f;
float rotMarteY = 0.0f;
float rotMarteZ = 0.0f;
float escMarte = 1.0f;

// Planeta Saturno
float rotSaturnoX = 0.0f;
float rotSaturnoY = 0.0f;
float rotSaturnoZ = 0.0f;
float escSaturno = 1.0f;

// Luna
float tierraCentroX = -18.0f;
float tierraCentroZ = -10.0f;
float radioLuna = 2.0f;
float anguloLuna = 0.0f;
float lunaX, lunaZ;

// Estrella 1
float starIniY = 10.0f;
float starRadio = 8.0f;
float starRadioY = 2.5f;
float starAngulo = 70.0f;
float starAnguloY = 0.0f;
float starAnguloInc = 0.25f;
float starAnguloYInc = 0.5f;
float starX, starZ, starY;

// Estrella 2
float starRadio2 = 10.0f;
float starX2, starZ2, starY2;

// Nave
float naveIniY = 13.0f;
float naveRadio = 5.0f;
float naveRadioY = 1.25f;
float naveAngulo = 0.0f;
float naveAnguloY = 0.0f;
float naveAnguloInc = 0.3f;
float naveAnguloYInc = 0.5f;
float naveX, naveZ, naveY;
float naveAnguloTambaleo = 0.0f;
float naveRotApertura = 30.0f;
float naveRotInc = 0.5f;
float naveRotX, naveRotZ;

// Meteoro
float meteoroIniY = 13.0f;
float meteoroRadio = 8.0f;
float meteoroRadioY = 1.75f;
float meteoroAngulo = 60.0f;
float meteoroAnguloY = 0.0f;
float meteoroAnguloInc = 0.2f;
float meteoroAnguloYInc = 0.1f;
float meteoroX, meteoroZ, meteoroY;


// Tiro parabólico del usuario
float tIncUsuario = 0.004f;
float v0Usuario = 2.0f;
float anguloUsuario = camera.GetPitch();
float gUsuario = 9.81f;
float anguloXZUsuario = camera.GetYaw();
float tUsuario = 0.0f;
float z0Usuario, x0Usuario, y0Usuario;
float yFinalUsuario = 4.2f;
float xUsuario = x0Usuario, zUsuario = z0Usuario, yUsuario = y0Usuario;
float rotUsuarioX, rotUsuarioY, rotUsuarioZ;
bool puedeTirarUsuario, enTiroUsuario, punto, mostrarControlesTiro;
float tempo = 0.0f;
float canastaX, canastaY, canastaZ, canastaRadio = 0.8f;

// Para todos los tiros parabólicos
float tIncGlobal = 0.004f;
float v0Global = 2.0f;
float anguloGlobal = 15.0f;

// Tiro parabólico en la Tierra
float gTierra = 9.81f;
float v0Tierra = v0Global;
float anguloTierra = anguloGlobal;
float anguloXZTierra = -30.0f;
float tTierra = 0.0f;
float z0Tierra = -7.0f;
float x0Tierra = -16.0f;
float y0Tierra = 5.0f;
float yFinalTierra = 4.2f;
float xTierra, zTierra, yTierra;

// Tiro parabólico en Marte
float gMarte = 3.711f;
float v0Marte = v0Global;
float anguloMarte = anguloGlobal;
float anguloXZMarte = -70.0f;
float tMarte = 0.0f;
float z0Marte = -16.0f;
float x0Marte = -7.5f;
float y0Marte = 5.0f;
float yFinalMarte = 4.2f;
float xMarte, zMarte, yMarte;

// Tiro parabólico en Saturno
float gSaturno = 10.44f;
float v0Saturno = v0Global;
float anguloSaturno = anguloGlobal;
float anguloXZSaturno = 210.0f;
float tSaturno = 0.0f;
float z0Saturno = -10.0f;
float x0Saturno = 15.0f;
float y0Saturno = 5.0f;
float yFinalSaturno = 4.2f;
float xSaturno, zSaturno, ySaturno;


// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
glm::vec3 PosIni(-95.0f, 1.0f, -45.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);

bool active;


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

// Keyframes
float posX = PosIni.x, posY = PosIni.y, posZ = PosIni.z, rotRodIzq = 0;

#define MAX_FRAMES 9
int i_max_steps = 190;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float incX;		//Variable para IncrementoX
	float incY;		//Variable para IncrementoY
	float incZ;		//Variable para IncrementoZ
	float rotRodIzq;
	float rotInc;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(starX,starY,starZ),
	glm::vec3(starX2,starY2,starZ2),
	glm::vec3(0,60,0),
	glm::vec3(0,60,0)
};

glm::vec3 LuzEstrella = glm::vec3(1.0f, 1.0f, 0.0f);


void saveFrame(void)
{

	//printf("posx %f\n", posX);

	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;

	KeyFrame[FrameIndex].rotRodIzq = rotRodIzq;


	FrameIndex++;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotRodIzq = KeyFrame[0].rotRodIzq;

}

void interpolation(void)
{

	KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	KeyFrame[playIndex].rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;

}

void reubicarCanasta() {
	float num = rand();
	canastaX = glm::sin(glm::radians(num - num / 360.0f)) * 6.0f;
	canastaY = (6000 + rand() % (9001 - 6000)) / 1000.0f;
	num = rand();
	canastaZ = glm::sin(glm::radians(num - num / 360.0f)) * 6.0f;
	//printf("x = %f, y = %f, z = %f", canastaX, canastaY, canastaZ);
}


bool verificarPunto() {
	return sqrtf(pow(xUsuario - canastaX, 2) + pow(zUsuario - canastaZ, 2)) < canastaRadio
		&& yUsuario >= canastaY - 0.3f && yUsuario <= canastaY + 0.3f;


}


int main()
{
	srand(time(NULL));
	reubicarCanasta();
	// Init GLFW
	glfwInit();




	// Set all the required options for GLFW
	/*(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "P L A N E T A R I O", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	//printf("%f", glfwGetTime());

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");
	Shader SkyBoxshader("Shaders/SkyBox.vs", "Shaders/SkyBox.frag");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.frag");

	// Fachada
	Model Fachada((char*)"Models/Fachada/Fachada.obj");
	Model Transparente((char*)"Models/Fachada/Transparente.obj");

	// Plantetas
	Model Saturno((char*)"Models/Planetas/Saturno/Saturno.obj");
	Model Tierra((char*)"Models/Planetas/Tierra/Tierra.obj");
	Model Marte((char*)"Models/Planetas/Marte/marte.obj");
	Model Luna((char*)"Models/Planetas/Luna/Luna.obj");
	Model Sol((char*)"Models/Planetas/Sol/Sol.obj");

	// Objetos
	Model Planta((char*)"Models/Objetos/Planta/Planta.obj");
	Model Estrella((char*)"Models/Objetos/Estrella/Estrella.obj");
	Model Cohete((char*)"Models/Objetos/Cohete/Cohete.obj");
	Model LanzaPelotas((char*)"Models/Objetos/LanzaPelotas/LanzaPelotas.obj");
	Model Pelota((char*)"Models/Objetos/Pelota/Pelota.obj");
	Model PelotaBask((char*)"Models/Objetos/Pelota/PelotaBask.obj");
	Model Canasta((char*)"Models/Objetos/Canasta/canasta.obj");
	Model Lampara((char*)"Models/Objetos/Lampara/Lampara.obj");
	Model Nave((char*)"Models/Objetos/Nave/Nave.obj");
	Model Robot((char*)"Models/Objetos/Robot/robot.obj");
	Model Bandera((char*)"Models/Objetos/Bandera/bandera.obj");
	Model Meteorito((char*)"Models/Objetos/Meteorito/meteorito.obj");
	Model Telescopio((char*)"Models/Objetos/Telescopio/telescopio.obj");

	Model LetreroMercurio((char*)"Models/Objetos/Letrero/letreroMercurio.obj");
	Model LetreroVenus((char*)"Models/Objetos/Letrero/letreroVenus.obj");
	Model LetreroTierra((char*)"Models/Objetos/Letrero/letreroTierra.obj");
	Model LetreroMarte((char*)"Models/Objetos/Letrero/letreroMarte.obj");
	Model LetreroJupiter((char*)"Models/Objetos/Letrero/letreroJupiter.obj");
	Model LetreroSaturno((char*)"Models/Objetos/Letrero/letreroSaturno.obj");
	Model LetreroUrano((char*)"Models/Objetos/Letrero/letreroUrano.obj");
	Model LetreroNeptuno((char*)"Models/Objetos/Letrero/letreroNeptuno.obj");

	// info
	Model LetreroMercurioInf((char*)"Models/Objetos/Informacion/letreroMercurioInf.obj");
	Model LetreroVenusInf((char*)"Models/Objetos/Informacion/letreroVenusInf.obj");
	Model LetreroTierraInf((char*)"Models/Objetos/Informacion/letreroTierraInf.obj");
	Model LetreroMarteInf((char*)"Models/Objetos/Informacion/letreroMarteInf.obj");
	Model LetreroJupiterInf((char*)"Models/Objetos/Informacion/letreroJupiterInf.obj");
	Model LetreroSaturnoInf((char*)"Models/Objetos/Informacion/letreroSaturnoInf.obj");
	Model LetreroUranoInf((char*)"Models/Objetos/Informacion/letreroUranoInf.obj");
	Model LetreroNeptunoInf((char*)"Models/Objetos/Informacion/letreroNeptunoInf.obj");

	Model LetreroPunto((char*)"Models/Objetos/Informacion/letreroPunto.obj");
	Model LetreroControlesTiro((char*)"Models/Objetos/Informacion/letreroVerMenu.obj");
	Model LetreroVerControlesTiro((char*)"Models/Objetos/Indicaciones/opciones.obj");
	Model LetreroEsc((char*)"Models/Objetos/Indicaciones/esc.obj");

	Model LetreroTierraInd((char*)"Models/Objetos/Indicaciones/tierraInd.obj");
	Model LetreroMarteInd((char*)"Models/Objetos/Indicaciones/marteInd.obj");
	Model LetreroSaturnoInd((char*)"Models/Objetos/Indicaciones/saturnoInd.obj");

	Model Letrero((char*)"Models/Objetos/Letrero/letrero.obj");
	Model Marciano((char*)"Models/Objetos/Marciano/marciano.obj");

	Model TextoMarciano((char*)"Models/Objetos/Marciano/marcianoTexto.obj");


	// Build and compile our shader program

	//Inicialización de KeyFrames

	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].incX = 0;
		KeyFrame[i].incY = 0;
		KeyFrame[i].incZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].rotInc = 0;
	}



	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] =
	{
		// Positions            // Normals              // Texture Coords
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,  	1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f
	};


	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};


	GLuint indices[] =
	{  // Note that we start from 0!
		0,1,2,3,
		4,5,6,7,
		8,9,10,11,
		12,13,14,15,
		16,17,18,19,
		20,21,22,23,
		24,25,26,27,
		28,29,30,31,
		32,33,34,35
	};

	// Positions all containers
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Texture Coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0); // Note that we skip over the other data in our buffer object (we don't need the normals/textures, only positions).
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);


	//SkyBox
	GLuint skyboxVBO, skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	// Load textures
	vector<const GLchar*> faces;
	faces.push_back("SkyBox/right.tga");
	faces.push_back("SkyBox/left.tga");
	faces.push_back("SkyBox/top.tga");
	faces.push_back("SkyBox/bottom.tga");
	faces.push_back("SkyBox/back.tga");
	faces.push_back("SkyBox/front.tga");

	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		animacion();


		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 20.0f);
		// == ==========================
		// Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
		// the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
		// by defining light types as classes and set their values in there, or by using a more efficient uniform approach
		// by using 'Uniform buffer objects', but that is something we discuss in the 'Advanced GLSL' tutorial.
		// == ==========================
		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);


		// Point light 1
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), LuzEstrella.x, LuzEstrella.y, LuzEstrella.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), LuzEstrella.x, LuzEstrella.y, LuzEstrella.z);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 1.8f);



		// Point light 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), LuzEstrella.x, LuzEstrella.y, LuzEstrella.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), LuzEstrella.x, LuzEstrella.y, LuzEstrella.z);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 1.8f);

		// Point light 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 0.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.032f);

		// Point light 4
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 1.0f, 0.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 1.0f, 0.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.032f);

		// SpotLight
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), 0, 60, 0);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), 0,90,0);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();


		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Bind diffuse map
		//glBindTexture(GL_TEXTURE_2D, texture1);*/

		// Bind specular map
		/*glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);*/


		glBindVertexArray(VAO);
		glm::mat4 tmp = glm::mat4(1.0f); //Temp

		view = camera.GetViewMatrix();

		// C A R G A D E M O D E L O S
		
		// F A C H A D A
		glm::mat4 model(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		Fachada.Draw(lightingShader);


		// P L A N E T A S

		// Sol
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 18.0f, 0.f));
		model = glm::rotate(model, glm::radians(rotPlaneta), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		Sol.Draw(lightingShader);

		// Tierra
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(tierraCentroX, 6.0f, tierraCentroZ));
		model = glm::scale(model, glm::vec3(escTierra, escTierra, escTierra));
		model = glm::rotate(model, glm::radians(rotTierraX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotTierraY), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotTierraZ), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		Tierra.Draw(lightingShader);

		// Marte
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-7.50f, 6.0f, -19.f));
		model = glm::scale(model, glm::vec3(escMarte, escMarte, escMarte));
		model = glm::rotate(model, glm::radians(rotMarteX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotMarteY), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotMarteZ), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		Marte.Draw(lightingShader);

		// Saturno
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(18.0f, 6.0f, -10.f));
		model = glm::scale(model, glm::vec3(escSaturno, escSaturno, escSaturno));
		model = glm::rotate(model, glm::radians(rotSaturnoX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotSaturnoY), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotSaturnoZ), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		Saturno.Draw(lightingShader);

		// l u n a
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(lunaX, 6.0f, lunaZ));
		model = glm::rotate(model, glm::radians(rotPlaneta), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		Luna.Draw(lightingShader);

		// O B J E T O S

		// Planta
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(3.5f, 4.25f, 9.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		Planta.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-3.5f, 4.25f, 9.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		Planta.Draw(lightingShader);

		// Estrella 1
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(starX, starY, starZ));
		model = glm::rotate(model, glm::radians(starAngulo * 2), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		Estrella.Draw(lightingShader);

		// Estrella 2
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(starX2, starY2, starZ2));
		model = glm::rotate(model, glm::radians(starAngulo * 2), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		Estrella.Draw(lightingShader);

		// Cohete
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 5.1f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		Cohete.Draw(lightingShader);

		// LanzaPelotas (Tierra)
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(x0Tierra, y0Tierra, z0Tierra));
		model = glm::rotate(model, glm::radians(60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		LanzaPelotas.Draw(lightingShader);

		// Pelota (Tierra)
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(xTierra, yTierra, zTierra));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		Pelota.Draw(lightingShader);

		// LanzaPelotas (Marte)
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(x0Marte, y0Marte, z0Marte));
		model = glm::rotate(model, glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		LanzaPelotas.Draw(lightingShader);

		// Pelota (Marte)
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(xMarte, yMarte, zMarte));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		Pelota.Draw(lightingShader);

		// LanzaPelotas (Saturno)
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(x0Saturno, y0Saturno, z0Saturno));
		model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		LanzaPelotas.Draw(lightingShader);

		// Pelota (Saturno)
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(xSaturno, ySaturno, zSaturno));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		Pelota.Draw(lightingShader);

		// Lámpara (Tierra)
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-18.0f, 10.45f, -10.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		Lampara.Draw(lightingShader);

		// Lámpara (Marte)
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-7.50f, 10.45f, -19.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		Lampara.Draw(lightingShader);

		// Lámpara (Saturno)
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(18.0f, 10.45f, -10.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		Lampara.Draw(lightingShader);

		// Nave
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(naveX, naveY, naveZ));
		model = glm::rotate(model, glm::radians(naveRotX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(naveRotZ), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		Nave.Draw(lightingShader);

		// Robot
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-4.00f, 4.0f, -17.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		Robot.Draw(lightingShader);

		// Bandera
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(1.5f, 4.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		Bandera.Draw(lightingShader);

		// Meteorito
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(meteoroX, meteoroY, meteoroZ));
		model = glm::rotate(model, glm::radians(meteoroAngulo * 0.5f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		Meteorito.Draw(lightingShader);

		// Telescopio
		model = glm::mat4(1);
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.5f, 4.0f, 8.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		Telescopio.Draw(lightingShader);

		// Letreros

		// Marte
		model = glm::mat4(1);
		model = glm::rotate(model, glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(1.0f, 4.0f, -10.75f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		LetreroMarte.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::rotate(model, glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(1.0f, 5.0f, -10.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		LetreroMarteInf.Draw(lightingShader);

		// Tierra
		model = glm::mat4(1);
		model = glm::rotate(model, glm::radians(60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(1.0f, 4.0f, -10.75f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		LetreroTierra.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::rotate(model, glm::radians(60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(1.0f, 5.0f, -10.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		LetreroTierraInf.Draw(lightingShader);

		// venus
		model = glm::mat4(1);
		model = glm::rotate(model, glm::radians(100.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(1.0f, 4.0f, -10.75f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		LetreroVenus.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::rotate(model, glm::radians(100.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(1.0f, 5.0f, -10.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		LetreroVenusInf.Draw(lightingShader);

		// mercurio
		model = glm::mat4(1);
		model = glm::rotate(model, glm::radians(140.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(1.0f, 4.0f, -10.75f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		LetreroMercurio.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::rotate(model, glm::radians(140.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(1.0f, 5.0f, -10.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		LetreroMercurioInf.Draw(lightingShader);

		// Neptuno
		model = glm::mat4(1);
		model = glm::rotate(model, glm::radians(220.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(1.0f, 4.0f, -10.75f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		LetreroNeptuno.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::rotate(model, glm::radians(222.5f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(1.0f, 5.0f, -10.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		LetreroNeptunoInf.Draw(lightingShader);

		// Urano
		model = glm::mat4(1);
		model = glm::rotate(model, glm::radians(260.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(1.0f, 4.0f, -10.75f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		LetreroUrano.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::rotate(model, glm::radians(260.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(1.0f, 5.0f, -10.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		LetreroUranoInf.Draw(lightingShader);


		// Saturno
		model = glm::mat4(1);
		model = glm::rotate(model, glm::radians(300.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(1.0f, 4.0f, -10.75f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		LetreroSaturno.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::rotate(model, glm::radians(300.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(1.0f, 5.0f, -10.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		LetreroSaturnoInf.Draw(lightingShader);

		// Jupiter
		model = glm::mat4(1);
		model = glm::rotate(model, glm::radians(340.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(1.0f, 4.0f, -10.75f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		LetreroJupiter.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::rotate(model, glm::radians(340.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(1.0f, 5.0f, -10.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		LetreroJupiterInf.Draw(lightingShader);

		// Marciano
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 3.9f, 8.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		Marciano.Draw(lightingShader);

		// Marciano
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 3.9f, 8.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		TextoMarciano.Draw(lightingShader);

		// Tierra
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(tierraCentroX, 8.0f, tierraCentroZ+5.6));
		model = glm::rotate(model, glm::radians(165.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		LetreroTierraInd.Draw(lightingShader);

		// Marte
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-12.50f, 8.0f, -17.1f));
		model = glm::rotate(model, glm::radians(120.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		LetreroMarteInd.Draw(lightingShader);

		// Saturno
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(14.0f, 8.0f, -14.5f));
		model = glm::rotate(model, glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		LetreroSaturnoInd.Draw(lightingShader);



		// Pelota (Usuario)

		if (puedeTirarUsuario) {
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(xUsuario, yUsuario, zUsuario));
			model = glm::rotate(model, glm::radians(rotUsuarioX), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotUsuarioZ), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::rotate(model, glm::radians(rotUsuarioY), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
			PelotaBask.Draw(lightingShader);

			//// Velocidad

			//int radio = sqrtf(powf(camera.GetFront().x, 2.0f) + powf(camera.GetFront().y, 2.0f) + powf(camera.GetFront().z, 2.0f));

			//model = glm::mat4(1);



			//model = glm::translate(model, glm::vec3(camera.GetPosition().x + radio * glm::cos(glm::radians(-camera.GetPitch() - 0.0f)) * glm::sin(glm::radians(-camera.GetYaw() - 270))
			//	, camera.GetPosition().y + radio * glm::sin(glm::radians(-camera.GetPitch() - 0.0f)) * glm::sin(glm::radians(-camera.GetYaw() - 270))
			//	, camera.GetPosition().z + radio * glm::cos(glm::radians(-camera.GetYaw() - 270))));

			//printf("pitch = %f, yaw = %f\n", camera.GetPitch(), camera.GetYaw());

			///*model = glm::translate(model, glm::vec3(camera.GetPosition().x + radio * glm::cos(glm::radians(-camera.GetPitch() - 0.0f)) * glm::sin(glm::radians(-camera.GetYaw() - 270.0f))
			//	, camera.GetPosition().y + radio * glm::sin(glm::radians(-camera.GetPitch() - 0.0f)) * glm::sin(glm::radians(-camera.GetYaw() - 270.0f))
			//	, camera.GetPosition().z + radio * glm::cos(glm::radians(-camera.GetYaw() - 270.0f))));*/
			//model = glm::rotate(model, glm::radians(rotUsuarioX), glm::vec3(1.0f, 0.0f, 0.0f));
			//model = glm::rotate(model, glm::radians(rotUsuarioZ), glm::vec3(0.0f, 0.0f, 1.0f));
			//model = glm::rotate(model, glm::radians(rotUsuarioY - 90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			//glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
			//LetreroPunto.Draw(lightingShader);
		}

		// Pelota (Usuario)

		glm::vec3 p = camera.GetPosition();
		glm::vec3 f = camera.GetFront();
		glm::vec3 r = camera.GetRight();
		glm::vec3 up = camera.GetUp();

		if (punto) {
			if (tempo < 80.0f) {
				tempo += 0.2f;
			}
			else {
				punto = false;
				tempo = 0.0f;
			}
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(p.x + f.x + 0.65f * r.x + 0.4f * up.x, p.y + f.y + 0.45f * r.y + 0.4f * up.y, p.z + f.z + 0.45f * r.z + 0.4f * up.z));
			model = glm::rotate(model, glm::radians(rotUsuarioX), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotUsuarioZ), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::rotate(model, glm::radians(rotUsuarioY-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
			LetreroPunto.Draw(lightingShader);
		}

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(p.x + f.x + 0.75f * r.x - 0.45f * up.x, p.y + f.y + 0.75f * r.y - 0.45f * up.y, p.z + f.z + 0.75f * r.z - 0.45f * up.z));
		model = glm::rotate(model, glm::radians(rotUsuarioX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotUsuarioZ), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(rotUsuarioY - 90.0f + rotMarcianoCamara), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		Marciano.Draw(lightingShader);


		if (mostrarControlesTiro) {
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(p.x + f.x, p.y + f.y, p.z + f.z));
			model = glm::rotate(model, glm::radians(rotUsuarioX), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotUsuarioZ), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::rotate(model, glm::radians(rotUsuarioY - 90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.35f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
			LetreroVerControlesTiro.Draw(lightingShader);
		}
		else {
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(p.x + f.x + 0.75f * r.x - 0.15f * up.x, p.y + f.y + 0.75f * r.y - 0.15f * up.y, p.z + f.z + 0.75f * r.z - 0.15f * up.z));
			model = glm::rotate(model, glm::radians(rotUsuarioX), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(rotUsuarioZ), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::rotate(model, glm::radians(rotUsuarioY - 90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.22f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
			LetreroControlesTiro.Draw(lightingShader);
		}


		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(p.x + f.x - 0.85f * r.x + 0.45f * up.x, p.y + f.y - 0.85f * r.y + 0.45f * up.y, p.z + f.z - 0.85f * r.z + 0.45f * up.z));
		model = glm::rotate(model, glm::radians(rotUsuarioX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotUsuarioZ), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(rotUsuarioY - 90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.22f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		LetreroEsc.Draw(lightingShader);

		// Canasta
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(canastaX, canastaY, canastaZ));
		model = glm::rotate(model, glm::radians(-camera.GetYaw() + 180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		Canasta.Draw(lightingShader);

		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		Transparente.Draw(lightingShader);
		glDisable(GL_BLEND);  //Desactiva el canal alfa
		glBindVertexArray(0);

		//Traslucidez

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::mat4(1);
		model = glm::scale(model, glm::vec3(1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		//objTras.Draw(lightingShader);
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glBindVertexArray(0);


		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		//model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		//glBindVertexArray(lightVAO);
		//for (GLuint i = 0; i < 4; i++)
		//{
		//	model = glm::mat4(1);
		//	model = glm::translate(model, pointLightPositions[i]);
		//	model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		//	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//	glDrawArrays(GL_TRIANGLES, 0, 36);
		//}
		//glBindVertexArray(0);


		// Draw skybox as last
		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
		SkyBoxshader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // Set depth function back to default




		// Swap the screen buffers
		glfwSwapBuffers(window);
	}




	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();




	return 0;
}

void actualizarUsuario() {
	xUsuario = x0Usuario = camera.GetFront().x + camera.GetPosition().x;
	yUsuario = y0Usuario = camera.GetFront().y + camera.GetPosition().y;
	zUsuario = z0Usuario = camera.GetFront().z + camera.GetPosition().z;
	
	//(pitch - pitchInt * 360.0f)
	anguloUsuario = camera.GetPitch();
	anguloXZUsuario = camera.GetYaw();
}

void animacion()
{
	float pitch = camera.GetPitch();
	int pitchInt = pitch / 360.0;
	rotUsuarioY = -camera.GetYaw();
	rotUsuarioX = -pitch * glm::sin(glm::radians(camera.GetYaw()));
	rotUsuarioZ = pitch * glm::cos(glm::radians(camera.GetYaw()));
	//printf("XZ = %f, Y = %f\n", anguloXZUsuario, anguloUsuario);
	if (puedeTirarUsuario && !enTiroUsuario) {
		actualizarUsuario();
	}

	// Rotación de planetas
	rotPlaneta += 0.2f;
	if (rotPlaneta >= 360.0f) {
		rotPlaneta = 0.0f;
	}

	// Rotación de marciano
	rotMarcianoCamara += 0.5f;
	if (rotMarcianoCamara >= 360.0f) {
		rotMarcianoCamara = 0.0f;
	}

	// Luna
	anguloLuna += 0.1f;
	if (anguloLuna >= 360.0f) {
		anguloLuna = 0.0f;
	}
	lunaX = tierraCentroX + radioLuna * glm::sin(glm::radians(anguloLuna));
	lunaZ = tierraCentroZ + radioLuna * glm::cos(glm::radians(anguloLuna));

	// Estrella 1
	starX = starRadio * glm::cos(glm::radians(starAngulo));
	starZ = starRadio * glm::sin(glm::radians(starAngulo));
	starY = starIniY + starRadioY * glm::sin(glm::radians(starAnguloY));
	pointLightPositions[0] = glm::vec3(starX, starY, starZ);

	starAngulo += starAnguloInc;
	if (starAngulo >= 360.0f) {
		starAngulo = 0.0f;
	}
	starAnguloY += starAnguloYInc;
	if (starAnguloY >= 360.0f) {
		starAnguloY = 0.0f;
	}

	// Estrella 2
	starX2 = starRadio2 * glm::cos(glm::radians(starAngulo + 180));
	starZ2 = starRadio2 * glm::sin(glm::radians(starAngulo + 180));
	starY2 = starIniY + starRadioY * glm::cos(glm::radians(starAnguloY));
	pointLightPositions[1] = glm::vec3(starX2, starY2, starZ2);

	// Nave
	naveX = naveRadio * glm::cos(glm::radians(naveAngulo));
	naveZ = naveRadio * glm::sin(glm::radians(naveAngulo));
	naveY = naveIniY + naveRadioY * glm::sin(glm::radians(naveAnguloY));

	naveAngulo += naveAnguloInc;
	if (naveAngulo >= 360.0f) {
		naveAngulo = 0.0f;
	}
	naveAnguloY += naveAnguloYInc;
	if (naveAnguloY >= 360.0f) {
		naveAnguloY = 0.0f;
	}

	naveRotX = glm::sin(glm::radians(naveAnguloTambaleo)) * naveRotApertura;
	naveRotZ = glm::cos(glm::radians(naveAnguloTambaleo)) * naveRotApertura;

	naveAnguloTambaleo += naveRotInc;
	if (naveAnguloTambaleo >= 360.0f) {
		naveAnguloTambaleo = 0.0f;
	}

	// Meteorito
	meteoroX = meteoroRadio * glm::cos(glm::radians(meteoroAngulo));
	meteoroZ = meteoroRadio * glm::sin(glm::radians(meteoroAngulo));
	meteoroY = meteoroIniY + meteoroRadioY * glm::cos(glm::radians(meteoroAnguloY));

	meteoroAngulo += meteoroAnguloInc;
	if (meteoroAngulo >= 360.0f) {
		meteoroAngulo = 0.0f;
	}
	meteoroAnguloY += meteoroAnguloYInc;
	if (meteoroAnguloY >= 360.0f) {
		meteoroAnguloY = 0.0f;
	}

	// Tiro parabólico en la Tierra

	zTierra = z0Tierra - v0Tierra * glm::cos(glm::radians(anguloTierra)) * glm::cos(glm::radians(anguloXZTierra)) * tTierra;
	xTierra = x0Tierra - v0Tierra * glm::cos(glm::radians(anguloTierra)) * glm::sin(glm::radians(anguloXZTierra)) * tTierra;
	yTierra = y0Tierra + v0Tierra * glm::sin(glm::radians(anguloTierra)) * tTierra - gTierra * 0.5f * tTierra * tTierra;
	tTierra += tIncGlobal;
	if (v0Tierra * glm::sin(glm::radians(anguloTierra)) - gTierra * tTierra < 0.0f && yTierra <= yFinalTierra) {
		tTierra = 0.0f;
	}

	// Tiro parabólico en Marte

	zMarte = z0Marte - v0Marte * glm::cos(glm::radians(anguloMarte)) * glm::cos(glm::radians(anguloXZMarte)) * tMarte;
	xMarte = x0Marte - v0Marte * glm::cos(glm::radians(anguloMarte)) * glm::sin(glm::radians(anguloXZMarte)) * tMarte;
	yMarte = y0Marte + v0Marte * glm::sin(glm::radians(anguloMarte)) * tMarte - gMarte * 0.5f * tMarte * tMarte;
	tMarte += tIncGlobal;
	if (v0Marte * glm::sin(glm::radians(anguloMarte)) - gMarte * tMarte < 0.0f && yMarte <= yFinalMarte) {
		tMarte = 0.0f;
	}
	
	// Tiro parabólico en Saturno

	zSaturno = z0Saturno - v0Saturno * glm::cos(glm::radians(anguloSaturno)) * glm::cos(glm::radians(anguloXZSaturno)) * tSaturno;
	xSaturno = x0Saturno - v0Saturno * glm::cos(glm::radians(anguloSaturno)) * glm::sin(glm::radians(anguloXZSaturno)) * tSaturno;
	ySaturno = y0Saturno + v0Saturno * glm::sin(glm::radians(anguloSaturno)) * tSaturno - gSaturno * 0.5f * tSaturno * tSaturno;
	tSaturno += tIncGlobal;
	if (v0Saturno * glm::sin(glm::radians(anguloSaturno)) - gSaturno * tSaturno < 0.0f && ySaturno <= yFinalSaturno) {
		tSaturno = 0.0f;
	}


	// Tiro Usuario
	if (enTiroUsuario) {
		zUsuario = z0Usuario +  v0Usuario * glm::cos(glm::radians(anguloUsuario)) * glm::sin(glm::radians(anguloXZUsuario)) * tUsuario;
		xUsuario = x0Usuario +  v0Usuario * glm::cos(glm::radians(anguloUsuario)) * glm::cos(glm::radians(anguloXZUsuario)) * tUsuario;
		yUsuario = y0Usuario + v0Usuario * glm::sin(glm::radians(anguloUsuario)) * tUsuario - gUsuario * 0.5f * tUsuario * tUsuario;
		tUsuario += tIncUsuario;
		bool bajando = v0Usuario * glm::sin(glm::radians(anguloUsuario)) - gUsuario * tUsuario < 0.0f;
		if (bajando && yUsuario <= yFinalUsuario) {
			tUsuario = 0.0f;
			enTiroUsuario = false;
			punto = false;
		}
		if (bajando && verificarPunto()) {
			//printf("PUNTO!");
			tUsuario = 0.0f;
			punto = true;
			tempo = 0.0f;
			enTiroUsuario = false;
			reubicarCanasta();
		}
	}

	//Movimiento del personaje

	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				//printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			posX += KeyFrame[playIndex].incX;
			posY += KeyFrame[playIndex].incY;
			posZ += KeyFrame[playIndex].incZ;

			rotRodIzq += KeyFrame[playIndex].rotInc;

			i_curr_steps++;
		}

	}
}


// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// TiroUsuario
	if (keys[GLFW_KEY_1])
	{
		if (!enTiroUsuario)
			puedeTirarUsuario = !puedeTirarUsuario;
	}
	// TiroUsuario
	if (keys[GLFW_KEY_2])
	{
		if (puedeTirarUsuario) {
			actualizarUsuario();
			tUsuario = 0.0f;
			enTiroUsuario = !enTiroUsuario;
		}
	}


	if (keys[GLFW_KEY_SPACE])
	{
		mostrarControlesTiro = !mostrarControlesTiro;
	}

	if (keys[GLFW_KEY_L])
	{
		if (play == false && (FrameIndex > 1))
		{

			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
		}

	}

	if (keys[GLFW_KEY_K])
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}

		rot = -25.0f;//Variable que maneja el giro de la camara

	}


	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{

	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

float IncRot(float rot) {
	rot += 0.1f;
	return rot >= 360.f ? 0.0f : rot;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{

	if (keys[GLFW_KEY_1])
	{

		movCamera = 0.01f;//Manda una velocidad de 0.01 a la camara automatica

	}

	if (keys[GLFW_KEY_2])
	{
		if (rotRodIzq < 80.0f)
			rotRodIzq += 1.0f;

	}

	if (keys[GLFW_KEY_3])
	{
		if (rotRodIzq > -45)
			rotRodIzq -= 1.0f;

	}

	// Usuario

	if (keys[GLFW_KEY_3])
	{
		tUsuario = 0.0f;
		v0Usuario += 2.0f;
		if (v0Usuario > 10.0f) {
			v0Usuario = v0Global;
		}
	}
	if (keys[GLFW_KEY_4])
	{
		tUsuario = 0.0f;
		gUsuario = gTierra;
	}
	if (keys[GLFW_KEY_5])
	{
		tUsuario = 0.0f;
		gUsuario = gMarte;
	}
	if (keys[GLFW_KEY_6])
	{
		tUsuario = 0.0f;
		gUsuario = gSaturno;
	}

	// Tierra
	if (keys[GLFW_KEY_Z])
	{
		escTierra += 0.001f;
		if (escTierra >= 2.5f) {
			escTierra = 1.0f;
		}
	}
	if (keys[GLFW_KEY_X])
	{
		rotTierraX = IncRot(rotTierraX);
	}
	if (keys[GLFW_KEY_C])
	{
		rotTierraY = IncRot(rotTierraY);
	}
	if (keys[GLFW_KEY_V])
	{
		rotTierraZ = IncRot(rotTierraZ);
	}
	if (keys[GLFW_KEY_B])
	{
		tTierra = 0.0f;
			v0Tierra += 2.0f;
		if (v0Tierra > 10.0f) {
			v0Tierra = v0Global;
		}
	}
	if (keys[GLFW_KEY_N])
	{
		tTierra = 0.0f;
			anguloTierra += 15.0f;
		if (anguloTierra > 90.0f) {
			anguloTierra = anguloGlobal;
		}
	}

	// Marte
	if (keys[GLFW_KEY_R])
	{
		escMarte += 0.001f;
		if (escMarte >= 2.5f) {
			escMarte = 1.0f;
		}
	}
	if (keys[GLFW_KEY_T])
	{
		rotMarteX = IncRot(rotMarteX);
	}
	if (keys[GLFW_KEY_Y])
	{
		rotMarteY = IncRot(rotMarteY);
	}
	if (keys[GLFW_KEY_U])
	{
		rotMarteZ = IncRot(rotMarteZ);
	}
	if (keys[GLFW_KEY_I])
	{
		tMarte = 0.0f;
			v0Marte += 2.0f;
		if (v0Marte > 10.0f) {
			v0Marte = v0Global;
		}
	}
	if (keys[GLFW_KEY_O])
	{
		tMarte = 0.0f;
			anguloMarte += 15.0f;
		if (anguloMarte > 90.0f) {
			anguloMarte = anguloGlobal;
		}
	}

	// Saturno
	if (keys[GLFW_KEY_F])
	{
		escSaturno += 0.001f;
		if (escSaturno >= 2.5f) {
			escSaturno = 1.0f;
		}
	}
	if (keys[GLFW_KEY_G])
	{
		rotSaturnoX = IncRot(rotSaturnoX);
	}
	if (keys[GLFW_KEY_H])
	{
		rotSaturnoY = IncRot(rotSaturnoY);
	}
	if (keys[GLFW_KEY_J])
	{
		rotSaturnoZ = IncRot(rotSaturnoZ);
	}
	if (keys[GLFW_KEY_K])
	{
		tSaturno = 0.0f;
			v0Saturno += 2.0f;
		if (v0Saturno > 10.0f) {
			v0Saturno = v0Global;
		}
	}
	if (keys[GLFW_KEY_L])
	{
		tSaturno = 0.0f;
		anguloSaturno += 15.0f;
		if (anguloSaturno > 90.0f) {
			anguloSaturno = anguloGlobal;
		}
	}

	

	//Mov Personaje
	if (keys[GLFW_KEY_H])
	{
		posZ += 1;
	}

	if (keys[GLFW_KEY_Y])
	{
		posZ -= 1;
	}

	if (keys[GLFW_KEY_G])
	{
		posX -= 1;
	}

	if (keys[GLFW_KEY_J])
	{
		posX += 1;
	}




	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}






}