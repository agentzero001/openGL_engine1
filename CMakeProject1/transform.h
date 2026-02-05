#pragma once
#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


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
		glm::mat4 lMat;
};
