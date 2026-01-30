#define TINYOBJLOADER_IMPLEMENTATION
#include "resource.h"


void loadModel(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, std::string model_path) {
    tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, model_path.c_str())) {
		throw std::runtime_error(warn + err);
	}

	std::unordered_map<Vertex, uint32_t> uniqueVertices{};

   	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			Vertex vertex{};
			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			vertex.texCoord = {
				attrib.texcoords[2 * index.texcoord_index + 0],
				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};

            vertex.norm = {
                attrib.normals[3 * index.normal_index + 0],
				attrib.normals[3 * index.normal_index + 1],
				attrib.normals[3 * index.normal_index + 2]
            };
			
			vertex.color = { .5f, .5f, .5f };
			
			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
			    vertices.push_back(vertex);
			}

			indices.push_back(uniqueVertices[vertex]);
		}
	}
}

GLuint loadTexture(const char* texImagePath) {
    GLuint textureID;
	
    textureID = SOIL_load_OGL_texture(texImagePath, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

    //mipmapping
    glBindTexture(GL_TEXTURE_2D, textureID);
    //GL_LINEAR_MIPMAP_LINEAR gives trilinear filtering
    //GL_LINEAR_MIPMAP_NEAREST gives linear filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


    //anisotropic filtering
    //if (glewIsSupported("GL_EXT_texture_filter_anisotropic")) {
    //    GLfloat anisoSetting = 0.0f;
    //    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoSetting);
    //    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisoSetting);
    //}

    if (textureID == 0) std::cout << "could not find texture file" << texImagePath << std::endl;
    return textureID;
}



GLuint loadCubeMap(const char *mapDir) {
    GLuint textureRef;

    std::string xp = mapDir; xp = xp + "/px.png";
    std::string xn = mapDir; xn = xn + "/nx.png";
    std::string yp = mapDir; yp = yp + "/py.png";
    std::string yn = mapDir; yn = yn + "/ny.png";
    std::string zp = mapDir; zp = zp + "/pz.png";
    std::string zn = mapDir; zn = zn + "/nz.png";

    textureRef = SOIL_load_OGL_cubemap(
		xp.c_str(),
		xn.c_str(),
		yp.c_str(),
		yn.c_str(),
		zp.c_str(),
		zn.c_str(),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS
	);



	if (textureRef == 0) std::cout << "failed to find cube map image file" << std::endl;
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureRef);
	//reduce seams
	
	// glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	return textureRef;

}


const std::vector<Vertex> surfaceVertices = {
	{{-1.0, -1.0, 0.0}, {}, {0.0, 0.0}, {0.0, 0.0, 1.0}},
	{{1.0, -1.0, 0.0}, {}, {1.0, 0.0}, {0.0, 0.0, 1.0}},
	{{1.0, 1.0, 0.0}, {}, {1.0, 1.0}, {0.0, 0.0, 1.0}},
	{{-1.0, 1.0, 0.0}, {}, {0.0, 1.0}, {0.0, 0.0, 1.0}}
};

const std::vector<uint16_t> surfaceIndices = {
	0, 1, 2, 2, 3, 0
};


void setupShadowBuffers(int screenX, int  screenY, GLuint &shadowBuffer, GLuint &shadowTex, int shadowTexScale) {
    glGenFramebuffers(1, &shadowBuffer);
    glGenTextures(1, &shadowTex);
    glBindTexture(GL_TEXTURE_2D, shadowTex);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_DEPTH_COMPONENT32,
        screenX * shadowTexScale , screenY * shadowTexScale,
        0, 
        GL_DEPTH_COMPONENT, 
        GL_FLOAT, 
        0
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void bindBuffers(
    int objInd,
	GLuint *vbo,
    std::vector<float>& vertValues,
    std::vector<float>& texValues,
    std::vector<float>& normValues,
    std::vector<int>& ind ) {

    glBindBuffer(GL_ARRAY_BUFFER, vbo[objInd * 4]);
    glBufferData(GL_ARRAY_BUFFER, vertValues.size() * sizeof(float), &vertValues[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[(objInd * 4) + 1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * sizeof(int), &ind[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[(objInd * 4)  + 2]);
    glBufferData(GL_ARRAY_BUFFER, normValues.size() * 4, &normValues[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[(objInd * 4) + 3]);
    glBufferData(GL_ARRAY_BUFFER, texValues.size() * 4, &texValues[0], GL_STATIC_DRAW);
}


std::vector<glm::vec3> createPerInstanceData(int numParticles) {
    std::vector<glm::vec3> perInstanceData;

    std::default_random_engine rndEngine((unsigned)time(nullptr));
	std::uniform_real_distribution<float> rndDist(0.0f, 1.0f);

    for (int i = 0; i < numParticles; i++) {
        float u = rndDist(rndEngine);
        float v = rndDist(rndEngine);
        float w = rndDist(rndEngine);

        float radius = 0.25f * std::cbrt(u);

        // Spherical coordinates
        float theta = 2.0f * 3.14159265358979323846f * v;   
        float phi   = std::acos(1.0f - 2.0f * w);         

        // Convert to Cartesian coordinates
        float x = radius * std::sin(phi) * std::cos(theta);
        float y = radius * std::sin(phi) * std::sin(theta);
        float z = radius * std::cos(phi);

        perInstanceData.push_back(glm::vec3(x, y, z));
    
    }
    return perInstanceData;
}

void createShaderStorageBuffers(int numParticles) {

    std::vector<Particle> particles(numParticles);
    std::default_random_engine rndEngine((unsigned)time(nullptr));
	std::uniform_real_distribution<float> rndDist(-1.0f, 1.0f);

    for (auto& particle: particles) {
        float x = rndDist(rndEngine) * 75.0;
        float z = rndDist(rndEngine) * 75.0;
        particle.pos = glm::vec3(x, 0.0, z);
        particle.velocity = glm::vec3(0.0, 1.0, 0.0);
        particle.velocity2 = glm::vec3(0.0, 0.0, 1.0);
    }

    GLuint instanceSSBO = 0;
    
    //generate buffer
    glGenBuffers(1, &instanceSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, instanceSSBO);

    //upload data
    glBufferData(GL_SHADER_STORAGE_BUFFER, numParticles * sizeof(Particle), particles.data(), GL_DYNAMIC_DRAW); 

    //bind buffer to shader
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, instanceSSBO);

}

void createShaderStorageBuffers2(int numParticles) {

    std::vector<Particle> particles(numParticles);
    std::default_random_engine rndEngine((unsigned)time(nullptr));
	std::uniform_real_distribution<float> rndDist(-1.0f, 1.0f);
    std::uniform_real_distribution<float> rndDist2(0.0f, 1.0f);

    for (auto& particle: particles) {
        float x = rndDist(rndEngine) * 250.0;
        float y = rndDist(rndEngine) * 250.0;
        float z = rndDist(rndEngine) * 250.0;
        particle.pos = glm::vec3(x, y, z);

        
        float u = rndDist2(rndEngine);
        float v = rndDist2(rndEngine);
        float w = rndDist2(rndEngine);

        float radius = 0.25f * std::cbrt(u);

        // Spherical coordinates
        float theta = 2.0f * 3.14159265358979323846f * v;   
        float phi   = std::acos(1.0f - 2.0f * w);         

        // Convert to Cartesian coordinates
        float x2 = radius * std::sin(phi) * std::cos(theta);
        float y2 = radius * std::sin(phi) * std::sin(theta);
        float z2 = radius * std::cos(phi);

        particle.velocity2 = glm::vec3(x2, y2, z2);
        particle.velocity = glm::vec3(-.03, -.1, 0.0);

    }    

    GLuint instanceSSBO = 0;
    
    //generate buffer
    glGenBuffers(1, &instanceSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, instanceSSBO);

    //upload data
    glBufferData(GL_SHADER_STORAGE_BUFFER, numParticles * sizeof(Particle), particles.data(), GL_DYNAMIC_DRAW); 

    //bind buffer to shader
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, instanceSSBO);

}