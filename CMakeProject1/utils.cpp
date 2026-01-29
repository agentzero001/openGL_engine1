#include "utils.h"

using namespace std;


void printShaderLog(GLuint shader) {
    int len = 0;
    int chWrittn = 0;
    char *log;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    if (len > 0) {
        log = (char *)malloc(len);
        glGetShaderInfoLog(shader, len, &chWrittn, log);
        cout << "Shader Info Log:" << log << endl;
        free(log);
    }
}


void printProgramLog(int prog) {
    int len = 0;
    int chWrittn = 0;
    char *log;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
    if (len > 0) {
        log = (char *)malloc(len);
        glGetProgramInfoLog(prog, len, &chWrittn, log);
        cout << "Program Info Log:" << log << endl;
        free(log);
    }
}



bool checkOpenGLError() {
    bool foundError = false;
    int glErr = glGetError();
    while (glErr != GL_NO_ERROR) {
        cout << "glError:" << glErr << endl;
        foundError = true;
        glErr = glGetError();
    }
    return foundError;
}

string readShaderSource(const char *filePath) {
    string content;
    ifstream fileStream(filePath, ios::in);
    string line = "";
    while (!fileStream.eof()) {
        getline(fileStream, line);
        content.append(line + "\n");
    }
    fileStream.close();
    return content;
}


GLuint createShaderProgram(const char *vp, const char *fp) {
    GLint vertCompiled;
    GLint fragCompiled;
    GLint linked; 

    //create empty shaders
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

    string vertShaderStr = readShaderSource(vp);
    string fragShaderStr = readShaderSource(fp);

    const char *vertShaderSrc = vertShaderStr.c_str();
    const char *fragShaderSrc = fragShaderStr.c_str();

    // //loads the GLSL code from the strings into the empty shader objects
    glShaderSource(vShader, 1, &vertShaderSrc, NULL);
    glShaderSource(fShader, 1, &fragShaderSrc, NULL);


    glCompileShader(vShader);
    checkOpenGLError();
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
    if (vertCompiled != 1) {
        cout << "vertex compilation failed" << endl;
        printShaderLog(vShader);
    }

    glCompileShader(fShader);
    checkOpenGLError();
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
    if (fragCompiled != 1) {
        cout << "fragment compilation failed" << endl;
        printShaderLog(fShader);
    }
    
    GLuint vfProgram = glCreateProgram();
    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);
    glLinkProgram(vfProgram);
    glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
    if (linked != 1) {
        cout << "linking failed" << endl;
        printProgramLog(vfProgram);
    }

    return vfProgram;
}



GLuint createShaderProgramC(const char* cp) {
    GLuint cShader = prepareShader(GL_COMPUTE_SHADER, cp);
    GLuint cProgram = glCreateProgram();
    glAttachShader(cProgram, cShader);
    finalizeShaderProgram(cProgram);
    return cProgram;
}

GLuint prepareShader(int shaderTYPE, const char *shaderPath) {
    GLint shaderCompiled;
    string shaderStr = readShaderSource(shaderPath);
    const char *shaderSrc = shaderStr.c_str();
    GLuint shaderRef = glCreateShader(shaderTYPE);
    glShaderSource(shaderRef, 1, &shaderSrc, NULL);
    glCompileShader(shaderRef);
    checkOpenGLError();
    glGetShaderiv(shaderRef, GL_COMPILE_STATUS, &shaderCompiled);
    if (shaderCompiled != 1)
    {
        if (shaderTYPE == 35633) cout << "Vertex ";
        if (shaderTYPE == 36488) cout << "Tess Control ";
        if (shaderTYPE == 36487) cout << "Tess Eval ";
        if (shaderTYPE == 36313) cout << "Geometry ";
        if (shaderTYPE == 35632) cout << "Fragment ";
        cout << "shader compilation error." << endl;
        printShaderLog(shaderRef);
    }
    return shaderRef;
}


int finalizeShaderProgram(GLuint sprogram) {
	GLint linked;
	glLinkProgram(sprogram);
	checkOpenGLError();
	glGetProgramiv(sprogram, GL_LINK_STATUS, &linked);
	if (linked != 1) {
		cout << "linking failed" << endl;
		printProgramLog(sprogram);
	}
	return sprogram;
}

bool toggleKey(int key, KeyboardHandler& keyboardHandler) {
    static std::unordered_map<int, bool> lastPressed;
    static std::unordered_map<int, bool> toggledState;

    bool pressedNow = keyboardHandler.isKeyPressed(key);

    if (pressedNow && !lastPressed[key]) {
        toggledState[key] = !toggledState[key];
    }

    lastPressed[key] = pressedNow;
    return toggledState[key];
}