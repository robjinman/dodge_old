/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <cstring>
#include <renderer/ogles2.0/Texture.hpp>
#include <renderer/ogles2.0/Renderer.hpp>
#include <PNG_CHECK.hpp>


using namespace rapidxml;


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
Texture::Texture(const xml_node<>* data) {
   pngInit();

   xml_attribute<>* attr = data->first_attribute();
   if (!attr || strcmp(attr->name(), "path") != 0)
      throw Exception("Error constructing texture from XML data; Expected 'path' attribute", __FILE__, __LINE__);

   constructTexture(attr->value());
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
