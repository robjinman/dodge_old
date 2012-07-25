/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2012
 */

#ifndef __FONT_HPP__
#define __FONT_HPP__


#include "definitions.hpp"
#include "Texture.hpp"
#include "Rectangle.hpp"


namespace Dodge {


class Font {
   public:
      Font(pTexture_t texture, float32_t texX, float32_t texY, float32_t texW, float32_t texH, int charW, int charH)
         : m_texture(texture), m_texSection(texX, texY, texW, texH), m_charW(charW), m_charH(charH) {}

      inline const pTexture_t getTexture() const;
      inline int getCharWidth() const;
      inline int getCharHeight() const;
      inline const Rectangle& getTextureSection() const;

   private:
      pTexture_t m_texture;
      Rectangle m_texSection;
      int m_charW;
      int m_charH;
};

typedef sharedPtr_t<Font> pFont_t;

//===========================================
// Font::getTexture
//===========================================
inline const pTexture_t Font::getTexture() const {
   return m_texture;
}

//===========================================
// Font::getTextureSection
//===========================================
inline const Rectangle& Font::getTextureSection() const {
   return m_texSection;
}

//===========================================
// Font::getCharWidth
//===========================================
inline int Font::getCharWidth() const {
   return m_charW;
}

//===========================================
// Font::getCharHeight
//===========================================
inline int Font::getCharHeight() const {
   return m_charH;
}


}


#endif /*!__FONT_HPP__*/
