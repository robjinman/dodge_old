#include <renderer/Font.hpp>
#include <StringId.hpp>
#include <xml/xml.hpp>


namespace Dodge {


//===========================================
// Font::Font
//===========================================
Font::Font(const XmlNode data)
   : Asset(internString("Font")) {

   XML_NODE_CHECK(data, Font);

   // TODO
}

//===========================================
// Font::Font
//===========================================
Font::Font(const Font& copy)
   : Asset(internString("Font")) {

   m_texture = copy.m_texture;
   m_texSection = copy.m_texSection;
   m_charW = copy.m_charW;
   m_charH = copy.m_charH;
}

//===========================================
// Font::Font
//===========================================
Font::Font(pTexture_t texture, float32_t texX, float32_t texY, float32_t texW, float32_t texH, int charW, int charH)
   : Asset(internString("Font")),
     m_texture(texture),
     m_texSection(texX, texY, texW, texH),
     m_charW(charW), m_charH(charH) {}

//===========================================
// Font::clone
//===========================================
Font* Font::clone() const {
   return new Font(*this);
}

//===========================================
// Font::getSize
//===========================================
size_t Font::getSize() const {
   return sizeof(Font);
}


}
