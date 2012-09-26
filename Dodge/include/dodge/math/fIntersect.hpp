/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __F_INTERSECT_HPP__
#define __F_INTERSECT_HPP__


namespace Dodge {

class Primitive;
class Vec2f;

namespace Math {


bool intersect(const Primitive& obj1, const Vec2f& pos1, const Primitive& obj2, const Vec2f& pos2);


}
}


#endif
