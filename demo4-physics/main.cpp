#include <map>
#include <vector>
#include <dodge/dodge.hpp>
#include <iostream>


using namespace std;
using namespace Dodge;


WinIO win;
EventManager eventManager;
vector<pEntity_t> entities;
WorldSpace worldSpace;
Graphics2d graphics2d;
map<int, bool> keyState;
double frameRate;
pTexture_t tex0;
pTexture_t tex1;


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

void updateEntities() {
   for (uint_t i = 0; i < entities.size(); ++i)
      entities[i]->update();
}

void drawEntities() {
   for (uint_t i = 0; i < entities.size(); ++i)
      entities[i]->draw();

   graphics2d.setFillColour(Colour(0.f, 0.f, 0.f, 0.f));
   graphics2d.setLineColour(Colour(1.f, 0.f, 0.f, 1.f));
   graphics2d.setLineWidth(3);

   for (uint_t i = 0; i < entities.size(); ++i)
      graphics2d.drawPrimitive(entities[i]->getShape(), entities[i]->getTranslation_abs().x, entities[i]->getTranslation_abs().y, 5);

   graphics2d.setLineColour(Colour(0.f, 1.f, 0.f, 1.f));
   for (uint_t i = 0; i < entities.size(); ++i)
      entities[i]->getBoundary().dbg_draw(6);

   graphics2d.setLineColour(Colour(0.f, 0.f, 1.f, 1.f));
   graphics2d.setLineWidth(1);
   worldSpace.dbg_draw(7);
}

void keyboard() {
   if (keyState[WinIO::KEY_LEFT]) entities[0]->rotate(1.f);
   if (keyState[WinIO::KEY_RIGHT]) entities[0]->rotate(-1.f);
   if (keyState[WinIO::KEY_UP]) entities[0]->scale(1.01f);
   if (keyState[WinIO::KEY_DOWN]) entities[0]->scale(0.99f);
   if (keyState[WinIO::KEY_A]) graphics2d.getCamera()->translate_x(-0.01f);
   if (keyState[WinIO::KEY_D]) graphics2d.getCamera()->translate_x(0.01f);
   if (keyState[WinIO::KEY_W]) graphics2d.getCamera()->translate_y(0.01f);
   if (keyState[WinIO::KEY_S]) graphics2d.getCamera()->translate_y(-0.01f);
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

void btn1Click(int x, int y) {
   y = win.getWindowHeight() - y;

   Renderer renderer;
   Vec2f viewPos = renderer.getCamera().getTranslation();

   // World coords
   float32_t wx = viewPos.x + static_cast<float32_t>(x) * gGetPixelSize().x;
   float32_t wy = viewPos.y + static_cast<float32_t>(y) * gGetPixelSize().y;

   unique_ptr<Polygon> poly(new Polygon);
   poly->addVertex(16.f / 480.f, 0.f  / 480.f);
   poly->addVertex(32.f / 480.f, 16.f / 480.f);
   poly->addVertex(16.f / 480.f, 32.f / 480.f);
   poly->addVertex(0.f  / 480.f, 16.f / 480.f);

   boost::shared_ptr<PhysicalSprite<Box2dPhysics> > sprite(
      new PhysicalSprite<Box2dPhysics>(internString("shape"), tex1, move(poly), EntityPhysics::options_t(true, false, 1.f, 0.3f))
   );

   sprite->setTranslation(wx, wy);
   sprite->setZ(2);
   sprite->setOnScreenSize(32.f / 480.f, 32.f / 480.f);
   sprite->setTextureSection(0, 0, 32, 32);

   sprite->addToWorld();
   entities.push_back(sprite);
   worldSpace.trackEntity(sprite);
}

void eraseEntity(pEntity_t entity) {
   entity->removeFromWorld();
   worldSpace.removeAndUntrackEntity(entity);

   for (uint_t i = 0; i < entities.size(); ++i) {
      if (entities[i] == entity) {
         entities.erase(entities.begin() + i);
         --i;
      }
   }
}

void entityTranslationHandler(EEvent* ev) {
   EEntityTranslation* event = static_cast<EEntityTranslation*>(ev);

   if (event->entity->getTranslation_abs().y < -1.f) {
      eraseEntity(event->entity);
   }
}

int main() {
   Box2dPhysics::loadSettings("physics.conf");
   win.init("Demo4 - physics", 640, 480, false);
   win.registerCallback(WinIO::EVENT_WINCLOSE, Functor<void, TYPELIST_0()>(quit));
   win.registerCallback(WinIO::EVENT_KEYDOWN, Functor<void, TYPELIST_1(int)>(keyDown));
   win.registerCallback(WinIO::EVENT_KEYUP, Functor<void, TYPELIST_1(int)>(keyUp));
   win.registerCallback(WinIO::EVENT_BTN1PRESS, Functor<void, TYPELIST_2(int, int)>(btn1Click));
   win.registerCallback(WinIO::EVENT_WINRESIZE, Functor<void, TYPELIST_2(int, int)>(onWindowResize));

   eventManager.registerCallback(internString("entityTranslation"), Functor<void, TYPELIST_1(EEvent*)>(entityTranslationHandler));

   graphics2d.init(640, 480);
   worldSpace.init(unique_ptr<Quadtree<pEntity_t> >(new Quadtree<pEntity_t>(1, Range(0.f, 0.f, 64.f / 48.f, 1.f))));

   tex0 = pTexture_t(new Texture("slab.png"));
   tex1 = pTexture_t(new Texture("sprite2.png"));

   unique_ptr<Polygon> poly(new Polygon);
   poly->addVertex(0.f   * gGetPixelSize().x, 0.f  * gGetPixelSize().y);
   poly->addVertex(256.f * gGetPixelSize().x, 0.f  * gGetPixelSize().y);
   poly->addVertex(256.f * gGetPixelSize().x, 32.f * gGetPixelSize().y);
   poly->addVertex(0.f  * gGetPixelSize().x, 32.f * gGetPixelSize().y);

   boost::shared_ptr<PhysicalSprite<Box2dPhysics> > sprite(
      new PhysicalSprite<Box2dPhysics>(internString("sprite1"), internString("shape"), tex0, move(poly), EntityPhysics::options_t(false, false, 1.f, 0.3f))
   );

   sprite->setTextureSection(0, 0, 256, 32);
   sprite->setTranslation(0.f, 0.f);
   sprite->setZ(1);
   sprite->setOnScreenSize(256.f * gGetPixelSize().x, 32.f * gGetPixelSize().y);
   sprite->setScale(Vec2f(2.f, 1.5f));
   sprite->setRotation(0.f);

   sprite->addToWorld();
   entities.push_back(sprite);
   worldSpace.trackEntity(sprite);

   while (1) {
      graphics2d.clear(Colour(0.5f, 0.5f, 0.5f, 1.f));
      win.doEvents();
      eventManager.doEvents();
      Box2dPhysics::update();
      keyboard();
      updateEntities();
      drawEntities();
      computeFrameRate();
      win.swapBuffers();
   }

   return 0;
}
