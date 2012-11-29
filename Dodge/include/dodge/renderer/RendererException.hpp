#ifndef __RENDERER_EXCEPTION_HPP__
#define __RENDERER_EXCEPTION_HPP__


#include <string>
#include "Renderer.hpp"
#include "../Exception.hpp"


namespace Dodge {


class RendererException : public Exception {
   public:
      RendererException(const std::string& msg, const char* file, unsigned int line)
         : Exception(msg, file, line) {}

      virtual Renderer::exceptionWrapper_t constructWrapper() const {
         return { Renderer::RENDERER_EXCEPTION, new RendererException(*this) };
      }

      virtual ~RendererException() throw() {}
};


}


#endif
