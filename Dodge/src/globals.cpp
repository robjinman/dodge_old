/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <globals.hpp>
#include <renderer/Renderer.hpp>
#include <WinIO.hpp>


using namespace std;


namespace Dodge {


bool init = false;
unique_ptr<StackAllocator> memStack;
float32_t targetFrameRate;
projectSettings_t settings;
int gFlag = 0; // TODO


//===========================================
// gInitialise
//===========================================
void gInitialise(const projectSettings_t& s) {
   if (init)
      throw Exception("Error initialising globals; Globals already initialised", __FILE__, __LINE__);

   settings = s;
   memStack = unique_ptr<StackAllocator>(new StackAllocator(s.globalStackSize));
   targetFrameRate = s.targetFrameRate;

   init = true;
}

//===========================================
// gGetTargetFrameRate
//===========================================
float32_t gGetTargetFrameRate() {
   if (!init)
      throw Exception("Error retrieving target frame rate; Globals not initialised", __FILE__, __LINE__);

   return targetFrameRate;
}

//===========================================
// gGetMemStack
//===========================================
StackAllocator& gGetMemStack() {
   if (!init)
      throw Exception("Error retrieving memory stack; Globals not initialised", __FILE__, __LINE__);

   return *memStack;
}

//===========================================
// gGetWorkingDir
//===========================================
const string& gGetWorkingDir() {
   if (!init)
      throw Exception("Error retrieving working directory; Globals not initialised", __FILE__, __LINE__);

   return settings.workingDir;
}

//===========================================
// gGetPixelSize
//===========================================
Vec2f gGetPixelSize() {
   if (!init)
      throw Exception("Error retrieving pixel size; Globals not initialised", __FILE__, __LINE__);

   Renderer& renderer = Renderer::getInstance();
   WinIO win;

   const Vec2f& view = renderer.getCamera().getViewSize();

   float32_t w = static_cast<float32_t>(win.getWindowWidth());
   float32_t h = static_cast<float32_t>(win.getWindowHeight());

   return Vec2f(view.x / w, view.y / h);
}


}
