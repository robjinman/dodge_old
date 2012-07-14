/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __EGL_CHECK_HPP__
#define __EGL_CHECK_HPP__


#include <sstream>
#include "Exception.hpp"


#define EGL_CHECK(x) \
   x; \
   { \
      EGLint eglError = eglGetError(); \
      if(eglError != EGL_SUCCESS) { \
         std::stringstream msg; \
         msg << "Program received EGL error (code = " << eglError << ")"; \
         throw Exception(msg.str(), __FILE__, __LINE__); \
      } \
   }


#endif
