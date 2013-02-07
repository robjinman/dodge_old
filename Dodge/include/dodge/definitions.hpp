/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __DEFINITIONS_HPP__
#define __DEFINITIONS_HPP__


#define sharedPtr_t std::shared_ptr
#define RAD_TO_DEG(x) ((x) * 57.29578f)
#define DEG_TO_RAD(x) ((x) * 0.01745329f)

#define LOOP_START \
   { \
      Dodge::Timer lpStart_timer;

#define LOOP_END \
      double lpEnd_wait = (1.0 / Dodge::gGetTargetFrameRate()) - lpStart_timer.getTime(); \
      if (lpEnd_wait < 0.0) lpEnd_wait = 0.0; \
      Dodge::sleepThread(lpEnd_wait); \
   }

#ifdef WIN32
   #define SSCANF sscanf
#else
   #define SSCANF sscanf
#endif


namespace Dodge {


const float PI = 3.14159265f;


#ifdef WIN32
typedef float float32_t;
typedef unsigned char byte_t;
#else
typedef float float32_t;
typedef unsigned char byte_t;
#endif
typedef unsigned int uint_t;


}


#endif
