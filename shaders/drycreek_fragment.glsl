#version 330 

smooth in vec2 vTexCoord; 
smooth in vec3 vNormal; 
smooth in vec3 vWorldPos; 
smooth in vec4 vEyeSpacePos; 

uniform sampler2D gSampler[5]; 
uniform sampler2D shadowMap; 

uniform vec4 vColor; 

uniform float fRenderHeight; 
uniform float fMaxTextureU; 
uniform float fMaxTextureV;

struct DirectionalLight
{
  vec3 vfColor;
  vec3 vDirection;
  float fAmbient;
};

vec4 GetDirectionalLightColor(DirectionalLight dirLight, vec3 vNormal)
{
  float fDiffuseIntensity = max(0.0, dot(vNormal, -dirLight.vDirection));
  float fMult = clamp(dirLight.fAmbient+fDiffuseIntensity, 0.0, 1.0);
  return vec4(dirLight.vfColor*fMult, 1.0);
}
uniform DirectionalLight sunLight;

out vec4 outputColor; 

void main() 
{ 
  vec3 vNormalized = normalize(vNormal);
  vec4 vTexColor = vec4(0.0);
  float fScale = vWorldPos.y/fRenderHeight;

  const float fRange1 = 0.15f; 
  const float fRange2 = 0.3f; 
  const float fRange3 = 0.65f; 
  const float fRange4 = 0.85f; 

/*
  if(fScale >= 0.0 && fScale <= fRange1)
  {
    vTexColor = texture(gSampler[0], vTexCoord); 
  }
  else if(fScale <= fRange2) 
  { 
    fScale -= fRange1; 
    fScale /= (fRange2-fRange1); 

    float fScale2 = fScale; 
    fScale = 1.0-fScale;  

    vTexColor += texture(gSampler[0], vTexCoord)*fScale; 
    vTexColor += texture(gSampler[1], vTexCoord)*fScale2; 
  } 
  else if(fScale <= fRange3)
  {
    vTexColor = texture(gSampler[1], vTexCoord); 
  }
  else if(fScale <= fRange4) 
  { 
    fScale -= fRange3; 
    fScale /= (fRange4-fRange3); 

    float fScale2 = fScale; 
    fScale = 1.0-fScale;  

    vTexColor += texture(gSampler[1], vTexCoord)*fScale; 
    vTexColor += texture(gSampler[2], vTexCoord)*fScale2;       
  } 
  else
  {
    vTexColor = texture(gSampler[2], vTexCoord); 
  }
*/

  if(fScale >= 0.0 && fScale <= fRange1)
  {
    vTexColor = vec4(1.0f, 0.0f, 0.0f, 0.0f); 
  }
  else if(fScale <= fRange2) 
  { 
    vTexColor = vec4(0.0f, 1.0f, 0.0f, 0.0f); 
  } 
  else if(fScale <= fRange3)
  {
    vTexColor = vec4(0.0f, 0.0f, 1.0f, 0.0f); 
  }
  else if(fScale <= fRange4) 
  { 
    vTexColor = vec4(1.0f, 1.0f, 0.0f, 0.0f);       
  } 
  else
  {
    vTexColor = vec4(1.0f, 0.0f, 1.0f, 0.0f); 
  }

  vec2 vPathCoord = vec2(vTexCoord.x/fMaxTextureU, vTexCoord.y/fMaxTextureV); 
  vec4 vPathIntensity = texture(gSampler[4], vPathCoord); // Black color means there is a path
  fScale = vPathIntensity.x; 

  vec4 vPathColor = texture(gSampler[3], vTexCoord);  
  vec4 vFinalTexColor = fScale * vTexColor + (1-fScale) * vPathColor; 

  vec4 vMixedColor = vFinalTexColor * vColor; 
  vec4 vDirLightColor = GetDirectionalLightColor(sunLight, vNormal); 


  outputColor = vMixedColor * (vDirLightColor); 
}        
