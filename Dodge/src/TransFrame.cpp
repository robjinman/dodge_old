#include <cstring>
#include <cstdio>
#include <Exception.hpp>
#include <TransFrame.hpp>


using namespace rapidxml;


namespace Dodge {


#ifdef DEBUG
void TransFrame::dbg_print(std::ostream& out, int tab) const {
   for (int i = 0; i < tab; i++) out << "\t";
   out << "delta: (" << delta.x << ", " << delta.y << ")\n";

   for (int i = 0; i < tab; i++) out << "\t";
   out << "rot: " << rot << "\n";

   for (int i = 0; i < tab; i++) out << "\t";
   out << "scale: (" << scale.x << ", " << scale.y << ")\n";
}
#endif

void TransFrame::assignData(const xml_node<>* data) {
   if (strcmp(data->name(), "TransFrame") != 0)
      throw Exception("Error parsing XML for instance of class Transformation", __FILE__, __LINE__);

   xml_node<>* node = data->first_node();
   if (node && strcmp(node->name(), "delta") == 0) {
      xml_node<>* child = node->first_node();
      if (child) delta.assignData(child);
      node = node->next_sibling();
   }
   if (node && strcmp(node->name(), "rot") == 0) {
      sscanf(node->value(), "%lf", &rot);
      node = node->next_sibling();
   }
   if (node && strcmp(node->name(), "scale") == 0) {
      xml_node<>* child = node->first_node();
      if (child) scale.assignData(child);
   }
}


}
