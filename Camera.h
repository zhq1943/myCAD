#pragma once
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

class Camera {
public:
	Camera(){};
    ~Camera() {};

    float Redius = 10.0f;
    float Theta = glm::radians(45.0f);
    float Phi = glm::radians(45.0f);
    glm::vec3 Target = glm::vec3(0.0f, 0.0f, 0.0f);

    glm::mat4 GetViewMatrix(){
        //球坐标转换为笛卡尔坐标
        float x = Redius*sin(Phi)*cos(Theta);
        float y = Redius*cos(Phi);
        float z = Redius*sin(Phi)*sin(Theta);

        glm::vec3 position = glm::vec3(x, y, z) + Target;
        return glm::lookAt(position, Target, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    glm::mat4 GetProjectMatrix() {
       return glm::perspective(glm::radians(45.0f), 1200.0f / 720.f, 0.1f, 100.0f);
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

    //处理缩放逻辑
    void Zoom(float yoffset){
        float sensitivity = 0.1f;
        Redius -= yoffset*sensitivity;
        if(Redius < 1.0f){
            Redius = 1.0f;
        }
        if(Redius > 50.0f){
            Redius = 50.0f;
        }
    }
   };
