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
AnimFrame::AnimFrame(const rapidxml::xml_node<>* data) {
   assignData(data);
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

//===========================================
// AnimFrame::assignData
//===========================================
void AnimFrame::assignData(const xml_node<>* data) {
   if (!data || strcmp(data->name(), "AnimFrame") != 0)
      throw Exception("Error parsing XML for instance of class AnimFrame; Expected 'AnimFrame' tag", __FILE__, __LINE__);

   const xml_node<>* node = data->first_node();

   if (node && strcmp(node->name(), "pos") == 0) {
      pos.assignData(node->first_node());
      node = node->next_sibling();
   }
   if (node && strcmp(node->name(), "dim") == 0) {
      dim.assignData(node->first_node());
      node = node->next_sibling();
   }
   if (node && strcmp(node->name(), "col") == 0) {
      col.assignData(node->first_node());
      node = node->next_sibling();
   }
   if (node && strcmp(node->name(), "shape") == 0) {     // TODO: PrimitiveDelta
//      const xml_node<>* child = node->first_node();
//      if (child) shape = primitiveFactory.create(child);
   }
}


}
