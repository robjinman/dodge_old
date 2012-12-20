/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __POLYGON_HPP__
#define __POLYGON_HPP__


#include <vector>
#include <boost/shared_ptr.hpp>
#include "Shape.hpp"
#include "../Vec2f.hpp"
#include "../../renderer/Model.hpp"
#include "../../Exception.hpp"
#include "../../renderer/Renderer.hpp"


namespace Dodge {


class Polygon : public Shape {
   public:
      Polygon();
      explicit Polygon(const XmlNode data);
      Polygon(const Polygon& copy);

      static const int MAX_VERTS = 8;

      inline const Vec2f& getVertex(int idx) const;
      inline const std::vector<Polygon>& getConvexChildren() const;
      inline int getNumVertices() const;
      inline void setVertex(int idx, const Vec2f& vert);
      inline void setVertex(int idx, float32_t x, float32_t y);
      void addVertex(const Vec2f& vert);
      inline void addVertex(float32_t x, float32_t y);
      void removeVertex(int idx);
      void insertVertex(int idx, const Vec2f& vert);
      inline void insertVertex(int idx, float32_t x, float32_t y);
      void clear();

      virtual Polygon* clone() const;

      virtual Vec2f getMinimum() const;
      virtual Vec2f getMaximum() const;
      virtual void rotate(float32_t deg, const Vec2f& pivot);
      virtual void scale(const Vec2f& sv);
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif

      virtual void setFillColour(const Colour& colour) const;
      virtual void setLineColour(const Colour& colour) const;
      virtual void setLineWidth(int lineWidth) const;

      virtual void setRenderTransform(float32_t x, float32_t y, int z) const;
      virtual void draw() const;

      Polygon& operator=(const Polygon& rhs);

      bool operator==(const Polygon& rhs) const;
      bool operator!=(const Polygon& rhs) const;

      virtual long typeId() const;
      virtual ~Polygon();

   private:
      void deepCopy(const Polygon& copy);

      void restructure();
      bool isConvex() const;
      void subdivide();
      void absorbChildren();

      void updateModels() const;
      void updateInteriorModel() const;
      void updateOutlineModel() const;

      std::vector<boost::shared_ptr<Vec2f> > m_verts;
      int m_nVerts;
      std::vector<Polygon> m_children;

      mutable PlainNonTexturedAlphaModel m_outlineModel;
      mutable PlainNonTexturedAlphaModel m_interiorModel;
      Renderer& m_renderer;

      Colour m_fillColour;
      Colour m_lineColour;
      Renderer::int_t m_lineWidth;
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
// Polygon::getConvexChildren
//===========================================
inline const std::vector<Polygon>& Polygon::getConvexChildren() const {
   return m_children;
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
   updateModels();
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


}


#endif
