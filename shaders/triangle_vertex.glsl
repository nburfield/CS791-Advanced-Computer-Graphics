#version 330

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 texture;

out vec2 teture0;

uniform mat4 mvpMatrix;

void main(void)
{
  vec4 v = vec4(v_position, 1.0);
  gl_Position = mvpMatrix * v;

  teture0 = texture;
}
