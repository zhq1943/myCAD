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
#include"grid.h"

#include"LineSegment.h"
#include"Arc.h"
#include"MyDXFReader.h"

Camera myContextCamera;
double lastX, lastY;
bool firstMouse = true;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	if (!ImGui::GetIO().WantCaptureMouse) {
		myContextCamera.ProcessScroll(float(yoffset));
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;



	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		if (myContextCamera.Mode == CameraMode::DIM_3D)
		{
			myContextCamera.Rotate(xoffset, yoffset);
		}
		else {
			myContextCamera.Panning(xoffset, yoffset);
		}
	}
	
}

void ImportDXF(const std::string& filePath, std::vector<std::shared_ptr<Geometry>>& sceneObjects) {
	// TODO: 实现导入 DXF 文件的逻辑
	dxfRW reader(filePath.c_str());
	MyDXFReader interface;

	if (reader.read(&interface, false)) {
		for (auto& obj : interface.tempObjects) {
			sceneObjects.push_back(obj);
		}
	}else{
		std::cout << "Failed to read DXF file" << std::endl;
	}
}

void ExportDXF(const std::string& filePath, std::vector<std::shared_ptr<Geometry>>& sceneObjects) {
	// TODO: 实现导出 DXF 文件的逻辑
	dxfRW writer(filePath.c_str());
	
	DXFExportHandeler handler(&sceneObjects, &writer);

	bool success = writer.write(&handler, DRW::AC1015, false);
	if (success)
	{
		std::cout << "DXF Export successfully to" << filePath << std::endl;
	}
	else {
		std::cerr << "Falied to export DXF!" << std::endl;
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
	glfwSetCursorPosCallback(window, mouse_callback);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	Shader shader("shader/default.vs", "shader/default.fs");
	Shader gridShader("shader/grid.vs", "shader/grid.fs");
	Shader sampleShader("shader/SimpleColor.vs", "shader/SimpleColor.fs");

	Grid infiniteGrid;

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

		if (ImGui::BeginPopup("AddObjectPopup")) {
			if (ImGui::MenuItem("Line Segment")) {
				sceneObjects.push_back(std::make_shared<LineSegment>("Line" + std::to_string(sceneObjects.size())));
			}

			if (ImGui::MenuItem("Arc/Ellipse")) {
				sceneObjects.push_back(std::make_shared<Arc>("Arc" + std::to_string(sceneObjects.size())));
			}

			ImGui::EndPopup();
		}

		if (ImGui::Button("Add Entity ..."))
		{
			ImGui::OpenPopup("AddObjectPopup");
		}

		ImGui::Separator();

		for (int i = 0; i < sceneObjects.size(); i++)
		{
			if (ImGui::Selectable(sceneObjects[i]->Name.c_str(), selectedIndex == i)) {
				selectedIndex = i;
				sceneObjects[i]->IsSelected = true;
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

		ImGui::Begin("View Controls");
		const char* modes[] = { "3D Perspective", "2D Top View" };
		int currentItem = (int)myContextCamera.Mode;

		if (ImGui::Combo("View Mode", &currentItem, modes, 2))
		{
			myContextCamera.Mode = (CameraMode)currentItem;

			if (myContextCamera.Mode == CameraMode::DIM_2D)
			{

			}
		}

		ImGui::End();

		if(ImGui::BeginMainMenuBar())
		{
			if(ImGui::BeginMenu("File"))
			{
				if(ImGui::MenuItem("Open"))
				{
					ImportDXF("sample/my_design.dxf",sceneObjects);
				}
				if(ImGui::MenuItem("Export as DXF"))
				{
					ExportDXF("sample/my_design.dxf",sceneObjects);
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		ImGui::Render();

		int display_w(0), display_h(0);
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// 在窗口调整大小时获取最新的宽高比
		float aspectRatio = (float)display_w / (float)display_h;
		glm::mat4 projMat = myContextCamera.GetProjectMatrix(aspectRatio);
		glm::mat4 viewMat = myContextCamera.GetViewMatrix();
		//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 1.0f));
		glm::mat4 pv = projMat * viewMat;
		shader.use();
		for(auto& obj:sceneObjects)
		{
			std::shared_ptr<Box> spBox = std::dynamic_pointer_cast<Box>(obj);
			if (spBox)
			{
				shader.setMat4("pv", pv);
				obj->Draw(shader);
				continue;
			}
			
			std::shared_ptr<LineSegment> spLine= std::dynamic_pointer_cast<LineSegment>(obj);
			std::shared_ptr<Arc> spArc= std::dynamic_pointer_cast<Arc>(obj);
			if (spLine || spArc)
			{
				sampleShader.setMat4("u_View", viewMat);
				sampleShader.setMat4("u_Projection", projMat);
				obj->Draw(sampleShader);
			}
		}

		
		infiniteGrid.Draw(gridShader, viewMat, projMat);
		
		glfwSwapBuffers(window);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}