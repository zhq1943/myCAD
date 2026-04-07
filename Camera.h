#pragma once
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
enum class CameraMode{DIM_3D, DIM_2D};
class Camera {
public:
	Camera(){};
    ~Camera() {};

    CameraMode Mode = CameraMode::DIM_3D;

    float Radius = 10.0f;
    float Theta = glm::radians(45.0f);
    float Phi = glm::radians(45.0f);

    //2d status param
    float OrthoSize = 5.0f;

    glm::vec3 Target = glm::vec3(0.0f, 0.0f, 0.0f);

    glm::mat4 GetViewMatrix(){
        if (Mode == CameraMode::DIM_3D)
        {
            //球坐标转换为笛卡尔坐标
            float x = Radius * sin(Phi) * cos(Theta);
            float y = Radius * cos(Phi);
            float z = Radius * sin(Phi) * sin(Theta);

            glm::vec3 position = glm::vec3(x, y, z) + Target;
            return glm::lookAt(position, Target, glm::vec3(0.0f, 1.0f, 0.0f));
        }
        else {
            return glm::lookAt(Target+glm::vec3(0.0f, 10.0f, 0.0f),Target,glm::vec3(0.0f, 0.0f, -1.0f));
        }
    }

    glm::mat4 GetProjectMatrix(float aspectRatio) {
        if (Mode == CameraMode::DIM_3D)
        {
            return glm::perspective(glm::radians(45.0f), 1200.0f / 720.f, 0.1f, 100.0f);
        }
        else {
            float halfW = OrthoSize * aspectRatio;
            float halfH = OrthoSize;
            return glm::ortho(-halfW, halfW, -halfH, halfH, -100.0f, 100.0f);
        }
    }

    void Rotate(float xoffset, float yoffset){
        float sensitivity = 0.01f;
        Theta -= xoffset*sensitivity;
        Phi -= yoffset*sensitivity;

        //约束phi防止翻转 万向锁
        if(Phi < glm::radians(1.0f)){
            Phi = glm::radians(1.0f);
        }
        if(Phi > glm::radians(180.0f)){
            Phi = glm::radians(180.0f);
        }
    }

    void Move(float xoffset, float yoffset) {
        Target += glm::vec3(xoffset, yoffset, 0);
    }

    //处理缩放逻辑
    void Zoom(float yoffset){
        float sensitivity = 0.1f;
        Radius -= yoffset*sensitivity;
        if(Radius < 1.0f){
            Radius = 1.0f;
        }
        if(Radius > 50.0f){
            Radius = 50.0f;
        }
    }
   
    void ProcessScroll(float yoffset) {
        if (Mode == CameraMode::DIM_3D) {
            Zoom(yoffset);
        }
        else {
            OrthoSize -= yoffset * 0.5f;
            if (OrthoSize < 0.1f) OrthoSize = 0.1f;
        }
    }

    void Panning(float xoffset, float yoffset) {
        float panSpeed = OrthoSize * 0.002f;
        Target.x -= xoffset * panSpeed;
        Target.z += yoffset * panSpeed;
    }
};
