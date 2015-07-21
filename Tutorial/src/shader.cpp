//Include Shader
#include <shader.h>
#include <iostream>

//Constructor
Shader::Shader()
   {
    //Set init values for buffer and size
    buffer = NULL;
    bufferSize = 0;
   }

//Copy Constructor
Shader::Shader(Shader &src)
   {
    //Check if the src has nothing
    if(src.bufferSize == 0)
       {
        buffer = NULL;
        bufferSize = 0;
       }
    else
       {
        //Set size to the src
        bufferSize = src.bufferSize;
    
        //Allocate buffer
        buffer = new char[bufferSize + 1];
    
        //Assign contents of  buffer
        for(int index = 0; index < bufferSize+1; index++)
           {
            buffer[index] = src.buffer[index];
           }
       }
   }

//Destructior
Shader::~Shader()
   {
    //Delete buffer
    delete buffer;
    buffer = NULL;
    bufferSize = 0;
   }

bool Shader::Initilize(const char * fileLoc, GLenum shaderType, GLuint &program)
{
  // Setup shader
  _shaderType = shaderType;
  shader = glCreateShader(shaderType);

  //Check if correct file
  if(!readFile(fileLoc))
  {
    std::cerr << "FAILED TO FIND " << shaderType << " SHADER FILE: " << fileLoc << std::endl;
    return false;
  }

  //compile the shader
  GLint shader_status;
  glShaderSource(shader, 1, (const GLchar**) &buffer, NULL);
  glCompileShader(shader);
  //check the compile status
  glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_status);
  if(!shader_status)
  {
    GLchar InfoLog[1024];
    glGetShaderInfoLog(shader, 1024, NULL, InfoLog);
    std::cerr << "FAILED TO COMPILE SHADER!" << std::endl << InfoLog << std::endl;
    return false;
  }

  //Now we link the shader into a program
  glAttachShader(program, shader);
  return true;
}

//Read in the file to fill buffer
bool Shader::readFile(const char *fileLoc)
{
  //Open the file
  FILE *source = fopen(fileLoc, "r");

  //check if file open fails
  if(source == NULL)
  {
    return false;
  }

  //Get the file size
  fseek(source, 0L, SEEK_END);
  bufferSize = ftell(source);

  //allocate the buffer
  fseek( source, 0L, SEEK_SET );
  buffer = new char[ bufferSize+1 ];

  //read into buffer
  fread( buffer, 1, bufferSize, source);

  //Add Null to the end
  buffer[bufferSize] = '\0';

  //Return success
  fclose(source);
  return true;
}   
