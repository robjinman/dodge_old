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
