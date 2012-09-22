/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __ANIM_FRAME_HPP__
#define __ANIM_FRAME_HPP__


#include <memory>
#include "Vec2i.hpp"
#include "Primitive.hpp"
#include "Colour.hpp"
#include "rapidxml/rapidxml.hpp"


namespace Dodge {


class AnimFrame {
   public:
      Vec2i pos, dim;
      std::unique_ptr<Primitive> shape;   // TODO: Eventually change this to PrimitiveDelta
      Colour col;

      AnimFrame();
      AnimFrame(Vec2i pos_, Vec2i dim_, std::unique_ptr<Primitive> shape_, const Colour& col_);
      AnimFrame(Vec2i pos_, Vec2i dim_, const Colour& col_);
      AnimFrame(const AnimFrame& copy);

      AnimFrame& operator=(const AnimFrame& rhs);

      virtual void assignData(const rapidxml::xml_node<>* data);
};


}


#endif
