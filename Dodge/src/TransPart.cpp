#include <cstring>
#include <cstdio>
#include <Exception.hpp>
#include <TransPart.hpp>
#include <globals.hpp>


using namespace std;


namespace Dodge {


//===========================================
// TransPart::Transframe
//===========================================
TransPart::TransPart(const XmlNode data) {
   try {
      XML_NODE_CHECK(data, TransPart);

      XmlAttribute attr = data.firstAttribute();
      XML_ATTR_CHECK(attr, duration);
      duration = attr.getFloat();

      XmlNode node = data.firstChild();
      XML_NODE_CHECK(node, transl);
      transl = Vec2f(node.firstChild());

      node = node.nextSibling();
      XML_NODE_CHECK(node, rot);

      XmlNode child = node.firstChild();
      XML_NODE_CHECK(child, angle);
      rot = child.getFloat();

      child = child.nextSibling();
      XML_NODE_CHECK(child, pivot);
      pivot = Vec2f(child.firstChild());

      node = node.nextSibling();
      XML_NODE_CHECK(node, scale);
      scale = Vec2f(node.firstChild());
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class TransPart; ");
      throw;
   }
}

#ifdef DEBUG
//===========================================
// TransPart::dbg_print
//===========================================
void TransPart::dbg_print(ostream& out, int tab) const {
   for (int i = 0; i < tab; i++) out << "\t";
   out << "TransPart\n";

   for (int i = 0; i < tab + 1; ++i) out << "\t";
   out << "duration: " << duration << "\n";

   for (int i = 0; i < tab + 1; ++i) out << "\t";
   out << "transl: (" << transl.x << ", " << transl.y << ")\n";

   for (int i = 0; i < tab + 1; ++i) out << "\t";
   out << "rot: " << rot << "\n";

   for (int i = 0; i < tab + 1; ++i) out << "\t";
   out << "pivot: (" << pivot.x << ", " << pivot.y << ")\n";

   for (int i = 0; i < tab + 1; ++i) out << "\t";
   out << "scale: (" << scale.x << ", " << scale.y << ")\n";
}
#endif


}
