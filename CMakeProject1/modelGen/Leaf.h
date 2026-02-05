#pragma once
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include <iostream>

class Leaf {
	private:
		int numVertices;
		int numIndices;
		std::vector<int> indices;
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> texCoords;
		std::vector<glm::vec3> normals;
		void init(int prec, float h, float w);
		float toRadians(float degrees);
		void computeNormals();

	public:
		Leaf(int prec, float h, float w);
		int getNumVertices();
		int getNumIndices();
		std::vector<int> getIndices();
		std::vector<glm::vec3> getVertices();
		std::vector<glm::vec2> getTexCoords();
		std::vector<glm::vec3> getNormals();
};