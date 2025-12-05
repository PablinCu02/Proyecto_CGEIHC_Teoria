/*
* =================================================================================
* COMPUTACIÓN GRÁFICA E INTERACCIÓN HUMANO-COMPUTADORA
* =================================================================================
*/

#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include "Model.h"
#include "Skybox.h"

// Headers de Iluminación 
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"

// Headers para Keyframes 
#include <fstream>pos
#include <iostream>
#include <string>

const float toRadians = 3.14159265f / 180.0f;

// =================================================================================
// VARIABLES GLOBALES Y OBJETOS PRINCIPALES
// =================================================================================

// Sistema Principal 
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;
Skybox skybox;
Skybox skyboxNoche;

// Texturas 
Texture brickTexture;
Texture dirtTexture;
//Texture plainTexture;
Texture pisoTexture;
Texture lamparaTexture;
Texture lampara2Texture;
Texture lampara3Texture;
Texture lampara4Texture;
Texture lampara5Texture;
Texture lampara6Texture;
Texture Juego_Pelota_Texture;
Texture PiedraTexture;
Texture AdvTime_FontTexture;
Texture Marco;
Texture Puerta;
Texture Sol;

// Modelos 
Model Juego_Pelota_M;
Model Pelota_M;
Model Piedra_M;
Model Marco_M;
Model Letrero_M;
Model PuertaDerecha_M;
Model PuertaIzquierda_M;
Model PiramideT_M;
Model PiramideS_M;
Model lampara_M;
Model Globo_M;
Model Chozas_M;
Model arbol_M;
Model BancaG_M;
Model BancaT_M;
Model CalendarioA_M;
Model Kiosco_M;
Model Pillar_M;
Model Ring_M;
Model PisoRing_M;
Model Cuerdas_M;
Model Coronas_M;
Model EstatuaLuchador_M;

// Modelos 
Model MickeyC_M;
Model MickeyT_M;
Model MickeyBD_M;
Model MickeyBI_M;
Model MickeyPD_M;
Model MickeyPI_M;
Model PeachC_M;
Model PeachT_M;
Model PeachBD_M;
Model PeachBI_M;
Model PeachPD_M;
Model PeachPI_M;
Model FinnC_M;
Model FinnT_M;
Model FinnBD_M;
Model FinnBI_M;
Model FinnPD_M;
Model FinnPI_M;
Model Jake_M;
Model NPC1_M;
Model NPC2_M;
Model NPC3_M;
Model Joshi_M;
Model Pluto_M;


// Materiales 
Material Material_brillante;
Material Material_opaco;

// Iluminación 
DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Control de Tiempo 
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// Shaders 
static const char* vShader = "shaders/shader_light.vert";
static const char* fShader = "shaders/shader_light.frag";

// Variables de Animación  (Letrero)
std::vector<glm::vec2> offsets = {
	glm::vec2(0.65f, 0.57f), // P
	glm::vec2(0.88f, 0.57f), // R
	glm::vec2(0.57f, 0.56f), // O
	glm::vec2(0.651f,0.46f), // Y
	glm::vec2(0.45f, 0.67f), // E
	glm::vec2(0.23f, 0.67f), // C
	glm::vec2(0.11f, 0.46f), // T
	glm::vec2(0.57f, 0.56f), // O
	glm::vec2(0.23f, 0.67f), // C
	glm::vec2(0.655f,0.67f), // G
	glm::vec2(0.45f, 0.67f), // E
	glm::vec2(0.86f, 0.67f), // I
	glm::vec2(0.77f, 0.67f), // H
	glm::vec2(0.23f, 0.67f)  // C
};
static float tiempoAcumulado = 0.0f;
static int indiceInicio = 0;
int letrasVisibles = 4;
float intervaloCambio = 0.7f;
glm::vec3 basePos(-5.0f, 2.5f, 275.5f);
float separacion = 1.5f;
glm::vec3 escalaLetras(1.5f, 2.0f, 2.0f);

//  Variables de Animación (Puertas)
bool abierta = false;
float ang = 0.0f;
float vel = 90.0f;
static bool teclaOPresionada = false;

bool puertaDerechaAbierta = false;
bool puertaDerechaAnimando = false;
bool teclaPPresionada = false;
float progresoAnim = 0.0f;
float duracionFase = 1.0f;
int fase = 0;
glm::vec3 puertaDerechaPos(0.0f);

// Variables de Animación 
float gravedad = -45.0f;
bool pelotaSaltando = false;
bool teclaJPresionada = false;
float pelotaVelocidadY = 0.0f;
float pelotaPosBaseY = 1.5f;
float fuerzaSalto = 30.0f;
float pelotaAmplitud = 80.0f;
float pelotaVelocidad = 50.6f;
bool jakeGolpeando = false;
float jakeAnimTimer = 0.0f;
float jakeAnguloGolpe = 0.0f;
float pelotaOffset = 0.0f;
float pelotaDireccion = 1.0f;
bool avanza = true;
bool lightsA[2] = { false, false };

// 
// KEYFRAMES (Globo)
// 

// Estructura para almacenar la posición y rotación de un Keyframe
struct FRAME
{
	float posX;
	float posY;
	float posZ;
	float rotY;
};

// Función para guardar Keyframes en un archivo .txt
void saveKeyframes(const std::vector<FRAME>& keyframes, const std::string& filename) {
	std::ofstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Error al abrir el archivo para guardar!" << std::endl;
		return;
	}
	// Guarda cada variable en una línea
	for (const auto& frame : keyframes) {
		file << frame.posX << " " << frame.posY << " " << frame.posZ << " " << frame.rotY << "\n";
	}
	file.close();
	printf("Keyframes guardados en '%s'\n", filename.c_str());
}

// Función para leer Keyframes desde un archivo .txt
std::vector<FRAME> readKeyframes(const std::string& filename) {
	std::vector<FRAME> keyframes;
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Error al abrir el archivo para leer!" << std::endl;
		return keyframes;
	}
	FRAME frame;
	while (file >> frame.posX >> frame.posY >> frame.posZ >> frame.rotY) {
		keyframes.push_back(frame);
	}
	file.close();
	printf("Keyframes cargados desde '%s'. Total: %d\n", (int)keyframes.size(), filename.c_str());
	return keyframes;
}

// Función de Interpolación 
float interpolation(float a, float b, float t) {
	return a + (b - a) * t;
}

// Función principal de Animación
void animate(float& posX, float& posY, float& posZ, float& rotY,
	std::vector<FRAME>& keyframes, int& frameIndex, float& playIndex, bool& play) {

	if (play && keyframes.size() > 1) {

		// Cuando llega a 1.0, pasamos al siguiente frame.
		if (playIndex > 1.0f) {
			playIndex = 0.0f;
			frameIndex++;
		}

		// Si llegamos al final de la animación, la detenemos y reiniciamos.
		if (frameIndex >= keyframes.size() - 1) {
			frameIndex = 0;
			play = false;
			printf("Animacion terminada.\n");
		}

		// Frames actual y siguiente
		int currentFrame = frameIndex;
		int nextFrame = frameIndex + 1;

		// Interpolamos la posición y rotación entre el frame actual y el siguiente
		posX = interpolation(keyframes[currentFrame].posX, keyframes[nextFrame].posX, playIndex);
		posY = interpolation(keyframes[currentFrame].posY, keyframes[nextFrame].posY, playIndex);
		posZ = interpolation(keyframes[currentFrame].posZ, keyframes[nextFrame].posZ, playIndex);
		rotY = interpolation(keyframes[currentFrame].rotY, keyframes[nextFrame].rotY, playIndex);

		// Aumentamos el índice de interpolación. 
		playIndex += 0.005f;
	}
}

// Variables de Animación por Keyframes del Globo
std::vector<FRAME> KeyFrameGlobo; // Vector para guardar frames
int FrameIndexGlobo = 0;
bool playGlobo = false;            // Bandera para reproducir animacion
float playIndexGlobo = 0.0f;       // Índice de interpolación 

// Posición y Rotación actual del Globo 
float globoPosX = 0.0f;
float globoPosY = 50.5f;
float globoPosZ = -180.0f;
float globoRotY = 90.0f;

bool teclaLPresionada = false;
bool teclaKPresionada = false;
bool teclaGPresionada = false;
bool teclaCPresionada = false;

// --- Variables para Spotlight con Teclado 
bool spotLights_On = true;
bool teclaTPresionada = false;


//funcion de calculo de normales por promedio de vertices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};
	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
	};

	unsigned int AdvTimeIndices[] = {
		0, 1, 2,
		0, 2, 3,
	};

	GLfloat AdvTimeVertices[] = {
		//   X      Y      Z     U       V        Nx    Ny    Nz
		-0.5f, 0.0f,  0.5f,   0.1f,   0.98f,   0.0f, -1.0f, 0.0f, // arriba dere  
		 0.5f, 0.0f,  0.5f,   0.01f,     0.98f,   0.0f, -1.0f, 0.0f, // arriba izq
		 0.5f, 0.0f, -0.5f,   0.01f,     0.9f,  0.0f, -1.0f, 0.0f, // abajo izq
		-0.5f, 0.0f, -0.5f,   0.1f,   0.9f,  0.0f, -1.0f, 0.0f  // abajo der
	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(AdvTimeVertices, AdvTimeIndices, 32, 6);
	meshList.push_back(obj5);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void setLetterUV(float* vertices, int col, int row)
{
	float uStep = 1.0f / 9.0f;  // 9 columnas
	float vStep = 1.0f / 3.0f;  // 3 filas
	float u0 = col * uStep;
	float v0 = 1.0f - (row + 1) * vStep;
	float u1 = u0 + uStep;
	float v1 = v0 + vStep;

	// Actualiza las coordenadas UV del plano
	vertices[3] = u0; vertices[4] = v1;  // arriba izq
	vertices[11] = u1; vertices[12] = v1;  // arriba der
	vertices[19] = u1; vertices[20] = v0;  // abajo der
	vertices[27] = u0; vertices[28] = v0;  // abajo izq
}


int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	// Configuración de Cámara 
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 50.0f, 0.5f);

	// Carga de Texturas 
	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	//plainTexture = Texture("Textures/plain.png");
	//plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/pasto.png");
	pisoTexture.LoadTextureA();
	lamparaTexture = Texture("Textures/StreetLamp1.png");
	lamparaTexture.LoadTextureA();
	lampara2Texture = Texture("Textures/StreetLamp2BaseColor.png");
	lampara2Texture.LoadTextureA();
	lampara3Texture = Texture("Textures/StreetLamp3Metallic.png");
	lampara3Texture.LoadTextureA();
	lampara4Texture = Texture("Textures/StreetLamp4Normal.png");
	lampara4Texture.LoadTextureA();
	lampara5Texture = Texture("Textures/StreetLamp5Opacity.png");
	lampara5Texture.LoadTextureA();
	lampara6Texture = Texture("Textures/StreetLamp6Roughness.png");
	lampara6Texture.LoadTextureA();
	Juego_Pelota_Texture = Texture("Textures/Juego_Pelota.png");
	Juego_Pelota_Texture.LoadTextureA();
	PiedraTexture = Texture("Textures/piedra.png");
	PiedraTexture.LoadTextureA();
	AdvTime_FontTexture = Texture("Textures/AdvTime_Font.png");
	AdvTime_FontTexture.LoadTextureA();
	Marco = Texture("Textures/Corrugated_Metal_Sheet_teendf3q_1K_BaseColor.png");
	Marco.LoadTextureA();
	Puerta = Texture("Textures/Wooden_Log_Wall_udlmecoew_1K_BaseColor.png");
	Puerta.LoadTextureA();
	Sol = Texture("Textures/sun.png");
	Sol.LoadTextureA();

	// Carga de Modelos 
	Juego_Pelota_M = Model();
	Juego_Pelota_M.LoadModel("Models/Juego_Pelota.obj");
	Pelota_M = Model();
	Pelota_M.LoadModel("Models/pelota.fbx");
	Piedra_M = Model();
	Piedra_M.LoadModel("Models/pelota.fbx");
	PiramideT_M = Model();
	PiramideT_M.LoadModel("Models/PiramideT.obj");
	PiramideS_M = Model();
	PiramideS_M.LoadModel("Models/PiramideSol.obj");
	lampara_M = Model();
	lampara_M.LoadModel("Models/lampara.obj");
	Globo_M = Model();
	Globo_M.LoadModel("Models/globo.obj");
	Chozas_M = Model();
	Chozas_M.LoadModel("Models/Chozas.obj");
	arbol_M = Model();
	arbol_M.LoadModel("Models/arboles.obj");
	BancaG_M = Model();
	BancaG_M.LoadModel("Models/banca.obj");
	BancaT_M = Model();
	BancaT_M.LoadModel("Models/Banca_Texturizada.obj");
	CalendarioA_M = Model();
	CalendarioA_M.LoadModel("Models/calendario_azteca.obj");
	Kiosco_M = Model();
	Kiosco_M.LoadModel("Models/kiosco.obj");
	Pillar_M = Model();
	Pillar_M.LoadModel("Models/pillar.obj");
	Ring_M = Model();
	Ring_M.LoadModel("Models/Ring.obj");
	PisoRing_M = Model();
	PisoRing_M.LoadModel("Models/PisoRing.obj");
	Cuerdas_M = Model();
	Cuerdas_M.LoadModel("Models/Cuerdas.obj");
	Coronas_M = Model();
	Coronas_M.LoadModel("Models/Coronas.obj");
	EstatuaLuchador_M = Model();
	EstatuaLuchador_M.LoadModel("Models/luchadores.obj");
	// Personajes
	MickeyC_M = Model();
	MickeyC_M.LoadModel("Models/MickeyC.obj");
	MickeyT_M = Model();
	MickeyT_M.LoadModel("Models/MickeyT.obj");
	MickeyBD_M = Model();
	MickeyBD_M.LoadModel("Models/MickeyBD.obj");
	MickeyBI_M = Model();
	MickeyBI_M.LoadModel("Models/MickeyBI.obj");
	MickeyPD_M = Model();
	MickeyPD_M.LoadModel("Models/MickeyPD.obj");
	MickeyPI_M = Model();
	MickeyPI_M.LoadModel("Models/MickeyPI.obj");
	PeachC_M = Model();
	PeachC_M.LoadModel("Models/PeachC.obj");
	PeachT_M = Model();
	PeachT_M.LoadModel("Models/PeachT.obj");
	PeachBD_M = Model();
	PeachBD_M.LoadModel("Models/PeachBD.obj");
	PeachBI_M = Model();
	PeachBI_M.LoadModel("Models/PeachBI.obj");
	PeachPD_M = Model();
	PeachPD_M.LoadModel("Models/PeachPD.obj");
	PeachPI_M = Model();
	PeachPI_M.LoadModel("Models/PeachPI.obj");
	FinnC_M = Model();
	FinnC_M.LoadModel("Models/FinnC.obj");
	FinnT_M = Model();
	FinnT_M.LoadModel("Models/FinnT.obj");
	FinnBD_M = Model();
	FinnBD_M.LoadModel("Models/FinnBD.obj");
	FinnBI_M = Model();
	FinnBI_M.LoadModel("Models/FinnBI.obj");
	FinnPD_M = Model();
	FinnPD_M.LoadModel("Models/FinnPD.obj");
	FinnPI_M = Model();
	FinnPI_M.LoadModel("Models/FinnPI.obj");
	Jake_M = Model();
	Jake_M.LoadModel("Models/jake.obj");
	NPC1_M = Model();
	NPC1_M.LoadModel("Models/NPC1.obj");
	NPC2_M = Model();
	NPC2_M.LoadModel("Models/NPC2.obj");
	NPC3_M = Model();
	NPC3_M.LoadModel("Models/NPC3.obj");
	Joshi_M = Model();
	Joshi_M.LoadModel("Models/Joshi.obj");
	Pluto_M = Model();
	Pluto_M.LoadModel("Models/Pluto.obj");

	// Modelos de Arco
	Marco_M = Model();
	Marco_M.LoadModel("Models/arco.obj");
	Letrero_M = Model();
	Letrero_M.LoadModel("Models/cartel.obj");
	PuertaDerecha_M = Model();
	PuertaDerecha_M.LoadModel("Models/puerta_der.obj");
	PuertaIzquierda_M = Model();
	PuertaIzquierda_M.LoadModel("Models/puerta_izq.obj");


	// Configuración de Skybox 
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");
	skybox = Skybox(skyboxFaces);

	//Skybox Noche
	std::vector<std::string> skyboxFacesNoche;
	skyboxFacesNoche.push_back("Textures/Skybox/cupertin-lake-night_rt.tga");
	skyboxFacesNoche.push_back("Textures/Skybox/cupertin-lake-night_lf.tga");
	skyboxFacesNoche.push_back("Textures/Skybox/cupertin-lake-night_dn.tga");
	skyboxFacesNoche.push_back("Textures/Skybox/cupertin-lake-night_up.tga");
	skyboxFacesNoche.push_back("Textures/Skybox/cupertin-lake-night_bk.tga");
	skyboxFacesNoche.push_back("Textures/Skybox/cupertin-lake-night_ft.tga");
	skyboxNoche = Skybox(skyboxFacesNoche);


	// Configuración de Materiales 
	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	// Configuración de Luces 
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);

	unsigned int pointLightCount = 0;
	pointLights[0] = PointLight(1.0f, 0.8f, 0.6f,
		0.0f, 1.0f,
		0.0f, 2.5f, 1.5f,
		1.0f, 0.07f, 0.017f);
	pointLightCount++;

	unsigned int spotLightCount = 0;



	// Posición de las luces tipo PointLight en las lámparas del camino
	pointLights[0] = PointLight(1.0f, 0.8f, 0.6f,
		0.0f, 1.0f,
		0.0f, 5.0f, 180.0f,
		1.0f, 0.07f, 0.017f);
	pointLightCount++;

	// LUCES FIJAS (Siempre encendidas de noche)
	// Lámpara en Z=110
	pointLights[1] = PointLight(1.0f, 0.8f, 0.6f,
		0.0f, 1.0f,
		0.0f, 5.0f, 110.0f,
		1.0f, 0.07f, 0.017f);
	pointLightCount++;

	// Lámpara en Z=210
	pointLights[2] = PointLight(1.0f, 0.8f, 0.6f,
		0.0f, 1.0f,
		0.0f, 5.0f, 210.0f,
		1.0f, 0.07f, 0.017f);
	pointLightCount++;

	// Luz cerca de las Chozas
	pointLights[3] = PointLight(1.0f, 0.8f, 0.6f,
		0.0f, 1.0f,
		0.0f, 5.0f, 60.0f,
		1.0f, 0.07f, 0.017f);
	pointLightCount++;

	// Variables Uniform 
	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;

	// Proyección 
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);


	// Control de Avatares 
	int cameraMode = 0;
	bool teclaVPresionada = false;

	// Switch del ersonaje activo (1=Mickey, 2=Peach, 3=Finn)
	int activeAvatar = 1;
	bool tecla1Presionada = false;
	bool tecla2Presionada = false;
	bool tecla3Presionada = false;

	float velocidadCaminata = 8.0f;

	// Variables de Mickey 
	float avatarPosX = 2.5f;
	float avatarPosY = -0.5f;
	float avatarPosZ = 280.0f;
	float avatarRotY = 0.0f;
	float anguloCaminataMickey = 0.0f;

	// Variables de Peach 
	float peachPosX = 10.0f;
	float peachPosY = -0.5f;
	float peachPosZ = 280.0f;
	float peachRotY = 0.0f;
	float anguloCaminataPeach = 0.0f;

	// Variables de Finn 
	float finnPosX = -5.0f;
	float finnPosY = -0.5f;
	float finnPosZ = 280.0f;
	float finnRotY = 0.0f;
	float anguloCaminataFinn = 0.0f;

	//Control Dia y Noche
	float cicloTimer = 0.0f;
	float cicloDuracion = 80.0f;
	float tiempoDelDia = 0.0f;


	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		glfwPollEvents();
		if (cameraMode == 0) {
			camera.keyControl(mainWindow.getsKeys(), deltaTime);
			camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		}

		// Movimiento Manual del Globo  
		float globoMoveSpeed = 20.0f; // Velocidad de movimiento del globo

		// Mover en -Z
		if (mainWindow.getsKeys()[GLFW_KEY_T]) {
			globoPosZ -= globoMoveSpeed * deltaTime;
		}
		// Mover en Z
		if (mainWindow.getsKeys()[GLFW_KEY_G]) {
			globoPosZ += globoMoveSpeed * deltaTime;
		}
		// Mover en -X
		if (mainWindow.getsKeys()[GLFW_KEY_F]) {
			globoPosX -= globoMoveSpeed * deltaTime;
		}
		// Mover en X
		if (mainWindow.getsKeys()[GLFW_KEY_H]) {
			globoPosX += globoMoveSpeed * deltaTime;
		}
		// Mover en Y
		if (mainWindow.getsKeys()[GLFW_KEY_UP]) {
			globoPosY += globoMoveSpeed * deltaTime;
		}
		// Mover en -Y
		if (mainWindow.getsKeys()[GLFW_KEY_DOWN]) {
			globoPosY -= globoMoveSpeed * deltaTime;
		}

		// Lógica de Animaciones 

		// Lógica del Letrero Deslizante
		tiempoAcumulado += deltaTime;
		if (tiempoAcumulado >= intervaloCambio) {
			tiempoAcumulado = 0.0f;
			indiceInicio++;
			if (indiceInicio > (int)offsets.size() - letrasVisibles)
				indiceInicio = 0;
		}

		// Puerta Izquierda con tecla "O"
		if (mainWindow.getsKeys()[GLFW_KEY_O]) {
			if (!teclaOPresionada) {
				abierta = !abierta;
				teclaOPresionada = true;
			}
		}
		else {
			teclaOPresionada = false;
		}
		float targetAng = abierta ? -90.0f : 0.0f;
		if (fabs(ang - targetAng) > 0.1f) {
			float step = vel * deltaTime / 10;
			if (ang < targetAng) ang = std::min(ang + step, targetAng);
			else ang = std::max(ang - step, targetAng);
		}

		// Lógica de Puerta Derecha con tecla "P"
		if (mainWindow.getsKeys()[GLFW_KEY_P]) {
			if (!teclaPPresionada && !puertaDerechaAnimando) {
				puertaDerechaAnimando = true;
				teclaPPresionada = true;
				if (!puertaDerechaAbierta) fase = 1;
				else fase = 3;
				progresoAnim = 0.0f;
			}
		}
		else {
			teclaPPresionada = false;
		}
		if (puertaDerechaAnimando) {
			progresoAnim += deltaTime / 20;
			float t = progresoAnim / duracionFase;
			if (fase == 1) {
				puertaDerechaPos.z = glm::min(t, 1.0f) * 0.5f;
				if (t >= 1.0f) { fase = 2; progresoAnim = 0.0f; }
			}
			else if (fase == 2) {
				puertaDerechaPos = glm::vec3(glm::min(t, 1.0f) * 4.0f, 0.0f, 1.0f);
				if (t >= 1.0f) { puertaDerechaAnimando = false; puertaDerechaAbierta = true; fase = 0; }
			}
			else if (fase == 3) {
				puertaDerechaPos = glm::vec3((1.0f - glm::min(t, 1.0f)) * 4.0f, 0.0f, 1.0f);
				if (t >= 1.0f) { fase = 4; progresoAnim = 0.0f; }
			}
			else if (fase == 4) {
				puertaDerechaPos.z = (1.0f - glm::min(t, 1.0f)) * 0.5f;
				if (t >= 1.0f) { puertaDerechaAnimando = false; puertaDerechaAbierta = false; fase = 0; }
			}
		}

		// Lógica Juego de Pelota con Tecla "J"
		if (mainWindow.getsKeys()[GLFW_KEY_J]) {
			if (!teclaJPresionada) {
				teclaJPresionada = true;
				pelotaDireccion = -pelotaDireccion;
				if (!jakeGolpeando) {
					jakeGolpeando = true;
					jakeAnimTimer = 0.5f;
				}
				if (!pelotaSaltando) {
					pelotaSaltando = true;
					pelotaVelocidadY = fuerzaSalto;
				}
			}
		}
		else {
			teclaJPresionada = false;
		}
		if (jakeGolpeando) {
			jakeAnimTimer -= deltaTime;
			if (jakeAnimTimer <= 0.0f) {
				jakeGolpeando = false;
				jakeAnguloGolpe = 0.0f;
			}
			else {
				if (jakeAnimTimer > 0.25f) jakeAnguloGolpe = 45.0f * ((0.5f - jakeAnimTimer) / 0.25f);
				else jakeAnguloGolpe = 45.0f * (jakeAnimTimer / 0.25f);
			}
		}
		if (pelotaSaltando) {
			pelotaVelocidadY += gravedad * deltaTime;
			pelotaPosBaseY += pelotaVelocidadY * deltaTime;
			if (pelotaPosBaseY <= 1.5f) {
				pelotaPosBaseY = 1.8f;
				pelotaSaltando = false;
				pelotaVelocidadY = 0.0f;
			}
		}

		// Lógica de Keyframes del Globo (Teclas 'L', 'K', 'G', 'C')
		if (mainWindow.getsKeys()[GLFW_KEY_L] && !teclaLPresionada) {
			teclaLPresionada = true;
			FRAME newFrame;
			newFrame.posX = globoPosX;
			newFrame.posY = globoPosY;
			newFrame.posZ = globoPosZ;
			newFrame.rotY = globoRotY;
			KeyFrameGlobo.push_back(newFrame);
			printf("Keyframe %d guardado: (%.2f, %.2f, %.2f)\n", (int)KeyFrameGlobo.size(), newFrame.posX, newFrame.posY, newFrame.posZ);
		}
		if (!mainWindow.getsKeys()[GLFW_KEY_L]) {
			teclaLPresionada = false;
		}
		if (mainWindow.getsKeys()[GLFW_KEY_K] && !teclaKPresionada) {
			teclaKPresionada = true;
			if (KeyFrameGlobo.size() > 1) {
				playGlobo = true;
				FrameIndexGlobo = 0;
				playIndexGlobo = 0.0f;
				printf("Reproduciendo animación del globo...\n");
			}
			else {
				printf("¡Error! Necesitas al menos 2 keyframes para reproducir.\n");
			}
		}
		if (!mainWindow.getsKeys()[GLFW_KEY_K]) {
			teclaKPresionada = false;
		}
		if (mainWindow.getsKeys()[GLFW_KEY_G] && !teclaGPresionada) {
			teclaGPresionada = true;
			saveKeyframes(KeyFrameGlobo, "globo_keyframes.txt");
		}
		if (!mainWindow.getsKeys()[GLFW_KEY_G]) {
			teclaGPresionada = false;
		}
		if (mainWindow.getsKeys()[GLFW_KEY_C] && !teclaCPresionada) {
			teclaCPresionada = true;
			KeyFrameGlobo = readKeyframes("globo_keyframes.txt");
		}
		if (!mainWindow.getsKeys()[GLFW_KEY_C]) {
			teclaCPresionada = false;
		}

		// Actualiza la animación del globo si se está reproduciendo
		animate(globoPosX, globoPosY, globoPosZ, globoRotY,
			KeyFrameGlobo, FrameIndexGlobo, playIndexGlobo, playGlobo);

		// Cambio de Cámara y Selección de Avatar 
		// Modos de cámara: 0, 1, 2
		if (mainWindow.getsKeys()[GLFW_KEY_V] && !teclaVPresionada) {
			teclaVPresionada = true;
			cameraMode = (cameraMode + 1) % 3;
		}
		if (!mainWindow.getsKeys()[GLFW_KEY_V]) {
			teclaVPresionada = false;
		}

		// Interruptor de Spotlights con letra "T"
		if (mainWindow.getsKeys()[GLFW_KEY_T] && !teclaTPresionada) {
			teclaTPresionada = true;
			spotLights_On = !spotLights_On; // Invierte el estado
		}
		if (!mainWindow.getsKeys()[GLFW_KEY_T]) {
			teclaTPresionada = false;
		}


		// Reseteamos las animaciones
		anguloCaminataMickey = 0.0f;
		anguloCaminataPeach = 0.0f;
		anguloCaminataFinn = 0.0f;

		if (cameraMode == 1)
		{
			// Selección de Avatar (Teclas 1, 2, 3) 
			if (mainWindow.getsKeys()[GLFW_KEY_1]) activeAvatar = 1;
			if (mainWindow.getsKeys()[GLFW_KEY_2]) activeAvatar = 2;
			if (mainWindow.getsKeys()[GLFW_KEY_3]) activeAvatar = 3;

			// Lógica de Movimiento 
			float avatarMoveSpeed = 15.0f;

			switch (activeAvatar)
			{
				// Control de Mickey 
			case 1:
				avatarRotY -= mainWindow.getXChange() * 0.5f;
				{
					glm::vec3 forward = glm::vec3(-sin(glm::radians(avatarRotY)), 0.0f, -cos(glm::radians(avatarRotY)));
					glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
					if (mainWindow.getsKeys()[GLFW_KEY_W]) { avatarPosX += forward.x * avatarMoveSpeed * deltaTime; avatarPosZ += forward.z * avatarMoveSpeed * deltaTime; }
					if (mainWindow.getsKeys()[GLFW_KEY_S]) { avatarPosX -= forward.x * avatarMoveSpeed * deltaTime; avatarPosZ -= forward.z * avatarMoveSpeed * deltaTime; }
					if (mainWindow.getsKeys()[GLFW_KEY_A]) { avatarPosX -= right.x * avatarMoveSpeed * deltaTime; avatarPosZ -= right.z * avatarMoveSpeed * deltaTime; }
					if (mainWindow.getsKeys()[GLFW_KEY_D]) { avatarPosX += right.x * avatarMoveSpeed * deltaTime; avatarPosZ += right.z * avatarMoveSpeed * deltaTime; }
					if (mainWindow.getsKeys()[GLFW_KEY_W] || mainWindow.getsKeys()[GLFW_KEY_S] || mainWindow.getsKeys()[GLFW_KEY_A] || mainWindow.getsKeys()[GLFW_KEY_D]) {
						anguloCaminataMickey = glm::sin(glfwGetTime() * velocidadCaminata) * 25.0f;
					}
				}
				break;

				// Control de Peach 
			case 2:
				peachRotY -= mainWindow.getXChange() * 0.5f;
				{
					glm::vec3 forward = glm::vec3(-sin(glm::radians(peachRotY)), 0.0f, -cos(glm::radians(peachRotY)));
					glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
					if (mainWindow.getsKeys()[GLFW_KEY_W]) { peachPosX += forward.x * avatarMoveSpeed * deltaTime; peachPosZ += forward.z * avatarMoveSpeed * deltaTime; }
					if (mainWindow.getsKeys()[GLFW_KEY_S]) { peachPosX -= forward.x * avatarMoveSpeed * deltaTime; peachPosZ -= forward.z * avatarMoveSpeed * deltaTime; }
					if (mainWindow.getsKeys()[GLFW_KEY_A]) { peachPosX -= right.x * avatarMoveSpeed * deltaTime; peachPosZ -= right.z * avatarMoveSpeed * deltaTime; }
					if (mainWindow.getsKeys()[GLFW_KEY_D]) { peachPosX += right.x * avatarMoveSpeed * deltaTime; peachPosZ += right.z * avatarMoveSpeed * deltaTime; }
				}
				break;

				// Control de Finn 
			case 3:
				finnRotY -= mainWindow.getXChange() * 0.5f;
				{
					glm::vec3 forward = glm::vec3(-sin(glm::radians(finnRotY)), 0.0f, -cos(glm::radians(finnRotY)));
					glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
					if (mainWindow.getsKeys()[GLFW_KEY_W]) { finnPosX += forward.x * avatarMoveSpeed * deltaTime; finnPosZ += forward.z * avatarMoveSpeed * deltaTime; }
					if (mainWindow.getsKeys()[GLFW_KEY_S]) { finnPosX -= forward.x * avatarMoveSpeed * deltaTime; finnPosZ -= forward.z * avatarMoveSpeed * deltaTime; }
					if (mainWindow.getsKeys()[GLFW_KEY_A]) { finnPosX -= right.x * avatarMoveSpeed * deltaTime; finnPosZ -= right.z * avatarMoveSpeed * deltaTime; }
					if (mainWindow.getsKeys()[GLFW_KEY_D]) {
						finnPosX += right.x * avatarMoveSpeed * deltaTime; finnPosZ += right.z * avatarMoveSpeed * deltaTime;
					}
				}
				break;
			}
		}


		cicloTimer += deltaTime;
		if (cicloTimer > cicloDuracion) {
			cicloTimer -= cicloDuracion;
		}
		tiempoDelDia = cicloTimer / cicloDuracion;
		float anguloSol = tiempoDelDia * 360.0f;
		float anguloSolRad = glm::radians(anguloSol);

		float intensidadSol_Difusa;   // Luz directa del sol
		float intensidadSol_Ambiente; // Luz del cielo 

		// ACTUALIZAR LUCES 
		float intensidadSol;
		float intensidadLamparas;

		float duracionTransicion = 0.1f;
		float t_atardecer_inicio = 0.5f - (duracionTransicion / 2.0f);
		float t_atardecer_fin = 0.5f + (duracionTransicion / 2.0f);
		float t_amanecer_inicio = 1.0f - (duracionTransicion / 2.0f);

		if (tiempoDelDia < t_atardecer_inicio)
		{
			intensidadSol_Difusa = 1.0f;
			intensidadSol_Ambiente = 0.3f;
			intensidadLamparas = 0.0f;
		}
		else if (tiempoDelDia < t_atardecer_fin)
		{
			float progreso = (tiempoDelDia - t_atardecer_inicio) / duracionTransicion;
			intensidadSol_Difusa = glm::mix(1.0f, 0.15f, progreso);
			intensidadSol_Ambiente = glm::mix(0.3f, 0.15f, progreso);
			intensidadLamparas = glm::mix(0.0f, 1.0f, progreso);
		}
		else if (tiempoDelDia < t_amanecer_inicio)
		{
			intensidadSol_Difusa = 0.15f;
			intensidadSol_Ambiente = 0.15f; // Luz de noche
			intensidadLamparas = 1.0f;
		}
		else //transicion de amanecer
		{
			float progreso = (tiempoDelDia - t_amanecer_inicio) / duracionTransicion;
			intensidadSol_Difusa = glm::mix(0.15f, 1.0f, progreso);
			intensidadSol_Ambiente = glm::mix(0.15f, 0.3f, progreso);
			intensidadLamparas = glm::mix(1.0f, 0.0f, progreso);
		}


		// Actualiza el Sol 
		mainLight.SetColor(1.0f, 1.0f, 1.0f);
		mainLight.SetDiffuseIntensity(intensidadSol_Difusa); // brillo del sol
		mainLight.SetAmbientIntensity(intensidadSol_Ambiente); // uz del cielo
		mainLight.SetDirection(glm::vec3(sin(anguloSolRad), -cos(anguloSolRad), 0.0f));

		// Actualizar las Lámparas 
		pointLights[0].SetDiffuseIntensity(intensidadLamparas);
		pointLights[0].SetAmbientIntensity(intensidadLamparas * 0.2f);

		pointLights[1].SetDiffuseIntensity(intensidadLamparas);
		pointLights[1].SetAmbientIntensity(intensidadLamparas * 0.2f);

		pointLights[2].SetDiffuseIntensity(intensidadLamparas);
		pointLights[2].SetAmbientIntensity(intensidadLamparas * 0.2f);

		pointLights[3].SetDiffuseIntensity(intensidadLamparas);
		pointLights[3].SetAmbientIntensity(intensidadLamparas * 0.2f);

		// Control de Lámparas con Teclado 
		float intensidadLamparasTeclado = 0.0f;
		if (spotLights_On) {
			intensidadLamparasTeclado = 1.0f; // Intensidad de encendido
		}

		// Aplicamos la intensidad
		pointLights[1].SetDiffuseIntensity(intensidadLamparasTeclado);
		pointLights[1].SetAmbientIntensity(intensidadLamparasTeclado * 0.2f);

		pointLights[2].SetDiffuseIntensity(intensidadLamparasTeclado);
		pointLights[2].SetAmbientIntensity(intensidadLamparasTeclado * 0.2f);

		pointLights[3].SetDiffuseIntensity(intensidadLamparasTeclado);
		pointLights[3].SetAmbientIntensity(intensidadLamparasTeclado * 0.2f);

		// RENDERIZADO 
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// DIBUJAR EL SKYBOX 
		//
		glm::mat4 viewMatrixSkybox;
		if (cameraMode) {
			float camOffsetX = sin(glm::radians(avatarRotY)) * 20.0f;
			float camOffsetZ = cos(glm::radians(avatarRotY)) * 20.0f;
			glm::vec3 posDeseada = glm::vec3(avatarPosX + camOffsetX, avatarPosY + 8.0f, avatarPosZ + camOffsetZ);
			glm::vec3 target = glm::vec3(avatarPosX, avatarPosY + 8.0f, avatarPosZ);
			viewMatrixSkybox = glm::lookAt(posDeseada, target, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		else {
			viewMatrixSkybox = camera.calculateViewMatrix();
		}

		// Decidimos qué skybox dibujar
		if (tiempoDelDia > 0.5) {
			skyboxNoche.DrawSkybox(viewMatrixSkybox, projection);
		}
		else {
			skybox.DrawSkybox(viewMatrixSkybox, projection);
		}


		// CONFIGURACIÓN DEL SHADER 
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		// CÁLCULO DE MATRIZ DE VISTA Y POSICIÓN DE OJO 

		glm::mat4 viewMatrix;
		glm::vec3 eyePosition;

		// Lógica de Cámara 
		switch (cameraMode)
		{
			// Modo 3ra Persona 
		case 1:
			// Elegir a quién seguir
			switch (activeAvatar)
			{
			case 1: // Mickey
			{
				float camOffsetX = sin(glm::radians(avatarRotY)) * 20.0f;
				float camOffsetZ = cos(glm::radians(avatarRotY)) * 20.0f;
				glm::vec3 posDeseada = glm::vec3(avatarPosX + camOffsetX, avatarPosY + 8.0f, avatarPosZ + camOffsetZ);
				glm::vec3 target = glm::vec3(avatarPosX, avatarPosY + 8.0f, avatarPosZ);
				viewMatrix = glm::lookAt(posDeseada, target, glm::vec3(0.0f, 1.0f, 0.0f));
				eyePosition = posDeseada;
			}
			break;
			case 2: // Peach
			{
				float camOffsetX = sin(glm::radians(peachRotY)) * 20.0f;
				float camOffsetZ = cos(glm::radians(peachRotY)) * 20.0f;
				glm::vec3 posDeseada = glm::vec3(peachPosX + camOffsetX, peachPosY + 8.0f, peachPosZ + camOffsetZ);
				glm::vec3 target = glm::vec3(peachPosX, peachPosY + 8.0f, peachPosZ);
				viewMatrix = glm::lookAt(posDeseada, target, glm::vec3(0.0f, 1.0f, 0.0f));
				eyePosition = posDeseada;
			}
			break;
			case 3: // Finn
			{
				float camOffsetX = sin(glm::radians(finnRotY)) * 20.0f;
				float camOffsetZ = cos(glm::radians(finnRotY)) * 20.0f;
				glm::vec3 posDeseada = glm::vec3(finnPosX + camOffsetX, finnPosY + 8.0f, finnPosZ + camOffsetZ);
				glm::vec3 target = glm::vec3(finnPosX, finnPosY + 8.0f, finnPosZ);
				viewMatrix = glm::lookAt(posDeseada, target, glm::vec3(0.0f, 1.0f, 0.0f));
				eyePosition = posDeseada;
			}
			break;
			}
			break;

		case 2:
		{
			camera.keyControl(mainWindow.getsKeys(), deltaTime);
			glm::vec3 pos = camera.getCameraPosition();
			pos.y = 150.0f;
			glm::vec3 target = glm::vec3(pos.x, 0.0f, pos.z);

			viewMatrix = glm::lookAt(pos, target, glm::vec3(0.0f, 0.0f, -1.0f));
			eyePosition = pos;
		}
		break;

		default:
			viewMatrix = camera.calculateViewMatrix();
			eyePosition = camera.getCameraPosition();
			break;
		}

		// Enviamos la matriz al shader
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniform3f(uniformEyePosition, eyePosition.x, eyePosition.y, eyePosition.z);


		// CONFIGURACIÓN DE LUCES 

		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		// Renderizado de la Escena 
		float radioOrbita = 800.0f;
		float solPosX, solPosY;
		//Ciclo dia/noche
		if (intensidadSol_Difusa > 0.2f)
		{
			solPosX = -cos(anguloSolRad) * radioOrbita;
			solPosY = abs(sin(anguloSolRad)) * radioOrbita;
		}
		else
		{
			solPosX = -cos(anguloSolRad) * radioOrbita;
			solPosY = -abs(sin(anguloSolRad)) * radioOrbita;
		}
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(solPosX, solPosY, -300.0f));
		model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Sol.UseTexture();
		Piedra_M.RenderModel();

		// Arco con Letrero
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.0f, -0.5f, 275.0f));//(-5.0f, -0.5f, 275.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(glm::vec2(0.0f)));
		Marco.UseTexture();
		Marco_M.RenderModel();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(glm::vec2(0.0f)));
		Letrero_M.RenderModel();

		// Letrero animado
		AdvTime_FontTexture.UseTexture();
		for (int i = 0; i < letrasVisibles; i++) {
			int idx = (indiceInicio + i) % offsets.size();
			glm::vec2 toffset = offsets[idx];
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glm::mat4 letraModel = glm::mat4(1.0f);
			float startX = basePos.x + (letrasVisibles - 1) * separacion;
			letraModel = glm::translate(letraModel, glm::vec3(startX - i * separacion, basePos.y, basePos.z));
			letraModel = glm::scale(letraModel, escalaLetras);
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(letraModel));
			meshList[4]->RenderMesh();
		}

		// Puertas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.0f, -0.5f, 275.0f));
		model = glm::rotate(model, -ang * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(glm::vec2(0.0f)));
		Puerta.UseTexture();
		PuertaIzquierda_M.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.0f, -0.5f, 275.0f));
		model = glm::translate(model, glm::vec3(puertaDerechaPos.x, puertaDerechaPos.z, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(glm::vec2(0.0f)));
		Puerta.UseTexture();
		PuertaDerecha_M.RenderModel();




		// Juego de Pelota
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(100.0f, -1.5f, -180.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(17.0f, 17.0f, 17.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(glm::vec2(0.0f)));
		Juego_Pelota_Texture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Juego_Pelota_M.RenderModel();

		// Jake
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(110.0f, -1.8f, -260.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(jakeAnguloGolpe), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Jake_M.RenderModel();

		// Pelota
		float pelotaBaseX = 114.5f;
		float pelotaBaseZ = -180.0f;
		pelotaOffset += pelotaVelocidad * pelotaDireccion * deltaTime;
		if (pelotaOffset > pelotaAmplitud) {
			pelotaOffset = pelotaAmplitud;
			pelotaDireccion = -1.0f;
		}
		else if (pelotaOffset < -pelotaAmplitud) {
			pelotaOffset = -pelotaAmplitud;
			pelotaDireccion = 1.0f;
		}
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(pelotaBaseX, pelotaPosBaseY, pelotaBaseZ + pelotaOffset));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(glm::vec2(0.0f)));
		PiedraTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Piedra_M.RenderModel();

		// Piso
		model = glm::mat4(1.0);
		modelaux = glm::mat4(1.0);
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		// Piramide Chichen
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(180.5f, -1.5f, 180.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.8f, 1.8f, 1.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Juego_Pelota_Texture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		PiramideT_M.RenderModel();

		// Piramide Sol
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-180.0f, -1.0f, -180.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		PiramideS_M.RenderModel();

		// Globo 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(globoPosX, globoPosY, globoPosZ));
		model = glm::rotate(model, glm::radians(globoRotY), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(14.0f, 14.0f, 14.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Globo_M.RenderModel();

		// Chozas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 60.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Chozas_M.RenderModel();

		// Arboles
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(210.0f, -0.5f, -210.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f, 4.2f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		arbol_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(190.0f, -0.5f, -165.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f, 4.2f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		arbol_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(170.0f, -0.5f, -115.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f, 4.2f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		arbol_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(190.0f, -0.5f, -75.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f, 4.2f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		arbol_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(190.0f, -0.5f, -25.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f, 4.2f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		arbol_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(170.0f, -0.5f, -5.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f, 4.2f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		arbol_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(190.0f, -0.5f, -0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f, 4.2f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		arbol_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.0f, -0.5f, 135.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f, 4.2f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		arbol_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.0f, -0.5f, 185.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f, 4.2f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		arbol_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.0f, -0.5f, 205.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f, 4.2f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		arbol_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-90.0f, -0.5f, 225.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f, 4.2f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		arbol_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-140.0f, -0.5f, 205.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f, 4.2f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		arbol_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-200.0f, -0.5f, 225.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f, 4.2f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		arbol_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-220.0f, -0.5f, 35.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f, 4.2f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		arbol_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-240.0f, -0.5f, 95.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f, 4.2f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		arbol_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-200.0f, -0.5f, 15.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f, 4.2f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		arbol_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-150.0f, -0.5f, 15.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f, 4.2f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		arbol_M.RenderModel();


		// Banca
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, -0.5f, -60.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BancaG_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, -0.5f, -60.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BancaG_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, -0.5f, -120.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BancaG_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, -0.5f, -120.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BancaG_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, -0.5f, -150.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BancaG_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, -0.5f, -150.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BancaG_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, -0.5f, -180.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BancaG_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, -0.5f, -180.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BancaG_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, -0.5f, -240.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BancaG_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, -0.5f, -240.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BancaG_M.RenderModel();


		// BancaT
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(30.0f, -0.5f, 240.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BancaT_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, -0.5f, 240.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BancaT_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(30.0f, -0.5f, 210.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BancaT_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, -0.5f, 210.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BancaT_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(30.0f, -0.5f, 180.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BancaT_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, -0.5f, 180.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BancaT_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(30.0f, -0.5f, 150.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BancaT_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, -0.5f, 150.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BancaT_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(30.0f, -0.5f, 120.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BancaT_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, -0.5f, 120.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BancaT_M.RenderModel();

		// Kiosco
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, -90.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, -210.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-145.0f, -0.5f, 90.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-185.0f, -0.5f, 90.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(145.0f, -0.5f, 50.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(210.0f, -0.5f, 50.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_M.RenderModel();

		// Pillar
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.0f, -0.5f, -50.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.2f, 5.2f, 5.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pillar_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-3.0f, -0.5f, -50.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.2f, 5.2f, 5.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pillar_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.0f, -0.5f, -110.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.2f, 5.2f, 5.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pillar_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-3.0f, -0.5f, -110.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.2f, 5.2f, 5.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pillar_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.0f, -0.5f, -170.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.2f, 5.2f, 5.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pillar_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-3.0f, -0.5f, -170.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.2f, 5.2f, 5.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pillar_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.0f, -0.5f, -230.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.2f, 5.2f, 5.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pillar_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-3.0f, -0.5f, -230.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.2f, 5.2f, 5.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pillar_M.RenderModel();



		//	Lampara
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 120.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f, 4.2f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		lampara_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 150.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f, 4.2f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		lampara_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 180.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f, 4.2f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		lampara_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 210.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f, 4.2f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		lampara_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 240.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f, 4.2f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		lampara_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 230.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f, 10.2f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		lampara_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.0f, -0.5f, 120.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.2f, 5.2f, 5.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pillar_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.0f, -0.5f, 220.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.2f, 5.2f, 5.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pillar_M.RenderModel();


		// Ring
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 12.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Ring_M.RenderModel();

		// PisoRing
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 12.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		PisoRing_M.RenderModel();

		// Cuerdas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 12.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cuerdas_M.RenderModel();

		// Coronas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 12.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Coronas_M.RenderModel();

		// EstatuaLuchador
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(32.0f, -0.5f, 32.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		EstatuaLuchador_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-32.0f, -0.5f, 32.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		EstatuaLuchador_M.RenderModel();

		// Calendario
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-180.0f, -0.5f, 180.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		CalendarioA_M.RenderModel();

		// PERSONAJES CON JERARQUÍA


		/*	//									//
					MICKEY MOUSE
		*/	//									//


		// Matriz Padre 
		// Posición general del avatar 
		glm::mat4 modelAvatar = glm::mat4(1.0);
		modelAvatar = glm::translate(modelAvatar, glm::vec3(avatarPosX, avatarPosY, avatarPosZ));
		modelAvatar = glm::rotate(modelAvatar, glm::radians(avatarRotY), glm::vec3(0.0f, 1.0f, 0.0f));
		modelAvatar = glm::scale(modelAvatar, glm::vec3(0.05f, 0.05f, 0.05f));

		// Torso (Padre) 
		glm::mat4 modelTorso = modelAvatar;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTorso));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		MickeyT_M.RenderModel();

		// Cabeza (Hijo del Torso) 
		glm::mat4 modelCabeza = modelTorso;
		modelCabeza = glm::translate(modelCabeza, glm::vec3(0.0f, -2.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCabeza));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		MickeyC_M.RenderModel();

		// Brazo Derecho (Hijo del Torso) 
		glm::mat4 modelBrazoD = modelTorso;
		modelBrazoD = glm::rotate(modelBrazoD, glm::radians(anguloCaminataMickey), glm::vec3(1.0f, 0.0f, 0.0f));
		modelBrazoD = glm::translate(modelBrazoD, glm::vec3(-1.0f, 3.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBrazoD));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		MickeyBD_M.RenderModel();

		// Brazo Izquierdo (Hijo del Torso) 
		glm::mat4 modelBrazoI = modelTorso;
		modelBrazoI = glm::rotate(modelBrazoI, glm::radians(-anguloCaminataMickey), glm::vec3(1.0f, 0.0f, 0.0f));
		modelBrazoI = glm::translate(modelBrazoI, glm::vec3(1.0f, 3.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBrazoI));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		MickeyBI_M.RenderModel();

		// Pierna Derecha (Hijo del Torso) 
		glm::mat4 modelPiernaD = modelTorso;
		modelPiernaD = glm::rotate(modelPiernaD, glm::radians(-anguloCaminataMickey), glm::vec3(1.0f, 0.0f, 0.0f));
		modelPiernaD = glm::translate(modelPiernaD, glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPiernaD));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		MickeyPD_M.RenderModel();

		// Pierna Izquierda (Hijo del Torso) 
		glm::mat4 modelPiernaI = modelTorso;
		modelPiernaI = glm::rotate(modelPiernaI, glm::radians(anguloCaminataMickey), glm::vec3(1.0f, 0.0f, 0.0f));
		modelPiernaI = glm::translate(modelPiernaI, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPiernaI));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		MickeyPI_M.RenderModel();


		/*	//									//


			/////		PEACH				//////


		*/	//									//


		// Matriz Padre Posición general 
		glm::mat4 modelAvatarPeach = glm::mat4(1.0);
		modelAvatarPeach = glm::translate(modelAvatarPeach, glm::vec3(peachPosX, peachPosY, peachPosZ));
		modelAvatarPeach = glm::rotate(modelAvatarPeach, glm::radians(peachRotY), glm::vec3(0.0f, 1.0f, 0.0f));
		modelAvatarPeach = glm::scale(modelAvatarPeach, glm::vec3(0.06f, 0.06f, 0.06f));

		// Torso (Padre de todas las partes)
		glm::mat4 modelTorsoPeach = modelAvatarPeach;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTorsoPeach));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		PeachT_M.RenderModel();

		// Cabeza (Hijo del Torso)
		glm::mat4 modelCabezaPeach = modelTorsoPeach;
		modelCabezaPeach = glm::translate(modelCabezaPeach, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCabezaPeach));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		PeachC_M.RenderModel();

		// Brazo Derecho (Hijo del Torso)
		glm::mat4 modelBrazoDPeach = modelTorsoPeach;
		modelBrazoDPeach = glm::translate(modelBrazoDPeach, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBrazoDPeach));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		PeachBD_M.RenderModel();

		// Brazo Izquierdo (Hijo del Torso)
		glm::mat4 modelBrazoIPeach = modelTorsoPeach;
		modelBrazoIPeach = glm::translate(modelBrazoIPeach, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBrazoIPeach));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		PeachBI_M.RenderModel();

		// Pierna Derecha (Hijo del Torso)
		glm::mat4 modelPiernaDPeach = modelTorsoPeach;
		modelPiernaDPeach = glm::translate(modelPiernaDPeach, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPiernaDPeach));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		PeachPD_M.RenderModel();


		// Pierna Izquierda (Hijo del Torso)
		glm::mat4 modelPiernaIPeach = modelTorsoPeach;
		modelPiernaIPeach = glm::translate(modelPiernaIPeach, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPiernaIPeach));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		PeachPI_M.RenderModel();



		/*	//									//


			/////		FINN				//////


		*/	//									//


		// Matriz Padre (Posición general en el mundo)
		glm::mat4 modelAvatarFinn = glm::mat4(1.0);
		modelAvatarFinn = glm::translate(modelAvatarFinn, glm::vec3(finnPosX, finnPosY, finnPosZ));
		modelAvatarFinn = glm::rotate(modelAvatarFinn, glm::radians(finnRotY), glm::vec3(0.0f, 1.0f, 0.0f));
		modelAvatarFinn = glm::scale(modelAvatarFinn, glm::vec3(0.9f, 0.9f, 0.9f));

		// Torso (Padre de todas las partes)
		glm::mat4 modelTorsoFinn = modelAvatarFinn;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTorsoFinn));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		FinnT_M.RenderModel();

		// Cabeza (Hijo del Torso)
		glm::mat4 modelCabezaFinn = modelTorsoFinn;
		modelCabezaFinn = glm::translate(modelCabezaFinn, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCabezaFinn));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		FinnC_M.RenderModel();

		// Brazo Derecho (Hijo del Torso)
		glm::mat4 modelBrazoDFinn = modelTorsoFinn;
		modelBrazoDFinn = glm::translate(modelBrazoDFinn, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBrazoDFinn));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		FinnBD_M.RenderModel();

		// Brazo Izquierdo (Hijo del Torso)
		glm::mat4 modelBrazoIFinn = modelTorsoFinn;
		modelBrazoIFinn = glm::translate(modelBrazoIFinn, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBrazoIFinn));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		FinnBI_M.RenderModel();

		// Pierna Derecha (Hijo del Torso)
		glm::mat4 modelPiernaDFinn = modelTorsoFinn;
		modelPiernaDFinn = glm::translate(modelPiernaDFinn, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPiernaDFinn));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		FinnPD_M.RenderModel();

		// Pierna Izquierda (Hijo del Torso)
		glm::mat4 modelPiernaIFinn = modelTorsoFinn;
		modelPiernaIFinn = glm::translate(modelPiernaIFinn, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPiernaIFinn));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		FinnPI_M.RenderModel();


		//	NPC1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-160.0f, -0.5f, 140.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.2f, 10.2f, 10.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		NPC1_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-185.0f, -0.5f, 140.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.2f, 10.2f, 10.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		NPC1_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-150.0f, -0.5f, 130.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.2f, 10.2f, 10.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		NPC1_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-140.0f, -0.5f, 130.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.2f, 10.2f, 10.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		NPC1_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-150.0f, -0.5f, -40.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.2f, 10.2f, 10.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		NPC1_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-140.0f, -0.5f, -40.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.2f, 10.2f, 10.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		NPC1_M.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(170.0f, -0.5f, 60.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.2f, 10.2f, 10.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		NPC1_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(150.0f, -0.5f, 60.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.2f, 10.2f, 10.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		NPC1_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(160.0f, -0.5f, 60.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.2f, 10.2f, 10.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		NPC1_M.RenderModel();

		//	NPC2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-180.0f, -0.5f, 140.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.2f, 10.2f, 10.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		NPC2_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-205.0f, -0.5f, 140.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.2f, 10.2f, 10.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		NPC2_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-180.0f, 22.0f, -80.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.2f, 10.2f, 10.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		NPC2_M.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(165.0f, -0.5f, 60.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.2f, 10.2f, 10.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		NPC2_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(175.0f, -0.5f, 60.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.2f, 10.2f, 10.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		NPC2_M.RenderModel();

		//	NPC3
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-200.0f, -0.5f, 140.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.2f, 10.2f, 10.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		NPC3_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-185.0f, -0.5f, 140.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.2f, 10.2f, 10.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		NPC3_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(195.0f, -0.5f, 60.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.2f, 10.2f, 10.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		NPC3_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(200.0f, -0.5f, 60.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.2f, 10.2f, 10.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		NPC3_M.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-180.0f, 22.0f, -80.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.2f, 10.2f, 10.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		NPC3_M.RenderModel();

		//Pluto 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(60.0f, 6.0f, -70.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pluto_M.RenderModel();


		//Joshi
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, 15.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Joshi_M.RenderModel();


		// Finalización del Frame 
		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}

