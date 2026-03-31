#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aBary;
uniform mat4 pv;
uniform mat4 u_Model;
out vec3 vBarycentric;
void main(){
    vBarycentric = aBary;
    gl_Position=pv*u_Model*vec4(aPos, 1.0);
}