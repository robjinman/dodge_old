/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2012
 */

#ifndef __POLY_HPP__
#define __POLY_HPP__


#ifdef DEBUG
#include <ostream>
#endif
#include "Vec2f.hpp"
#include "Exception.hpp"
#include "rapidxml.hpp"


namespace Dodge {


class Poly {
   public:
      static const int MAX_VERTS = 8;

      Poly();
      Poly(const Poly& poly);

      inline const Vec2f& getVertex(int idx) const;
      inline int getNumVertices() const;
      inline void setVertex(int idx, const Vec2f& vert);
      void addVertex(const Vec2f& vert);
      void removeVertex(int idx);
      void insertVertex(int idx, const Vec2f& vert);
      inline void clear();

      void rotate(double rads, const Vec2f& = Vec2f(0, 0));
      void scale(const Vec2f& sv);
      Vec2f computeMinimum() const;
      Vec2f computeMaximum() const;

      virtual void assignData(const rapidxml::xml_node<>* data);
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif

      bool operator==(const Poly& rhs) const;
      inline bool operator!=(const Poly& rhs) const;

   private:
      Vec2f m_verts[Poly::MAX_VERTS];
      int m_nVerts;
};

//===========================================
// Poly::operator!=
//===========================================
inline bool Poly::operator!=(const Poly& rhs) const {
   return !(*this == rhs);
}

//===========================================
// Poly::getVertex
//===========================================
inline const Vec2f& Poly::getVertex(int idx) const {
   if (idx > m_nVerts - 1 || idx < 0)
      throw Exception("Index out of range", __FILE__, __LINE__);

   return m_verts[idx];
}

//===========================================
// Poly::getNumVertices
//===========================================
inline int Poly::getNumVertices() const {
   return m_nVerts;
}

//===========================================
// Poly::setVertex
//===========================================
inline void Poly::setVertex(int idx, const Vec2f& vert) {
   if (idx > m_nVerts - 1 || idx < 0)
      throw Exception("Index out of range", __FILE__, __LINE__);

   m_verts[idx] = vert;
}

//===========================================
// Poly::clear
//===========================================
inline void Poly::clear() {
   m_nVerts = 0;
}


}


#endif
