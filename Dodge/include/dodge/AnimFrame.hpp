/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __ANIM_FRAME_HPP__
#define __ANIM_FRAME_HPP__


#include <memory>
#include "math/Vec2i.hpp"
#include "math/shapes/Shape.hpp"
#include "renderer/Colour.hpp"
#include "xml/xml.hpp"
#ifdef DEBUG
#include <ostream>
#endif


namespace Dodge {


class AnimFrame {
   public:
      std::unique_ptr<Vec2f> worldOffset;
      Vec2i pos, dim;
      std::unique_ptr<Shape> shape;   // TODO: Eventually change this to ShapeDelta
      std::unique_ptr<Vec2f> size;
      Colour col;

      uint_t number;

      explicit AnimFrame(const XmlNode data);
      AnimFrame(Vec2i pos_, Vec2i dim_, std::unique_ptr<Shape> shape_, const Colour& col_);
      AnimFrame(Vec2i pos_, Vec2i dim_, const Colour& col_);
      AnimFrame(const AnimFrame& copy);

      size_t getSize() const;

      AnimFrame& operator=(const AnimFrame& rhs);

#ifdef DEBUG
      void dbg_print(std::ostream& out, int tab = 0) const;
#endif
};


}


#endif
