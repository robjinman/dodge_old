#ifndef __QUAD_HPP__
#define __QUAD_HPP__


#include "Polygon.hpp"


namespace Dodge {


class Quad : public Polygon {
   public:
      Quad();
      explicit Quad(const XmlNode data);
      explicit Quad(const Vec2f& dimensions);
      Quad(const Vec2f& A, const Vec2f& B, const Vec2f& C, const Vec2f& D);

      inline void setVertex(int idx, float32_t x, float32_t y);
      inline void setVertex(int idx, const Vec2f& vert);

      inline const Vec2f& getVertex(int idx) const;

      inline void clear();

#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab) const;
#endif

      virtual ~Quad() {}

   private:
      void rangeCheck(int idx) const;
};

//===========================================
// Quad::rangeCheck
//===========================================
inline void Quad::rangeCheck(int idx) const {
   if (idx < 0 || idx > 3)
      throw Exception("Error setting vertex of Quad; index out of range", __FILE__, __LINE__);
}

//===========================================
// Quad::setVertex
//===========================================
inline void Quad::setVertex(int idx, float32_t x, float32_t y) {
   rangeCheck(idx);
   Polygon::setVertex(idx, x, y);
}

//===========================================
// Quad::setVertex
//===========================================
inline void Quad::setVertex(int idx, const Vec2f& vert) {
   rangeCheck(idx);
   Polygon::setVertex(idx, vert);
}

//===========================================
// Quad::getVertex
//===========================================
inline const Vec2f& Quad::getVertex(int idx) const {
   rangeCheck(idx);
   return Polygon::getVertex(idx);
}

//===========================================
// Quad::clear
//===========================================
inline void Quad::clear() {
   throw Exception("Method Quad::clear() is disallowed", __FILE__, __LINE__);
}


}


#endif
