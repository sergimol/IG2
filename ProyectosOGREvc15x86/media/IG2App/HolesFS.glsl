#version 330 core

uniform sampler2D texturaS;
uniform sampler2D texturaM; 
uniform sampler2D texturaC;

in vec2 vUv0; // out del vertex shader
out vec4 fFragColor; // out del fragment shader

void main() {
	vec3 colorStones = vec3(texture(texturaS, vUv0)); 
    vec3 colorMetal = vec3(texture(texturaM, vUv0)); 
    vec3 colorCorrosion = vec3(texture(texturaC, vUv0)); 
	
	if(colorCorrosion.r <= 0.6){
		if(gl_FrontFacing)
            fFragColor = vec4(colorMetal, 1.0);
        else
            fFragColor = vec4(colorStones, 1.0);
	}
	else
		discard;
}