#version 330 core
in vec3 nearPoint;
in vec3 farPoint;
out vec4 FragColor;

uniform mat4 view;
uniform mat4 projection;

vec4 grid(vec3 fragPos3D, float scale){
	vec2 coord = fragPos3D.xz*scale;
	vec2 derivative = fwidth(coord);

	vec2 grid = abs(fract(coord - 0.5) -0.5)/derivative;
	float line = min(grid.x, grid.y);
	float colorDiff = 1.0 - min(line, 1.0);

	vec4 color = vec4(0.3,0.3,0.3, colorDiff);

	float minimumz = fwidth(fragPos3D.z);
	float minimumx = fwidth(fragPos3D.x);
	if(fragPos3D.x > -0.1*minimumx && fragPos3D.x < 0.1*minimumx)
		color = vec4(0.0, 0.0, 1.0, colorDiff);
	if(fragPos3D.z > -0.1*minimumz && fragPos3D.z < 0.1*minimumz)
		color = vec4(1.0, 0.0, 0.0, colorDiff);

	return color;
}

void main(){
	float t = -nearPoint.y/(farPoint.y - nearPoint.y);
	if(t < 0) discard;

	vec3 fragPos3D = nearPoint + t*(farPoint - nearPoint);

	float linearDepth = (2.0*0.1)/(100.0 + 0.1 - gl_FragCoord.z*(100.0 - 0.1));
	float fading = max(0.0, (0.5 - linearDepth));

	FragColor = grid(fragPos3D, 1.0);
	FragColor.a *= fading;
}