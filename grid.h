#pragma once
#include<glad/glad.h>
#include<vector>
#include"shader.h"

class Grid {
public:
	unsigned int vao, vbo;
	Grid() {
		float vertices[] = {
			1,1,0, -1,1,0,-1,-1,0,
			1,1,0,-1,-1,0,1,-1,0
		};

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	};

	void Draw(Shader& shader, const glm::mat4& view, const glm::mat4& proj) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE); // 禁用深度写入

		shader.use();
		shader.setMat4("view", view);
		shader.setMat4("projection", proj);

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDepthMask(GL_TRUE); // 重新启用深度写入
		glDisable(GL_BLEND);
	}
};