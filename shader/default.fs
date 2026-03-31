#version 330 core
in vec3 vBarycentric;
out vec4 FragColor;

uniform vec3 u_MeshColor;
uniform vec3 u_EdgeColor;
uniform float u_EdgeWidth;

float edgeFactor(){
	vec3 d = fwidth(vBarycentric);
	vec3 a3 = smoothstep(vec3(0.0f), d*u_EdgeWidth, vBarycentric);
	return min(min(a3.x, a3.y), a3.z);
}
void main() {
	float e = edgeFactor();
	//混合面颜色和线颜色
	vec3 finalColor = mix(u_EdgeColor, u_MeshColor, e);
	FragColor = vec4(finalColor, 1.0f);

}
