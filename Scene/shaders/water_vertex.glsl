#version 330

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 texture;

out vec2 teture0;

uniform mat4 mvpMatrix;
uniform float waveTime;
uniform float waveWidth;
uniform float waveHeight;

void main(void)
{
  vec4 v = vec4(v_position, 1.0);
  // v.z = cos(waveWidth * v.x + waveTime) * (waveHeight * (1-texture.x));
  
  v.z = sin(waveWidth * v.x + waveTime) * cos(waveWidth * v.y + waveTime) * (waveHeight * 2);

  gl_Position = mvpMatrix * v;

  teture0 = texture;
}
