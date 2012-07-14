/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2012
 */

#ifndef __DEFINITIONS_HPP__
#define __DEFINITIONS_HPP__


#include <memory>


#define sharedPtr_t std::shared_ptr
#define RAD_TO_DEG(x) ((x) * 57.29578f)
#define DEG_TO_RAD(x) ((x) * 0.01745329f)


namespace Dodge {


const float PI = 3.14159265;


#ifdef WIN32
typedef float float32_t;
#elif defined LINUX
typedef float float32_t;
#else
typedef float float32_t;
#endif
typedef unsigned int uint_t;


}


#endif
