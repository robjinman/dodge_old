/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __GRAPHICS_2D_HPP__
#define __GRAPHICS_2D_HPP__


#include <cml/cml.h>
#include "Renderer.hpp"
#include "math/Vec2i.hpp"
#include "math/Vec2f.hpp"
#include "Colour.hpp"
#include "math/primitives/Primitive.hpp"


namespace Dodge {


class Font;
class Texture;

class Graphics2d {
   public:
      void init(int w, int h);

      inline void setFillColour(const Colour& colour) const;
      inline void setLineColour(const Colour& colour) const;
      inline void setLineWidth(int width) const;

      inline const Vec2f& getPixelSize() const;
      void setViewPos(float32_t x, float32_t y);

      inline void drawPrimitive(const Primitive& primitive, float32_t x, float32_t y, int z,
         float32_t angle = 0.f, const Vec2f& pivot = Vec2f(0.f, 0.f)) const;

      inline void drawImage(const Texture& image, float32_t srcX, float32_t srcY, float32_t srcW, float32_t srcH,
         float32_t destX, float32_t destY, int z) const;
      void drawImage(const Texture& image, float32_t srcX, float32_t srcY, float32_t srcW, float32_t srcH,
         float32_t destX, float32_t destY, int z, float32_t angle, const Vec2f& pivot, const Vec2f& scale) const;

      inline void drawText(const Font& font, const std::string& text, float32_t x, float32_t y, int z) const;
      void drawText(const Font& font, const std::string& text, float32_t x, float32_t y, int z,
         float32_t angle, const Vec2f& pivot, const Vec2f& scale) const;

      void clear(const Colour& col = Colour(0.f, 0.f, 0.f, 1.f)) const;

   private:
      static bool m_init;
      static Renderer m_renderer;
      static boost::shared_ptr<Renderer::Brush> m_renderBrush;
      static cml::matrix44f_c m_projectionMatrix;
      static Vec2f m_pixSz;
};

//===========================================
// Graphics2d::setFillColour
//===========================================
inline void Graphics2d::setFillColour(const Colour& colour) const {
   if (!m_init)
      throw Exception("Error setting fill colour; Graphics2d not initialised", __FILE__, __LINE__);

   Renderer::colourElement_t fCol[] = {colour.r, colour.g, colour.b, colour.a};
   m_renderBrush->setFillColour(fCol);
}

//===========================================
// Graphics2d::setLineColour
//===========================================
inline void Graphics2d::setLineColour(const Colour& colour) const {
   if (!m_init)
      throw Exception("Error setting line colour; Graphics2d not initialised", __FILE__, __LINE__);

   Renderer::colourElement_t lCol[] = {colour.r, colour.g, colour.b, colour.a};
   m_renderBrush->setLineColour(lCol);
}

//===========================================
// Graphics2d::setLineWidth
//===========================================
inline void Graphics2d::setLineWidth(int width) const { // TODO: use Renderer::integer_t
   if (!m_init)
      throw Exception("Error setting line width; Graphics2d not initialised", __FILE__, __LINE__);

   m_renderBrush->setLineWidth(width);
}

//===========================================
// Graphics2d::getPixelSize
//===========================================
inline const Vec2f& Graphics2d::getPixelSize() const {
   if (!m_init)
      throw Exception("Error fetching pixel size; Graphics2d not initialised", __FILE__, __LINE__);

   return m_pixSz;
}

//===========================================
// Graphics2d::drawPrimitive
//===========================================
inline void Graphics2d::drawPrimitive(const Primitive& primitive, float32_t x, float32_t y, int z,
   float32_t angle, const Vec2f& pivot) const {

   if (!m_init)
      throw Exception("Error drawing primitive; Graphics2d not initialised", __FILE__, __LINE__);

   m_renderer.attachBrush(m_renderBrush);
   primitive.draw(x, y, z, angle, pivot);
}

//===========================================
// Graphics2d::drawImage
//===========================================
inline void Graphics2d::drawImage(const Texture& image, float32_t srcX, float32_t srcY, float32_t srcW, float32_t srcH,
   float32_t destX, float32_t destY, int z) const {

   drawImage(image, srcX, srcY, srcW, srcH, destX, destY, z, 0.f, Vec2f(0.f, 0.f), Vec2f(0.f, 0.f));
}

//===========================================
// Graphics2d::drawText
//===========================================
inline void Graphics2d::drawText(const Font& font, const std::string& text, float32_t x, float32_t y, int z) const {
   drawText(font, text, x, y, z, 0.f, Vec2f(0.f, 0.f), Vec2f(0.f, 0.f));
}


}


#endif /*!__GRAPHICS_2D_HPP__*/
