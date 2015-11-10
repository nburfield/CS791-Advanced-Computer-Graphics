#version 330

in vec2 teture0;
in vec3 vNormal;
in vec3 gEyeWorldPos;
in vec4 vEyeSpacePos;
uniform sampler2D gColorMap;

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

  /*
  vec3 WorldPos = texture(gColorMap, teture0).xyz;
  vec4 vPointLightColor = CalcPointLight(vEyeSpacePos.xyz, vNormal);
  vec4 vPointLightColor = GetPointLightColor(sunLight, vNormal, vEyeSpacePos.xyz); 
  fragColor = texture(gColorMap, teture0) * vPointLightColor;
  */
}
