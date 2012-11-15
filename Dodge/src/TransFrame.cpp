#include <cstring>
#include <cstdio>
#include <Exception.hpp>
#include <TransFrame.hpp>
#include <globals.hpp>


using namespace std;


namespace Dodge {


//===========================================
// TransFrame::Transframe
//===========================================
TransFrame::TransFrame(const XmlNode data) {
   try {
      XML_NODE_CHECK(data, TransFrame);

      XmlNode node = data.firstChild();
      XML_NODE_CHECK(node, delta);
      delta = Vec2f(node.firstChild());

      node = node.nextSibling();
      XML_NODE_CHECK(node, rot);
      rot = node.getFloat();

      node = node.nextSibling();
      XML_NODE_CHECK(node, scale);
      scale = Vec2f(node.firstChild());
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class TransFrame; ");
      throw;
   }
}

#ifdef DEBUG
//===========================================
// TransFrame::dbg_print
//===========================================
void TransFrame::dbg_print(ostream& out, int tab) const {
   for (int i = 0; i < tab; i++) out << "\t";
   out << "TransFrame\n";

   for (int i = 0; i < tab + 1; ++i) out << "\t";
   out << "delta: (" << delta.x << ", " << delta.y << ")\n";

   for (int i = 0; i < tab + 1; ++i) out << "\t";
   out << "rot: " << rot << "\n";

   for (int i = 0; i < tab + 1; ++i) out << "\t";
   out << "scale: (" << scale.x << ", " << scale.y << ")\n";
}
#endif


}
