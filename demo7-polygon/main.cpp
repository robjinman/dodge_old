#include <map>
#include <dodge/dodge.hpp>
#include <iostream>


using namespace std;
using namespace Dodge;


WinIO win;
EventManager eventManager;
pShape_t shape;
Graphics2d graphics2d;
map<int, bool> keyState;
double frameRate;


void quit() {
   exit(0);
}

void keyDown(int key) {
   switch (key) {
      case WinIO::KEY_ESCAPE: quit(); break;
      case WinIO::KEY_F: cout << "Frame rate: " << frameRate << "fps\n" << flush; break;
   }

   keyState[key] = true;
}

void keyUp(int key) { keyState[key] = false; }

void draw() {
   shape->draw();
}

void keyboard() {
   const Range& bounds = shape->getBoundary();
   Vec2f centre = bounds.getSize() / 2.f;

   if (keyState[WinIO::KEY_LEFT]) shape->rotate(0.6f, centre);
   else if (keyState[WinIO::KEY_RIGHT]) shape->rotate(-0.6f, centre);
}

void computeFrameRate() {
   static Timer timer;
   static long i = 0;
   i++;

   if (i % 100 == 0) {
      frameRate = 100.0 / timer.getTime();
      timer.reset();
   }
}

void onWindowResize(int w, int h) {
   Renderer renderer;
   renderer.onWindowResize(w, h);
   graphics2d.getCamera()->setProjection(static_cast<float32_t>(w) / static_cast<float32_t>(h), 1.f);
}

int main() {
   win.init("Demo7 - polygon", 640, 480, false);
   win.registerCallback(WinIO::EVENT_WINCLOSE, Functor<void, TYPELIST_0()>(quit));
   win.registerCallback(WinIO::EVENT_KEYDOWN, Functor<void, TYPELIST_1(int)>(keyDown));
   win.registerCallback(WinIO::EVENT_KEYUP, Functor<void, TYPELIST_1(int)>(keyUp));
   win.registerCallback(WinIO::EVENT_WINRESIZE, Functor<void, TYPELIST_2(int, int)>(onWindowResize));

   graphics2d.init(640, 480);

   unique_ptr<Polygon> pPoly(new Polygon);
   pPoly->addVertex(Vec2f(0.09, 0.0));
   pPoly->addVertex(Vec2f(0.31, 0.0));
   pPoly->addVertex(Vec2f(0.4, 0.23));
   pPoly->addVertex(Vec2f(0.2, 0.4));
   pPoly->addVertex(Vec2f(0.0, 0.23));

   shape = pShape_t(new Shape(internString("polygon"), internString("bigPentagon"), move(pPoly)));
   shape->setZ(0);
   shape->setTranslation(0.5f, 0.4f);

   shape->getRenderBrush()->setFillColour(Colour(0.f, 1.f, 0.f, 1.f));
   shape->getRenderBrush()->setLineColour(Colour(0.f, 0.f, 1.f, 1.f));
   shape->getRenderBrush()->setLineWidth(6);

   while (1) {
      graphics2d.clear(Colour(0.5f, 0.5f, 0.5f, 1.f));
      win.doEvents();
      eventManager.doEvents();
      keyboard();
      shape->update();
      draw();
      computeFrameRate();
      win.swapBuffers();
   }

   return 0;
}
