/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __F_OVERLAP_HPP__
#define __F_OVERLAP_HPP__


namespace Dodge {

class Shape;
class Vec2f;

namespace Math {


extern bool overlap(const Shape& obj1, const Vec2f& pos1, const Shape& obj2, const Vec2f& pos2);


}
}


#endif
