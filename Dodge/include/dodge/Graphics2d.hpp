/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __GRAPHICS_2D_HPP__
#define __GRAPHICS_2D_HPP__


#include <cml/cml.h>
#include "Renderer.hpp"
#include "Texture.hpp"
#include "Vec2i.hpp"
#include "Vec2f.hpp"
#include "Colour.hpp"


namespace Dodge {


class Font;
class Poly;
class CompoundPoly;

class Graphics2d {
   public:
      void init(int w, int h);

      inline const Vec2f& getPixelSize() const;

      void setViewPos(float32_t x, float32_t y);

      void drawLine(const Vec2f& p1, const Vec2f& p2, int z, float32_t width, const Colour& col) const;

      void drawImage(const Texture& image, float32_t destX, float32_t destY, int destZ, float32_t destW, float32_t destH,
         float32_t srcX, float32_t srcY, float32_t srcW, float32_t srcH, float32_t a, const Colour& col = Colour(1.0, 1.0, 1.0, 1.0)) const;

      void drawImage(const Texture& image, float32_t destX, float32_t destY, int destZ, float32_t srcX, float32_t srcY, float32_t w,
         float32_t h, float32_t a, const Colour& col = Colour(1.0, 1.0, 1.0, 1.0)) const;

      void drawPlainAlphaQuad(float32_t x, float32_t y, int z, float32_t w, float32_t h, float32_t a, const Colour& col) const;

      void drawPlainAlphaPoly(const Poly& poly, float32_t x, float32_t y, int z, float32_t a, const Colour& col) const;

      void drawPlainAlphaCompoundPoly(const CompoundPoly& poly, float32_t x, float32_t y, int z, float32_t a, const Colour& col) const;

      void drawText(const Font& font, const std::string& text, float32_t x, float32_t y, int z, float32_t a,
         const Vec2f& pivot, const Vec2f& scale, const Colour& col) const;

      void drawText(const Font& font, const std::string& text, float32_t x, float32_t y, int z, const Vec2f& scale, const Colour& col) const;

      void clear() const;

      void clear(const Colour& col) const;

   private:
      static Renderer m_renderer;
      static cml::matrix44f_c m_orthographic;
      static Vec2f m_pixSz;
};

//===========================================
// Graphics2d::getPixelSize
//===========================================
inline const Vec2f& Graphics2d::getPixelSize() const {
   return m_pixSz;
}


}


#endif /*!__GRAPHICS_2D_HPP__*/
