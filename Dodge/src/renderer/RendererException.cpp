/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */


#include <renderer/Renderer.hpp>


namespace Dodge {


Renderer::exceptionWrapper_t RendererException::constructWrapper() const {
   Renderer::exceptionWrapper_t ret(Renderer::RENDERER_EXCEPTION, new RendererException(*this));
   return ret;
}


}
