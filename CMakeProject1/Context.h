#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stack>
#include <vector>
#include <functional>
#include "utils.h"
#include "keyboard.h"
#include "properties.h"
#include "resource.h"
#include "ImGui/imgui.h"
#include "ImGui/backends/imgui_impl_glfw.h"
#include "ImGui/backends/imgui_impl_opengl3.h"
#include "modelGen/Torus.h"
#include "modelGen/Sphere.h"
#include "modelGen/Leaf.h"



#define numVAOs 1
#define numVBOs 40

class Transform {
	public:
		Transform(
			std::function<glm::vec3(float)>positionFunc,
			std::function<float(float)>scaleFunc,
			std::function<float(float)>rotationFunc,
			const glm::vec3& rotationAxis
		) : 
		positionFunc(std::move(positionFunc)),
		scaleFunc(std::move(scaleFunc)),
		rotationFunc(std::move(rotationFunc)),
		rotationAxis(rotationAxis)  {}
		
		//function overloading!!
		Transform(
			const glm::vec3& position,
			float scale,
			float rotation,
			const glm::vec3& rotationAxis
		) :
		positionFunc([position](float) { return position; }),
		scaleFunc([scale](float) { return scale; }),
		rotationFunc([rotation](float) { return rotation; }),
		rotationAxis(rotationAxis) {}

		glm::mat4 getMatrix(float t) const {
			glm::mat4 mat(1.0f);
			mat = glm::translate(mat, positionFunc(t));
			mat = glm::rotate(mat, rotationFunc(t), rotationAxis);
			mat = glm::scale(mat, glm::vec3(scaleFunc(t)));
			return mat;
		}

	private:
	 	glm::vec3 rotationAxis;
		std::function<float(float)>scaleFunc;
		std::function<glm::vec3(float)> positionFunc;
		std::function<float(float)>rotationFunc;
};


enum objects {
    TORUS,
    SPHERE,
    ROOM,
    PLANE,
	CUBE
};

// tinyobj::attrib_t attrib;

class Context {
	
	public:
		Context(GLFWwindow* _window);
		void run();

	private:
		void init();
		void display(GLFWwindow* window, KeyboardHandler& keyboardHandler);
		void setupVertices();
		void installLights(glm::mat4 vMatrix);
		void drawObject(std::stack<glm::mat4>& mvStack, Transform* transform, int id, int numVertices, int texId);
		void drawObjectsInstanced(int id, int texId, int numVertices);
		void drawObjectShadow(std::stack<glm::mat4>& mvStack, Transform* transform, int id, int numVertices);
		void drawObjectsInstancedShadow(int id, int numVertices);
		void drawCubeMap();
		void createTransformations();
		void passOne(void);
		void passTwo(void);
		void updateShaderStorageBuffer();

		GLFWwindow* _window;
		GLuint renderingProgram1, renderingProgram2, renderingProgramCube, computeShaderProgram;
		GLuint vao[numVAOs];
		GLuint vbo[numVBOs];

		GLuint mvLocCube, projLocCube;
		GLuint mvLoc, projLoc, vLoc, nLoc, tfLoc, sLoc1, sLoc2;
		GLuint globalAmbLoc, ambLoc, diffLoc, specLoc, posLoc;
		GLuint mAmbLoc, mDiffLoc, mSpecLoc, mShiLoc;
		GLuint isInstancedLoc, startTimeLoc;

		GLuint startTimeLocC, tfLocC, switchVelocityLocC;

		std::vector<GLuint> textures;
		GLuint cubeMapTexture;

		KeyboardHandler keyboardhandler;
		glm::mat4 mMat, vMat, mvMat, invTrMat;
		glm::mat4 lightVmatrix, shadowMVP1, shadowMVP2;

		float lightPos[3];

		Transform* t1;//, t2, t3;
		Transform* t2;
		Transform* t3;
		Transform* t4;
		Transform* t5;
		Transform* t6;
		Transform* tLight;
		Transform* surfaceTransform1;
		Transform* surfaceTransform2;

		std::vector<std::function<glm::vec3(float)>> translationFunctions1;
		std::vector<Transform> transformations1;

		std::vector<std::function<glm::vec3(float)>> translationFunctions2;
		std::vector<Transform> transformations2;

		std::vector<glm::vec3> perInstanceData;
};



void window_reshape_callback(GLFWwindow* window, int newWidth, int newHeight);
void updateFrameTime(float current);
// void setupShadowBuffers();



