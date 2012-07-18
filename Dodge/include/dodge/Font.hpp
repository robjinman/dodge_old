/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2012
 */

#ifndef __FONT_HPP__
#define __FONT_HPP__


#include "definitions.hpp"
#include "Texture.hpp"


namespace Dodge {


class Font {
   public:
      Font(pTexture_t texture, const Vec2f& position, const Vec2f& dimensions, int charW, int charH)
         : m_texture(texture), m_pos(position), m_dim(dimensions), m_charW(charW), m_charH(charH) {}

      inline const pTexture_t getTexture() const;
      inline int getCharWidth() const;
      inline int getCharHeight() const;
      inline const Vec2f& getTextureSectionPosition() const;
      inline const Vec2f& getTextureSectionDimensions() const;

   private:
      pTexture_t m_texture;
      Vec2f m_pos;
      Vec2f m_dim;
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
// Font::getTextureSectionPosition
//===========================================
inline const Vec2f& Font::getTextureSectionPosition() const {
   return m_pos;
}

//===========================================
// Font::getTextureSectionDimensions
//===========================================
inline const Vec2f& Font::getTextureSectionDimensions() const {
   return m_dim;
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
