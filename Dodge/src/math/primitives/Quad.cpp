#include <cstring>
#include <Exception.hpp>
#include <math/primitives/Quad.hpp>
#include <StringId.hpp>


using namespace rapidxml;
using namespace std;


namespace Dodge {


//===========================================
// Quad::Quad
//===========================================
Quad::Quad(const Vec2f& A, const Vec2f& B, const Vec2f& C, const Vec2f& D) {
   addVertex(A);
   addVertex(B);
   addVertex(C);
   addVertex(D);
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

//===========================================
// Quad::assignData
//===========================================
void Quad::assignData(const xml_node<>* data) {
   if (strcmp(data->name(), "Quad") != 0)
      throw Exception("Error parsing XML for instance of class Quad", __FILE__, __LINE__);

   clear();
   int n = 0;

   xml_node<>* node = data->first_node();
   while (node) {
      if (strcmp(node->name(), "Vec2f") == 0) {
         Vec2f vert;
         vert.assignData(node);

         addVertex(vert);

         ++n;
      }
      node = node->next_sibling();
   }

   if (n != 4)
      throw Exception("Error parsing XML for instance of class Quad; expected 4 vertices", __FILE__, __LINE__);
}


}
