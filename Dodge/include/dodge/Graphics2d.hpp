/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __GRAPHICS_2D_HPP__
#define __GRAPHICS_2D_HPP__


#include <cml/cml.h>
#include "Renderer.hpp"
#include "Vec2i.hpp"
#include "Vec2f.hpp"
#include "Colour.hpp"
#include "Primitive.hpp"


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
      static Renderer::Brush& m_renderBrush;
      static cml::matrix44f_c m_projectionMatrix;
      static Vec2f m_pixSz;
};

//===========================================
// Graphics2d::setFillColour
//===========================================
inline void Graphics2d::setFillColour(const Colour& colour) const {
   m_renderBrush.fillColour[0] = colour.r;
   m_renderBrush.fillColour[1] = colour.g;
   m_renderBrush.fillColour[2] = colour.b;
   m_renderBrush.fillColour[3] = colour.a;
}

//===========================================
// Graphics2d::setLineColour
//===========================================
inline void Graphics2d::setLineColour(const Colour& colour) const {
   m_renderBrush.lineColour[0] = colour.r;
   m_renderBrush.lineColour[1] = colour.g;
   m_renderBrush.lineColour[2] = colour.b;
   m_renderBrush.lineColour[3] = colour.a;
}

//===========================================
// Graphics2d::setLineWidth
//===========================================
inline void Graphics2d::setLineWidth(int width) const {
   m_renderBrush.lineWidth = width;
}

//===========================================
// Graphics2d::getPixelSize
//===========================================
inline const Vec2f& Graphics2d::getPixelSize() const {
   return m_pixSz;
}

//===========================================
// Graphics2d::drawPrimitive
//===========================================
inline void Graphics2d::drawPrimitive(const Primitive& primitive, float32_t x, float32_t y, int z,
   float32_t angle, const Vec2f& pivot) const {

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
