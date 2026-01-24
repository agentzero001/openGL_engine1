#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>

void printShaderLog(GLuint shader);
void printProgramLog(int prog);
bool CheckOpenGLError();
std::string readShaderSource(const char *filePath);
GLuint createShaderProgram(const char *vp, const char *fp);
void loadModel();
