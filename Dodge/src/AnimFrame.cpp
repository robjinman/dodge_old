#include <cstdlib>
#include <cstring>
#include <Exception.hpp>
#include <AnimFrame.hpp>


using namespace std;
using namespace rapidxml;


namespace Dodge {


//===========================================
// AnimFrame::AnimFrame
//===========================================
AnimFrame::AnimFrame(const XmlNode data) {
   if (data.isNull() || data.name() != "AnimFrame")
      throw XmlException("Error parsing XML for instance of class AnimFrame; Expected 'AnimFrame' tag", __FILE__, __LINE__);

   XmlNode node = data.firstChild();

   if (node.isNull() || node.name() != "pos")
      throw XmlException("Error parsing XML for instance of class AnimFrame; Expected 'pos' tag", __FILE__, __LINE__);

   pos = Vec2i(node.firstChild());
   node = node.nextSibling();

   if (node.isNull() || node.name() != "dim")
      throw XmlException("Error parsing XML for instance of class AnimFrame; Expected 'dim' tag", __FILE__, __LINE__);

   dim = Vec2i(node.firstChild());
   node = node.nextSibling();

   if (node.isNull() || node.name() != "col")
      throw XmlException("Error parsing XML for instance of class AnimFrame; Expected 'col' tag", __FILE__, __LINE__);

   col = Colour(node.firstChild());
   node = node.nextSibling();

   if (!node.isNull() && node.name() == "shape") {     // TODO: PrimitiveDelta
//      m_shape = unique_ptr<Primitive>(primitiveFactory.create(node.firstChild()));
   }
}

//===========================================
// AnimFrame::AnimFrame
//===========================================
AnimFrame::AnimFrame(Vec2i pos_, Vec2i dim_, std::unique_ptr<Primitive> shape_, const Colour& col_)
   : pos(pos_), dim(dim_), col(col_) {

   shape = std::move(shape_);
}

//===========================================
// AnimFrame::AnimFrame
//===========================================
AnimFrame::AnimFrame(Vec2i pos_, Vec2i dim_, const Colour& col_)
   : pos(pos_), dim(dim_), col(col_) {}

//===========================================
// AnimFrame::AnimFrame
//===========================================
AnimFrame::AnimFrame(const AnimFrame& copy) {
   pos = copy.pos;
   dim = copy.dim;
   shape = copy.shape ? unique_ptr<Primitive>(copy.shape->clone()) : unique_ptr<Primitive>();
   col = copy.col;
}

#ifdef DEBUG
//===========================================
// AnimFrame::AnimFrame
//===========================================
void AnimFrame::dbg_print(std::ostream& out, int tab) const {
   for (int i = 0; i < tab; i++) out << "\t";
   out << "AnimFrame\n";

   for (int i = 0; i < tab + 1; ++i) out << "\t";
   out << "pos: (" << pos.x << ", " << pos.y << ")\n";

   for (int i = 0; i < tab + 1; ++i) out << "\t";
   out << "dim: (" << dim.x << ", " << dim.y << ")\n";

   for (int i = 0; i < tab + 1; ++i) out << "\t";
   out << "col: (" << col.r << ", " << col.g << ", " << col.b << ", " << col.a << ")\n";

   for (int i = 0; i < tab + 1; ++i) out << "\t";
   out << "shape:\n";
   if (shape) shape->dbg_print(out, tab + 1);
}
#endif

//===========================================
// AnimFrame::operator=
//===========================================
AnimFrame& AnimFrame::operator=(const AnimFrame& rhs) {
   pos = rhs.pos;
   dim = rhs.dim;
   shape = unique_ptr<Primitive>(rhs.shape->clone());
   col = rhs.col;

   return *this;
}


}
