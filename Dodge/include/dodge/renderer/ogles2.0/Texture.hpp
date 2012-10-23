/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __TEXTURE_HPP__
#define __TEXTURE_HPP__


#include <GLES2/gl2.h>
#include "../../definitions.hpp"
#include "../../pnglite/pnglite.h"


namespace Dodge {


// PNG/OGLES2 implementation
class Texture {
   public:
      Texture(const char* file);

      inline GLint getWidth() const;
      inline GLint getHeight() const;
      inline const byte_t* getData() const;
      inline const GLuint& getHandle() const;

   private:
      png_t m_png;
      byte_t* m_data;
      GLint m_width;
      GLint m_height;
      GLuint m_handle;
};

typedef sharedPtr_t<Texture> pTexture_t;

//===========================================
// Texture::getWidth
//===========================================
inline GLint Texture::getWidth() const {
   return m_width;
}

//===========================================
// Texture::getHeight
//===========================================
inline GLint Texture::getHeight() const {
   return m_height;
}

//===========================================
// Texture::getData
//===========================================
inline const byte_t* Texture::getData() const {
   return m_data;
}

//===========================================
// Texture::getHandle
//===========================================
inline const GLuint& Texture::getHandle() const {
   return m_handle;
}


}


#endif
