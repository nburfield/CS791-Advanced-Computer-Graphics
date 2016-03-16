#version 330

in vec2 teture0;
in vec3 vNormal;
in vec3 gEyeWorldPos;
uniform sampler2D gColorMap;

//out vec4 fragColor;

layout (location = 0) out vec3 WorldPosOut;   
layout (location = 1) out vec3 DiffuseOut;     
layout (location = 2) out vec3 NormalOut;     
layout (location = 3) out vec3 TexCoordOut;

void main(void)
{
	WorldPosOut     = gEyeWorldPos;          
	DiffuseOut      = texture(gColorMap, teture0).xyz;  
	NormalOut       = normalize(vNormal);         
	TexCoordOut     = vec3(teture0, 0.0); 
}
