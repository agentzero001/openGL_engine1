#pragma once
#include <vector>
#include <stack>
#include <glm/matrix.hpp>
	
float* goldAmbient();
float* goldDiffuse();
float* goldSpecular();
float  goldShininess();

float* silverAmbient();
float* silverDiffuse();
float* silverSpecular();
float  silverShininess();

float* bronzeAmbient();
float* bronzeDiffuse();
float* bronzeSpecular();
float  bronzeShininess();


extern float d, x, y, distance, rSpeed, direction, cameraSpeed;

extern int listLength;

extern float cameraX;
extern float cameraY; 
extern float cameraZ; 

extern float PI;
extern float PIhalf;

extern float globalAmbient[4];
extern float lightAmbient[4]; 
extern float lightDiffuse[4];
extern float lightSpecular[4];

extern glm::vec3 torusLoc;
extern glm::vec3 cubeLoc;
extern glm::vec3 cameraLoc;

extern glm::vec3 currentLightPos;
extern glm::vec3 origin;
extern glm::vec3 up;

extern glm::vec3 rotationX;
extern glm::vec3 rotationY;
extern glm::vec3 rotationZ;

extern bool spacePressedLastFrame;
extern bool lookAtCenter;
extern bool spacePressedNow;
extern bool kPressed;

extern float scaleFactorCubes;

extern float cubeVertexPositions[108];

extern float cubeTextureCoord[72];

extern float earthX1(float t);
extern float earthY1(float t);
extern float earthZ1(float t);

extern float moonX1(float t);
extern float moonY1(float t);
extern float moonZ1(float t);

extern glm::vec3 tFunc1(float t);

extern glm::vec3 tFuncFloor(float t);

extern glm::mat4 b;

extern float earthX(float t);
extern float earthY(float t);
extern float earthZ(float t);

extern float moonX(float t);
extern float moonY(float t);
extern float moonZ(float t);

