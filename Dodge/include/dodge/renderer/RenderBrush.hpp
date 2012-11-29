#ifndef __RENDER_BRUSH_HPP__
#define __RENDER_BRUSH_HPP__


#include <mutex>
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
      inline Colour getFillColour() const;
      inline Colour getLineColour() const;
      inline int getLineWidth() const;

   private:
      Colour m_fillColour;
      Colour m_lineColour;
      int m_lineWidth;

      mutable std::mutex m_mutex;
};

//===========================================
// RenderBrush::setFillColour
//===========================================
inline void RenderBrush::setFillColour(const Colour& fCol) {
   m_mutex.lock();
   m_fillColour = fCol;
   m_mutex.unlock();
}

//===========================================
// RenderBrush::setLineColour
//===========================================
inline void RenderBrush::setLineColour(const Colour& lCol) {
   m_mutex.lock();
   m_lineColour = lCol;
   m_mutex.unlock();
}

//===========================================
// RenderBrush::setLineWidth
//===========================================
inline void RenderBrush::setLineWidth(int w) {
   m_mutex.lock();
   m_lineWidth = w;
   m_mutex.unlock();
}

//===========================================
// RenderBrush::getFillColour
//===========================================
inline Colour RenderBrush::getFillColour() const {
   m_mutex.lock();
   Colour cpy = m_fillColour;
   m_mutex.unlock();

   return cpy;
}

//===========================================
// RenderBrush::getLineColour
//===========================================
inline Colour RenderBrush::getLineColour() const {
   m_mutex.lock();
   Colour cpy = m_lineColour;
   m_mutex.unlock();

   return cpy;
}

//===========================================
// RenderBrush::getLineWidth
//===========================================
inline int RenderBrush::getLineWidth() const {
   m_mutex.lock();
   int cpy = m_lineWidth;
   m_mutex.unlock();

   return cpy;
}


}


#endif
