/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __RENDERER_EXCEPTION_HPP__
#define __RENDERER_EXCEPTION_HPP__


#include <string>
#include "../Exception.hpp"


namespace Dodge {


enum exceptionType_t {
   UNKNOWN_EXCEPTION,
   EXCEPTION,
   RENDERER_EXCEPTION
   // ...
};

struct exceptionWrapper_t {
   exceptionWrapper_t(exceptionType_t t, void* d)
      : type(t), data(d) {}

   exceptionType_t type;
   void* data;
};


class RendererException : public Exception {
   public:
      RendererException(const std::string& msg, const char* file, unsigned int line)
         : Exception(msg, file, line) {}

      virtual exceptionWrapper_t constructWrapper() const {
         exceptionWrapper_t ret(RENDERER_EXCEPTION, new RendererException(*this));
         return ret;
      }

      virtual ~RendererException() throw() {}
};


}


#endif
