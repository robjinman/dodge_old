#include <cstdio>
#include <sys/time.h>
#include <map>
#include <vector>
#include <iostream>
#include <dodge/dodge.hpp>


using namespace Dodge;
using namespace std;


Graphics2d graphics2d;
map<int, bool> keys;
double frameRate;
vector<pEntity_t> entities;
Range dragRegion;
WorldSpace worldSpace;
EventManager eventManager;


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
      case WinIO::KEY_N: std::cout << "worldSpace.dbg_getNumEntities() = " << worldSpace.dbg_getNumEntities() << "\n"; break;

      case WinIO::KEY_1: worldSpace.insertEntity(entities[0]); break;
      case WinIO::KEY_2: worldSpace.insertEntity(entities[1]); break;
      case WinIO::KEY_3: worldSpace.insertEntity(entities[2]); break;
      case WinIO::KEY_4: worldSpace.insertEntity(entities[3]); break;
      case WinIO::KEY_5: worldSpace.insertEntity(entities[4]); break;
      case WinIO::KEY_6: worldSpace.insertEntity(entities[5]); break;

      case WinIO::KEY_Q: worldSpace.removeEntity(entities[0]); break;
      case WinIO::KEY_W: worldSpace.removeEntity(entities[1]); break;
      case WinIO::KEY_E: worldSpace.removeEntity(entities[2]); break;
      case WinIO::KEY_R: worldSpace.removeEntity(entities[3]); break;
      case WinIO::KEY_T: worldSpace.removeEntity(entities[4]); break;
      case WinIO::KEY_Y: worldSpace.removeEntity(entities[5]); break;
   }
}

void keyboard() {
   pSprite_t mainGuy = boost::static_pointer_cast<Sprite>(entities[0]);

   if (keys[WinIO::KEY_RIGHT]) {
      mainGuy->playAnimation(internString("anim0"));
      mainGuy->translate_x(0.001);
   }
   if (keys[WinIO::KEY_LEFT]) {
      mainGuy->playAnimation(internString("anim0"));
      mainGuy->translate_x(-0.001);
   }
   if (keys[WinIO::KEY_SPACE]) {
      mainGuy->rotate(0.8f, graphics2d.getPixelSize() * 16.f);
   }
   if (keys[WinIO::KEY_UP]) {
      entities[1]->scale(1.01);
   }
   if (keys[WinIO::KEY_DOWN]) {
      entities[1]->scale(0.99);
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

   dragRegion = Range(xx, yy, 0.f, 0.f);
}

void btn1Release(int x, int y) {
   float32_t xx = graphics2d.getPixelSize().x * static_cast<float32_t>(x);
   float32_t yy = 1.f - graphics2d.getPixelSize().y * static_cast<float32_t>(y);

   Vec2f size = Vec2f(xx, yy) - dragRegion.getPosition();

   pShape_t shape(new Shape(internString("rectangle"), std::unique_ptr<Primitive>(new Box(size))));
   shape->setTranslation(dragRegion.getPosition());
   shape->setZ(4);

   shape->getRenderBrush()->fillColour[0] = 1.f;
   shape->getRenderBrush()->fillColour[1] = 0.f;
   shape->getRenderBrush()->fillColour[2] = 0.f;
   shape->getRenderBrush()->fillColour[3] = 0.2f;

   entities.push_back(shape);
   worldSpace.insertEntity(shape);
}

int main(int argc, char** argv) {
   WinIO win;
   win.init("OpenGLES 2.0 Demo", 640, 480, false);
   win.registerCallback(WinIO::EVENT_WINCLOSE, Functor<void, TYPELIST_0()>(quit));
   win.registerCallback(WinIO::EVENT_KEYDOWN, Functor<void, TYPELIST_1(int)>(keyDown));
   win.registerCallback(WinIO::EVENT_KEYUP, Functor<void, TYPELIST_1(int)>(keyUp));
   win.registerCallback(WinIO::EVENT_BTN1PRESS, Functor<void, TYPELIST_2(int, int)>(btn1Click));
   win.registerCallback(WinIO::EVENT_BTN1RELEASE, Functor<void, TYPELIST_2(int, int)>(btn1Release));

   worldSpace.init(std::unique_ptr<Quadtree<pEntity_t> >(new Quadtree<pEntity_t>(1, Range(0.f, 0.f, 64.f / 48.f, 1.f))));

   graphics2d.init(640, 480);

   pTexture_t tex0(new Texture("data/textures/man.png"));

   float32_t w = 32.f * graphics2d.getPixelSize().x;
   float32_t h = 32.f * graphics2d.getPixelSize().y;

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

   pSprite_t proto(new Sprite(internString("type0"), tex0));
   proto->addAnimation(&anim0);
   proto->setShape(std::unique_ptr<Primitive>(new Quad(Vec2f(0.f, 0.f), Vec2f(w, 0.f), Vec2f(w, h), Vec2f(0.f, h))));

   entities.push_back(pSprite_t(new Sprite(*proto, internString("mainDude"))));
   entities[0]->setTranslation(0.3f, 0.3f);
   entities[0]->setZ(2);
   worldSpace.trackEntity(entities[0]);

   for (int i = 1; i < 6; ++i) {
      float32_t deg = (i - 1) * (360 / 5);
      float32_t r = w * 2.f;

      entities.push_back(pSprite_t(new Sprite(*proto)));
      entities[i]->setParent(entities[0].get());
      entities[i]->setTranslation(r * cos(DEG_TO_RAD(deg)), r * sin(DEG_TO_RAD(deg)));
      entities[i]->setZ(2);
      worldSpace.trackEntity(entities[i]);
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
      worldSpace.dbg_draw(3, Colour(1.f, 0.f, 0.f, 1.f));

      for (uint_t i = 0; i < entities.size(); ++i) {
         entities[i]->update();
         entities[i]->draw(Vec2f(0.f, 0.f));
         entities[i]->getBoundary().dbg_draw(4, Colour(0.f, 1.f, 0.f, 0.5f));
         graphics2d.setFillColour(Colour(1.f, 0.f, 0.f, 0.5f));
         graphics2d.setLineWidth(0);
         entities[i]->getShape().draw(entities[i]->getTranslation_abs().x, entities[i]->getTranslation_abs().y, 5);
      }

      eventManager.doEvents();

      win.swapBuffers();
   }

   return 0;
}
