/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __PNG_CHECK_HPP__
#define __PNG_CHECK_HPP__


#include <sstream>
#include "Exception.hpp"


#define PNG_CHECK(x) \
   { \
      int pngError = x; \
      if(pngError != PNG_NO_ERROR) { \
         std::stringstream msg; \
         msg << "Program received PNG error (code = " << pngError << ")"; \
         throw Exception(msg.str(), __FILE__, __LINE__); \
      } \
   }


#endif /*!__PNG_CHECK_HPP__*/
