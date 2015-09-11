#version 330
//uniform vec2 pos;
layout (location = 0) in vec3 poses;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec3 Normal;

uniform struct Matrices 
{ 
   mat4 projMatrix; 
   mat4 modelMatrix; 
   mat4 viewMatrix;                                                                            
   mat4 normalMatrix; 
} matrices; 

uniform mat4 HeightmapScaleMatrix; 

smooth out vec2 vTexCoord; 
smooth out vec3 vNormal; 
smooth out vec3 vWorldPos; 
smooth out vec4 vEyeSpacePos; 

void main(void)
{
  mat4 mvp = matrices.projMatrix * matrices.viewMatrix * matrices.modelMatrix;

  gl_Position = mvp*vec4(poses.x, poses.y*30, poses.z,1);
   
  vEyeSpacePos = matrices.viewMatrix * matrices.modelMatrix * vec4(poses, 1.0); 
   
  vTexCoord = TexCoord; 
  vNormal = Normal; 
    
  vec4 vWorldPosLocal = matrices.modelMatrix * HeightmapScaleMatrix * vec4(poses, 1.0); 
  vWorldPos = vWorldPosLocal.xyz; 
}
