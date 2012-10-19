/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __COLOUR_HPP__
#define __COLOUR_HPP__


#include "definitions.hpp"
#include "Renderer.hpp"
#include "rapidxml/rapidxml.hpp"


namespace Dodge {


class Colour {
   public:
      Renderer::colourElement_t r, g, b, a;

      Colour()
         : r(0), g(0), b(0), a(0) {}

      Colour(Renderer::colourElement_t r_, Renderer::colourElement_t g_, Renderer::colourElement_t b_, Renderer::colourElement_t a_)
         : r(r_), g(g_), b(b_), a(a_) {}

      Colour(const Renderer::colourElement_t col[4])
         : r(col[0]), g(col[1]), b(col[2]), a(col[3]) {}

      virtual void assignData(const rapidxml::xml_node<>* data);
};


}


#endif
