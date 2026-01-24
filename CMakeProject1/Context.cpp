#include "Context.h"

std::stack<glm::mat4> mvStack;
std::vector<float> nums1;
std::vector<float> nums2;

glm::vec3 lightPosV;

float aspect;

glm::mat4 pMat, lightPmatrix;

int listLength;

float* matAmb = bronzeAmbient();
float* matDif = bronzeDiffuse();
float* matSpe = bronzeSpecular();
float matShi = bronzeShininess();

Torus torus(1.2f, 0.5f, 96);
Sphere sphere(12);

std::vector<float> cubeValues;
std::vector<float> cubeNvalues;
std::vector<float> cubeTvalues;
std::vector<int> cubeInd;
std::vector<Vertex> cube_vertices;
std::vector<uint32_t> cube_indices;


std::vector<float> roomCubeValues;
std::vector<float> roomCubeNvalues;
std::vector<float> roomCubeTvalues;
std::vector<int> roomCubeInd;
std::vector<Vertex> room_vertices;
std::vector<uint32_t> room_indices;

std::vector<float> sphereValues;
std::vector<float> sphereNvalues;
std::vector<float> sphereTvalues;
std::vector<glm::vec3> sphereVert;
std::vector<glm::vec3> sphereNorm; 
std::vector<glm::vec2> sphereTex;
std::vector<int> sphereInd;

std::vector<float> torusValues;
std::vector<float> torusNvalues;
std::vector<float> torusTvalues;
std::vector<glm::vec3> torusVert;
std::vector<glm::vec3> torusNorm; 
std::vector<glm::vec2> torusTex;
std::vector<int> torusInd;

std::vector<float> surfaceValues;
std::vector<float> surfaceNvalues;
std::vector<float> surfaceTvalues;
std::vector<int> surfaceInd;





float dt;
float lastTime;
float startTime;
float elapsedTime;

float deltaTime = .0f;
float lastFrame = .0f;

float factor = 2.0f;
float units = 4.0f;

int shadowTexScale = 4;
int screenX, screenY;

float lightFOV = 1.0472f * 2.0;

GLuint shadowTex, shadowBuffer;


int numParticles = 20000;




auto f = [](float t) -> glm::vec3 { return glm::vec3(earthX(t), earthY(t), earthZ(t)); };
auto ft = [](float t) -> glm::vec3 { return glm::vec3(earthX(t- PI*2.0f), earthY(t), earthZ(t- PI*2.0f)); };

auto f2 = [](float t) -> glm::vec3 { return glm::vec3(20.0f* cos(cos(-t)), moonY(t), 20.0f * sin(sin(-t))); };
auto f2t = [](float t) -> glm::vec3 { return glm::vec3(20.0f* cos(cos(-t )- PI), moonY(t), 20.0f * sin(sin(-t )- PI)); };

auto fm = [](float t) -> glm::vec3 { return glm::vec3(moonX(t), moonY(t), moonZ(t)); };

auto sFunc1 = [] (float t) -> float {return (sin(t * 20.0f) * .5 ) + 1.0f; };
auto constantT = [] (float t) -> float {return t*4.0f; };
auto constantOne = [] (float t) -> float {return 1.0f; };
auto constantHalf = [] (float t) -> float {return 0.5f; };
auto constantZero = [] (float t) -> float {return 0.0f; };
auto constantTen = [] (float t) -> float {return 10.0f; };
auto constantX = [] (float t) -> float {return 5.0f; };

auto getLightPos = [] (float t) -> glm::vec3 { return glm::vec3(sin(t) * 50.0f, 40.0f, 0.0f); };

auto surfacePos = [] (float t) -> glm::vec3 { return glm::vec3(0.0f, 10.0f, 0.0f); };

auto constantPIhalfN = [] (float t) -> float {return -PIhalf; };




Context::Context(GLFWwindow* _window) : _window(_window), keyboardhandler(_window) {}

void Context::run() {

    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(_window);
	glfwPollEvents();

	init();

	while (!glfwWindowShouldClose(_window)) {

		display(_window, keyboardhandler);//, *keyboardhandler);
		glfwSwapBuffers(_window);
		glfwPollEvents();
	}

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

}

void Context::init() {

    glfwSetWindowSizeCallback(_window, window_reshape_callback);

	renderingProgram1 = createShaderProgram(
		"C:/Users/jensm/source/repos/CMakeProject1/res/shaders/lighting.vert",
		"C:/Users/jensm/source/repos/CMakeProject1/res/shaders/lighting.frag"
	);
    renderingProgram2 = createShaderProgram(
		"C:/Users/jensm/source/repos/CMakeProject1/res/shaders/shadow.vert",
		"C:/Users/jensm/source/repos/CMakeProject1/res/shaders/shadow.frag"
	);

    renderingProgramCube = createShaderProgram(
		"C:/Users/jensm/source/repos/CMakeProject1/res/shaders/cubeMapShader.vert",
		"C:/Users/jensm/source/repos/CMakeProject1/res/shaders/cubeMapShader.frag"
	);

    renderingProgramInstanced = createShaderProgram(
		"C:/Users/jensm/source/repos/CMakeProject1/res/shaders/instance.vert",
		"C:/Users/jensm/source/repos/CMakeProject1/res/shaders/instance.frag"
	);

    
    setupVertices();
    setupShadowBuffers(screenX, screenY, shadowBuffer, shadowTex, shadowTexScale);
    // setupShadowBuffers();

    mvLoc = glGetUniformLocation(renderingProgram1, "mv_matrix");
    vLoc =  glGetUniformLocation(renderingProgram1, "v_matrix");
    projLoc = glGetUniformLocation(renderingProgram1, "proj_matrix");
    nLoc = glGetUniformLocation(renderingProgram1, "norm_matrix");
    tfLoc = glGetUniformLocation(renderingProgram1, "dt");
    isInstancedLoc = glGetUniformLocation(renderingProgram1, "isInstanced");
    startTimeLoc = glGetUniformLocation(renderingProgram1, "startTime");
    
    //shadow mvp
    sLoc1 = glGetUniformLocation(renderingProgram1, "shadow_mvp");
    sLoc2 = glGetUniformLocation(renderingProgram2, "shadow_mvp");


    globalAmbLoc = glGetUniformLocation(renderingProgram1, "globalAmbient");
    ambLoc = glGetUniformLocation(renderingProgram1, "light.ambient");
    diffLoc = glGetUniformLocation(renderingProgram1, "light.diffuse");
    specLoc = glGetUniformLocation(renderingProgram1, "light.specular");
    posLoc = glGetUniformLocation(renderingProgram1, "light.position");
    mAmbLoc = glGetUniformLocation(renderingProgram1, "material.ambient");
    mDiffLoc = glGetUniformLocation(renderingProgram1, "material.diffuse");
    mSpecLoc = glGetUniformLocation(renderingProgram1, "material.specular");
    mShiLoc = glGetUniformLocation(renderingProgram1, "material.shininess");


    mvLocCube = glGetUniformLocation(renderingProgramCube, "mv_matrix");
	projLocCube = glGetUniformLocation(renderingProgramCube, "proj_matrix");

    vLocInstanced = glGetUniformLocation(renderingProgramInstanced, "v_matrix");
	projLocInstanced = glGetUniformLocation(renderingProgramInstanced, "proj_matrix");
    tfLocInstanced = glGetUniformLocation(renderingProgramInstanced, "tf");

    createTransformations();

}

void Context::display(GLFWwindow* window, KeyboardHandler& keyboardHandler) {
    x += d;
    y -= d;
    if ((x > 2.0) || (x < 0.0)) {   
        d = -d;
    }


    
    glUniform1i(startTimeLoc, GL_FALSE);
    kPressed = keyboardHandler.isKeyPressed(GLFW_KEY_K);
    if (kPressed) {
        elapsedTime = glfwGetTime() - startTime;
        glUniform1i(startTimeLoc, GL_TRUE);
        glUniform1f(tfLoc, elapsedTime);
    }
    else startTime = (float)glfwGetTime();

    spacePressedNow = keyboardHandler.isKeyPressed(GLFW_KEY_SPACE);
    if (spacePressedNow && !spacePressedLastFrame) {
        lookAtCenter = !lookAtCenter; 
    }
    spacePressedLastFrame = spacePressedNow;       



    
   



    glClear(GL_DEPTH_BUFFER_BIT);
    // glClearColor(0.1f, 0.1f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
    // glEnable(GL_MULTISAMPLE);

    
    
    if (!lookAtCenter) { vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));}
    // else               { vMat = glm::lookAt(glm::vec3(cameraX, cameraY, cameraZ), f(dt), up);}
     else               { vMat = glm::lookAt(glm::vec3(cameraX, cameraY, cameraZ), glm::vec3(0.0f, 0.0f, 0.0f), up);}
        // { vMat = glm::lookAt(glm::vec3(cameraX, cameraY, cameraZ), origin, up);}

    // drawObjectsInstanced(SPHERE, STATUE,  sphereInd.size());

    // drawCubeMap();

    
    glBindFramebuffer(GL_FRAMEBUFFER, shadowBuffer);
    glViewport(0, 0, screenX * shadowTexScale, screenY * shadowTexScale);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowTex, 0);
    glDrawBuffer(GL_NONE);
    glEnable(GL_POLYGON_OFFSET_FILL); //fighting shadow artifacts
	glPolygonOffset(factor, units);	  
    passOne();

    glDisable(GL_POLYGON_OFFSET_FILL);	
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, screenX, screenY);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, shadowTex);
    glDrawBuffer(GL_FRONT);
    passTwo();

    
    

 
    while (!mvStack.empty()) {
        mvStack.pop();
    };
    
    if (keyboardHandler.isKeyPressed(GLFW_KEY_S)) {
        cameraZ += cameraSpeed;
    }
    if (keyboardHandler.isKeyPressed(GLFW_KEY_W)) {
        cameraZ -= cameraSpeed;
    }
    if (keyboardHandler.isKeyPressed(GLFW_KEY_D)) {
        cameraX += cameraSpeed;
    }
    if (keyboardHandler.isKeyPressed(GLFW_KEY_A)) {
        cameraX -= cameraSpeed;
    }
    if (keyboardHandler.isKeyPressed(GLFW_KEY_Q)) {
        cameraY += cameraSpeed;
    }
    if (keyboardHandler.isKeyPressed(GLFW_KEY_E)) {
        cameraY -= cameraSpeed;
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Demo window");
    ImGui::SliderFloat("distance", &distance, 0, 6);
    ImGui::SliderFloat("cycle Speed", &rSpeed, 0, 6);
    ImGui::SliderFloat("lightPosX", &currentLightPos.x, -5, 80);
    ImGui::SliderFloat("lightPosY", &currentLightPos.y, -30, 80);
    ImGui::SliderFloat("lightPosZ", &currentLightPos.z, -30, 80);
    ImGui::SliderFloat("factor", &factor, 0, 20);
    ImGui::SliderFloat("units", &units, 0, 20);
    ImGui::Text("LightPosV.x: %2f", lightPosV.x);
    ImGui::Text("LightPosV.y: %2f", lightPosV.y);
    ImGui::Text("LightPosV.z: %2f", lightPosV.z);
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // updateFrameTime(dt);
}

void Context::drawObject(std::stack<glm::mat4>& mvStack, Transform* transform, int id, int numVertices, int texId) {

    mMat = transform->getMatrix(dt);
    mvStack.push(mvStack.top() * mMat);
    invTrMat = glm::transpose(glm::inverse(mvStack.top()));
    shadowMVP2 = b * lightPmatrix * lightVmatrix * mMat;

    glUniformMatrix4fv(sLoc1, 1, GL_FALSE, glm::value_ptr(shadowMVP2)); 
    glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat)); 
    glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(vMat)); 
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
    glUniform1i(isInstancedLoc, GL_FALSE);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo[id * 4]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[(id * 4) + 2]);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[(id * 4) + 3]);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);
    glBindTexture(GL_TEXTURE_2D, textures[texId]);
 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[(id * 4) + 1]);  
    
    glDisableVertexAttribArray(3);
    //to tell openGL which buffer contains the indices
    //openGL is able to recognize the presence of a GL_ELEMENT_ARRAY_BUFFER and utilize it to access the vertex attributes
    glDrawElements(GL_TRIANGLES, numVertices, GL_UNSIGNED_INT, 0); 

}



void Context::drawObjectsInstanced(int id, int texId, int numVertices) {

    glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(vMat));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
    glUniform1i(isInstancedLoc, GL_TRUE);

    //vertices
    glBindBuffer(GL_ARRAY_BUFFER, vbo[(id * 4)]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    //normals
    glBindBuffer(GL_ARRAY_BUFFER, vbo[(id * 4) + 2]);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, 0);
    glEnableVertexAttribArray(1);

    //texCoords
    glBindBuffer(GL_ARRAY_BUFFER, vbo[(id * 4) + 3]);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    glBindTexture(GL_TEXTURE_2D, textures[texId]);

    //indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[(id * 4) + 1]);   

    //perInstance data
    glBindBuffer(GL_ARRAY_BUFFER, vbo[numVBOs - 3]);
    glVertexAttribPointer(3, 3, GL_FLOAT,GL_FALSE, sizeof(glm::vec3), (void*)0);
    glVertexAttribDivisor(3, 1);
    glEnableVertexAttribArray(3);

    glDrawElementsInstanced(GL_TRIANGLES, numVertices, GL_UNSIGNED_INT, 0, numParticles);
    // glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 10);
}

void Context::drawObjectShadow(std::stack<glm::mat4>& mvStack, Transform* transform, int id, int numVertices) {   
    mMat = transform->getMatrix(dt);
    mvStack.push(mvStack.top() * mMat);
    shadowMVP1 = lightPmatrix * lightVmatrix * mvStack.top();
    glUniformMatrix4fv(sLoc2, 1, GL_FALSE, glm::value_ptr(shadowMVP1)); 
    glBindBuffer(GL_ARRAY_BUFFER, vbo[id * 4]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[(id * 4) + 1]);
    glDrawElements(GL_TRIANGLES, numVertices, GL_UNSIGNED_INT, 0);    
}

void Context::drawObjectsInstancedShadow(int id, int numVertices) {
    glUniformMatrix4fv(sLoc2, 1, GL_FALSE, glm::value_ptr(shadowMVP1)); 
    glBindBuffer(GL_ARRAY_BUFFER, vbo[id * 4]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[(id * 4) + 1]);
    glDrawElementsInstanced(GL_TRIANGLES, numVertices, GL_UNSIGNED_INT, 0, 1000);
}

void Context::drawCubeMap() {
    glUseProgram(renderingProgramCube);
    mMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    mvMat = vMat * mMat;
    glUniformMatrix4fv(mvLocCube, 1, GL_FALSE, glm::value_ptr(mvMat));
    glUniformMatrix4fv(projLocCube, 1, GL_FALSE, glm::value_ptr(pMat));

    glBindBuffer(GL_ARRAY_BUFFER, vbo[numVBOs - 1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[numVBOs - 2]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);	
	glDisable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glEnable(GL_DEPTH_TEST);
}


void Context::setupVertices() {
    loadModel(room_vertices, room_indices, ROOM_CUBE_PATH);
  
    for (auto& vertex : room_vertices) {
        roomCubeValues.push_back(vertex.pos.x);
        roomCubeValues.push_back(vertex.pos.y);
        roomCubeValues.push_back(vertex.pos.z);
        roomCubeTvalues.push_back(vertex.texCoord.s * 4.0f);
        roomCubeTvalues.push_back(vertex.texCoord.t * 4.0f);
        roomCubeNvalues.push_back(vertex.norm.x);
        roomCubeNvalues.push_back(vertex.norm.y);
        roomCubeNvalues.push_back(vertex.norm.z);
    }


    loadModel(cube_vertices, cube_indices, CUBE_PATH);
  
    for (auto& vertex : cube_vertices) {
        cubeValues.push_back(vertex.pos.x);
        cubeValues.push_back(vertex.pos.y);
        cubeValues.push_back(vertex.pos.z);
        cubeTvalues.push_back(vertex.texCoord.s);
        cubeTvalues.push_back(vertex.texCoord.t);
        cubeNvalues.push_back(vertex.norm.x);
        cubeNvalues.push_back(vertex.norm.y);
        cubeNvalues.push_back(vertex.norm.z);
    }

    for (auto& vertex : surfaceVertices) {
        surfaceValues.push_back(vertex.pos.x);
        surfaceValues.push_back(vertex.pos.y);
        surfaceValues.push_back(vertex.pos.z);
        surfaceTvalues.push_back(vertex.texCoord.s);
        surfaceTvalues.push_back(vertex.texCoord.t);
        surfaceNvalues.push_back(vertex.norm.x);
        surfaceNvalues.push_back(vertex.norm.y);
        surfaceNvalues.push_back(vertex.norm.z);
    }
    torusInd = torus.getIndices();
    torusVert = torus.getVertices();
    torusNorm = torus.getNormals();
    torusTex = torus.getTexCoords();

    sphereInd = sphere.getIndices();
    sphereVert = sphere.getVertices();
    sphereNorm = sphere.getNormals();
    sphereTex = sphere.getTexCoords();

    for (uint32_t i = 0; i < room_indices.size(); i++) {
        int value = room_indices[i];
        roomCubeInd.push_back(value);
    }

    for (uint32_t i = 0; i < cube_indices.size(); i++) {
        int value = cube_indices[i];
        cubeInd.push_back(value);
    }

    for (uint32_t i = 0; i < surfaceIndices.size(); i++) {
        int value = surfaceIndices[i];
        surfaceInd.push_back(value);
    }

    for (int i = 0; i < torus.getNumVertices(); i++) {
        torusValues.push_back(torusVert[i].x);
        torusValues.push_back(torusVert[i].y);
        torusValues.push_back(torusVert[i].z);
        torusTvalues.push_back(torusTex[i].s);
        torusTvalues.push_back(torusTex[i].t);
        torusNvalues.push_back(torusNorm[i].x);
        torusNvalues.push_back(torusNorm[i].y);
        torusNvalues.push_back(torusNorm[i].z);
    }

    for (int i = 0; i < sphere.getNumVertices(); i++) {
        sphereValues.push_back(sphereVert[i].x);
        sphereValues.push_back(sphereVert[i].y);
        sphereValues.push_back(sphereVert[i].z);
        sphereTvalues.push_back(sphereTex[i].s);
        sphereTvalues.push_back(sphereTex[i].t);
        sphereNvalues.push_back(sphereNorm[i].x);
        sphereNvalues.push_back(sphereNorm[i].y);
        sphereNvalues.push_back(sphereNorm[i].z);
    }

    // cubeMapTexture = loadCubeMap(CUBE_MAP_DIR);
    // glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    textures.reserve(texturePaths.size());
    for (const auto& path : texturePaths) {
        textures.push_back(loadTexture(path.string().c_str()));
    }

    perInstanceData = createPerInstanceData(numParticles);
    for (const glm::vec3& value : perInstanceData) {
        // std::cout << value.x << std::endl;
    }


    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);
    glGenBuffers(numVBOs, vbo);

    bindBuffers(CUBE, vbo, cubeValues, cubeTvalues, cubeNvalues, cubeInd);
    bindBuffers(TORUS,  vbo, torusValues, torusTvalues, torusNvalues, torusInd);
    bindBuffers(SPHERE, vbo, sphereValues, sphereTvalues, sphereNvalues, sphereInd);
    bindBuffers(ROOM, vbo, roomCubeValues, roomCubeTvalues, roomCubeNvalues, roomCubeInd);
    bindBuffers(PLANE, vbo, surfaceValues, surfaceTvalues, surfaceNvalues, surfaceInd);
    
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo[numVBOs - 1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexPositions) * 4, cubeVertexPositions, GL_STATIC_DRAW);


    glBindBuffer(GL_ARRAY_BUFFER, vbo[numVBOs - 2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeTextureCoord) * 4, cubeTextureCoord, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[numVBOs - 3]);

    glBufferData(
        GL_ARRAY_BUFFER,
        perInstanceData.size() * sizeof(glm::vec3),
        perInstanceData.data(),
        GL_STATIC_DRAW
    );


    glfwGetFramebufferSize(_window, &screenX, &screenY);
    aspect = (float)screenX / (float)screenY;
    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f); //1.0472 rad = 60 deg
    lightPmatrix = glm::perspective(lightFOV, aspect, 0.1f, 1000.0f); //1.0472 rad = 60 deg



}


void window_reshape_callback(GLFWwindow* window, int newWidth, int newHeight) {
	aspect = (float)newWidth / (float)newHeight;
    // glViewport(0, 0, newWidth, newHeight);
    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
    lightPmatrix = glm::perspective(lightFOV, aspect, 0.1f, 1000.0f);
    screenX = newWidth;
    screenY = newHeight;
    setupShadowBuffers(screenX, screenY, shadowBuffer, shadowTex, shadowTexScale);   
    
}

void Context::installLights( glm::mat4 vMatrix) {

    lightPosV = glm::vec3(vMatrix * glm::vec4(currentLightPos, 1.0));
    lightPos[0] = lightPosV.x;
    lightPos[1] = lightPosV.y;
    lightPos[2] = lightPosV.z;

    //set the uniform light and material values in the shader
    glProgramUniform4fv(renderingProgram1, globalAmbLoc, 1, globalAmbient);
    glProgramUniform4fv(renderingProgram1, ambLoc, 1, lightAmbient);
    glProgramUniform4fv(renderingProgram1, diffLoc, 1, lightDiffuse);
    glProgramUniform4fv(renderingProgram1, specLoc, 1, lightSpecular);
    glProgramUniform3fv(renderingProgram1, posLoc, 1, lightPos);

    glProgramUniform4fv(renderingProgram1, mAmbLoc, 1, matAmb);
    glProgramUniform4fv(renderingProgram1, mDiffLoc, 1, matDif);
    glProgramUniform4fv(renderingProgram1, mSpecLoc, 1, matSpe);
    glProgramUniform1f(renderingProgram1, mShiLoc, matShi);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
}


void Context::passOne() {
    lightVmatrix = glm::lookAt(currentLightPos, origin, up);

    glUseProgram(renderingProgram2);

   	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
   
    mvStack.push(glm::mat4(1.0f));


 

    // drawObjectShadow(mvStack, t1, 0, torus.getNumIndices());
    // drawObjectShadow(mvStack, t3, 0, torus.getNumIndices());
    // for (Transform& transformation : transformations1) {
    //     Transform* t_ptr = &transformation;
    //     drawObjectShadow(mvStack, t_ptr, 1, sphere.getNumIndices());
    //     mvStack.pop();
    // }
    // for (Transform& transformation : transformations2) {
    //     Transform* t_ptr = &transformation;
    //     drawObjectShadow(mvStack, t_ptr, 1, sphere.getNumIndices());
    //     mvStack.pop();
    // }
    // mvStack.pop();
    // drawObjectShadow(mvStack, t5, 0, torus.getNumIndices());
    // for (Transform& transformation : transformations1) {
    //     Transform* t_ptr = &transformation;
    //     drawObjectShadow(mvStack, t_ptr, 1, sphere.getNumIndices());
    //     mvStack.pop();
    // }
    // for (Transform& transformation : transformations2) {
    //     Transform* t_ptr = &transformation;
    //     drawObjectShadow(mvStack, t_ptr, 1, sphere.getNumIndices());
    //     mvStack.pop();
    // }

  
    // drawObjectShadow(mvStack, t4, 0, torus.getNumIndices());
    // drawObjectShadow(mvStack, t3, 0, torus.getNumIndices());
    // for (Transform& transformation : transformations1) {
    //     Transform* t_ptr = &transformation;
    //     drawObjectShadow(mvStack, t_ptr, 1, sphere.getNumIndices());
    //     mvStack.pop();
    // }
    // for (Transform& transformation : transformations2) {
    //     Transform* t_ptr = &transformation;
    //     drawObjectShadow(mvStack, t_ptr, 1, sphere.getNumIndices());
    //     mvStack.pop();
    // }
    // mvStack.pop();
    // drawObjectShadow(mvStack, t5, 0, torus.getNumIndices());
    // for (Transform& transformation : transformations1) {
    //     Transform* t_ptr = &transformation;
    //     drawObjectShadow(mvStack, t_ptr, 1, sphere.getNumIndices());
    //     mvStack.pop();
    // }
    // for (Transform& transformation : transformations2) {
    //     Transform* t_ptr = &transformation;
    //     drawObjectShadow(mvStack, t_ptr, 1, sphere.getNumIndices());
    //     mvStack.pop();
    // }

    
    // drawObjectShadow(mvStack, surfaceTransform2, PLANE, surfaceInd.size());
    // mvStack.pop();
    

    while (!mvStack.empty()) {
        mvStack.pop();
    };

}

void Context::passTwo() {

    glClear(GL_DEPTH_BUFFER_BIT);

    glUseProgram(renderingProgram1);

    glActiveTexture(GL_TEXTURE0);

    installLights(vMat);
    mvStack.push(vMat);  

 
    
    // drawObject(mvStack, t1, 0, torus.getNumIndices(), 0);
    // drawObject(mvStack, t3, 0, torus.getNumIndices(), 0);
    // for (Transform& transformation : transformations1) {
    //     Transform* t_ptr = &transformation;
    //     drawObject(mvStack, t_ptr, 1, sphere.getNumIndices(), 3);
    //     mvStack.pop();
    // }
    // for (Transform& transformation : transformations2) {
    //     Transform* t_ptr = &transformation;
    //     drawObject(mvStack, t_ptr, 1, sphere.getNumIndices(), 3);
    //     mvStack.pop();
    // }
    // mvStack.pop();
    // drawObject(mvStack, t5, 0, torus.getNumIndices(), 0);
    // for (Transform& transformation : transformations1) {
    //     Transform* t_ptr = &transformation;
    //     drawObject(mvStack, t_ptr, 1, sphere.getNumIndices(), 3);
    //     mvStack.pop();
    // }
    // for (Transform& transformation : transformations2) {
    //     Transform* t_ptr = &transformation;
    //     drawObject(mvStack, t_ptr, 1, sphere.getNumIndices(), 3);
    //     mvStack.pop();
    // }
    // mvStack.pop();
    // mvStack.pop();


    // drawObject(mvStack, t4, 0, torus.getNumIndices(), 0);
    // drawObject(mvStack, t3, 0, torus.getNumIndices(), 0);
    // for (Transform& transformation : transformations1) {
    //     Transform* t_ptr = &transformation;
    //     drawObject(mvStack, t_ptr, 1, sphere.getNumIndices(), 3);
    //     mvStack.pop();
    // }
    // for (Transform& transformation : transformations2) {
    //     Transform* t_ptr = &transformation;
    //     drawObject(mvStack, t_ptr, 1, sphere.getNumIndices(), 3);
    //     mvStack.pop();
    // }
    // mvStack.pop();
    // drawObject(mvStack, t5, 0, torus.getNumIndices(), 0);
    // for (Transform& transformation : transformations1) {
    //     Transform* t_ptr = &transformation;
    //     drawObject(mvStack, t_ptr, 1, sphere.getNumIndices(), 3);
    //     mvStack.pop();
    // }
    // for (Transform& transformation : transformations2) {
    //     Transform* t_ptr = &transformation;
    //     drawObject(mvStack, t_ptr, 1, sphere.getNumIndices(), 3);
    //     mvStack.pop();
    // }
    // mvStack.pop();
    // mvStack.pop();


    // drawObject(mvStack, surfaceTransform1, PLANE, surfaceInd.size(), 1);
    // mvStack.pop();
    drawObject(mvStack, surfaceTransform2, PLANE, surfaceInd.size(), 1);
    mvStack.pop();
    // drawObject(mvStack, surfaceTransform2, CUBE, roomCubeInd.size(), STATUE);

    drawObjectsInstanced(SPHERE, ONYX, sphereInd.size());
    // mvStack.pop();


    glFrontFace(GL_CW);
    drawObject(mvStack, t6, ROOM, roomCubeInd.size(), GRASS);
    
    

    while (!mvStack.empty()) {
        mvStack.pop();
    };
}


void Context::createTransformations() {

    t1 = new Transform(f, constantOne, constantT, rotationY);
    t2 = new Transform(glm::vec3(0.0f, -75.0f, 0.0f), 28.0f, 0.0f, rotationX);
    t6 = new Transform(glm::vec3(0.0f, 0.0f, 0.0f), 80.0f, 0.0f, rotationX);
    t3 = new Transform(f2, constantHalf, constantT, rotationX);
    t4 = new Transform(ft, constantOne, constantT, rotationY);
    t5 = new Transform(f2t, constantHalf, constantT, rotationX);
    tLight = new Transform(getLightPos, constantTen, constantZero, rotationX);

    surfaceTransform1 = new Transform(glm::vec3(0.0f, 10.0f, 0.0f), 3.0f, -PIhalf/2.0f, rotationX);
    surfaceTransform2 = new Transform(surfacePos, constantX, constantT, rotationZ);



    auto makeTranslationFunction1 = [](float offset) {
        return [offset](float t) -> glm::vec3 {
            return glm::vec3(
                cos(t*2.0f + offset) * 9.0f,
                sin((t) + offset) * 9.0f,
                sin(t + offset) * 9.0f
            );
        };
    };

    auto makeTranslationFunction2 = [](float offset) {
        return [offset](float t) -> glm::vec3 {
            return glm::vec3(
                cos(t*2.0f + offset) * 18.0f,
                -sin(((t+PIhalf)) + offset) * 18.0f,
                sin((t * .5f) + offset) * 18.0f
            );
        };
    };
  
    for (float i = 0.0f; i < PI * 2.0f; i+=.3f) {
        translationFunctions1.push_back(makeTranslationFunction1(i));
        translationFunctions2.push_back(makeTranslationFunction2(i));

    }

    for (auto& func : translationFunctions1) {
        transformations1.push_back(Transform(func, constantOne, constantT, rotationX));
    }

    for (auto& func : translationFunctions2) {
        transformations2.push_back(Transform(func, constantOne, constantT, rotationX));
    }
}