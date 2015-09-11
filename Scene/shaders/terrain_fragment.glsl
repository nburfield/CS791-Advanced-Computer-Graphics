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

/*
vec4 GetDirectionalLight(BaseLight light, vec3 vNormal, vec3 vE, vec3 Position)
{
  vec3 N = normalize(vNormal);
  vec3 E = normalize(-vE);
  vec3 L = normalize(Position);
  vec3 H = normalize(L + E);
  vec4 ambient = vec4(light.Color, 1.0) * light.AmbientIntensity;


  float Kd = max( dot(N, L), 0.0 ); //factor
  vec4  diffuse = Kd* vec4(light.Color,1.0) * light.DiffuseIntensity; ;
  
  float Ks = pow( max(dot(N, H), 0.0), 0.1 );
  vec4  specular = Ks * vec4(light.Color,1.0);
  if( dot(L, N) < 0.0 )
  {
    specular = vec4(0.0, 0.0, 0.0, 1.0);
  }

  vec4 color = diffuse + specular + ambient;
  color.a = 0.0;
  return color;
}
*/

vec4 GetPointLightColor(PointLight pointLight, vec3 vNormal, vec3 vE)
{
  float fDiffuseIntensity = max(0.0, dot(vNormal, pointLight.Position));
  float fMult = clamp(pointLight.Base.AmbientIntensity + fDiffuseIntensity, 0.0, 1.0);
  return vec4(pointLight.Base.Color * fMult, 1.0);
}

/*
vec4 GetPointLightColor(PointLight pointLight, vec3 vNormal, vec3 vE)
{
  vec3 N = normalize(vNormal);
  vec3 E = normalize(-vE);
  vec3 L = normalize(pointLight.Position);
  vec3 H = normalize(L + E);
  vec4 ambient = vec4(pointLight.Base.Color, 1.0) * pointLight.Base.AmbientIntensity;
  float Distance = length(-L);

  vec4 Color = GetDirectionalLight(pointLight, vNormal, vE);
  float AttenuationNum =  pointLight.Atten.Constant + (pointLight.Atten.Linear * Distance) + (pointLight.Atten.Exp * Distance * Distance);
  return Color/AttenuationNum;
}
*/


uniform PointLight sunLight;

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

  vec2 vPathCoord = vec2(vTexCoord.x/fMaxTextureU, vTexCoord.y/fMaxTextureV); 
  vec4 vPathIntensity = texture(gSampler[4], vPathCoord); // Black color means there is a path
  fScale = vPathIntensity.x; 

  vec4 vPathColor = texture(gSampler[3], vTexCoord);  
  vec4 vFinalTexColor = fScale * vTexColor + (1-fScale) * vPathColor; 

  vec4 vMixedColor = vFinalTexColor * vColor; 
  vec4 vPointLightColor = GetPointLightColor(sunLight, vNormal, vEyeSpacePos.xyz); 


  outputColor = vMixedColor * (vPointLightColor); 
}        
