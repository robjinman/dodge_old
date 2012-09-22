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
AnimFrame::AnimFrame()
   : pos(0, 0), dim(0, 0) {}

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

//===========================================
// AnimFrame::assignData
//===========================================
void AnimFrame::assignData(const xml_node<>* data) {
   if (strcmp(data->name(), "AnimFrame") != 0)
      throw Exception("Error parsing XML for instance of class AnimFrame", __FILE__, __LINE__);

   const xml_node<>* node = data->first_node();

   if (node && strcmp(node->name(), "pos") == 0) {
      const xml_node<>* child = node->first_node();
      if (child) pos.assignData(child);
      node = node->next_sibling();
   }
   if (node && strcmp(node->name(), "dim") == 0) {
      const xml_node<>* child = node->first_node();
      if (child) dim.assignData(child);
      node = node->next_sibling();
   }
   if (node && strcmp(node->name(), "col") == 0) {
      const xml_node<>* child = node->first_node();
      if (child) col.assignData(child);
      node = node->next_sibling();
   }
   if (node && strcmp(node->name(), "shape") == 0) {     // TODO: PrimitiveDelta
//      const xml_node<>* child = node->first_node();
//      if (child) shape = primitiveFactory.create(child);
   }
}


}
