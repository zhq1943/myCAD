#pragma once
#include<glad/glad.h>
#include<vector>
#include<cstring>
#include"Shader.h"
#include"Geometry.h"
#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"
class LineSegment :public Geometry {
public:
	glm::vec3 Start = glm::vec3(0.0f);
	glm::vec3 End = glm::vec3(1.0f, 0.0f, 0.0f);
	unsigned int VAO, VBO;

	LineSegment(std::string name) {
		Name = name;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		updateGeometry();
	}

	void updateGeometry() override {
		float vertices[] = {Start.x, Start.y, Start.z, End.x, End.y, End.z};
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

		glBindVertexArray(VAO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}

	void Draw(Shader& shader) override {
		shader.setMat4("u_Model", glm::mat4(1.0f));
		shader.setVec3("u_Color", IsSelected ? glm::vec3(0, 1, 0) : glm::vec3(1, 1, 1));
		glBindVertexArray(VAO);
		glDrawArrays(GL_LINES, 0, 2);
	}

	void UpdateUI() override {
		if (ImGui::DragFloat3("Start", &Start[0], 0.1f) || ImGui::DragFloat3("End", &End[0], 0.1f)) {
			updateGeometry();
		}
	}
};