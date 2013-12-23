#include <cstdio>
#include <sys/time.h>
#include <map>
#include <vector>
#include <dodge/dodge.hpp>


using namespace Dodge;
using namespace std;


Renderer& renderer = Renderer::getInstance();
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
      case WinIO::KEY_P: sprites[0]->dbg_print(std::cout, 0); break;
   }
}

void keyboard() {
   if (keys[WinIO::KEY_RIGHT]) {
      sprites[0]->playAnimation(internString("anim0"));
      sprites[0]->translate_x(0.0005);
   }
   if (keys[WinIO::KEY_SPACE]) {
      sprites[0]->rotate(0.5f, gGetPixelSize() * 16.f);
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

int main(int argc, char** argv) {
   gInitialise();

   WinIO win;
   win.init("OpenGLES 2.0 Demo", 640, 480, false);
   win.registerCallback(WinIO::EVENT_WINCLOSE, Functor<void, TYPELIST_0()>(quit));
   win.registerCallback(WinIO::EVENT_KEYDOWN, Functor<void, TYPELIST_1(int)>(keyDown));
   win.registerCallback(WinIO::EVENT_KEYUP, Functor<void, TYPELIST_1(int)>(keyUp));

   Functor<void, TYPELIST_0()> fMakeContext(&win, &WinIO::createGLContext);
   Functor<void, TYPELIST_0()> fSwap(&win, &WinIO::swapBuffers);

   renderer.start(fMakeContext, fSwap);

   pCamera_t camera(new Camera(640.0 / 480.0, 1.f));
   renderer.attachCamera(camera);

   pTexture_t tex0(new Texture("data/textures/man.png"));

   std::vector<AnimFrame> aFrames;
   aFrames.push_back(AnimFrame(Vec2i(0, 0), Vec2i(32, 32), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2i(32, 0), Vec2i(32, 32), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2i(64, 0), Vec2i(32, 32), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2i(96, 0), Vec2i(32, 32), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2i(128, 0), Vec2i(32, 32), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2i(160, 0), Vec2i(32, 32), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2i(192, 0), Vec2i(32, 32), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2i(224, 0), Vec2i(32, 32), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2i(256, 0), Vec2i(32, 32), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2i(288, 0), Vec2i(32, 32), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2i(320, 0), Vec2i(32, 32), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2i(352, 0), Vec2i(32, 32), Colour(1.0, 1.0, 1.0, 1.0)));
   pAnimation_t anim0(new Animation(internString("anim0"), 0.5f, aFrames));

   float32_t w = 32.f * gGetPixelSize().x;
   float32_t h = 32.f * gGetPixelSize().y;

   Sprite proto(internString("type0"), tex0);
   proto.addAnimation(anim0);
   proto.setOnScreenSize(w, h);
   proto.setTextureSection(Vec2i(0, 0), Vec2i(32, 32));

   sprites.push_back(pSprite_t(new Sprite(proto, internString("mainDude"))));
   sprites[0]->setFillColour(Colour(1.f, 1.f, 1.f, 1.f));
   sprites[0]->setTranslation(0.3, 0.3);
   sprites[0]->setZ(1);
   sprites[0]->addToWorld();

   for (int i = 1; i < 6; ++i) {
      float32_t deg = (i - 1) * (360 / 5);
      float32_t r = w * 2.f;

      sprites.push_back(pSprite_t(new Sprite(proto)));
      sprites[i]->setParent(sprites[0].get());
      sprites[i]->setFillColour(Colour(1.f, 1.f, 1.f, 1.f));
      sprites[i]->setTranslation(r * cos(DEG_TO_RAD(deg)), r * sin(DEG_TO_RAD(deg)));
      sprites[i]->setZ(1);
      sprites[i]->addToWorld();
   }

   while (1) {
      LOOP_START;

      win.doEvents();
      keyboard();
      computeFrameRate();

      for (uint_t i = 0; i < sprites.size(); ++i) {
         sprites[i]->update();
         sprites[i]->draw();
      }

      renderer.tick(Colour(0.5f, 0.6f, 0.8f, 1.f));

      LOOP_END;
   }

   return 0;
}
