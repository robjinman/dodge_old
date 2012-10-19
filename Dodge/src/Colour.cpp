/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <cstring>
#include <cstdio>
#include <Exception.hpp>
#include <Colour.hpp>


using namespace rapidxml;


namespace Dodge {


//===========================================
// Colour::assignData
//===========================================
void Colour::assignData(const xml_node<>* data) {
   if (strcmp(data->name(), "Colour") != 0)
      throw Exception("Error parsing XML for instance of class Colour", __FILE__, __LINE__);

   const xml_attribute<>* attr = data->first_attribute();
   if (attr && strcmp(attr->name(), "r") == 0) {
      sscanf(attr->value(), "%f", &r);
      attr = attr->next_attribute();
   }
   if (attr && strcmp(attr->name(), "g") == 0) {
      sscanf(attr->value(), "%f", &g);
      attr = attr->next_attribute();
   }
   if (attr && strcmp(attr->name(), "b") == 0) {
      sscanf(attr->value(), "%f", &b);
      attr = attr->next_attribute();
   }
   if (attr && strcmp(attr->name(), "a") == 0) {
      sscanf(attr->value(), "%f", &a);
   }
}


}
