#include <cstdio>
#include <sys/time.h>
#include <map>
#include <dodge/dodge.hpp>


using namespace Dodge;
using namespace std;


map<int, bool> keys;
double frameRate;
pSprite_t sprite0;


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
      case WinIO::KEY_F: std::cout << "frame rate: " << frameRate << "\n"; break;
      case WinIO::KEY_SPACE: sprite0->playAnimation(internString("anim0"));
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

   pTexture_t tex0(new Texture("data/textures/ss8x8squares128x128g.png"));
   sprite0 = pSprite_t(new Sprite(internString("sprite0"), internString("type0"), tex0));

   std::vector<AnimFrame> aFrames;
   aFrames.push_back(AnimFrame(Vec2f(0.f, 0.f), Vec2f(16.f, 16.f), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2f(16.f, 0.f), Vec2f(16.f, 16.f), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2f(32.f, 0.f), Vec2f(16.f, 16.f), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2f(48.f, 0.f), Vec2f(16.f, 16.f), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2f(64.f, 0.f), Vec2f(16.f, 16.f), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2f(80.f, 0.f), Vec2f(16.f, 16.f), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2f(96.f, 0.f), Vec2f(16.f, 16.f), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2f(112.f, 0.f), Vec2f(16.f, 16.f), Colour(1.0, 1.0, 1.0, 1.0)));
   Animation anim0(internString("anim0"), 25.f, aFrames);

   sprite0->addAnimation(&anim0);
   sprite0->setPosition(0.5f, 0.5f, 1.f);

   while (1) {
      win.doEvents();
      keyboard();
      computeFrameRate();

      graphics2d.clear(Colour(0.5, 0.6, 0.8, 1.0));

      sprite0->update();
      sprite0->draw(Vec2f(0.f, 0.f));

      win.swapBuffers();
   }

   return 0;
}
