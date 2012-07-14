#include <cstdio>
#include <sys/time.h>
#include <map>
#include "Graphics2d.hpp"
#include "WinIO.hpp"
#include "Poly.hpp"
#include "Font.hpp"


using namespace Dodge;
using namespace std;


map<int, bool> keys;
double frameRate;


void quit() {
   exit(0);
}

void keyUp(int code) {
   keys[code] = false;
}

void keyDown(int code) {
   keys[code] = true;

   switch (code) {
      case WinIO::KEY_ESCAPE: quit(); break;
      case WinIO::KEY_F: std::cout << "frame rate: " << frameRate << "\n";
   }
}

void keyboard() {}

void computeFrameRate() {
   static long i = 0;
   ++i;

   if (i % 100 == 0) {
      static timeval now;
      static timeval prev;

      gettimeofday(&now, NULL);

      double t = now.tv_sec - prev.tv_sec;
      t += static_cast<double>(now.tv_usec - prev.tv_usec) / 1000000.0;

      if (i > 100)
         frameRate = 100.0 / t;

      prev = now;
   }
}

int main(int argc, char** argv) {
   WinIO win;
   win.init("OpenGLES 2.0 Demo", 640, 480, false);
   win.registerCallback(WinIO::EVENT_WINCLOSE, Functor<void, TYPELIST_0()>(quit));
   win.registerCallback(WinIO::EVENT_KEYDOWN, Functor<void, TYPELIST_1(int)>(keyDown));
   win.registerCallback(WinIO::EVENT_KEYUP, Functor<void, TYPELIST_1(int)>(keyUp));

   Graphics2d graphics2d;
   graphics2d.init(640, 480);

   Texture img1("data/textures/bricks.png");
   Texture img2("data/textures/smile.png");

   pTexture_t pImgFont1(new Texture("data/textures/font2.png"));
   Dodge::Font font1(pImgFont1, Vec2f(0, 0), Vec2f(852, 792), 71, 98);

   Poly poly;
   poly.addVertex(Vec2f(0.09, 0.0));
   poly.addVertex(Vec2f(0.31, 0.0));
   poly.addVertex(Vec2f(0.4, 0.23));
   poly.addVertex(Vec2f(0.2, 0.4));
   poly.addVertex(Vec2f(0.0, 0.23));

   while (1) {
      win.doEvents();
      keyboard();
      computeFrameRate();

      graphics2d.clear(Colour(0.5, 0.6, 0.8, 1.0));
      graphics2d.drawPlainAlphaQuad(0.4, 0.2, 0, 0.5, 0.5, 15.0, Colour(1.0, 0.0, 0.0, 1.0));
      graphics2d.drawImage(img1, 0.38, 0.3, 1, 0.45, 0.45, 0.0, 0.0, img1.getWidth(), img1.getHeight(), 0.0, Colour(1.0, 1.0, 1.0, 1.0));
      graphics2d.drawPlainAlphaPoly(poly, 0.7, 0.35, 2, -18.0, Colour(0.0, 0.9, 0.0, 1.0));
      graphics2d.drawImage(img2, 0.6, 0.3, 3, 0.4, 0.4, 0.0, 0.0, img2.getWidth(), img2.getHeight(), 0.0, Colour(0.9, 0.8, 0.4, 0.6));
      graphics2d.drawImage(img2, 0.3, 0.5, 4, 0.3, 0.3, 0.0, 0.0, img2.getWidth(), img2.getHeight(), 0.0, Colour(1.0, 1.0, 1.0, 1.0));
      graphics2d.drawText(font1, "I'm a potato", 0.0, 0.4, 5, -14.0, Vec2f(0.5, 0.0), Vec2f(0.5, 0.5), Colour(0.0, 0.0, 1.0, 1.0));

      win.swapBuffers();
   }

   return 0;
}
