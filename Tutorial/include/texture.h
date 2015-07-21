/*
  The texture.h was refrenced From: http://ogldev.atspace.co.uk/www/tutorial22/tutorial22.html

  Additions have been made to it from: 
                                      Nolan Burfield
                                      Andy Garcia
                                      Hardy Thrower

  This class is strictly responsible for reading in the texture of an object using the
  image magick library. It saves the ingame data in a BLOB.
*/

#ifndef TEXTURE_H
#define	TEXTURE_H

#include <string>
#include <iostream>

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED

#if defined(__APPLE__) || defined(MACOSX)
  #include <OpenGL/gl3.h>
  #include <OpenGL/GLU.h>
#else //linux as default
  #include <GL/glew.h>
  #include <GL/glu.h>
#endif
#include <Magick++.h>

class Texture
   {
	public:
		Texture(GLenum TextureTarget, const std::string& FileName);

		bool Load();

		void Bind(GLenum TextureUnit);

	private:
		std::string m_fileName;
		GLenum m_textureTarget;
		GLuint m_textureObj;
		Magick::Image* m_pImage;
		Magick::Blob m_blob;
   };


#endif	/* TEXTURE_H */

