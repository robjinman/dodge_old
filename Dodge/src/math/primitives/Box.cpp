#include <cstring>
#include <Exception.hpp>
#include <math/primitives/Box.hpp>
#include <StringId.hpp>
#include <cml/cml.h>


using namespace cml;
using namespace rapidxml;
using namespace std;


namespace Dodge {


Renderer Box::m_renderer = Renderer();


//===========================================
// Box::typeId
//===========================================
long Box::typeId() const {
   static long typeId = internString("Box");

   return typeId;
}

//===========================================
// Box::clone
//===========================================
Primitive* Box::clone() const {
   return new Box(*this);
}

#ifdef DEBUG
//===========================================
// Box::dbg_print
//===========================================
void Box::dbg_print(std::ostream& out, int tab) const {
   for (int i = 0; i < tab; ++i) out << "\t";
   out << "Box\n";

   for (int i = 0; i < tab + 1; ++i) out << "\t";
   out << "size: (" << m_size.x << ", " << m_size.y << ")\n";
}
#endif

//===========================================
// Box::assignData
//===========================================
void Box::assignData(const xml_node<>* data) {
   if (strcmp(data->name(), "Box") != 0)
      throw Exception("Error parsing XML for instance of class Box", __FILE__, __LINE__);

   xml_node<>* node = data->first_node();

   if (node && strcmp(node->name(), "Vec2f") == 0)
      m_size.assignData(node);
}

//===========================================
// Box::drawSolid
//===========================================
void Box::drawSolid(float32_t x, float32_t y, int z, float32_t angle, const Vec2f& pivot) const {
   m_renderer.setMode(Renderer::NONTEXTURED_ALPHA);

   float32_t fZ = static_cast<float32_t>(z);
   float32_t w = m_size.x;
   float32_t h = m_size.y;

   Renderer::vertexElement_t verts[] = {
      w, 0.f, fZ,      // Bottom right
      w, h, fZ,        // Top right
      0.f, 0.f, fZ,    // Bottom left
      w, h, fZ,        // Top right
      0.f, h, fZ,      // Top left
      0.f, 0.f, fZ     // Bottom left
   };

   matrix44f_c rotation;
   matrix44f_c translation;
   matrix44f_c modelView; // TODO: use pivot

   matrix_rotation_euler(rotation, 0.f, 0.f, DEG_TO_RAD(angle), euler_order_xyz);
   matrix_translation(translation, x, y, 0.f);
   modelView = translation * rotation;

   m_renderer.setMatrix(modelView.data());
   m_renderer.setGeometry(verts, Renderer::TRIANGLES, 6);
   m_renderer.render();
}

//===========================================
// Box::drawHollow
//===========================================
void Box::drawHollow(float32_t x, float32_t y, int z, float32_t angle, const Vec2f& pivot) const {
   // TODO
}

//===========================================
// Box::draw
//===========================================
void Box::draw(float32_t x, float32_t y, int z, float32_t angle, const Vec2f& pivot) const {
   const Renderer::Brush& brush = m_renderer.getBrush();

   if (brush.fillColour[3] != 0.f)
      drawSolid(x, y, z, angle, pivot);

   if (brush.lineColour[3] != 0.f && brush.lineWidth > 0)
      drawHollow(x, y, z, angle, pivot);
}

//===========================================
// Box::getMinimum
//===========================================
Vec2f Box::getMinimum() const {
   return Vec2f(0.f, 0.f);
}

//===========================================
// Box::getMaximum
//===========================================
Vec2f Box::getMaximum() const {
   return Vec2f(m_size.x, m_size.y);
}

//===========================================
// Box::rotate
//===========================================
void Box::rotate(double rads, const Vec2f& pivot) {}

//===========================================
// Box::scale
//===========================================
void Box::scale(const Vec2f& sv) {
   m_size.x *= sv.x;
   m_size.y *= sv.y;
}


}
