#pragma once
#include<glad/glad.h>
#include<vector>
#include<cstring>
#include"Shader.h"
#include"Geometry.h"
#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"
class Arc :public Geometry {
public:
	glm::vec3 Center = glm::vec3(0.0f);
	float RadiusX = 1.0f;
	float RadiusY = 1.0f;
	float StartAngle = 0.0f;
	float EndAngle = 180.0f;
	int Segments = 64;
	unsigned int VAO, VBO;
	int vertexCount;

	Arc(std::string name) {
		Name = name;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		updateGeometry();
	}

	void updateGeometry() override {
		std::vector<float> vertices;
		float startRad = glm::radians(StartAngle);
		float endRad = glm::radians(EndAngle);

		for (int i = 0; i <= Segments; ++i)
		{
			float t = (float)i / Segments;
			float angle = startRad + t * (endRad - startRad);

			vertices.push_back(Center.x + RadiusX * cos(angle));
			vertices.push_back(Center.y);
			vertices.push_back(Center.z + RadiusY * sin(angle));
		}

		vertexCount = vertices.size() / 3;
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

		glBindVertexArray(VAO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}

	void Draw(Shader& shader) override {
		shader.setMat4("u_Model", glm::mat4(1.0f));
		shader.setVec3("u_Color", IsSelected == true ? glm::vec3(0, 1, 0) : glm::vec3(1, 1, 1));
		glBindVertexArray(VAO);
		glDrawArrays(GL_LINE_STRIP, 0, vertexCount);
	}

	void UpdateUI() override {
		bool changed = false;
		changed |= ImGui::DragFloat3("Center", &Center[0], 0.1f);
		changed |= ImGui::DragFloat("Radius X", &RadiusX, 0.1f, 0.1f);
		changed |= ImGui::DragFloat("Radius Y", &RadiusY, 0.1f, 0.1f);
		changed |= ImGui::SliderFloat("Start Angle", &StartAngle, 0.0f, 360.0f);
		changed |= ImGui::SliderFloat("End Angle", &EndAngle, 0.0f, 360.0f);

		if (changed) updateGeometry();
	}
};