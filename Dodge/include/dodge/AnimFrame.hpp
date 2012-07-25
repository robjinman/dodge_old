/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __ANIM_FRAME_HPP__
#define __ANIM_FRAME_HPP__


#include "Vec2i.hpp"
#include "CompoundPoly.hpp"
#include "Colour.hpp"
#include "rapidxml/rapidxml.hpp"


namespace Dodge {


class AnimFrame {
   public:
      Vec2i pos, dim;
      CompoundPoly poly;
      Colour col;
      bool hasPoly;

      AnimFrame()
         : pos(0, 0), dim(0, 0), poly(), col(), hasPoly(false) {}

      AnimFrame(Vec2i pos_, Vec2i dim_, CompoundPoly poly_, const Colour& col_)
         : pos(pos_), dim(dim_), poly(poly_), col(col_), hasPoly(true) {}

      AnimFrame(Vec2i pos_, Vec2i dim_, const Colour& col_)
         : pos(pos_), dim(dim_), col(col_), hasPoly(false) {}

      virtual void assignData(const rapidxml::xml_node<>* data);
};


}


#endif
