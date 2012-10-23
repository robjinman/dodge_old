/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __GLOBALS_HPP__
#define __GLOBALS_HPP__


#include "definitions.hpp"
#include "StackAllocator.hpp"
#include "math/Vec2f.hpp"


namespace Dodge {


const size_t GLOBAL_STACK_SIZE = 102400; // 100KB
extern StackAllocator gMemStack;

extern Vec2f gGetPixelSize();


};


#endif
