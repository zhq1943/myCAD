#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<iostream>
#include<string>
#include<memory>
#include<vector>

#include"Shader.h"
#include"Camera.h"
#include"Box.h"


Camera myContextCamera;
double lastX, lastY;
bool firstMouse = true;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	if (!ImGui::GetIO().WantCaptureMouse) {
		myContextCamera.Zoom(float(yoffset));
	}
}

int main() {
	if (!glfwInit()) return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1280, 720, "Simple CAD Tool", NULL, NULL);
	if (!window) { glfwTerminate(); return -1; }
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Faild to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetScrollCallback(window, scroll_callback);

	glEnable(GL_DEPTH_TEST);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	Shader shader("shader/default.vs", "shader/default.fs");

	std::vector<std::shared_ptr<Geometry>> sceneObjects;
	int selectedIndex = -1;


	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Scene Tree");
		if (ImGui::Button("Add New Box"))
		{
			std::string name = "Box" + std::to_string(sceneObjects.size());
			sceneObjects.push_back(std::make_shared<Box>(name));
		}

		ImGui::Separator();

		for (int i = 0; i < sceneObjects.size(); i++)
		{
			if (ImGui::Selectable(sceneObjects[i]->Name.c_str(), selectedIndex == i)) {
				selectedIndex = i;
			}
		}

		ImGui::End();

		ImGui::Begin("Inspector");
		if (selectedIndex != -1) {
			sceneObjects[selectedIndex]->UpdateUI();
		}
		else {
			ImGui::Text("Select an object to edit");
		}

		ImGui::End();


		ImGui::Begin("File Operations");
		if (ImGui::Button("Export as STL"))
		{
			//myBox.ExportToSTL("my_design.stl");
		}
		ImGui::End();

		ImGui::Render();

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);

			if (firstMouse) {
				lastX = xpos; lastY = ypos;
				firstMouse = false;
			}

			float xoffset = (float)(xpos - lastX);
			float yoffset = (float)(lastY - ypos);
			lastX = xpos; lastY = ypos;

			if (!ImGui::GetIO().WantCaptureKeyboard)
			{
				myContextCamera.Rotate(xoffset, yoffset);
			}
		}
		else {
			firstMouse = true;
		}


		int display_w(0), display_h(0);
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 1.0f));
		glm::mat4 pv = myContextCamera.GetProjectMatrix() * myContextCamera.GetViewMatrix();
		shader.use();
		for(auto& obj:sceneObjects)
		{
			shader.setMat4("pv", pv);
			obj->Draw(shader);
		}
		
		glfwSwapBuffers(window);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}