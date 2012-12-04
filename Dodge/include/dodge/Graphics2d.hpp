/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __GRAPHICS_2D_HPP__
#define __GRAPHICS_2D_HPP__


#include "renderer/Renderer.hpp"
#include "renderer/Camera.hpp"
#include "renderer/RenderBrush.hpp"
#include "math/Vec2i.hpp"
#include "math/Vec2f.hpp"
#include "math/primitives/Primitive.hpp"
#include "Exception.hpp"


namespace Dodge {


class Range;
class Font;
class Texture;

class Graphics2d {
   public:
      void init(int w, int h);

      inline void setFillColour(const Colour& colour) const;
      inline void setLineColour(const Colour& colour) const;
      inline void setLineWidth(int width) const;

      void drawImage(const Texture& image, const Range& src, const Range& dest, int z,
         float32_t angle = 0.f, const Vec2f& pivot = Vec2f(0.f, 0.f)) const;

      void drawText(const Font& font, const Vec2f& size, const std::string& text, float32_t x,
         float32_t y, int z, float32_t angle = 0.f, const Vec2f& pivot = Vec2f(0.f, 0.f)) const;

      void clear(const Colour& col = Colour(0.f, 0.f, 0.f, 1.f)) const;

      inline void setCamera(pCamera_t camera);
      inline pCamera_t getCamera() const;

   private:
      static bool m_init;
      static Renderer m_renderer;
      static boost::shared_ptr<RenderBrush> m_renderBrush;
      static pCamera_t m_camera;
};

//===========================================
// Graphics2d::setFillColour
//===========================================
inline void Graphics2d::setFillColour(const Colour& colour) const {
   if (!m_init)
      throw Exception("Error setting fill colour; Graphics2d not initialised", __FILE__, __LINE__);

   m_renderBrush->setFillColour(colour);
}

//===========================================
// Graphics2d::setLineColour
//===========================================
inline void Graphics2d::setLineColour(const Colour& colour) const {
   if (!m_init)
      throw Exception("Error setting line colour; Graphics2d not initialised", __FILE__, __LINE__);

   m_renderBrush->setLineColour(colour);
}

//===========================================
// Graphics2d::setLineWidth
//===========================================
inline void Graphics2d::setLineWidth(Renderer::int_t width) const {
   if (!m_init)
      throw Exception("Error setting line width; Graphics2d not initialised", __FILE__, __LINE__);

   m_renderBrush->setLineWidth(width);
}

//===========================================
// Graphics2d::setCamera
//===========================================
inline void Graphics2d::setCamera(pCamera_t camera) {
   m_renderer.attachCamera(camera);
}

//===========================================
// Graphics2d::getCamera
//===========================================
inline pCamera_t Graphics2d::getCamera() const {
   return m_camera;
}


}


#endif /*!__GRAPHICS_2D_HPP__*/
