#ifndef __RENDER_BRUSH_HPP__
#define __RENDER_BRUSH_HPP__


#include "../definitions.hpp"
#include "../Asset.hpp"
#include "Colour.hpp"


namespace Dodge {


class XmlNode;

class RenderBrush : virtual public Asset {
   public:
      RenderBrush()
         : m_fillColour(1.f, 1.f, 1.f, 1.f), m_lineColour(0.f, 0.f, 0.f, 1.f), m_lineWidth(1) {}

      RenderBrush(const RenderBrush& copy);
      RenderBrush(const Colour& fCol, const Colour& lCol, int lineW);
      RenderBrush(const XmlNode data);

      virtual RenderBrush* clone() const;

      inline void setFillColour(const Colour& fCol);
      inline void setLineColour(const Colour& lCol);
      inline void setLineWidth(int w);
      inline const Colour& getFillColour() const;
      inline const Colour& getLineColour() const;
      inline int getLineWidth() const;

   private:
      Colour m_fillColour;
      Colour m_lineColour;
      int m_lineWidth;
};

//===========================================
// RenderBrush::setFillColour
//===========================================
inline void RenderBrush::setFillColour(const Colour& fCol) {
   m_fillColour = fCol;
}

//===========================================
// RenderBrush::setLineColour
//===========================================
inline void RenderBrush::setLineColour(const Colour& lCol) {
   m_lineColour = lCol;
}

//===========================================
// RenderBrush::setLineWidth
//===========================================
inline void RenderBrush::setLineWidth(int w) {
   m_lineWidth = w;
}

//===========================================
// RenderBrush::getFillColour
//===========================================
inline const Colour& RenderBrush::getFillColour() const {
   return m_fillColour;
}

//===========================================
// RenderBrush::getLineColour
//===========================================
inline const Colour& RenderBrush::getLineColour() const {
   return m_lineColour;
}

//===========================================
// RenderBrush::getLineWidth
//===========================================
inline int RenderBrush::getLineWidth() const {
   return m_lineWidth;
}


}


#endif
