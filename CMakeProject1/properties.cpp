#include "properties.h"

float globalAmbient[4] = { 0.7f, 0.7f, 0.7f, 1.0f };
float lightAmbient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
float lightDiffuse[4] = { .5f, .5f, .5f, 1.0f }; 
float lightSpecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

//GOLD material - ambient, diffuse, specular, and shininess
float* goldAmbient()  { static float a[4] = { 0.2473f, 0.1995f, 0.0745f, 1 }; return (float*)a; }
float* goldDiffuse()  { static float a[4] = { 0.7516, 0.6065f, 0.2265f, 1 }; return (float*)a; }
float* goldSpecular() { static float a[4] = { 6283, 0.5558, 0.3661f, 1 }; return (float*)a; }
float goldShininess() { return 51.2f; }

//SILVER material - ambient, diffuse, specular, and shininess
float* silverAmbient()  { static float a[4] = { 0.1923f, 0.1923f, 0.1923f, 1 }; return (float*)a; }
float* silverDiffuse()  { static float a[4] = { 0.5075f, 0.5075f, 0.5075f, 1 }; return (float*)a; }
float* silverSpecular() { static float a[4] = { 0.5083f, 0.5083f, 0.5083f, 1 }; return (float*)a; }
float silverShininess() { return 51.2f; }

//BRONZE material - ambient, diffuse, specular and shininess
float* bronzeAmbient()  { static float a[4] = { 0.2125f, 0.1275f, 0.0540f, 1 }; return (float*)a; }
float* bronzeDiffuse()  { static float a[4] = { 0.7140f, 0.4284f, 0.1814f, 1 }; return (float*)a; }
float* bronzeSpecular() { static float a[4] = { 0.3935f, 0.2719f, 0.1667f, 1 }; return (float*)a; }
float bronzeShininess() { return 25.6f; }

float x = 1.0f, y = 9.0f, d = .1f;
float distance = 2.0f;
float rSpeed = 1.0f;
float direction = 1.0f;
float cameraSpeed = 0.2f;


float cameraX = -2.0f;
float cameraY =  1.2f;
float cameraZ =  8.0f;

float PI = 3.14159265359f;
float PIhalf = PI / 2.0f;

glm::vec3 currentLightPos(-0.1f, 25.0f, 25.0f);
glm::vec3 origin(0.0f, 0.0f, 0.0f);
glm::vec3 up(0.0f, 1.0f, 0.0f);
glm::vec3 torusLoc(.9f, 0.0f, 0.0f);
glm::vec3 cubeLoc(0.0f, -60.0f, 0.0f);
glm::vec3 cameraLoc(0.0f, 0.2f, 6.0f);

glm::vec3 rotationX(1.0f, 0.0f, 0.0f);
glm::vec3 rotationY(0.0f, 1.0f, 0.0f);
glm::vec3 rotationZ(0.0f, 0.0f, 1.0f);


bool spacePressedLastFrame = false;
bool lookAtCenter = false;
bool spacePressedNow = false;
bool kPressed = false;

float scaleFactorCubes = .4f;

float cubeVertexPositions[108] =
	{	-1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f, -1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
		1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
		1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f
};



float cubeTextureCoord[72] =
	{	1.00f, 0.6666666f, 1.00f, 0.3333333f, 0.75f, 0.3333333f,	// back face lower right
		0.75f, 0.3333333f, 0.75f, 0.6666666f, 1.00f, 0.6666666f,	// back face upper left
		0.75f, 0.3333333f, 0.50f, 0.3333333f, 0.75f, 0.6666666f,	// right face lower right
		0.50f, 0.3333333f, 0.50f, 0.6666666f, 0.75f, 0.6666666f,	// right face upper left
		0.50f, 0.3333333f, 0.25f, 0.3333333f, 0.50f, 0.6666666f,	// front face lower right
		0.25f, 0.3333333f, 0.25f, 0.6666666f, 0.50f, 0.6666666f,	// front face upper left
		0.25f, 0.3333333f, 0.00f, 0.3333333f, 0.25f, 0.6666666f,	// left face lower right
		0.00f, 0.3333333f, 0.00f, 0.6666666f, 0.25f, 0.6666666f,	// left face upper left
		0.25f, 0.3333333f, 0.50f, 0.3333333f, 0.50f, 0.0000000f,	// bottom face upper right
		0.50f, 0.0000000f, 0.25f, 0.0000000f, 0.25f, 0.3333333f,	// bottom face lower left
		0.25f, 1.0000000f, 0.50f, 1.0000000f, 0.50f, 0.6666666f,	// top face upper right
		0.50f, 0.6666666f, 0.25f, 0.6666666f, 0.25f, 1.0000000f		// top face lower left
};

    
float earthX1(float t) { return cos(t); }
float earthY1(float t) { return 0; }
float earthZ1(float t) { return sin(t); }

float moonX1(float t) { return cos(t); }
float moonY1(float t) { return 0; }
float moonZ1(float t) { return sin(t); }


glm::vec3 tFunc1(float t) { return glm::vec3(earthX1(t), earthY1(t), earthZ1(t));  };

glm::vec3 tFuncFloor(float t) {return glm::vec3(0.0f, -62.0f, 0.0f);};

glm::mat4 b(
	0.5f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.5f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.5f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f
);


float earthX(float t) { return cos(-t *.5) * 20.0f; }
float earthY(float t) { return 0; }
float earthZ(float t) { return sin(-t *.5 ) * 20.0f; }

float moonX(float t) { return cos(t *2.0f) * 10.0f; }
float moonY(float t) { return 0; }
float moonZ(float t) { return sin(t *2.0f) * 10.0f; }

