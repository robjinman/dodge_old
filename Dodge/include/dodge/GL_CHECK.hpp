/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __GL_CHECK_HPP__
#define __GL_CHECK_HPP__


#include <sstream>
#include "Exception.hpp"


#define GL_CHECK(x) \
   x; \
   { \
      GLenum glError = glGetError(); \
      if(glError != GL_NO_ERROR) { \
         std::stringstream msg; \
         msg << "Program received OpenGL error (code = 0x" << std::hex << glError << ")"; \
         throw Exception(msg.str(), __FILE__, __LINE__); \
      } \
   }


#endif
