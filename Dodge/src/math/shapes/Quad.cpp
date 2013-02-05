#include <cstring>
#include <Exception.hpp>
#include <math/shapes/Quad.hpp>
#include <StringId.hpp>


using namespace std;


namespace Dodge {


//===========================================
// Quad::Quad
//===========================================
Quad::Quad(const XmlNode data)
   : Asset(internString("Quad")) {

   try {
      XML_NODE_CHECK(data, Quad);

      Polygon::clear();
      int n = 0;

      XmlNode node = data.firstChild();
      while (!node.isNull() && node.name() == "Vec2f") {
         Vec2f vert(node);
         addVertex(vert);

         ++n;
         node = node.nextSibling();
      }

      if (n != 4)
         throw XmlException("Expected 4 vertices", __FILE__, __LINE__);
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class Quad; ");
      throw;
   }
}

//===========================================
// Quad::Quad
//===========================================
Quad::Quad()
   : Asset(internString("Quad")) {

   addVertex(Vec2f());
   addVertex(Vec2f());
   addVertex(Vec2f());
   addVertex(Vec2f());
}

//===========================================
// Quad::Quad
//===========================================
Quad::Quad(const Vec2f& A, const Vec2f& B, const Vec2f& C, const Vec2f& D)
   : Asset(internString("Quad")) {

   addVertex(A);
   addVertex(B);
   addVertex(C);
   addVertex(D);
}

//===========================================
// Quad::Quad
//===========================================
Quad::Quad(const Vec2f& d)
   : Asset(internString("Quad")) {

   addVertex(Vec2f(0.f, 0.f));
   addVertex(Vec2f(d.x, 0.f));
   addVertex(d);
   addVertex(Vec2f(0.f, d.y));
}

//===========================================
// Quad::Quad
//===========================================
Quad::Quad(const Quad& copy)
   : Asset(internString("Quad")),
     Polygon(copy) {}

//===========================================
// Quad::clone
//===========================================
Asset* Quad::clone() const {
   return new Quad(*this);
}

//===========================================
// Quad::operator=
//===========================================
Quad& Quad::operator=(const Quad& rhs) {
   Polygon::operator=(rhs);
   return *this;
}

//===========================================
// Quad::getSize
//===========================================
size_t Quad::getSize() const {
   return sizeof(Quad) - sizeof(Polygon) + Polygon::getSize();
}

#ifdef DEBUG
//===========================================
// Quad::dbg_print
//===========================================
void Quad::dbg_print(std::ostream& out, int tab) const {
   for (int i = 0; i < tab; ++i) out << "\t";
   out << "Quad\n";

   for (int v = 0; v < 4; ++v) {
      for (int i = 0; i < tab + 1; ++i) out << "\t";
      out << "vert " << v << ": (" << getVertex(v).x << ", " << getVertex(v).y << ")\n";
   }
}
#endif


}
