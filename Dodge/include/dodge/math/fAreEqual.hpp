/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __F_ARE_EQUAL_HPP__
#define __F_ARE_EQUAL_HPP__


#include "primitives/Primitive.hpp"


extern bool operator==(const Dodge::Primitive& obj1, const Dodge::Primitive& obj2);

inline bool operator!=(const Dodge::Primitive& obj1, const Dodge::Primitive& obj2) {
   return !(obj1 == obj2);
}


#endif
