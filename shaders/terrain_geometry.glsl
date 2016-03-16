#version 330

layout(points) in;                                                                  
layout(points) out;                                                                 
layout(max_vertices = 30) out;

out float one;
out vec2 TexCoord;

smooth in vec2 vTexCoord[]; 
smooth in vec3 vNormal[]; 
smooth in vec3 vWorldPos[]; 
smooth in vec4 vEyeSpacePos[];

smooth out vec2 TCoord; 
smooth out vec3 Normal; 
smooth out vec3 WorldPos; 
smooth out vec4 EyeSpacePos;   

void main()                                                                         
{
    TCoord = vTexCoord[0];
    Normal = vNormal[0];
    WorldPos = vWorldPos[0];
    EyeSpacePos = vEyeSpacePos[0];

    float gTime = 1.0;
    float gBillboardSize = 5.0;
    float gTimeVal = gTime / (2 * 3.14);
    vec3 Pos = gl_in[0].gl_Position.xyz;                                            
    vec3 toCamera = normalize(vWorldPos[0] - Pos);                                    
    vec3 up = vec3(0.0, 1.0, 0.0);                                                  
    vec3 right = cross(toCamera, up) * gBillboardSize;
    vec3 original = Pos;
    mat4 gVP = mat4(1.0f);
    float gDT = 1.0;
                                                                                    
    Pos -= right;
    gl_Position = gVP * vec4(Pos, 1.0);                                             
    TexCoord = vec2(0.0, 0.0);                                                      
    EmitVertex();                                                                   
                                                                                    
    Pos.y += gBillboardSize;
    Pos.z += abs(cos(gTimeVal - gDT + sin(original.x) + sin(original.z)));
    Pos.x += abs(sin(gTimeVal + cos(original.z)));
    gl_Position = gVP * vec4(Pos, 1.0);                                             
    TexCoord = vec2(0.0, 1.0);                                                      
    EmitVertex();

    Pos.y -= gBillboardSize;
    Pos.z -= abs(5 * cos(gTimeVal + sin(original.x) + sin(original.z)));
    Pos.x -= abs(2 * sin(gTimeVal + cos(original.z)));
    Pos += right;                                                                   
    gl_Position = gVP * vec4(Pos, 1.0);                                             
    TexCoord = vec2(1.0, 0.0);                                                      
    EmitVertex();                                                                   
                                                                                    
    Pos.y += gBillboardSize;
    Pos.z += abs(5 * cos(gTimeVal + sin(original.x) + sin(original.z)));
    Pos.x += abs(2 * sin(gTimeVal + cos(original.z))); 
    gl_Position = gVP * vec4(Pos, 1.0);                                             
    TexCoord = vec2(1.0, 1.0);                                                      
    EmitVertex();                                                                   
                                                                                    
    EndPrimitive();                                                                 
}                                                                                   
