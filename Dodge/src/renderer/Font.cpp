#include <renderer/Font.hpp>
#include <StringId.hpp>
#include <xml/xml.hpp>
#include <AssetManager.hpp>


namespace Dodge {


//===========================================
// Font::Font
//===========================================
Font::Font(const XmlNode data)
   : Asset(internString("Font")) {

   try {
      AssetManager assetManager;

      XML_NODE_CHECK(data, Font);

      XmlNode node = data.firstChild();
      XML_NODE_CHECK(node, texture);

      XmlAttribute attr = node.firstAttribute();
      XML_ATTR_CHECK(attr, ptr);

      long id = attr.getLong();
      m_texture = boost::dynamic_pointer_cast<Texture>(assetManager.getAssetPointer(id));

      if (!m_texture)
         throw XmlException("Bad texture asset id", __FILE__, __LINE__);

      node = node.nextSibling();
      XML_NODE_CHECK(node, textureSection);
      m_texSection = Range(node.firstChild());

      node = node.nextSibling();
      XML_NODE_CHECK(node, charSize);
      Vec2f sz(node.firstChild());
      m_charW = static_cast<int>(sz.x);
      m_charH = static_cast<int>(sz.y);
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class Font; ");
      throw;
   }
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
Asset* Font::clone() const {
   return new Font(*this);
}

//===========================================
// Font::getSize
//===========================================
size_t Font::getSize() const {
   return sizeof(Font);
}


}
