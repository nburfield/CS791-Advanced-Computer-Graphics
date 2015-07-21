#version 330

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 texcoord;
layout (location = 2) in vec3 normal;

uniform mat4 mvpMatrix;
uniform mat4 gWorld;

out vec3 Normal0;
out vec2 TexCoord0;
out vec3 WorldPos0;

void main(void)
{
  gl_Position = mvpMatrix * vec4(v_position, 1.0);

  TexCoord0 = texcoord;
  Normal0 = (gWorld * vec4(normal, 0.0)).xyz;
  WorldPos0 = (gWorld * vec4(v_position, 1.0)).xyz;
  //color = vec4(normal, 1.0);
  //UV = texcoord;
}
