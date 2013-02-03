/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <cstring>
#include <renderer/ogl/Texture.hpp>
#include <renderer/ogl/Renderer.hpp>
#include <PNG_CHECK.hpp>
#include <StringId.hpp>


using namespace std;


namespace Dodge {


//===========================================
// Texture::Texture
//===========================================
Texture::Texture(const char* file)
   : Asset(internString("Texture")),
     m_renderer(Renderer::getInstance()) {

   pngInit();
   constructTexture(file);
}

//===========================================
// Texture::Texture
//===========================================
Texture::Texture(const XmlNode data)
   : Asset(internString("Texture")),
     m_renderer(Renderer::getInstance()) {

   try {
      XML_NODE_CHECK(data, Texture);

      pngInit();

      XmlAttribute attr = data.firstAttribute();
      XML_ATTR_CHECK(attr, path);
      string path = attr.getString();

      constructTexture(path.data());
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class Texture; ");
      throw;
   }
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

   m_renderer.loadTexture(m_data, m_png.width, m_png.height, &m_handle);
}

//===========================================
// Texture::getSize
//===========================================
size_t Texture::getSize() const {
   return sizeof(Texture) + m_width * m_height * 4;
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

//===========================================
// Texture::~Texture
//===========================================
Texture::~Texture() {
   m_renderer.unloadTexture(m_handle);
   delete[] m_data;
}


}
