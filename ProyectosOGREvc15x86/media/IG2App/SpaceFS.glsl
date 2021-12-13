#version 330 core
uniform sampler2D texturaL; 
uniform sampler2D texturaM; 

in vec2 vUv0; // out del vertex shader
in vec2 vUv1;
out vec4 fFragColor; // out del fragment shader

void main() {
	vec3 colorL = vec3(texture(texturaL, vUv0)); // acceso a téxel
	vec3 colorM = vec3(texture(texturaM, vUv1)); // configuración!

	fFragColor = vec4(colorL * colorM, 1.0); // out
}