/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __FONT_HPP__
#define __FONT_HPP__


#include "../definitions.hpp"
#include "Texture.hpp"
#include "../Asset.hpp"
#include "../Range.hpp"


namespace Dodge {


class XmlNode;

class Font : virtual public Asset {
   public:
      Font(const XmlNode data);
      Font(pTexture_t texture, float32_t texX, float32_t texY, float32_t texW, float32_t texH, int charW, int charH);
      Font(const Font& copy);

      virtual size_t getSize() const;
      virtual Asset* clone() const;

      inline const pTexture_t getTexture() const;
      inline int getCharWidth() const;
      inline int getCharHeight() const;
      inline const Range& getTextureSection() const;

   private:
      pTexture_t m_texture;
      Range m_texSection;
      int m_charW;
      int m_charH;
};

typedef boost::shared_ptr<Font> pFont_t;

//===========================================
// Font::getTexture
//===========================================
inline const pTexture_t Font::getTexture() const {
   return m_texture;
}

//===========================================
// Font::getTextureSection
//===========================================
inline const Range& Font::getTextureSection() const {
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
