#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <array>
#include <tiny_obj_loader.h>
#include <stb_image.h>
#include <cmath>
#include <unordered_map>
#include <glm/gtx/hash.hpp>
#include <random>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL2/SOIL2.h>
#include <filesystem>

inline std::string ROOM_CUBE_PATH = "C:/Users/jensm/source/repos/CMakeProject1/res/models/roomCube.obj";
inline std::string CUBE_PATH = "C:/Users/jensm/source/repos/CMakeProject1/res/models/cube.obj";
inline const std::string BRICK_TEXTURE_PATH =  std::string(RESOURCE_DIR) + "/textures/brick.jpg";
// inline const char* GRASS_TEXTURE_PATH = "C:/Users/jensm/source/repos/CMakeProject1/res/textures/grass.jpg";
// inline const char* EARTH_TEXTURE_PATH = "C:/Users/jensm/source/repos/CMakeProject1/res/textures/earth.jpg";
// inline const char* SUN_TEXTURE_PATH = "C:/Users/jensm/source/repos/CMakeProject1/res/textures/sun.png";
// inline const char* MOON_TEXTURE_PATH = "C:/Users/jensm/source/repos/CMakeProject1/res/textures/moon.jpg";
// inline const char* STATUE_TEXTURE_PATH = "C:/Users/jensm/source/repos/CMakeProject1/res/textures/statue.jpg";

namespace fs = std::filesystem;

inline const fs::path TextureDir = "C:/Users/jensm/source/repos/CMakeProject1/res/textures";

inline const fs::path BRICK_TEXTURE  = TextureDir / "brick.jpg";
inline const fs::path GRASS_TEXTURE  = TextureDir / "onyx.png";
inline const fs::path EARTH_TEXTURE  = TextureDir / "earth.jpg";
inline const fs::path SUN_TEXTURE    = TextureDir / "sun.png";
inline const fs::path MOON_TEXTURE   = TextureDir / "moon.jpg";
inline const fs::path STATUE_TEXTURE = TextureDir / "statue.jpg";
inline const fs::path ONYX_TEXTURE = TextureDir / "marble.jpg";


inline const std::vector<fs::path> texturePaths = {
    BRICK_TEXTURE,
    GRASS_TEXTURE,
    EARTH_TEXTURE,
    SUN_TEXTURE,
    MOON_TEXTURE,
    STATUE_TEXTURE,
    ONYX_TEXTURE
};

enum Textures {
    BRICK,
    GRASS,
    EARTH,
    SUN,
    MOON,
    STATUE,
    ONYX
};

inline const char* CUBE_MAP_DIR = "C:/Users/jensm/source/repos/CMakeProject1/res/textures/sunset";

struct Vertex {
	bool operator==(const Vertex& other) const {
        return pos == other.pos && color == other.color && texCoord == other.texCoord;
    }

	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoord;
	glm::vec3 norm;
};

struct Particle {
    // glm::vec3 pos;
    alignas(16) glm::vec3 velocity;
    // glm::vec4 color;
};

namespace std {
    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vertex) const {
			return ((hash<glm::vec3>()(vertex.pos) 
				^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) 
				^ (hash<glm::vec2>()(vertex.texCoord) << 1);
		}
    };
}

void loadModel(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, std::string model_path);
GLuint loadTexture(const char* texImagePath);
GLuint loadCubeMap(const char *mapDir);
void setupShadowBuffers(int screenX, int  screenY, GLuint &shadowBuffer, GLuint &shadowTex, int shadowTexScale);
void bindBuffers(
    int objInd,
	GLuint *vbo,
    std::vector<float>& vertValues,
    std::vector<float>& texValues,
    std::vector<float>& normValues,
    std::vector<int>& ind );

std::vector<glm::vec3> createPerInstanceData(int numParticles);
void createShaderStorageBuffers(int numParticles);

extern const std::vector<Vertex> surfaceVertices;
extern const std::vector<uint16_t> surfaceIndices;


extern float vertexPositions[108];

// extern std::vector<float> perInstanceData;

