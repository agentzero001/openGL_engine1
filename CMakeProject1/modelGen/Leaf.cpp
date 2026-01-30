#include "Leaf.h"










Leaf::Leaf(int prec) {
    
}


int Leaf::getNumVertices() { return numVertices; }
int Leaf::getNumIndices() { return numIndices; }
std::vector<int> Leaf::getIndices() { return indices; }
std::vector<glm::vec3> Leaf::getVertices() { return vertices; }