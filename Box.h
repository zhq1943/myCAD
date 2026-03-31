#pragma once
#include<glad/glad.h>
#include<vector>
#include<cstring>
#include"Shader.h"

class Box {
	float Length, Width, Height;
	unsigned int VAO, VBO, EBO;
public:
	Box(float l = 1.0f, float w = 1.0f, float h = 1.0f)
	:Length(l), Width(w), Height(h){
		initBuffers();
		updateGeometry();
	}


	void Draw(Shader& shader, bool showWireFrame) {
		//绘制填充面
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		shader.setVec4("u_Color", glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));

		if (showWireFrame)
		{
			//开启多边形偏移，防止线条被遮挡
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(1.0f, 1.0f);
		}
		

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		if (showWireFrame)
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
		Length = l; Width = w; Height = h;
		updateGeometry();
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
		float x = Length / 2.0f;
		float y = Width / 2.0f;
		float z = Height / 2.0f;

		float newCoords[] = {
			-x, -y, -z,  x, -y, -z,  x,  y, -z, -x,  y, -z, // 0-3
			-x, -y,  z,  x, -y,  z,  x,  y,  z, -x,  y,  z  // 4-7
		};

		memcpy(vertices, newCoords, sizeof(newCoords));

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	}
};
