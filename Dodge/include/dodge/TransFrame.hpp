/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __TRANSFRAME_HPP__


#ifdef DEBUG
#include <ostream>
#endif
#include "rapidxml/rapidxml.hpp"
#include "math/Vec2f.hpp"


namespace Dodge {


class TransFrame {
   public:
      Vec2f delta;
      double rot;
      Vec2f scale;

      TransFrame()
         : delta(0, 0), rot(0), scale(0, 0) {}

      TransFrame(const Vec2f& d, double r, const Vec2f& s)
         : delta(d), rot(r), scale(s) {}

#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif
      virtual void assignData(const rapidxml::xml_node<>* data);
};


}


#endif
