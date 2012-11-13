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
   string msg("Error parsing XML for instance of class TransFrame");

   XML_NODE_CHECK(msg, data, TransFrame);

   XmlNode node = data.firstChild();
   XML_NODE_CHECK(msg, node, delta);
   delta = Vec2f(node.firstChild());

   node = node.nextSibling();
   XML_NODE_CHECK(msg, node, rot);
   sscanf(node.value().data(), "%f", &rot);

   node = node.nextSibling();
   XML_NODE_CHECK(msg, node, scale);
   scale = Vec2f(node.firstChild());
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
