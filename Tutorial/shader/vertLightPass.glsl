#version 330                                                                        

layout (location = 0) in vec3 Position; 

uniform mat4 mvpMatrix;

void main()
{          
    gl_Position = mvpMatrix * vec4(Position, 1.0);
}
