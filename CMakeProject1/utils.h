#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>

void printShaderLog(GLuint shader);
void printProgramLog(int prog);
bool checkOpenGLError();

std::string readShaderSource(const char *filePath);
GLuint createShaderProgram(const char *vp, const char *fp);

GLuint createShaderProgramC(const char* cp);
GLuint prepareShader(int shaderTYPE, const char *shaderPath);
int finalizeShaderProgram(GLuint sprogram);

