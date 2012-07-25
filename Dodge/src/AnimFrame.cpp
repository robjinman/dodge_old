#include <cstdlib>
#include <cstring>
#include <Exception.hpp>
#include <AnimFrame.hpp>


using namespace rapidxml;


namespace Dodge {


void AnimFrame::assignData(const xml_node<>* data) {
   if (strcmp(data->name(), "AnimFrame") != 0)
      throw Exception("Error parsing XML for instance of class AnimFrame", __FILE__, __LINE__);

   const xml_node<>* node = data->first_node();

   if (node && strcmp(node->name(), "pos") == 0) {
      const xml_node<>* child = node->first_node();
      if (child && strcmp(child->name(), "Vec2i") == 0) {
         pos.assignData(child);
      }
      node = node->next_sibling();
   }
   if (node && strcmp(node->name(), "dim") == 0) {
      const xml_node<>* child = node->first_node();
      if (child && strcmp(child->name(), "Vec2i") == 0) {
         dim.assignData(child);
      }
      node = node->next_sibling();
   }
   if (node && strcmp(node->name(), "col") == 0) {
      const xml_node<>* child = node->first_node();
      if (child && strcmp(child->name(), "Colour") == 0) {
         col.assignData(child);
      }
      node = node->next_sibling();
   }
   if (node && strcmp(node->name(), "boundingPoly") == 0) {
      const xml_node<>* child = node->first_node();
      if (child && strcmp(child->name(), "CompoundPoly") == 0) {
         poly.assignData(child);
      }
   }
}


}
