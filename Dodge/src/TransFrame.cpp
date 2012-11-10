#include <cstring>
#include <cstdio>
#include <Exception.hpp>
#include <TransFrame.hpp>


using namespace rapidxml;


namespace Dodge {


//===========================================
// TransFrame::Transframe
//===========================================
TransFrame::TransFrame(const XmlNode data) {
   if (data.isNull() || data.name() != "TransFrame")
      throw XmlException("Error parsing XML for instance of class Transformation; Expected 'TransFrame' tag", __FILE__, __LINE__);

   XmlNode node = data.firstChild();
   if (node.isNull() || node.name() != "delta")
      throw XmlException("Error parsing XML for instance of class Transformation; Expected 'delta' tag", __FILE__, __LINE__);

   delta = Vec2f(node.firstChild());
   node = node.nextSibling();

   if (node.isNull() || node.name() != "rot")
      throw XmlException("Error parsing XML for instance of class Transformation; Expected 'rot' tag", __FILE__, __LINE__);

   sscanf(node.value().data(), "%f", &rot);
   node = node.nextSibling();

   if (node.isNull() || node.name() != "scale")
      throw XmlException("Error parsing XML for instance of class Transformation; Expected 'scale' tag", __FILE__, __LINE__);

   scale = Vec2f(node.firstChild());
}


#ifdef DEBUG
//===========================================
// TransFrame::dbg_print
//===========================================
void TransFrame::dbg_print(std::ostream& out, int tab) const {
   for (int i = 0; i < tab; i++) out << "\t";
   out << "delta: (" << delta.x << ", " << delta.y << ")\n";

   for (int i = 0; i < tab; i++) out << "\t";
   out << "rot: " << rot << "\n";

   for (int i = 0; i < tab; i++) out << "\t";
   out << "scale: (" << scale.x << ", " << scale.y << ")\n";
}
#endif


}
