/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __COLOUR_HPP__
#define __COLOUR_HPP__


#include <GLES2/gl2.h>
#include "../../xml/xml.hpp"


namespace Dodge {


class Colour {
   public:
      GLfloat r, g, b, a;

      Colour()
         : r(0), g(0), b(0), a(0) {}

      explicit Colour(const XmlNode data);

      Colour(GLfloat r_, GLfloat g_, GLfloat b_, GLfloat a_)
         : r(r_), g(g_), b(b_), a(a_) {}

      Colour(const GLfloat col[4])
         : r(col[0]), g(col[1]), b(col[2]), a(col[3]) {}
};


}


#endif
