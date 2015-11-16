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

struct BaseLight
{
  vec3 Color;
  float AmbientIntensity;
  float DiffuseIntensity;
};

struct Attenuation
{
  float Constant;
  float Linear;
  float Exp;
};  

struct PointLight
{
  BaseLight Base;
  vec3 Position;
  Attenuation Atten;
};

uniform PointLight sunLight;


vec4 GetPointLightColor(PointLight pointLight, vec3 vNormal, vec3 vE)
{
  float fDiffuseIntensity = max(0.0, dot(vNormal, pointLight.Position));
  float fMult = clamp(pointLight.Base.AmbientIntensity + fDiffuseIntensity, 0.0, 1.0);
  return vec4(pointLight.Base.Color * fMult, 1.0);
}

// out vec4 outputColor;

layout (location = 0) out vec3 WorldPosOut;   
layout (location = 1) out vec3 DiffuseOut;     
layout (location = 2) out vec3 NormalOut;     
layout (location = 3) out vec3 TexCoordOut;    


void main() 
{ 
  vec3 vNormalized = normalize(vNormal);
  vec4 vTexColor = vec4(0.0);
  float fScale = vWorldPos.y/fRenderHeight;

  const float fRange1 = 0.15f; 
  const float fRange2 = 0.3f; 
  const float fRange3 = 0.35f; 
  const float fRange4 = 0.55f; 


  if(fScale >= 0.0 && fScale <= fRange1)
  {
    vTexColor = texture(gSampler[3], vTexCoord);
  }
  else if(fScale <= fRange2) 
  { 
    fScale -= fRange1; 
    fScale /= (fRange2-fRange1); 

    float fScale2 = fScale; 
    fScale = 1.0-fScale;  

    vTexColor += texture(gSampler[3], vTexCoord)*fScale; 
    vTexColor += texture(gSampler[0], vTexCoord)*fScale2; 
  } 
  else if(fScale <= fRange3)
  {
    vTexColor = texture(gSampler[0], vTexCoord); 
  }
  else if(fScale <= fRange4) 
  { 
    fScale -= fRange3; 
    fScale /= (fRange4-fRange3); 

    float fScale2 = fScale; 
    fScale = 1.0-fScale;  

    vTexColor += texture(gSampler[0], vTexCoord)*fScale; 
    vTexColor += texture(gSampler[2], vTexCoord)*fScale2;       
  } 
  else
  {
    vTexColor = texture(gSampler[2], vTexCoord); 
  }

  vec2 vPathCoord = vec2(vTexCoord.x/fMaxTextureU, vTexCoord.y/fMaxTextureV); 
  vec4 vPathIntensity = texture(gSampler[4], vPathCoord); // Black color means there is a path

  if(vPathIntensity.x < 0.1 && vPathIntensity.y > 0.9 && vPathIntensity.z < 0.1)
  {
    vTexColor = texture(gSampler[1], vTexCoord);
    fScale = vPathIntensity.y;
  }
  else if(vPathIntensity.x < 0.1 && vPathIntensity.y < 0.1 && vPathIntensity.z > 0.9)
  {
    vTexColor = texture(gSampler[2], vTexCoord);
    fScale = vPathIntensity.z;
  }
  else
  {
    fScale = vPathIntensity.x;
  }
  

  vec4 vPathColor = texture(gSampler[3], vTexCoord);  
  vec4 vFinalTexColor = fScale * vTexColor + (1-fScale) * vPathColor; 

  vec4 vMixedColor = vFinalTexColor * vColor; 
  vec4 vPointLightColor = GetPointLightColor(sunLight, vNormal, vEyeSpacePos.xyz); 


  // outputColor = vMixedColor * (vPointLightColor);

  WorldPosOut     = vWorldPos;
  DiffuseOut      = vFinalTexColor.xyz; //texture(gColorMap, TexCoord0).xyz;
  NormalOut       = vNormalized;
  TexCoordOut     = vec3(vTexCoord, 0.0);  
}
