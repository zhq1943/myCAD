#pragma once
#include<glad/glad.h>
#include<vector>
#include<cstring>
#include"Shader.h"
#include"Geometry.h"

class Box:public Geometry {
	unsigned int VAO, VBO, EBO;
	bool  mbShowWireFrame;
public:
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Barycentric;
	};
	float Size[3] = { 1.0f,1.0f,1.0f };
	Box(std::string name){
		Name = name;
		mbShowWireFrame = false;
		initBuffers();
		updateGeometry();
	}


	void Draw(Shader& shader) {
		glm::mat4 model = glm::translate(glm::mat4(1.0f), Position);
		shader.setMat4("u_Model", model);

		//绘制填充面
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		shader.setVec4("u_Color", glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));

		if (mbShowWireFrame)
		{
			//开启多边形偏移，防止线条被遮挡
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(1.0f, 1.0f);
		}
		

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		if (mbShowWireFrame)
		{
			glDisable(GL_POLYGON_OFFSET_FILL);
			//绘制黑色边框
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glLineWidth(2.0f);
			shader.setVec4("u_Color", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			//恢复默认填充模式
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		
		glBindVertexArray(0);
	}

	void Update(float l, float w, float h) {
		Size[0] = l; Size[1] = w; Size[2] = h;
		updateGeometry();
	}

	void UpdateUI() override {
		ImGui::InputText("Name", &Name[0], 64);
		if (ImGui::DragFloat3("Position", &Position[0], 0.1f)) {
			
		}

		if (ImGui::DragFloat3("Size", Size, 0.1f, 0.1f, 10.0f))
		{
			updateGeometry();
		}

		if (ImGui::Checkbox("WireFrame",&mbShowWireFrame))
		{

		}
	}

	//添加导出STL格式的函数
	void ExportToSTL(const std::string& filename) {
		std::ofstream outFile(filename);
		if (!outFile.is_open())
		{
			return;
		}

		outFile << "solid SimpleCAD_box\n";

		for (int i = 0; i < 36; i += 3) {
			glm::vec3 v1 = getVertexPos(indices[i]);
			glm::vec3 v2 = getVertexPos(indices[i + 1]);
			glm::vec3 v3 = getVertexPos(indices[i + 2]);

			glm::vec3 normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));

			outFile << "facet normal" << normal.x << " " << normal.y << " " << normal.z << "\n";
			outFile << "    outer loop\n";
			outFile << "          vertex " << v1.x << " " << v1.y << " " << v1.z << "\n";
			outFile << "          vertex " << v2.x << " " << v2.y << " " << v2.z << "\n";
			outFile << "          vertex " << v3.x << " " << v3.y << " " << v3.z << "\n";
			outFile << "     endloop\n";
			outFile << "endfacet\n";
		}

		outFile << "endsolid SimpleCAD_Box\n";
		outFile.close();
		std::cout << "STL Exported to: " << filename << std::endl;
	}

	glm::vec3 getVertexPos(int index) {
		return glm::vec3(vertices[index * 3], vertices[index * 3 + 1], vertices[index * 3 + 2]);
	}

private:
	float vertices[24];
	unsigned int indices[36] = {
		0, 1, 2, 2, 3, 0,  4, 5, 6, 6, 7, 4, // 前后
		0, 4, 7, 7, 3, 0,  1, 5, 6, 6, 2, 1, // 左右
		3, 2, 6, 6, 7, 3,  0, 1, 5, 5, 4, 0  // 上下
	};
	void initBuffers() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), NULL, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}

	void updateGeometry() {
		float x = Size[0] / 2.0f;
		float y = Size[1] / 2.0f;
		float z = Size[2] / 2.0f;

		float newCoords[] = {
			-x, -y, -z,  x, -y, -z,  x,  y, -z, -x,  y, -z, // 0-3
			-x, -y,  z,  x, -y,  z,  x,  y,  z, -x,  y,  z  // 4-7
		};

		memcpy(vertices, newCoords, sizeof(newCoords));

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	}
};
