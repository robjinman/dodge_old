/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __TRANSFRAME_HPP__


#ifdef DEBUG
#include <ostream>
#endif
#include "xml/xml.hpp"
#include "math/Vec2f.hpp"


namespace Dodge {


class TransFrame {
   public:
      Vec2f delta;
      float32_t rot;
      Vec2f scale;

      TransFrame()
         : delta(0, 0), rot(0), scale(0, 0) {}

      TransFrame(const XmlNode data);

      TransFrame(const Vec2f& d, float32_t r, const Vec2f& s)
         : delta(d), rot(r), scale(s) {}

#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif
};


}


#endif
