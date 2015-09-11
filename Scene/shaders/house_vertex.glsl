#version 330

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 texture;
layout (location = 2) in vec3 normal;

out vec2 teture0;
out vec3 vNormal;
out vec4 vEyeSpacePos;
out vec3 gEyeWorldPos;

uniform struct Matrices 
{ 
   mat4 projMatrix; 
   mat4 modelMatrix; 
   mat4 viewMatrix;                                                                            
   mat4 normalMatrix; 
} matrices;


void main(void)
{
  gl_Position = matrices.projMatrix * matrices.viewMatrix * matrices.modelMatrix * vec4(v_position, 1.0);
  vEyeSpacePos = matrices.viewMatrix * matrices.modelMatrix * vec4(v_position, 1.0);
  teture0 = texture;
  vNormal = normal;
  gEyeWorldPos = matrices.viewMatrix[3].xyz;
}
