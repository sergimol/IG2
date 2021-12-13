#version 330 core

uniform sampler2D texturaS;
uniform sampler2D texturaM; 
uniform sampler2D texturaC;

uniform vec3 lightDiffuse;
uniform vec4 lightPosition;

uniform vec3 ocreSiena;
uniform vec3 azulCeruleo;

in vec2 vUv0; // out del vertex shader
in vec3 vCamNormal;
out vec4 fFragColor; // out del fragment shader

void main() {    
    vec3 colorCorrosion = vec3(texture(texturaC, vUv0)); 
	
	if(colorCorrosion.r <= 0.6){
		if(gl_FrontFacing){
			vec3 colorMetal = vec3(texture(texturaM, vUv0)) * ocreSiena; 
			
            vec3 lightDir = normalize(vec3(lightPosition));
			float diff = max(dot(vCamNormal, lightDir), 0.0); // positive normal
			vec3 diffuse = vec3(lightDiffuse) * diff;

			fFragColor = vec4(colorMetal * diffuse, 1.0);
		}
        else {
            vec3 colorStones = vec3(texture(texturaS, vUv0)); // acceso a téxel
			colorStones *= azulCeruleo;
        
			vec3 lightDir = normalize(vec3(lightPosition));
			float diff = max(dot(-vCamNormal, lightDir), 0.0); // negative normal
			vec3 diffuse = vec3(lightDiffuse) * diff;

			fFragColor = vec4(colorStones * diffuse, 1.0);
		}
	}
	else
		discard;
}