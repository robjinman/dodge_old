/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <globals.hpp>
#include <renderer/Renderer.hpp>
#include <WinIO.hpp>


namespace Dodge {


StackAllocator gMemStack(GLOBAL_STACK_SIZE);

Vec2f gGetPixelSize() {
   Renderer renderer;
   WinIO win;

   const Vec2f& view = renderer.getCamera().getViewSize();

   float32_t w = static_cast<float32_t>(win.getWindowWidth());
   float32_t h = static_cast<float32_t>(win.getWindowHeight());

   return Vec2f(view.x / w, view.y / h);
}


}
