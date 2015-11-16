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
  vNormal = (matrices.viewMatrix * vec4(normal, 0.0)).xyz;
  gEyeWorldPos = (matrices.viewMatrix * vec4(v_position, 1.0)).xyz; // matrices.viewMatrix[3].xyz;
}

/*
#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec3 Normal;

uniform mat4 gWVP;
uniform mat4 gWorld;


out vec2 TexCoord0;
out vec3 Normal0;
out vec3 WorldPos0;


void main()
{       
    gl_Position    = gWVP * vec4(Position, 1.0);
    TexCoord0      = TexCoord;                  
    Normal0        = (gWorld * vec4(Normal, 0.0)).xyz;   
    WorldPos0      = (gWorld * vec4(Position, 1.0)).xyz;
}
*/
