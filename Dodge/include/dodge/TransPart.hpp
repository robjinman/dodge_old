/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __TRANS_PART_HPP__
#define __TRANS_PART_HPP__


#ifdef DEBUG
#include <ostream>
#endif
#include "xml/xml.hpp"
#include "math/Vec2f.hpp"


namespace Dodge {


class TransPart {
   public:
      float32_t duration;
      Vec2f transl;
      float32_t rot;
      Vec2f scale;

      TransPart()
         : duration(0.f), transl(0, 0), rot(0), scale(0, 0) {}

      explicit TransPart(const XmlNode data);

      TransPart(float32_t d, const Vec2f& t, float32_t r, const Vec2f& s)
         : duration(d), transl(t), rot(r), scale(s) {}

#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif
};


}


#endif
