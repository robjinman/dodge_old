/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <cstring>
#include <cstdio>
#include <Exception.hpp>
#include <math/Vec2f.hpp>
#include <math/Vec2i.hpp>


using namespace rapidxml;


namespace Dodge {


//===========================================
// Vec2i::Vec2i
//===========================================
Vec2i::Vec2i(const Vec2f& v) : x(v.x), y(v.y) {}

//===========================================
// Vec2i::Vec2i
//===========================================
void Vec2i::assignData(const xml_node<>* data) {
   if (strcmp(data->name(), "Vec2i") != 0)
      throw Exception("Error parsing XML for instance of class Vec2i", __FILE__, __LINE__);

   xml_attribute<>* attr = data->first_attribute();
   if (attr && strcmp(attr->name(), "x") == 0) {
      sscanf(attr->value(), "%d", &x);
      attr = attr->next_attribute();
   }
   if (attr && strcmp(attr->name(), "y") == 0) {
      sscanf(attr->value(), "%d", &y);
      attr = attr->next_attribute();
   }
}

//===========================================
// Vec2i::rotate
//===========================================
void Vec2i::rotate(const Vec2i& p, float32_t a) {
   float32_t x_ = x;
   x = (x - p.x) * cos(DEG_TO_RAD(a)) - (y - p.y) * sin(DEG_TO_RAD(a)) + p.x;
   y = (x_ - p.x) * sin(DEG_TO_RAD(a)) + (y - p.y) * cos(DEG_TO_RAD(a)) + p.y;
}


}
