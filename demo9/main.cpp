#include <vector>
#include <dodge/dodge.hpp>
#include <iostream>


using namespace std;
using namespace Dodge;
using namespace cml;


WinIO win;
Renderer renderer;
std::vector<pPrimitive_t> shapes;
double frameRate;


void quit() {
   renderer.stop();
   win.destroyWindow();
   exit(0);
}

void keyDown(int key) {
   switch (key) {
      case WinIO::KEY_ESCAPE: quit(); break;
      case WinIO::KEY_F:
         cout << "Frame rate (main loop): " << frameRate << "fps\n";
         cout << "Frame rate (render loop): " << renderer.getFrameRate() << "fps\n" << flush;
      break;
   }
}

void computeFrameRate() {
   static Timer timer;
   static long i = 0;
   i++;

   if (i % 1000 == 0) {
      frameRate = 1000.0 / timer.getTime();
      timer.reset();
   }
}

void onWindowResize(int w, int h) {
   Renderer renderer;
   renderer.onWindowResize(w, h);
   renderer.getCamera().setProjection(static_cast<float32_t>(w) / static_cast<float32_t>(h), 1.f);
}

int main() {
   try {
      win.init("Demo9 - renderer", 640, 480, false);
      win.registerCallback(WinIO::EVENT_WINCLOSE, Functor<void, TYPELIST_0()>(quit));
      win.registerCallback(WinIO::EVENT_KEYDOWN, Functor<void, TYPELIST_1(int)>(keyDown));
      win.registerCallback(WinIO::EVENT_WINRESIZE, Functor<void, TYPELIST_2(int, int)>(onWindowResize));

      renderer.start();

      pCamera_t camera(new Camera(640.0 / 480.0, 1.f));
      renderer.attachCamera(camera);

      renderer.setBgColour(Colour(1.0, 1.0, 0.0, 1.0));

      pPrimitive_t shape(new LineSegment(Vec2f(0.f, 0.f), Vec2f(1.f, 1.f)));
      shape->setLineWidth(4);
      shape->setLineColour(Colour(1.f, 0.f, 0.f, 1.f));
      shape->render();

      while (1) {
         win.doEvents();
         computeFrameRate();
         renderer.checkForErrors();
      }
   }
   catch (Exception& e) {
      e.prepend("An error occurred; ");
      cerr << e.what() << flush;

      quit();
   }

   return 0;
}
