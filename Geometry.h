#pragma once
#include<string>
#include<glm/glm.hpp>

class Geometry {
public:
	std::string Name;
	glm::vec3 Position = glm::vec3(0.0f);
	bool IsSelected = false;

	virtual void Draw(class Shader& shader) = 0;
	virtual void UpdateUI() = 0;
	virtual ~Geometry() {};
};
