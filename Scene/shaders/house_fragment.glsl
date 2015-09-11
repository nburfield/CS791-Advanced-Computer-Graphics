#version 330

in vec2 teture0;
in vec3 vNormal;
in vec3 gEyeWorldPos;
in vec4 vEyeSpacePos;
uniform sampler2D gColorMap;


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

vec4 CalcLightInternal(BaseLight Light, vec3 LightDirection, vec3 WorldPos, vec3 Normal)
{
    vec4 AmbientColor = vec4(Light.Color * Light.AmbientIntensity, 1.0);
    float DiffuseFactor = dot(Normal, -LightDirection);

    vec4 DiffuseColor  = vec4(0, 0, 0, 0);
    vec4 SpecularColor = vec4(0, 0, 0, 0);

    if (DiffuseFactor > 0.0) {
        DiffuseColor = vec4(Light.Color * Light.DiffuseIntensity * DiffuseFactor, 1.0);

        vec3 VertexToEye = normalize(gEyeWorldPos - WorldPos);
        vec3 LightReflect = normalize(reflect(LightDirection, Normal));
        float SpecularFactor = dot(VertexToEye, LightReflect);        
        if (SpecularFactor > 0.0) {
            SpecularFactor = pow(SpecularFactor, 0.10);
            SpecularColor = vec4(Light.Color * 0.10 * SpecularFactor, 1.0);
        }
    }

    return (AmbientColor + DiffuseColor + SpecularColor);
}

vec4 CalcPointLight(vec3 WorldPos, vec3 Normal)
{
    vec3 LightDirection = WorldPos - sunLight.Position;
    float Distance = length(LightDirection);
    LightDirection = normalize(LightDirection);

    vec4 Color = CalcLightInternal(sunLight.Base, LightDirection, WorldPos, Normal);

    float Att =  sunLight.Atten.Constant +
                         sunLight.Atten.Linear * Distance +
                         sunLight.Atten.Exp * Distance * Distance;

    Att = max(1.0, Att);

    return Color / Att;
}


out vec4 fragColor;

void main(void)
{
  vec3 WorldPos = texture(gColorMap, teture0).xyz;

  vec4 vPointLightColor = CalcPointLight(vEyeSpacePos.xyz, vNormal);
  //vec4 vPointLightColor = GetPointLightColor(sunLight, vNormal, vEyeSpacePos.xyz); 
  fragColor = texture(gColorMap, teture0) * vPointLightColor;
}
