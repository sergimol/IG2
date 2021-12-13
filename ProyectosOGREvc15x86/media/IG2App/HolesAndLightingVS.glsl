#version 330 core

in vec4 vertex;
in vec3 normal;
in vec2 uv0;

uniform mat4 modelViewProjMat;
uniform mat4 normalMat;  

out vec2 vUv0;
out vec3 vCamNormal;

void main(){
	vUv0 = uv0;
    vCamNormal = normalize(mat3(normalMat) * normal);
	gl_Position = modelViewProjMat * vertex;
}