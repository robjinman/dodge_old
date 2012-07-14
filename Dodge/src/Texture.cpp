/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include "Texture.hpp"
#include "Renderer.hpp"
#include "PNG_CHECK.hpp"


namespace Dodge {


//===========================================
// Texture::Texture
//===========================================
Texture::Texture(const char* file) : ITexture(file) {
   static bool once = false;
   if (!once) {
      PNG_CHECK(png_init(0, 0));
      once = true;
   }

   PNG_CHECK(png_open_file(&m_png, file));

   m_data = new unsigned char[m_png.bpp * m_png.width * m_png.height];

   PNG_CHECK(png_get_data(&m_png, m_data));

   m_width = m_png.width;
   m_height = m_png.height;

   Renderer renderer;
   m_id = renderer.newTexture(m_data, m_png.width, m_png.height);
}


}
