/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __POLYGON_HPP__
#define __POLYGON_HPP__


#include <vector>
#include <boost/shared_ptr.hpp>
#include "Primitive.hpp"
#include "../Vec2f.hpp"
#include "../../Exception.hpp"
#include "../../Renderer.hpp"


namespace Dodge {


class Polygon : public Primitive {
   public:
      Polygon();
      Polygon(const Polygon& copy);

      static const int MAX_VERTS = 8;

      inline const Vec2f& getVertex(int idx) const;
      inline int getNumVertices() const;
      inline void setVertex(int idx, const Vec2f& vert);
      inline void setVertex(int idx, float32_t x, float32_t y);
      void addVertex(const Vec2f& vert);
      inline void addVertex(float32_t x, float32_t y);
      void removeVertex(int idx);
      void insertVertex(int idx, const Vec2f& vert);
      inline void insertVertex(int idx, float32_t x, float32_t y);
      inline void clear();

      virtual Primitive* clone() const;

      virtual Vec2f getMinimum() const;
      virtual Vec2f getMaximum() const;
      virtual void rotate(double deg, const Vec2f& pivot);
      virtual void scale(const Vec2f& sv);

      virtual void assignData(const rapidxml::xml_node<>* data);
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab) const;
#endif

      virtual void draw(float32_t x, float32_t y, int z, float32_t angle = 0.f,
         const Vec2f& pivot = Vec2f(0.f, 0.f)) const;

      bool operator==(const Polygon& rhs) const;
      bool operator!=(const Polygon& rhs) const;

      virtual long typeId() const;
      virtual ~Polygon() {}

   private:
      void restructure();
      bool isConvex() const;
      void subdivide();
      void absorbChildren();

      void drawHollow(float32_t x, float32_t y, int z, float32_t angle, const Vec2f& pivot) const;
      void drawSolid(float32_t x, float32_t y, int z, float32_t angle, const Vec2f& pivot) const;

      std::vector<boost::shared_ptr<Vec2f> > m_verts;
      int m_nVerts;
      std::vector<Polygon> m_children;

      static Renderer m_renderer;
};

//===========================================
// Polygon::getVertex
//===========================================
inline const Vec2f& Polygon::getVertex(int idx) const {
   if (idx > m_nVerts - 1 || idx < 0)
      throw Exception("Index out of range", __FILE__, __LINE__);

   return *m_verts[idx];
}

//===========================================
// Polygon::getNumVertices
//===========================================
inline int Polygon::getNumVertices() const {
   return m_nVerts;
}

//===========================================
// Polygon::setVertex
//===========================================
inline void Polygon::setVertex(int idx, const Vec2f& vert) {
   if (idx > m_nVerts - 1 || idx < 0)
      throw Exception("Index out of range", __FILE__, __LINE__);

   *m_verts[idx] = vert;

   restructure();
}

//===========================================
// Polygon::setVertex
//===========================================
inline void Polygon::setVertex(int idx, float32_t x, float32_t y) {
   setVertex(idx, Vec2f(x, y));
}

//===========================================
// Polygon::addVertex
//===========================================
inline void Polygon::addVertex(float32_t x, float32_t y) {
   addVertex(Vec2f(x, y));
}

//===========================================
// Polygon::insertVertex
//===========================================
inline void Polygon::insertVertex(int idx, float32_t x, float32_t y) {
   insertVertex(idx, Vec2f(x, y));
}

//===========================================
// Polygon::clear
//===========================================
inline void Polygon::clear() {
   m_nVerts = 0;
   m_children.clear();
}


}


#endif
