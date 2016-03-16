#version 330                                                                        
                                                                                    
uniform sampler2D gColorMap;                                                        

in vec4 pos;                                                                   
in vec2 TexCoord;                                                                   
in vec3 WorldPos;
in vec3 Normal;     

layout (location = 0) out vec3 WorldPosOut;
layout (location = 1) out vec3 DiffuseOut;
layout (location = 2) out vec3 NormalOut;
layout (location = 3) out vec3 TexCoordOut;
                                                                                    
void main()                                                                         
{
    vec3 tempf = (texture(gColorMap, TexCoord)).xyz;
    if (tempf.x >= 0.9 && tempf.y >= 0.9 && tempf.z >= 0.9)
    {           
      discard;                                                                    
    }
    else
    {
        WorldPosOut     = WorldPos;
        DiffuseOut      = (texture(gColorMap, TexCoord)).xyz;
        NormalOut       = normalize(Normal);
        TexCoordOut     = vec3(TexCoord, 0.0);
    }
}
