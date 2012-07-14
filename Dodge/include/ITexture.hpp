/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __I_TEXTURE_HPP__
#define __I_TEXTURE_HPP__


#include "IRenderer.hpp"


namespace Dodge {


class ITexture { 
   public:
      ITexture(const char* file) {}

      inline int getWidth() const;
      inline int getHeight() const;
      inline const unsigned char* getData() const;
      inline const IRenderer::textureId_t& getId() const;

   protected:
      unsigned char* m_data;
      int m_width;
      int m_height;
      IRenderer::textureId_t m_id;
};

//===========================================
// ITexture::getWidth
//===========================================
inline int ITexture::getWidth() const {
   return m_width;
}

//===========================================
// ITexture::getHeight
//===========================================
inline int ITexture::getHeight() const {
   return m_height;
}

//===========================================
// ITexture::getData
//===========================================
inline const unsigned char* ITexture::getData() const {
   return m_data;
}

//===========================================
// ITexture::getId
//===========================================
inline const IRenderer::textureId_t& ITexture::getId() const {
   return m_id;
}


}


#endif
