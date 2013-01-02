/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __TEXTURE_HPP__
#define __TEXTURE_HPP__


#include <GLES2/gl2.h>
#include <boost/shared_ptr.hpp>
#include "../../xml/xml.hpp"
#include "../../definitions.hpp"
#include "../../../pnglite/pnglite.h"
#include "../../Asset.hpp"


namespace Dodge {


// PNG/OGLES2 implementation
class Texture : virtual public Asset {
   public:
      Texture(const XmlNode data);
      Texture(const char* file);

      virtual size_t getSize() const;
      virtual Texture* clone() const;

      inline GLint getWidth() const;
      inline GLint getHeight() const;
      inline const byte_t* getData() const;
      inline const GLuint& getHandle() const;

   private:
      void constructTexture(const char* file);
      void pngInit() const;

      png_t m_png;
      byte_t* m_data;
      GLint m_width;
      GLint m_height;
      GLuint m_handle;
};

typedef boost::shared_ptr<Texture> pTexture_t;

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
