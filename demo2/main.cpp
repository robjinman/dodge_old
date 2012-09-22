#include <cstdio>
#include <sys/time.h>
#include <map>
#include <vector>
#include <dodge/dodge.hpp>


using namespace Dodge;
using namespace std;


Graphics2d graphics2d;
map<int, bool> keys;
double frameRate;
vector<pSprite_t> sprites;


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
   }
}

void keyboard() {
   if (keys[WinIO::KEY_RIGHT]) {
      sprites[0]->playAnimation(internString("anim0"));
      sprites[0]->translate_x(0.001);
   }
   if (keys[WinIO::KEY_LEFT]) {
      sprites[0]->playAnimation(internString("anim0"));
      sprites[0]->translate_x(-0.001);
   }
   if (keys[WinIO::KEY_SPACE]) {
      sprites[0]->rotate(0.8f, graphics2d.getPixelSize() * 16.f);
   }
   if (keys[WinIO::KEY_UP]) {
      sprites[1]->scale(1.01);
   }
   if (keys[WinIO::KEY_DOWN]) {
      sprites[1]->scale(0.99);
   }
}

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

void btn1Click(int x, int y) {
   float32_t xx = graphics2d.getPixelSize().x * static_cast<float32_t>(x);
   float32_t yy = 1.f - graphics2d.getPixelSize().y * static_cast<float32_t>(y);

   std::cout << "Mouse click at pixel coords (" << x << ", " << y << ")\n";
   std::cout << "Mouse click at game coords (" << xx << ", " << yy << ")\n";
}

void btn1Release(int x, int y) {
   float32_t xx = graphics2d.getPixelSize().x * static_cast<float32_t>(x);
   float32_t yy = 1.f - graphics2d.getPixelSize().y * static_cast<float32_t>(y);

   std::cout << "Mouse release at pixel coords (" << x << ", " << y << ")\n";
   std::cout << "Mouse release at game coords (" << xx << ", " << yy << ")\n";
}

int main(int argc, char** argv) {
   WinIO win;
   win.init("OpenGLES 2.0 Demo", 640, 480, false);
   win.registerCallback(WinIO::EVENT_WINCLOSE, Functor<void, TYPELIST_0()>(quit));
   win.registerCallback(WinIO::EVENT_KEYDOWN, Functor<void, TYPELIST_1(int)>(keyDown));
   win.registerCallback(WinIO::EVENT_KEYUP, Functor<void, TYPELIST_1(int)>(keyUp));
   win.registerCallback(WinIO::EVENT_BTN1PRESS, Functor<void, TYPELIST_2(int, int)>(btn1Click));
   win.registerCallback(WinIO::EVENT_BTN1RELEASE, Functor<void, TYPELIST_2(int, int)>(btn1Release));

   graphics2d.init(640, 480);

   Quadtree<pEntity_t> quadtree(3, Range(0.f, 0.f, 64.f / 48.f, 1.f));

   pTexture_t tex0(new Texture("data/textures/man.png"));

   std::vector<AnimFrame> aFrames;
   aFrames.push_back(AnimFrame(Vec2f(0.f, 0.f), Vec2f(32.f, 32.f), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2f(32.f, 0.f), Vec2f(32.f, 32.f), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2f(64.f, 0.f), Vec2f(32.f, 32.f), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2f(96.f, 0.f), Vec2f(32.f, 32.f), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2f(128.f, 0.f), Vec2f(32.f, 32.f), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2f(160.f, 0.f), Vec2f(32.f, 32.f), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2f(192.f, 0.f), Vec2f(32.f, 32.f), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2f(224.f, 0.f), Vec2f(32.f, 32.f), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2f(256.f, 0.f), Vec2f(32.f, 32.f), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2f(288.f, 0.f), Vec2f(32.f, 32.f), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2f(320.f, 0.f), Vec2f(32.f, 32.f), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2f(352.f, 0.f), Vec2f(32.f, 32.f), Colour(1.0, 1.0, 1.0, 1.0)));
   Animation anim0(internString("anim0"), 24.f, aFrames);

   float32_t w = 32.f * graphics2d.getPixelSize().x;
   float32_t h = 32.f * graphics2d.getPixelSize().y;

   Sprite proto(internString("type0"), tex0);
   proto.addAnimation(&anim0);

   sprites.push_back(pSprite_t(new Sprite(proto, internString("mainDude"))));
   sprites[0]->setTranslation(0.3f, 0.3f, 2.f);

   for (int i = 1; i < 6; ++i) {
      float32_t deg = (i - 1) * (360 / 5);
      float32_t r = w * 2.f;

      sprites.push_back(pSprite_t(new Sprite(proto)));
      sprites[i]->setParent(sprites[0].get());
      sprites[i]->setTranslation(r * cos(DEG_TO_RAD(deg)), r * sin(DEG_TO_RAD(deg)), 2.f);
   }

   Polygon poly;
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
      graphics2d.setFillColour(Colour(0.f, 1.f, 0.f, 1.f));
      graphics2d.setLineColour(Colour(0.f, 0.f, 1.f, 1.f));
      graphics2d.setLineWidth(6);
      graphics2d.drawPrimitive(poly, 0.5f, 0.4f, 1);
      quadtree.dbg_draw(3, Colour(1.f, 0.f, 0.f, 1.f));

      for (uint_t i = 0; i < sprites.size(); ++i) {
         sprites[i]->update();
         sprites[i]->draw(Vec2f(0.f, 0.f));
      }

      win.swapBuffers();
   }

   return 0;
}
