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
		//float IsFeatureEdge;
		Vertex(glm::vec3 pos, glm::vec3 Bary) {
			Position = pos;
			Barycentric = Bary;
		};

		Vertex() {};
	};
	float Size[3] = { 1.0f,1.0f,1.0f };
	Box(std::string name){
		Name = name;
		mbShowWireFrame = false;
		initBuffers();
		updateGeometry();
	}


	void Draw(Shader& shader) override{
		glm::mat4 model = glm::translate(glm::mat4(1.0f), Position);
		shader.setMat4("u_Model", model);

		//绘制填充模式
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		shader.setVec3("u_MeshColor", glm::vec3(0.5f, 0.5f, 0.5f));
		shader.setVec3("u_EdgeColor", mbShowWireFrame?glm::vec3(1.0, 0.5f, 0.0f):glm::vec3(0.5f));
		shader.setfloat("u_EdgeWidth", 1.0f);


		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
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

	//导出为STL格式的方法
	void ExportToSTL(const std::string& filename) {
		std::ofstream outFile(filename);
		if (!outFile.is_open())
		{
			return;
		}

		outFile << "solid SimpleCAD_box\n";

		for (int i = 0; i < 36; i += 3) {
			glm::vec3 v1 = getVertexPos(i);
			glm::vec3 v2 = getVertexPos(i + 1);
			glm::vec3 v3 = getVertexPos(i + 2);

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
		return glm::vec3(vData[index].Position);
	}

private:
	std::vector<Vertex> vData;
	/*unsigned int indices[36] = {
		0, 1, 2, 2, 3, 0,  4, 5, 6, 6, 7, 4, // 前面
		0, 4, 7, 7, 3, 0,  1, 5, 6, 6, 2, 1, // 侧面
		3, 2, 6, 6, 7, 3,  0, 1, 5, 5, 4, 0  // 顶面
	};*/
	void initBuffers() {
		vData.resize(36);
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		//glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, vData.size()*sizeof(Vertex), NULL, GL_DYNAMIC_DRAW);

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}

	void updateGeometry() {
		float x = Size[0] / 2.0f;
		float y = Size[1] / 2.0f;
		float z = Size[2] / 2.0f;
		vData.clear();
		auto addFace = [&](glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
			vData.push_back(Vertex(p0, glm::vec3(1,0,0)));
			vData.push_back(Vertex(p1, glm::vec3(0,1,0)));
			vData.push_back(Vertex(p2, glm::vec3(0,1,1)));

			vData.push_back(Vertex(p2, glm::vec3(1,0,0)));
			vData.push_back(Vertex(p3, glm::vec3(0,1,0)));
			vData.push_back(Vertex(p0, glm::vec3(0,1,1)));
		};

		//计算出八个基本顶点的坐标
		glm::vec3 v0(-x, -y, -z);
		glm::vec3 v1(x, -y, -z);
		glm::vec3 v2(x, y, -z);
		glm::vec3 v3(-x, y, -z);
		glm::vec3 v4(-x, -y, z);
		glm::vec3 v5(x, -y, z);
		glm::vec3 v6(x, y, z);
		glm::vec3 v7(-x, y, z);
		//定义立方体12个三角形（6个面，每个面2个）
		// 手动指定每个三角形的中线坐标分量

		// 构建36个顶点（12个三角形），每个三角形顶点赋予重心坐标

		// 定义12个三角形的顶点索引（每3个索引构成一个三角形）
		int triangleIndices[12][3] = {
			{0, 1, 2}, {2, 3, 0},  // 前面 (z = -z)
			{4, 5, 6}, {6, 7, 4},  // 后面 (z = z)
			{0, 4, 7}, {7, 3, 0},  // 左面 (x = -x)
			{1, 5, 6}, {6, 2, 1},  // 右面 (x = x)
			{3, 2, 6}, {6, 7, 3},  // 顶面 (y = y)
			{0, 1, 5}, {5, 4, 0}   // 底面 (y = -y)
		};

		// 重心坐标，每个三角形的三个顶点分别赋予 (1,0,0), (0,1,0), (0,0,1)
		glm::vec3 barycentricCoords[3] = {
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f)
		};

		glm::vec3 vertexPositions[8] = { v0, v1, v2, v3, v4, v5, v6, v7 };



		// 为每个三角形生成3个顶点
		addFace(vertexPositions[0], vertexPositions[1], vertexPositions[2], vertexPositions[3]);
		addFace(vertexPositions[4], vertexPositions[5], vertexPositions[6], vertexPositions[7]);
		addFace(vertexPositions[0], vertexPositions[4], vertexPositions[7], vertexPositions[3]);
		addFace(vertexPositions[1], vertexPositions[5], vertexPositions[6], vertexPositions[2]);
		addFace(vertexPositions[3], vertexPositions[2], vertexPositions[6], vertexPositions[7]);
		addFace(vertexPositions[0], vertexPositions[1], vertexPositions[5], vertexPositions[4]);


		// 更新VBO数据，每个顶点包含Position(3 floats) + Barycentric(3 floats) = 6 floats
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vData.size()*sizeof(Vertex), vData.data());

		/*float newCoords[] = {
			-x, -y, -z,  x, -y, -z,  x,  y, -z, -x,  y, -z, // 0-3
			-x, -y,  z,  x, -y,  z,  x,  y,  z, -x,  y,  z  // 4-7
		};

		memcpy(vertices, newCoords, sizeof(newCoords));

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);*/
	}
};