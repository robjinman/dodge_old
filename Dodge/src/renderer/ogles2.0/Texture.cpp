/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <cstring>
#include <renderer/ogles2.0/Texture.hpp>
#include <renderer/ogles2.0/Renderer.hpp>
#include <PNG_CHECK.hpp>


using namespace std;


namespace Dodge {


//===========================================
// Texture::Texture
//===========================================
Texture::Texture(const char* file) {
   pngInit();
   constructTexture(file);
}

//===========================================
// Texture::Texture
//===========================================
Texture::Texture(const XmlNode data) {
   string msg("Error parsing XML for instance of class Texture");

   XML_NODE_CHECK(msg, data, Texture);

   pngInit();

   XmlAttribute attr = data.firstAttribute();
   XML_ATTR_CHECK(msg, attr, path);

   constructTexture(attr.value().data());
}

//===========================================
// Texture::constructTexture
//===========================================
void Texture::constructTexture(const char* file) {
   PNG_CHECK(png_open_file(&m_png, file));

   m_data = new byte_t[m_png.bpp * m_png.width * m_png.height];
   PNG_CHECK(png_get_data(&m_png, m_data));

   m_width = m_png.width;
   m_height = m_png.height;

   PNG_CHECK(png_close_file(&m_png));

   Renderer renderer;
   m_handle = renderer.newTexture(m_data, m_png.width, m_png.height);
}

//===========================================
// Texture::clone
//===========================================
Texture* Texture::clone() const {
   throw Exception("Cannot clone Texture objects; Feature not implemented", __FILE__, __LINE__);
}

//===========================================
// Texture::pngInit
//===========================================
void Texture::pngInit() const {
   static bool init = false;

   if (!init) {
      PNG_CHECK(png_init(0, 0));
      init = true;
   }
}


}
