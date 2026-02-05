#include "Leaf.h"

using namespace std;

Leaf::Leaf(int prec, float h, float w) {
    init(prec, h, w);
    computeNormals();
}

void Leaf::init(int prec, float h, float w) {
    float p = 2.0;
    for (int n=-prec; n<=prec; n++) {
        float t = n * 1/(float)prec;
        float x = (h * t) + h;
        float z = w * (1.0f - pow(abs(t), p));
        float y = pow(t+1.0f, 2.0f);
        vertices.push_back(glm::vec3(x, y, z));
        texCoords.push_back(glm::vec2(0.0f, 0.0f));
    }
    for (int n=-prec; n<=prec; n++) {
        float t = n * 1/(float)prec;
        float x = (h * t) + h;
        float z = -(w * (1.0f - pow(abs(t), p)));
        float y = pow(t+1.0f, 2.0f);
        vertices.push_back(glm::vec3(x, y, z));
        texCoords.push_back(glm::vec2(0.0f, 0.0f));
    }
    int N = vertices.size() / 2;
    for (int i = 0; i<N-1; i++) {
        int ui = i;
        int ui1 = i+1;
        int li = i+N;
        int li1 = i+N+1;
        indices.push_back(ui);
        indices.push_back(li);
        indices.push_back(ui1);
        indices.push_back(ui1);
        indices.push_back(li);
        indices.push_back(li1);
    }
    numVertices = vertices.size();
    numIndices = indices.size();
}

void Leaf::computeNormals() {
    normals.resize(vertices.size(), glm::vec3(0.0f));
    for (size_t i = 0; i < indices.size(); i += 3) {
        unsigned  i0 = indices[i];
        unsigned  i1 = indices[i + 1];
        unsigned  i2 = indices[i + 2]; 

        glm::vec3 v0 = vertices[i0];
        glm::vec3 v1 = vertices[i1];
        glm::vec3 v2 = vertices[i2];  
        
        glm::vec3 e1 = v1 - v0;
        glm::vec3 e2 = v2 - v0;
        glm::vec3 faceNormal = glm::cross(e1, e2);

        float len = glm::length(faceNormal);
        if (len > 1e-6f)
            faceNormal /= len;
        else
            faceNormal = glm::vec3(0.0f);

        normals[i0] += faceNormal;
        normals[i1] += faceNormal;
        normals[i2] += faceNormal;
    }

    // std::cout << normals[2].x << " " << normals[2].y << " " << normals[2].z << std::endl;
 }

int Leaf::getNumVertices() { return numVertices; }
int Leaf::getNumIndices() { return numIndices; }
std::vector<int> Leaf::getIndices() { return indices; }
std::vector<glm::vec3> Leaf::getVertices() { return vertices; }
std::vector<glm::vec2> Leaf::getTexCoords() { return texCoords; }
std::vector<glm::vec3> Leaf::getNormals() { return normals; }