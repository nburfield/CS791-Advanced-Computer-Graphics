#version 330

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 texture;
layout (location = 2) in vec3 normal;

out vec2 teture0;
out vec3 vNormal;
out vec3 gEyeWorldPos;

uniform mat4 modelMatrix;
uniform mat4 mvpMatrix;
uniform float waveTime;
uniform float waveWidth;
uniform float waveHeight;

void main(void)
{
  vec4 v = vec4(v_position, 1.0);
  v.z = cos(waveWidth * v.x + waveTime) * (waveHeight * (1-texture.x));

  vec3 n = normal;
  //n.z = cos(waveWidth * n.x + waveTime) * (waveHeight * (1-texture.x));
  
  //v.z = sin(waveWidth * v.x + waveTime) * cos(waveWidth * v.y + waveTime) * (waveHeight * (1-texture.x));

  gl_Position = mvpMatrix * v;

  gEyeWorldPos = (modelMatrix * v).xyz;
  vNormal = n;
  teture0 = texture;
}
