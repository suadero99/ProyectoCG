/*---------------------------------------------------------*/
/* ----------------  Proyecto Final CGeIHC      -----------*/
/*-----------------    2022-2   ---------------------------*/
/*------------- Alumnos: ----------------------------------*/
/*------------- Colin Santos Luis Froylan -----------------*/
/*------------- Najera Noyola Karla Andrea ----------------*/
/*---------------------------------------------------------*/

#include <Windows.h>

#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	//Texture

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>

//Librería de audio:
#if defined(WIN32)
#include <conio.h>
#endif
#include <irrKlang/irrKlang.h>

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

//#pragma comment(lib, "winmm.lib")

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor *monitors;

void getResolution(void);

// camera
Camera camera(glm::vec3(0.0f, 10.0f, 100.0f));
float MovementSpeed = 5.0f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);
glm::vec3 lightPositionSun(0.0f, 550.0f, 0.0f);
glm::vec3 luzColor(0.0f, 0.0f, 0.0f);
float cont = 0.0f;

// posiciones
//float x = 0.0f;
//float y = 0.0f;
bool	animacion = false,
		recorrido1 = true,
		recorrido2 = false,
		recorrido3 = false,
		recorrido4 = false,
		reversaAuto = false; //para "pingpong" en auto

int estadoAuto = 0;
//0 para detenidomovCamion
//1 para primera reversa
//2 para primera elevación
//3 para avance después de elevación
//4 para descenso
//5 para detenido al final?


//Para Gato camion
float	giroLlanta = 0.0f;
float	movCamion_x = 118.0f,
		movCamion_y = 0.0f,
		movCamion_z = 115.0f,
		orientaCamion = 180.0f;
bool	animacion_camion;
int		estado_camion=1;

//Para ovni (secuestro de Futaba)
float	orientaOvni = 0.0f,
		movOvni_x = 10.0f,
		movOvni_y = 30.0f,
		movOvni_z = 60.0f;
bool    animacion_ovni;
int     estado_Ovni=0;
float	escalaFutaba1 = 0.0f,
		escalaFutaba2 = 0.35f,
		movFutaba_y=-0.1;
int		contOvni = 0;


//Para tren
float	orientaCabina = 0.0f,
		movCabina_x = 51.0f,
		movCabina_y = -0.5f,
		movCabina_z = -90.0f,
		orientaVagon = 0.0f,
		movVagon_x = 70.0f,
		movVagon_y = 0.2f,
		movVagon_z = -90.0f;
int		estadoCabina = 0,
		estadoVagon = 0;
bool	animacion_tren;

float	movSol = 0.0f;

//Para animación de reloj
float	giroMins = 0.0f,
		giroHoras = 0.0f;
int		estado_reloj = 0;
bool	animacion_reloj;

//Para color de iluminación
float	colorR = 0.0f,
		colorG = 1.0f,
		colorB = 1.0f;

//Para animación del sol
bool	animacion_sol;

//Para globos de dialogo
float   eglobo_Joker = 0.0f,
		eglobo_Ann = 0.0f,
		eglobo_Akechi = 0.0f,
		eglobo_Morgana = 0.0f;
bool	animacion_globos;
int		estado_globos=0;
float	mov_globoY = 0.0f,
		mov_globoXZ = 0.0f;

//Keyframes (Manipulación y dibujo)
//Para cachetadas
//Joker
float giroCabezaJoker_y = 0.0f;
//Akechi
float giroBrazoAkechi_x = 0.0f; //o 90.0f?
//Ann
float giroBrazoDerechoAnn_x = 0.0f;
float giroBrazoDerechoAnn_y = 0.0f;

//Incrementos
float incGiroCabezaJoker_y = 0.0f;
//Akechi
float incGiroBrazoAkechi_x = 0.0f;
//Ann
float incGiroBrazoDerechoAnn_x = 0.0f;
float incGiroBrazoDerechoAnn_y = 0.0f;

#define MAX_FRAMES 9
int i_max_steps = 60;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames

	//Para cachetadas
	//Joker
	float giroCabezaJoker_y;
	//Akechi
	float giroBrazoAkechi_x;
	//Ann
	float giroBrazoDerechoAnn_x;
	float giroBrazoDerechoAnn_y;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl;

	KeyFrame[FrameIndex].giroCabezaJoker_y = giroCabezaJoker_y;
	KeyFrame[FrameIndex].giroBrazoAkechi_x = giroBrazoAkechi_x;
	KeyFrame[FrameIndex].giroBrazoDerechoAnn_x = giroBrazoDerechoAnn_x;
	KeyFrame[FrameIndex].giroBrazoDerechoAnn_y = giroBrazoDerechoAnn_y;
	/*
	std::cout << "giroCabezaJoker_y = " << giroCabezaJoker_y << std::endl;
	std::cout << "giroBrazoAkechi_x = " << giroBrazoAkechi_x << std::endl;
	std::cout << "giroBrazoDerechoAnn_x = " << giroBrazoDerechoAnn_x << std::endl;
	std::cout << "giroBrazoDerechoAnn_y = " << giroBrazoDerechoAnn_y << std::endl;
	*/

	FrameIndex++;
}

//Para nuestra animación por KeyFrames
void insertarFrames(void) {
	//Frame 0:
	giroCabezaJoker_y = 0.0f;
	giroBrazoAkechi_x = 0.0f;
	giroBrazoDerechoAnn_x = 0.0f;
	giroBrazoDerechoAnn_y = 0.0f;
	if (FrameIndex < MAX_FRAMES)
	{
		saveFrame();
	}
	//Frame 1
	giroCabezaJoker_y = 0.0f;
	giroBrazoAkechi_x = 0.0f;
	giroBrazoDerechoAnn_x = -18.6f;
	giroBrazoDerechoAnn_y = -107.7f;
	if (FrameIndex < MAX_FRAMES)
	{
		saveFrame();
	}
	//Frame 2
	giroCabezaJoker_y = -24.6f;
	giroBrazoAkechi_x = 0.0f;
	giroBrazoDerechoAnn_x = 17.1f;
	giroBrazoDerechoAnn_y = -115.2f;
	if (FrameIndex < MAX_FRAMES)
	{
		saveFrame();
	}
	//Frame 3
	giroCabezaJoker_y = 0.600001f;
	giroBrazoAkechi_x = 0.0f;
	giroBrazoDerechoAnn_x = 59.6999f;
	giroBrazoDerechoAnn_y = -115.2f;
	if (FrameIndex < MAX_FRAMES)
	{
		saveFrame();
	}
	//Frame 4
	giroCabezaJoker_y = 44.9999f;
	giroBrazoAkechi_x = 0.0f;
	giroBrazoDerechoAnn_x = 26.1f;
	giroBrazoDerechoAnn_y = -115.2f;
	if (FrameIndex < MAX_FRAMES)
	{
		saveFrame();
	}
	//Frame 5
	giroCabezaJoker_y = 4.5f;
	giroBrazoAkechi_x = 0.0f;
	giroBrazoDerechoAnn_x = -68.6999f;
	giroBrazoDerechoAnn_y = -47.0999f;
	if (FrameIndex < MAX_FRAMES)
	{
		saveFrame();
	}
	//Frame 6
	giroCabezaJoker_y = 4.49999f;
	giroBrazoAkechi_x = 172.801f;
	giroBrazoDerechoAnn_x = -68.6998f;
	giroBrazoDerechoAnn_y = -5.99983f;
	if (FrameIndex < MAX_FRAMES)
	{
		saveFrame();
	}
	//Frame 7
	giroCabezaJoker_y = 0.0f;
	giroBrazoAkechi_x = 0.0f;
	giroBrazoDerechoAnn_x = 0.0f;
	giroBrazoDerechoAnn_y = 0.0f;
	if (FrameIndex < MAX_FRAMES)
	{
		saveFrame();
	}
}

void resetElements(void)
{
	giroCabezaJoker_y = KeyFrame[0].giroCabezaJoker_y;
	giroBrazoAkechi_x = KeyFrame[0].giroBrazoAkechi_x;
	giroBrazoDerechoAnn_x = KeyFrame[0].giroBrazoDerechoAnn_x;
	giroBrazoDerechoAnn_y = KeyFrame[0].giroBrazoDerechoAnn_y;
}

void interpolation(void)
{
	incGiroCabezaJoker_y = (KeyFrame[playIndex + 1].giroCabezaJoker_y - KeyFrame[playIndex].giroCabezaJoker_y) / i_max_steps;
	incGiroBrazoAkechi_x = (KeyFrame[playIndex + 1].giroBrazoAkechi_x - KeyFrame[playIndex].giroBrazoAkechi_x) / i_max_steps;
	incGiroBrazoDerechoAnn_x = (KeyFrame[playIndex + 1].giroBrazoDerechoAnn_x - KeyFrame[playIndex].giroBrazoDerechoAnn_x) / i_max_steps;
	incGiroBrazoDerechoAnn_y = (KeyFrame[playIndex + 1].giroBrazoDerechoAnn_y - KeyFrame[playIndex].giroBrazoDerechoAnn_y) / i_max_steps;
}

void animate(void)
{
	//Animación del sol
	if (animacion_sol) {
		lightPositionSun.x = 500.0f * cos(movSol);
		lightPositionSun.y = 500.0f * sin(movSol);
		if (lightPositionSun.y < 0 || lightPositionSun.x < 0) {
			movSol += 0.005f;
		}
		else {
			movSol += 0.0025f;
		}
	}
	

	//Para reloj:
	if (animacion_reloj) {
		giroMins += 0.3f;
		if (giroMins >=360.0f) {
			giroHoras += 30.0f;
			giroMins = 0.0f;
			
		}
	}

	//Para cachetadas a Joker


	//--------------------Animación de tren
	if (animacion_tren) {
		switch (estadoCabina) {
		case 0:
			orientaCabina = 0.0f;
			if (movCabina_x>=-115.0f) {
				movCabina_x -= 1.0f;
			} else {
				estadoCabina = 1;
			}
			break;
		case 1:
			orientaCabina = 90.0f;
			if (movCabina_z <= 115.0f) {
				movCabina_z += 1.0f;
			}
			else {
				estadoCabina = 2;
			}
			break;
		case 2:
			orientaCabina = 180.0f;
			if (movCabina_x <= -15.0f) {
				movCabina_x += 1.0f;
			}
			else {
				estadoCabina = 3;
			}
			break;
		case 3:
			orientaCabina = 205.23f;
			if (movCabina_x <= 90.0f) {
				movCabina_x += 1.0;
				movCabina_z -= 0.47116f;
			}
			else {
				//cout << "Z final: " << movVagon_z << endl;
				//cout << "X final: " << movVagon_x << endl;
				movCabina_z = 65.0f;
				estadoCabina = 4;
			}
			break;
		case 4:
			orientaCabina = 270.0f;
			if (movCabina_z >= -90.0f) {
				movCabina_z -= 1.0f;
			}
			else {
				estadoCabina = 0;
			}
			break;
		}
		switch (estadoVagon) {
		case 0:
			orientaVagon = 0.0f;
			if (movVagon_x >= -115.0f) {
				movVagon_x -= 1.0f;
			}
			else {
				estadoVagon = 1;
			}
			break;
		case 1:
			orientaVagon = 90.0f;
			if (movVagon_z <= 115.0f) {
				movVagon_z += 1.0f;
			}
			else {
				estadoVagon = 2;
			}
			break;
		case 2:
			orientaVagon = 180.0f;
			//cout << "Z inicial: " << movVagon_z << endl;
			if (movVagon_x <= -15.0f) {
				movVagon_x += 1.0f;
			}
			else {
				estadoVagon = 3;
				//cout << "X inicial: " << movVagon_x << endl;
			}
			break;
		case 3:
			//cout << "Z: " << movVagon_z << endl;
			//cout << "X: " << movVagon_x << endl;
			//m = xf-xi / zf - zi = (90 + 14) / (65 - 116) = 104/-49 = -2.1224f
			//theta = -64.77f; 270 - theta = angulo
			orientaVagon = 205.23f; 
			if (movVagon_x <= 90.0f) {
				movVagon_x += 1.0;
				movVagon_z -= 0.47116f;
			}
			else {
				//cout << "Z final: " << movVagon_z << endl;
				//cout << "X final: " << movVagon_x << endl;
				movVagon_z = 65.0f;
				estadoVagon = 4;
			}
			break;
		case 4:
			orientaVagon = 270.0f;
			if (movVagon_z >= -90.0f) {
				movVagon_z -= 1.0f;
			}
			else {
				estadoVagon = 0;
			}
			break;
		}
	}

	//Para gato camion
	if (animacion_camion) {
		//giroLlanta += 0.2f;
		//orientaCamion += 0.2f;
		switch (estado_camion) {
			case 1:
				if (movCamion_z >= 70.0f) {
					giroLlanta += 1.0f;
					movCamion_z -= 1.0f;
				}
				else {
					estado_camion++;
				}
				break;
			case 2: //Salto 1
				if (movCamion_z >= 60.0f) {
					giroLlanta += 0.1f;
					movCamion_z -= 1.3;
					movCamion_y += 0.3f;
				}
				else {
					estado_camion++;
				}
				break;
			case 3:
				if (movCamion_z >= -60.0f) {
					giroLlanta += 1.0f;
					movCamion_z -= 1.0f;
					if (movCamion_y > 0.0f)
						movCamion_y -= 0.3f;
					else
						movCamion_y = 0.0f;
				}
				else {
					estado_camion++;
					movCamion_y = 0.0f;
				}
				break;
			case 4: //Salto 2
				if (movCamion_z >= -70.0f) {
					giroLlanta += 0.1f;
					movCamion_z -= 1.3;
					movCamion_y += 0.3f;
				}
				else {
					estado_camion++;
				}
				break;
			case 5:
				if (movCamion_z >= -107.0f) {
					giroLlanta += 1.0f;
					movCamion_z -= 1.0f;
					if (movCamion_y > 0.0f)
						movCamion_y -= 0.3f;
					else
						movCamion_y = 0.0f;
				}
				else {
					estado_camion++;
					movCamion_y = 0.0f;
					orientaCamion = -90.0f;
					movCamion_z = -107.0f;
				}
				break;
			case 6:
				if (movCamion_x >= -7.0f) {
					giroLlanta += 1.0f;
					movCamion_x -= 1.0f;
				}
				else {
					estado_camion++;
					orientaCamion = 0.0f;
					movCamion_x = -7.0f;
				}
				break;
			case 7:
				if (movCamion_z <= -70.0f) {
					giroLlanta += 1.0f;
					movCamion_z += 1.0f;
				}
				else {
					estado_camion++;
				}
				break;
			case 8: //Salto 3
				if (movCamion_z <= -60.0f) {
					giroLlanta += 0.1f;
					movCamion_z += 1.3;
					movCamion_y += 0.3f;
				}
				else {
					estado_camion++;
				}
				break;
			case 9:
				if (movCamion_z <= 60.0f) {
					giroLlanta += 1.0f;
					movCamion_z += 1.0f;
					if (movCamion_y > 0.0f)
						movCamion_y -= 0.3f;
					else
						movCamion_y = 0.0f;
				}
				else {
					estado_camion++;
					movCamion_y = 0.0f;
				}
				break;
			case 10: //Salto 4
				if (movCamion_z <= 70.0f) {
					giroLlanta += 0.1f;
					movCamion_z += 1.3;
					movCamion_y += 0.3f;
				}
				else {
					estado_camion++;
				}
				break;
			case 11:
				if (movCamion_z <= 115.0f) {
					giroLlanta += 1.0f;
					movCamion_z += 1.0f;
					if (movCamion_y > 0.0f)
						movCamion_y -= 0.3f;
					else
						movCamion_y = 0.0f;
				}
				else {
					estado_camion++;
					movCamion_y = 0.0f;
					orientaCamion = 90.0f;
				}
				break;
			case 12:
				if (movCamion_x <= 118.0f) {
					giroLlanta += 1.0f;
					movCamion_x += 1.0f;
				}
				else {
					estado_camion = 1;
					orientaCamion = 180.0f;
					movCamion_x = 118.0f;
				}
				break;
		}
	}

	//Animacion 4: Secuestro de Futaba
	if (animacion_ovni) {
		orientaOvni -= 1.0f;
		switch (estado_Ovni) {
			case 0:
				if (movOvni_z>=85) {
					estado_Ovni = 1;
				}
				else {
					movOvni_x += 0.02;
					movOvni_z += 0.1;
				}
				break;
			case 1:
				//Desaparece Futaba 2 y aparece Futaba 1
				escalaFutaba2 = 0.0f;
				escalaFutaba1 = 0.35f;
				estado_Ovni = 2;
				break;
			case 2:
				//Futaba 1 viaja hacia arriba con escala, rotación y traslación en Y
				if (escalaFutaba1>=0) {
					//Movimiento y decremeneto
					escalaFutaba1 -= 0.001;
					movFutaba_y += 0.1;
				}
				else {
					estado_Ovni = 3;
				}
				break;
			case 3:
				//Pequeño delay xD
				if (contOvni<=120) {
					contOvni++;
				}
				else {
					estado_Ovni = 4;
					contOvni = 0;
				}
				break;
			case 4:
				//Futaba hacia abajo con escala, rotación y traslación en Y 
				if (escalaFutaba1 < 0.35) {
					//Movimiento y decremeneto
					escalaFutaba1 += 0.001;
					movFutaba_y -= 0.1;
				}
				else {
					estado_Ovni = 5;
				}
				break;
			case 5:
				//Futaba 1 desaparece y aparece Futaba 1
				escalaFutaba2 = 0.35f;
				escalaFutaba1 = 0.0f;
				estado_Ovni = 6;
				break;
			case 6:
				//Ovni regresa a posición de inicio
				if (movOvni_z <= 60) {
					estado_Ovni = 7;
				}
				else {
					movOvni_x -= 0.02;
					movOvni_z -= 0.1;
				}
				break;

		}
	}
	else {
		orientaOvni -= 0.2f; //Garantiza que el ovni siempre esté en movimiento
	}


	//------------------Para luz que cambia de color
	if (colorR <= 1.0f) {
		colorR += 0.01;
	}
	else {
		colorR = 0.0f;
	}

	if (colorG >= 0.0f) {
		colorG -= 0.01;
	}
	else {
		colorG = 1.0f;
	}

	if (colorB >= 0.0f) {
		colorB -= 0.02;
	}
	else {
		colorB = 1.0f;
	}

	//Animación 6: Globos de dialogo
	if (animacion_globos) {
		switch (estado_globos) {
		case 0:
			if (eglobo_Akechi<1.5f) {
				eglobo_Akechi+=0.05;
				mov_globoY += 0.01;
				mov_globoXZ += 0.01;
			}
			else {
				estado_globos = 1;
			}
			break;
		case 1:
			if (eglobo_Akechi >0.0f) {
				eglobo_Akechi-=0.05;
			}
			else {
				estado_globos = 2;
				mov_globoY = 0.0f;
				mov_globoXZ = 0.0f;
			}
			break;
		case 2:
			if (eglobo_Joker < 1.5f) {
				eglobo_Joker += 0.05;
				eglobo_Morgana += 0.05;
				mov_globoY += 0.01;
				mov_globoXZ += 0.01;
			}
			else {
				estado_globos = 3;
			}
			break;
		case 3:
			if (eglobo_Joker > 0.0f) {
				eglobo_Joker -= 0.05;
				eglobo_Morgana -= 0.05;
			}
			else {
				estado_globos = 4;
				mov_globoY = 0.0f;
				mov_globoXZ = 0.0f;
			}
			break;
		case 4:
			if (eglobo_Ann < 1.5f) {
				eglobo_Ann += 0.05;
				mov_globoY += 0.01;
				mov_globoXZ += 0.01;
			}
			else {
				estado_globos = 5;
			}
			break;
		case 5:
			if (eglobo_Ann > 0.0f) {
				eglobo_Ann -= 0.05;
			}
			else {
				estado_globos = 6;
				mov_globoY = 0.0f;
				mov_globoXZ = 0.0f;
			}
			break;
		case 6:
			if (eglobo_Morgana < 1.5f) {
				eglobo_Akechi += 0.05;
				eglobo_Morgana += 0.05;
				mov_globoY += 0.01;
				mov_globoXZ += 0.01;
			}
			else {
				estado_globos = 7;
			}
			break;
		case 7:
			if (eglobo_Morgana > 0.0f) {
				eglobo_Morgana -= 0.05;
				eglobo_Akechi -= 0.05;
			}
			else {
				estado_globos = 8;
				mov_globoY = 0.0f;
				mov_globoXZ = 0.0f;
			}
			break;
		case 8:
			if (eglobo_Ann < 1.5f) {
				eglobo_Ann += 0.05;
				mov_globoY += 0.01;
				mov_globoXZ += 0.01;
			}
			else {
				estado_globos = 9;
			}
			break;
		case 9:
			if (eglobo_Ann > 0.0f) {
				eglobo_Ann -= 0.05;
			}
			else {
				estado_globos = 10;
				mov_globoY = 0.0f;
				mov_globoXZ = 0.0f;
			}
			break;
		case 10:
			if (eglobo_Akechi < 1.5f) {
				eglobo_Akechi += 0.05;
				mov_globoY += 0.01;
				mov_globoXZ += 0.01;
			}
			else {
				estado_globos = 11;
			}
			break;
		case 11:
			if (eglobo_Akechi > 0.0f) {
				eglobo_Akechi -= 0.05;
			}
			else {
				estado_globos = 12;
				mov_globoY = 0.0f;
				mov_globoXZ = 0.0f;
			}
			break;
		case 12:
			if (eglobo_Joker < 1.5f) {
				eglobo_Joker += 0.05;
				mov_globoY += 0.01;
				mov_globoXZ += 0.01;
			}
			else {
				estado_globos = 13;
			}
			break;
		case 13:
			if (eglobo_Joker > 0.0f) {
				eglobo_Joker -= 0.05;
			}
			else {
				estado_globos = 14;
				mov_globoY = 0.0f;
				mov_globoXZ = 0.0f;
			}
			break;
		case 14:
			if (eglobo_Ann < 1.5f) {
				eglobo_Morgana += 0.05;
				eglobo_Ann += 0.05;
				mov_globoY += 0.01;
				mov_globoXZ += 0.01;
			}
			else {
				estado_globos = 15;
			}
			break;
		case 15:
			if (eglobo_Ann > 0.0f) {
				eglobo_Morgana -= 0.05;
				eglobo_Ann -= 0.05;
			}
			else {
				estado_globos = 16;
				mov_globoY = 0.0f;
				mov_globoXZ = 0.0f;
			}
			break;
		case 16:
			if (eglobo_Akechi < 1.5f) {
				eglobo_Akechi += 0.05;
				mov_globoY += 0.01;
				mov_globoXZ += 0.01;
			}
			else {
				estado_globos = 17;
			}
			break;
		case 17:
			if (eglobo_Akechi > 0.0f) {
				eglobo_Akechi -= 0.05;
			}
			else {
				estado_globos = 18;
				mov_globoY = 0.0f;
				mov_globoXZ = 0.0f;
			}
			break;
		case 18:
			if (eglobo_Morgana < 1.5f) {
				eglobo_Morgana += 0.05;
				mov_globoY += 0.01;
				mov_globoXZ += 0.01;
			}
			else {
				estado_globos = 19;
			}
			break;
		case 19:
			if (eglobo_Morgana > 0.0f) {
				eglobo_Morgana -= 0.05;
			}
			else {
				estado_globos = 0;
				mov_globoY = 0.0f;
				mov_globoXZ = 0.0f;
			}
			break;
		}
	}

	//Animación por keyframes
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
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
			giroCabezaJoker_y += incGiroCabezaJoker_y;
			giroBrazoAkechi_x += incGiroBrazoAkechi_x;
			giroBrazoDerechoAnn_x += incGiroBrazoDerechoAnn_x;
			giroBrazoDerechoAnn_y += incGiroBrazoDerechoAnn_y;

			i_curr_steps++;
		}
	}
}

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}


int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	// --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	//Inicio de sound engine
	irrklang::ISoundEngine* bg_music = irrklang::createIrrKlangDevice();

	if (!bg_music)
		return 0; //Error con la música de fondo

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CGeIHC", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");

	vector<std::string> faces
	{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg"
	};

	Skybox skybox = Skybox(faces);

	//Reproducir música de fondo
	//La apago porque sino vamos a enloquecer xD
	//bg_music->play2D("resources\\sounds\\bg_music\\The_Whims_of_Fate.flac", true);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models
	// -----------
	Model piso("resources/objects/piso/piso.obj");
	Model qfront("resources/objects/qfront/qfront_chido.obj");
	Model magnet("resources/objects/magnet/magnet.obj");
	Model torniquetes("resources/objects/estacion/torniquetes.obj");
	Model estacion("resources/objects/estacion/estacion.obj");
	Model espera("resources/objects/estacion/espera_trenes.obj");
	Model piramide("resources/objects/piramide/piramide.obj");
	Model hachiko("resources/objects/hachiko/buchiko_estatua.obj");

	//Vehiculos
	Model ovni("resources/objects/ovni/ovni.obj");
	Model vagon("resources/objects/tren/tren.obj");
	Model cabina("resources/objects/tren/cabina.obj");
	Model camion("resources/objects/camion_gato/magiccatbus.obj"); //Camión gato
	Model rueda("resources/objects/camion_gato/llantita_no_lonja.obj"); //ruedas de camión gato

	//Flora
	Model arbol("resources/objects/flora/green_tree.obj");
	Model arbusto("resources/objects/flora/arbusto.obj");
	Model circulo("resources/objects/flora/circulo-para-arbol.obj");
	Model planta("resources/objects/flora/planta_amarilla.obj");

	//Para reloj
	Model minutos("resources/objects/reloj/manecilla_minutos.obj"); //Manecilla de minutos
	Model horas("resources/objects/reloj/manecilla_horas.obj"); //Manecilla de horas

	//Morgana
	Model cabezaMorgana("resources/objects/morgana/cabeza.obj");
	Model torsoMorgana("resources/objects/morgana/torso.obj");
	Model brazoMorgana("resources/objects/morgana/brazo_completo.obj");
	Model piernaMorgana("resources/objects/morgana/pierna.obj");
	Model patasCorriendoMorgana("resources/objects/morgana/patas_correr.obj");

	//Joker
	Model cabezaJoker("resources/objects/joker/cabeza.obj");
	Model torsoJoker("resources/objects/joker/torso.obj");
	Model brazoJoker("resources/objects/joker/brazo.obj");
	Model piernaJoker("resources/objects/joker/pierna.obj");

	//Novio Joker
	Model cabezaAkechi("resources/objects/novio_joker/cabeza.obj");
	Model torsoAkechi("resources/objects/novio_joker/torso.obj");
	Model brazoAkechi("resources/objects/novio_joker/brazo.obj");
	Model piernaAkechi("resources/objects/novio_joker/pierna.obj");

	//Ann
	Model cabezaAnn("resources/objects/ann/cabeza.obj");
	Model torsoAnn("resources/objects/ann/torso.obj");
	Model brazoAnn("resources/objects/ann/brazo.obj");
	Model piernaAnn("resources/objects/ann/pierna1.obj");
	Model pierna2Ann("resources/objects/ann/pierna2.obj");

	//Futaba 1 (Flotando)
	ModelAnim Futaba1("resources/objects/Futaba/Floating/Floating.dae");
	Futaba1.initShaders(animShader.ID);

	//Futaba 2 (Gritando)
	ModelAnim Futaba2("resources/objects/Futaba/Yelling/Yelling.dae");
	Futaba2.initShaders(animShader.ID);

	//Globo de dialogo
	Model globo("resources/objects/globo_dialogo/globo_con_dialogo.obj");


	//Inicialización de KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].giroCabezaJoker_y = 0.0f;
		KeyFrame[i].giroBrazoAkechi_x = 0.0f;
		KeyFrame[i].giroBrazoDerechoAnn_x = 0.0f;
		KeyFrame[i].giroBrazoDerechoAnn_y = 0.0f;
	}

	insertarFrames();

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);
		
		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		staticShader.use();
		//Setup Advanced Lights
		// Iluminación
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", glm::vec3(0.125f, 0.125f, 0.125f));
		staticShader.setVec3("dirLight.diffuse", glm::vec3(0.125f, 0.125f, 0.125f));
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));

		//Sol
		staticShader.setVec3("pointLight[0].position", lightPositionSun);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.3f, 0.3f, 0.3f));
		staticShader.setFloat("pointLight[0].constant", 0.08f);
		staticShader.setFloat("pointLight[0].linear", 0.0009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.000004f);

		staticShader.setVec3("pointLight[1].position", glm::vec3(150.0f, 5.0f, 0.0f));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 0.08f);
		staticShader.setFloat("pointLight[1].linear", 0.009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.0032f);

		//Luz que cambia de color
		staticShader.setVec3("pointLight[2].position", glm::vec3(-80.0f, 5.0f, 10.0f));
		staticShader.setVec3("pointLight[2].ambient", glm::vec3(colorR, colorG, colorB));
		staticShader.setVec3("pointLight[2].diffuse", glm::vec3(colorR, colorG, colorB));
		staticShader.setVec3("pointLight[2].specular", glm::vec3(colorR, colorG, colorB));
		staticShader.setFloat("pointLight[2].constant", 0.08f);
		staticShader.setFloat("pointLight[2].linear", 0.009f);
		staticShader.setFloat("pointLight[2].quadratic", 0.0032f);

		staticShader.setFloat("material_shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//// Light
		glm::vec3 lightColor = glm::vec3(0.6f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);
		

		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		//Remember to activate the shader with the animation
		animShader.use();
		animShader.setMat4("projection", projection);
		animShader.setMat4("view", view);
	
		animShader.setVec3("material.specular", glm::vec3(0.5f));
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", lightDirection);
		animShader.setVec3("viewPos", camera.Position);

		//Dibujo Futaba 1
		model = glm::translate(glm::mat4(1.0f), glm::vec3(15.0f, movFutaba_y, 85.0f));
		model = glm::scale(model, glm::vec3(escalaFutaba1));
		animShader.setMat4("model", model);
		Futaba1.Draw(animShader);

		//Dibujo Futaba 2
		model = glm::translate(glm::mat4(1.0f), glm::vec3(15.0f, 0.0f, 85.0f));
		model = glm::scale(model, glm::vec3(escalaFutaba2));
		animShader.setMat4("model", model);
		Futaba2.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario (Objetos estáticos)
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//Piso
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);

		//Dibujo edificio QFront
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-92.0f, -1.0f, 40.0f));
		model = glm::rotate(model, glm::radians(138.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.5f,2.5f,3.1f));
		staticShader.setMat4("model", model);
		qfront.Draw(staticShader);

		//Dibujo edificio Magnet
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-65.0f, -0.7f, -38.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.7f));
		staticShader.setMat4("model", model);
		magnet.Draw(staticShader);

		//Dibujo torniquetes
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(55.0f, 2.3f, -2.0f));
		model = glm::scale(model, glm::vec3(2.35f));
		staticShader.setMat4("model", model);
		torniquetes.Draw(staticShader);

		//Dibujo estacion
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(55.0f, -1.0f, -37.0f));
		model = glm::scale(model, glm::vec3(2.35f));
		staticShader.setMat4("model", model);
		estacion.Draw(staticShader);

		//Dibujo espera
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(55.0f, -1.0f, -72.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.35f));
		staticShader.setMat4("model", model);
		espera.Draw(staticShader);

		//Dibujo pirámide
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(25.0f, -0.7f, 60.0f));
		model = glm::scale(model, glm::vec3(3.5f));
		staticShader.setMat4("model", model);
		piramide.Draw(staticShader);

		//Hachiko
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(20.0f, -0.9f, 0.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.2f));
		staticShader.setMat4("model", model);
		hachiko.Draw(staticShader);

		//Circulos cerca de Hachiko
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(15.0f, -0.9f, -5.0f));
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.2f));
		staticShader.setMat4("model", model);
		circulo.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(25.0f, -0.9f, -5.0f));
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.2f));
		staticShader.setMat4("model", model);
		circulo.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(25.0f, -0.9f, -35.0f));
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.2f));
		staticShader.setMat4("model", model);
		circulo.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(15.0f, -0.9f, -65.0f));
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.2f));
		staticShader.setMat4("model", model);
		circulo.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(25.0f, -0.9f, -65.0f));
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.2f));
		staticShader.setMat4("model", model);
		circulo.Draw(staticShader);

		//Cabina
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(movCabina_x, movCabina_y, movCabina_z));
		model = glm::rotate(model, glm::radians(90.0f + orientaCabina), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		staticShader.setMat4("model", model);
		cabina.Draw(staticShader);

		//Vagon
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(movVagon_x, movVagon_y, movVagon_z));
		model = glm::rotate(model, glm::radians(90.0f + orientaVagon), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		staticShader.setMat4("model", model);
		vagon.Draw(staticShader);

		//Reloj
		//Manecilla minutos
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(55.0f, 33.7f, -59.4f));
		model = glm::rotate(model, glm::radians(-giroMins), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(2.35f));
		staticShader.setMat4("model", model);
		minutos.Draw(staticShader);
		//Manecilla horas
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(55.0f, 33.7f, -59.2f));
		model = glm::rotate(model, glm::radians(-giroHoras), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(2.35f,1.55f,2.35f));
		staticShader.setMat4("model", model);
		horas.Draw(staticShader);

		//Ovni
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(movOvni_x, movOvni_y, movOvni_z));
		model = glm::rotate(model, glm::radians(orientaOvni), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		//staticShader.setVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		ovni.Draw(staticShader);

		//**********************Plantitas**********************************
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(65.0f, -0.7f, 95.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.2f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(95.0f, -0.7f, 96.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.2f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(92.0f, -0.7f, 86.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.2f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(75.0f, -0.7f, 90.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.2f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(90.0f, -0.7f, 92.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.2f));
		staticShader.setMat4("model", model);
		arbusto.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(80.0f, -0.7f, 85.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.2f));
		staticShader.setMat4("model", model);
		arbusto.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(80.0f, -0.7f, 95.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.4f));
		staticShader.setMat4("model", model);
		planta.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(85.0f, -0.7f, 95.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.4f));
		staticShader.setMat4("model", model);
		planta.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(63.0f, -0.7f, 95.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.4f));
		staticShader.setMat4("model", model);
		planta.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(73.0f, -0.7f, 90.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.4f));
		staticShader.setMat4("model", model);
		planta.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(76.0f, -0.7f, 99.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.4f));
		staticShader.setMat4("model", model);
		planta.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(92.0f, -0.7f, 98.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.4f));
		staticShader.setMat4("model", model);
		planta.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(82.0f, -0.7f, 96.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.2f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(72.0f, -0.7f, 95.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.2f));
		staticShader.setMat4("model", model);
		arbusto.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-52.0f, -0.7f, 96.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.2f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-32.0f, -0.7f, 90.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.2f));
		staticShader.setMat4("model", model);
		arbusto.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-35.0f, -0.7f, 86.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.2f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-40.0f, -0.7f, 78.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.2f));
		staticShader.setMat4("model", model);
		arbusto.Draw(staticShader);


		// -------------------------------------------------------------------------------------------------------------------------
		// Camión gato
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(movCamion_x, movCamion_y, movCamion_z));
		tmp = model = glm::rotate(model, glm::radians(orientaCamion), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(2.5f));
		staticShader.setMat4("model", model);
		//staticShader.setVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		camion.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(1.7f, 0.0f, 2.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(giroLlanta), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		rueda.Draw(staticShader);	//delantera der

		model = glm::translate(tmp, glm::vec3(-1.7f, 0.0f, 2.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(giroLlanta), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		rueda.Draw(staticShader);	//delantera izq

		model = glm::translate(tmp, glm::vec3(1.7f, 0.0f, -2.4f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(giroLlanta), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		rueda.Draw(staticShader);	//trasera der

		model = glm::translate(tmp, glm::vec3(-1.7f, 0.0f, -2.4f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(giroLlanta), glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		rueda.Draw(staticShader);	//trasera izq

		// -------------------------------------------------------------------------------------------------------------------------
		// Morgana
		// -------------------------------------------------------------------------------------------------------------------------

		//Torso
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(13.0f, -1.0f, -30.0f));
		tmp = model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.3f));
		staticShader.setMat4("model", model);
		torsoMorgana.Draw(staticShader);

		//Brazo derecho
		model = glm::translate(tmp, glm::vec3(-0.2f, 0.2f, 0.0f));
		//model = glm::translate(model, glm::vec3(0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.3f));
		staticShader.setMat4("model", model);
		brazoMorgana.Draw(staticShader);

		//Brazo izquierdo
		model = glm::translate(tmp, glm::vec3(0.2f, 0.2f, 0.0f));
		//model = glm::translate(model, glm::vec3(0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.3f));
		staticShader.setMat4("model", model);
		brazoMorgana.Draw(staticShader);

		//Cabeza
		model = glm::translate(tmp, glm::vec3(0.0f, 0.35f, -0.05f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0));
		model = glm::scale(model, glm::vec3(0.3f));
		staticShader.setMat4("model", model);
		cabezaMorgana.Draw(staticShader);

		//Pierna Izq
		model = glm::translate(tmp, glm::vec3(0.15f, -0.35f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f));
		staticShader.setMat4("model", model);
		piernaMorgana.Draw(staticShader);

		//Pierna Der
		model = glm::translate(tmp, glm::vec3(-0.15f, -0.35f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f));
		staticShader.setMat4("model", model);
		piernaMorgana.Draw(staticShader);

		//Patas corriendo
		model = glm::translate(tmp, glm::vec3(0.0f, -0.35f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f));
		staticShader.setMat4("model", model);
		patasCorriendoMorgana.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Joker
		// -------------------------------------------------------------------------------------------------------------------------
		//Torso
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(15.0f, 0.0f, -29.8f));
		tmp = model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.5f));
		staticShader.setMat4("model", model);
		torsoJoker.Draw(staticShader);

		//Brazo derecho
		model = glm::translate(tmp, glm::vec3(-0.51f, 0.5f, 0.0f));
		//model = glm::translate(model, glm::vec3(0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		staticShader.setMat4("model", model);
		brazoJoker.Draw(staticShader);

		//Brazo izquierdo
		model = glm::translate(tmp, glm::vec3(0.51f, 0.5f, 0.0f));
		//model = glm::translate(model, glm::vec3(0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		staticShader.setMat4("model", model);
		brazoJoker.Draw(staticShader);

		//Cabeza
		model = glm::translate(tmp, glm::vec3(0.0f, 0.70f, 0.0f));
		model = glm::rotate(model, glm::radians(giroCabezaJoker_y), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0));
		model = glm::scale(model, glm::vec3(0.5f));
		staticShader.setMat4("model", model);
		cabezaJoker.Draw(staticShader);

		//Pierna Der
		model = glm::translate(tmp, glm::vec3(-0.28f, -0.69f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		staticShader.setMat4("model", model);
		piernaJoker.Draw(staticShader);

		//Pierna Izq
		model = glm::translate(tmp, glm::vec3(0.28f, -0.69f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		staticShader.setMat4("model", model);
		piernaJoker.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Akechi
		// -------------------------------------------------------------------------------------------------------------------------

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(20.0f, 0.0f, -30.0f));
		tmp = model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.5f));
		staticShader.setMat4("model", model);
		torsoAkechi.Draw(staticShader);

		//Brazo derecho
		model = glm::translate(tmp, glm::vec3(-0.51f, 0.5f, 0.0f));
		//model = glm::translate(model, glm::vec3(0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(giroBrazoAkechi_x), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		staticShader.setMat4("model", model);
		brazoAkechi.Draw(staticShader);

		//Brazo izquierdo
		model = glm::translate(tmp, glm::vec3(0.51f, 0.5f, 0.0f));
		//model = glm::translate(model, glm::vec3(0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-giroBrazoAkechi_x), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		staticShader.setMat4("model", model);
		brazoAkechi.Draw(staticShader);

		//Cabeza
		model = glm::translate(tmp, glm::vec3(0.0f, 0.71f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0));
		model = glm::scale(model, glm::vec3(0.5f));
		staticShader.setMat4("model", model);
		cabezaAkechi.Draw(staticShader);

		//Pierna Der
		model = glm::translate(tmp, glm::vec3(-0.28f, -0.69f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		staticShader.setMat4("model", model);
		piernaAkechi.Draw(staticShader);

		//Pierna Izq
		model = glm::translate(tmp, glm::vec3(0.28f, -0.69f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		staticShader.setMat4("model", model);
		piernaAkechi.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Ann
		// -------------------------------------------------------------------------------------------------------------------------

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(15.0f, 0.0f, -29.0f));
		tmp = model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.5f));
		staticShader.setMat4("model", model);
		torsoAnn.Draw(staticShader);

		//Brazo derecho
		model = glm::translate(tmp, glm::vec3(-0.51f, 0.5f, 0.0f));
		//model = glm::translate(model, glm::vec3(0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(giroBrazoDerechoAnn_x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(giroBrazoDerechoAnn_y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		staticShader.setMat4("model", model);
		brazoAnn.Draw(staticShader);

		//Brazo izquierdo
		model = glm::translate(tmp, glm::vec3(0.51f, 0.5f, 0.0f));
		//model = glm::translate(model, glm::vec3(0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		staticShader.setMat4("model", model);
		brazoAnn.Draw(staticShader);

		//Cabeza
		model = glm::translate(tmp, glm::vec3(0.0f, 0.70f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0));
		model = glm::scale(model, glm::vec3(0.5f));
		staticShader.setMat4("model", model);
		cabezaAnn.Draw(staticShader);

		//Pierna Izq
		model = glm::translate(tmp, glm::vec3(0.27f, -0.71f, 0.02f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		staticShader.setMat4("model", model);
		pierna2Ann.Draw(staticShader);

		//Pierna Der
		model = glm::translate(tmp, glm::vec3(-0.27f, -0.71f, 0.07f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		staticShader.setMat4("model", model);
		piernaAnn.Draw(staticShader);

		//Globos de dialogo
		//Globo Akechi
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(20.0f, 3.0f + mov_globoY, -30.0f+mov_globoXZ));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(eglobo_Akechi));
		staticShader.setMat4("model", model);
		globo.Draw(staticShader);
		//Globo Joker
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(15.0+mov_globoXZ, 3.0+mov_globoY, -29.8f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(eglobo_Joker));
		staticShader.setMat4("model", model);
		globo.Draw(staticShader);
		//Globo Ann
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(15.0f+mov_globoXZ, 3.0f+mov_globoY, -29.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(eglobo_Ann));
		staticShader.setMat4("model", model);
		globo.Draw(staticShader);
		//Globo Morgana
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(13.0f, 2.0f + mov_globoY, -30.0f + mov_globoXZ));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(eglobo_Morgana));
		staticShader.setMat4("model", model);
		globo.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario
		// -------------------------------------------------------------------------------------------------------------------------

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------
		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		//std::cout <<"frame time = " << frameTime << " milli sec"<< std::endl;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	bg_music->drop(); //Borrar música de fondo

	skybox.Terminate();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	//To Configure Model
	//Para keyframes
	/*if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		giroCabezaJoker_y += 0.3;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		giroCabezaJoker_y -= 0.3;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		giroBrazoAkechi_x += 0.3;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		giroBrazoAkechi_x -= 0.3;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		giroBrazoDerechoAnn_y += 0.3;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		giroBrazoDerechoAnn_y -= 0.3;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		giroBrazoDerechoAnn_x += 0.3;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		giroBrazoDerechoAnn_x -= 0.3;*/
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		lightPosition.x++;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		lightPosition.x--;

	//Animación 0: Luz del zol
	if (key == GLFW_KEY_0 && action == GLFW_PRESS) {
		animacion_sol ^= true;
	}

	//Animacion 1: Manecillas del reloj
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		animacion_reloj ^= true;
		giroHoras = 0;
		giroMins = 0;
	}

	//Animación 2: Movimiento del tren
	if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		animacion_tren ^= true;
		estadoCabina = 0;
		estadoVagon = 0;
		orientaCabina = 0.0f;
		movCabina_x = 51.0f;
		movCabina_z = -90.0f;
		orientaVagon = 0.0f;
		movVagon_x = 70.0f;
		movVagon_y = 0.2f;
		movVagon_z = -90.0f;
	}

	//Animación 3: Camión
	if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
		animacion_camion ^= true;
		giroLlanta = 0.0f;
		movCamion_x = 118.0f,
		movCamion_y = 0.0f,
		movCamion_z = 115.0f,
		orientaCamion = 180.0f;
		estado_camion = 1;
	}

	//Animación 4: Secuestro de Futaba
	if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
		animacion_ovni ^= true;	
	}
	//Uso una tecla diferente para reiniciarlo
	if (key == GLFW_KEY_O && action == GLFW_PRESS) {
			animacion_ovni = false;
			orientaOvni = 0.0f;
			movOvni_x = 10.0f;
			movOvni_y = 30.0f; 
			movOvni_z = 60.0f;
			estado_Ovni = 0;
			escalaFutaba1 = 0.0f;
			escalaFutaba2 = 0.35f;
			movFutaba_y = -0.1;
			contOvni = 0;
	}

	//To play KeyFrame animation 
	if (key == GLFW_KEY_5 && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			std::cout << "Play animation" << std::endl;
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
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

	if (key == GLFW_KEY_6 && action == GLFW_PRESS) {
		animacion_globos ^= true;
	}
	//Uso una tecla diferente para reiniciarlo
	if (key == GLFW_KEY_P && action == GLFW_PRESS) {
		animacion_globos= false;
		eglobo_Joker = 0.0f;
		eglobo_Ann = 0.0f;
		eglobo_Akechi = 0.0f;
		eglobo_Morgana = 0.0f;
		estado_globos = 0;
		mov_globoY = 0.0f;
		mov_globoXZ = 0.0f;
		
	}

	//To Save a KeyFrame
	/*if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
	}*/
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}