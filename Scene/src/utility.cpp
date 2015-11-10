#include <iostream>
#include <fstream>
#include <sys/time.h>

#include <utility.h>

bool ReadFile(const char* pFileName, string& outFile)
{
  ifstream f(pFileName);
  
  bool ret = false;
  
  if (f.is_open())
  {
    string line;
    while (getline(f, line))
    {
      outFile.append(line);
      outFile.append("\n");
    }
    
    f.close();
    
    ret = true;
  }
  else
  {
    OGLDEV_FILE_ERROR(pFileName);
  }
  
  return ret;
}


float RandomFloat()
{
    float Max = RAND_MAX;
    return ((float)RANDOM() / Max);
}


void OgldevError(const char* pFileName, uint line, const char* pError)
{
  fprintf(stderr, "%s:%d: %s\n", pFileName, line, pError);   
}


void OgldevFileError(const char* pFileName, uint line, const char* pFileError)
{
  fprintf(stderr, "%s:%d: unable to open file `%s`\n", pFileName, line, pFileError);    
}

