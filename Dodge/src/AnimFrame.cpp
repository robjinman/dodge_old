#include <cstdlib>
#include <cstring>
#include <Exception.hpp>
#include <AnimFrame.hpp>
#include <ShapeFactory.hpp>


using namespace std;


namespace Dodge {


//===========================================
// AnimFrame::AnimFrame
//===========================================
AnimFrame::AnimFrame(const XmlNode data) {
   try {
      XML_NODE_CHECK(data, AnimFrame);

      XmlNode node = data.firstChild();

      XML_NODE_CHECK(node, pos);

      pos = Vec2i(node.firstChild());
      node = node.nextSibling();

      XML_NODE_CHECK(node, dim);

      dim = Vec2i(node.firstChild());
      node = node.nextSibling();

      XML_NODE_CHECK(node, col);

      col = Colour(node.firstChild());
      node = node.nextSibling();

      if (!node.isNull() && node.name() == "shape") {     // TODO: ShapeDelta
         ShapeFactory primitiveFactory;
         shape = unique_ptr<Shape>(primitiveFactory.create(node.firstChild()));
      }
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class AnimFrame; ");
      throw;
   }
}

//===========================================
// AnimFrame::AnimFrame
//===========================================
AnimFrame::AnimFrame(Vec2i pos_, Vec2i dim_, std::unique_ptr<Shape> shape_, const Colour& col_)
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
   shape = copy.shape ? unique_ptr<Shape>(copy.shape->clone()) : unique_ptr<Shape>();
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
   shape = unique_ptr<Shape>(rhs.shape->clone());
   col = rhs.col;

   return *this;
}


}
