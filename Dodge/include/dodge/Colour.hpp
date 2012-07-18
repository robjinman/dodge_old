/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __COLOUR_HPP__
#define __COLOUR_HPP__


#include "definitions.hpp"
#include "rapidxml/rapidxml.hpp"


namespace Dodge {


class Colour {
   public:
      float32_t r, g, b, a;

      Colour()
         : r(1.0), g(1.0), b(1.0), a(1.0) {}

      Colour(float32_t r_, float32_t g_, float32_t b_, float32_t a_)
         : r(r_), g(g_), b(b_), a(a_) {}

      virtual void assignData(const rapidxml::xml_node<>* data);
};


}


#endif
