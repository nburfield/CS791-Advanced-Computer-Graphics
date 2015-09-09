#version 330

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 texture;

out vec2 teture0;

uniform mat4 mvpMatrix;

void main(void)
{
  gl_Position = mvpMatrix * vec4(v_position, 1.0);
  teture0 = texture;
}
