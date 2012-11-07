#include <map>
#include <dodge/dodge.hpp>
#include <iostream>


using namespace std;
using namespace Dodge;


WinIO win;
EventManager eventManager;
pSprite_t sprite1, sprite2;
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
   sprite1->draw();
   sprite2->draw();
}

void keyboard() {
   if (keyState[WinIO::KEY_LEFT]) sprite1->rotate(1.f);
   if (keyState[WinIO::KEY_RIGHT]) sprite1->rotate(-1.f);
   if (keyState[WinIO::KEY_UP]) sprite1->scale(1.01f);
   if (keyState[WinIO::KEY_DOWN]) sprite1->scale(0.99f);
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

int main() {
   Box2dPhysics::loadSettings("physics.conf");
   win.init("Demo4 - physics", 640, 480, false);
   win.registerCallback(WinIO::EVENT_WINCLOSE, Functor<void, TYPELIST_0()>(quit));
   win.registerCallback(WinIO::EVENT_KEYDOWN, Functor<void, TYPELIST_1(int)>(keyDown));
   win.registerCallback(WinIO::EVENT_KEYUP, Functor<void, TYPELIST_1(int)>(keyUp));

   graphics2d.init(640, 480);

   pTexture_t tex0(new Texture("sprite1.png"));
   pTexture_t tex1(new Texture("sprite2.png"));

//   unique_ptr<Box2dPhysics> spr1Phys(new Box2dPhysics(false, false, 1.f, 0.3f));
//   sprite1 = pPhysicalSprite_t(new PhysicalSprite(move(spr1Phys), internString("sprite1"), internString("shape"), tex0));
   sprite1 = boost::shared_ptr<PhysicalSprite<Box2dPhysics> >(new PhysicalSprite<Box2dPhysics>(internString("sprite1"), internString("shape"), tex0, EntityPhysics::options_t(false, false, 1.f, 0.3f)));
   sprite1->setTextureSection(0, 0, 128, 64);

   unique_ptr<Polygon> poly(new Polygon);
   poly->addVertex(0.f   * gGetPixelSize().x, 0.f  * gGetPixelSize().y);
   poly->addVertex(128.f * gGetPixelSize().x, 0.f  * gGetPixelSize().y);
   poly->addVertex(128.f * gGetPixelSize().x, 64.f * gGetPixelSize().y);
   poly->addVertex(96.f  * gGetPixelSize().x, 32.f * gGetPixelSize().y);
   poly->addVertex(32.f  * gGetPixelSize().x, 32.f * gGetPixelSize().y);
   poly->addVertex(0.f   * gGetPixelSize().x, 64.f * gGetPixelSize().y);

   sprite1->setShape(move(poly));
   sprite1->setTranslation(128.f * gGetPixelSize().x, 128.f * gGetPixelSize().y);
   sprite1->setZ(1);
   sprite1->setScale(Vec2f(2.f, 1.5f));
   sprite1->setRotation(0.f);

//   unique_ptr<Box2dPhysics> spr2Phys(new Box2dPhysics(false, false, 1.f, 0.3f));
//   sprite2 = pPhysicalSprite_t(new PhysicalSprite(move(spr2Phys), internString("sprite2"), internString("shape"), tex1));
   sprite2 = boost::shared_ptr<PhysicalSprite<Box2dPhysics> >(new PhysicalSprite<Box2dPhysics>(internString("sprite2"), internString("shape"), tex1, EntityPhysics::options_t(true, false, 1.f, 0.3f)));
   sprite2->setTranslation(320.f * gGetPixelSize().x, 400.f * gGetPixelSize().y);
   sprite2->setZ(2);
   sprite2->setTextureSection(0, 0, 32, 32);

   unique_ptr<Polygon> poly2(new Polygon);
   poly2->addVertex(16.f * gGetPixelSize().x, 0.f  * gGetPixelSize().y);
   poly2->addVertex(32.f * gGetPixelSize().x, 16.f * gGetPixelSize().y);
   poly2->addVertex(16.f * gGetPixelSize().x, 32.f * gGetPixelSize().y);
   poly2->addVertex(0.f  * gGetPixelSize().x, 16.f * gGetPixelSize().y);

   sprite2->setShape(move(poly2));

   sprite1->addToWorld();
   sprite2->addToWorld();

   while (1) {
      graphics2d.clear(Colour(0.5f, 0.5f, 0.5f, 1.f));
      win.doEvents();
      eventManager.doEvents();
      Box2dPhysics::update();
      keyboard();
      sprite1->update();
      sprite2->update();
      draw();
      computeFrameRate();
      win.swapBuffers();
   }

   return 0;
}
