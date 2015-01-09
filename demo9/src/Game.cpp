#include <iostream>
#include <memory>
#include "Game.hpp"
#include "Block.hpp"
#include "Platform.hpp"


using namespace std;
using namespace Dodge;


//===========================================
// Game::quit
//===========================================
void Game::quit() {
   m_pendingExit = true;
}

//===========================================
// Game::onKeyDown
//===========================================
void Game::onKeyDown(int key) {
   m_keyStates[key] = true;

   switch (key) {
      case WinIO::KEY_F:
         cout << "Framerate = " << m_frameRate << "\n";
      break;
      case WinIO::KEY_ESCAPE:
         quit();
      break;
   }
}

//===========================================
// Game::onKeyUp
//===========================================
void Game::onKeyUp(int key) {
   m_keyStates[key] = false;
}

//===========================================
// Game::onLeftClick
//===========================================
void Game::onLeftClick(int x, int y) {
   WinIO win;
   Renderer& renderer = Renderer::getInstance();

   y = win.getWindowHeight() - y;

   Vec2f viewPos = renderer.getCamera().getTranslation();

   // World coords
   float32_t wx = viewPos.x + static_cast<float32_t>(x) * gGetPixelSize().x;
   float32_t wy = viewPos.y + static_cast<float32_t>(y) * gGetPixelSize().y;

   vector<pEntity_t> ents;
   m_worldSpace.getEntities(Range(wx, wy, 0.001, 0.001), ents);

   for (auto i = ents.begin(); i != ents.end(); ++i) {
      const Range& r = (*i)->getBoundary();
      if (r.contains(Vec2f(wx, wy))) {
         cout << getInternedString((*i)->getName()) << "\n";

         Entity* parent = (*i)->getParent();
         cout << "Parent: " << (parent ? getInternedString(parent->getName()) : "NULL") << "\n";

         cout << "Rotation: " << (*i)->getRotation_abs() << "\n";
      }
   }
}

//===========================================
// Game::onRightClick
//===========================================
void Game::onRightClick(int x, int y) {
   WinIO win;
   Renderer& renderer = Renderer::getInstance();

   y = win.getWindowHeight() - y;

   Vec2f viewPos = renderer.getCamera().getTranslation();

   // World coords
   float32_t wx = viewPos.x + static_cast<float32_t>(x) * gGetPixelSize().x;
   float32_t wy = viewPos.y + static_cast<float32_t>(y) * gGetPixelSize().y;

   pEntity_t ent(new Block);
   ent->setTranslation(wx, wy);
   ent->setZ(3);
   ent->addToWorld();
   m_worldSpace.trackEntity(ent);

   m_entities.push_back(ent);
}

//===========================================
// Game::onWindowResize
//===========================================
void Game::onWindowResize(int w, int h) {
   Renderer& renderer = Renderer::getInstance();

   renderer.onWindowResize(w, h);
   renderer.getCamera().setProjection(static_cast<float32_t>(w) / static_cast<float32_t>(h), 1.f);
}

//===========================================
// Game::computeFrameRate
//===========================================
void Game::computeFrameRate() {
   static Timer timer;

   m_frameRate = 1.0 / timer.getTime();
   timer.reset();
}

//===========================================
// Game::keyboard
//===========================================
void Game::keyboard() {
   if (m_keyStates[WinIO::KEY_LEFT])
      m_ePlatform1->rotate(0.5f);

   if (m_keyStates[WinIO::KEY_RIGHT])
      m_ePlatform1->rotate(-0.5f);

   if (m_keyStates[WinIO::KEY_UP])
      m_ePlatform0->translate_y(-0.005);

   if (m_keyStates[WinIO::KEY_DOWN])
      m_ePlatform0->translate_y(0.005);
}

//===========================================
// Game::launch
//===========================================
int Game::launch(int argc, char** argv) {
   m_pendingExit = false;
   m_frameRate = 0.0;

   projectSettings_t settings;
   settings.targetFrameRate = 120;
   settings.globalStackSize = 102400;
   settings.workingDir = ".";

   gInitialise(settings);

   Box2dPhysics::loadSettings("physics.conf");

   EventManager eventManager;

   WinIO win;
   win.init("Dodge Engine :: Shapes and Physics", 600, 480);
   win.createGLContext();

   win.registerCallback(WinIO::EVENT_WINCLOSE, Functor<void, TYPELIST_0()>(this, &Game::quit));
   win.registerCallback(WinIO::EVENT_KEYDOWN, Functor<void, TYPELIST_1(int)>(this, &Game::onKeyDown));
   win.registerCallback(WinIO::EVENT_KEYUP, Functor<void, TYPELIST_1(int)>(this, &Game::onKeyUp));
   win.registerCallback(WinIO::EVENT_BTN1PRESS, Functor<void, TYPELIST_2(int, int)>(this, &Game::onLeftClick));
   win.registerCallback(WinIO::EVENT_BTN3PRESS, Functor<void, TYPELIST_2(int, int)>(this, &Game::onRightClick));
   win.registerCallback(WinIO::EVENT_WINRESIZE, Functor<void, TYPELIST_2(int, int)>(this, &Game::onWindowResize));

   m_worldSpace.init(unique_ptr<Quadtree<pEntity_t> >(new Quadtree<pEntity_t>(1, Range(0.f, 0.f, 64.f / 48.f, 1.f))));

   Renderer& renderer = Renderer::getInstance();

   Functor<void, TYPELIST_0()> fMakeContext(&win, &WinIO::createGLContext);
   Functor<void, TYPELIST_0()> fSwap(&win, &WinIO::swapBuffers);

   renderer.start(fMakeContext, fSwap);

   unique_ptr<Polygon> poly(new Polygon);
   poly->addVertex(0.04, 0.17);
   poly->addVertex(0.00, 0.09);
   poly->addVertex(0.04, 0.01);
   poly->addVertex(0.13, 0.00);
   poly->addVertex(0.20, 0.05);
   poly->addVertex(0.19, 0.14);
   poly->addVertex(0.12, 0.20);

   m_ePoly = pEntity_t(new Entity(internString("BigShape0"), internString("Polygon")));
   m_ePoly->setShape(move(poly));
   m_ePoly->setFillColour(Colour(1.0, 0.0, 0.0, 1.0));
   m_ePoly->setLineColour(Colour(0.0, 1.0, 0.0, 1.0));
   m_ePoly->setLineWidth(3);

   const Range& box = m_ePoly->getBoundary();
   Vec2f m = box.getSize() / 2.f;
   Vec2f p = Vec2f(0.3f, 0.5f) - m;

   m_ePoly->setTranslation(p);

   m_ePoly->addToWorld();
   m_worldSpace.trackEntity(m_ePoly);

   m_ePlatform0 = pEntity_t(new Platform(0.2, 0.01));
   m_ePlatform0->setLineColour(Colour(0.0, 0.0, 1.0, 1.0));
   m_ePlatform0->setLineWidth(2);

   m_ePoly->addChild(m_ePlatform0);

   m_ePlatform0->setTranslation(0.2, 0.2);
   m_ePlatform0->setZ(5);

   m_ePlatform0->addToWorld();
   m_worldSpace.trackEntity(m_ePlatform0);

   m_ePlatform1 = pEntity_t(new Platform(0.2, 0.01));
   m_ePlatform1->setLineColour(Colour(0.0, 1.0, 0.0, 1.0));
   m_ePlatform1->setLineWidth(2);

   m_ePlatform0->addChild(m_ePlatform1);

   m_ePlatform1->setTranslation(0.2, 0.2);
   m_ePlatform1->setZ(5);

   m_ePlatform1->addToWorld();
   m_worldSpace.trackEntity(m_ePlatform1);

   m_eFloor = pEntity_t(
      new PhysicalEntity<Box2dPhysics>(
         internString("floor0"), internString("floor"),
         unique_ptr<Shape>(new Quad(Vec2f(10.0, 0.1))),
         EntityPhysics::options_t(false, false, 1.f, 10.f)
      )
   );

   m_eFloor->setLineColour(Colour(1.0, 1.0, 1.0, 1.0));
   m_eFloor->setLineWidth(2);
   m_eFloor->setFillColour(Colour(0.5, 0.0, 0.5, 1.0));
   m_eFloor->setTranslation(-5.0, 0.0);

   m_eFloor->addToWorld();

//   float32_t a = 0.f;
   while (!m_pendingExit) {
      LOOP_START

      computeFrameRate();

      win.doEvents();
      eventManager.doEvents();
      Box2dPhysics::step();

      keyboard();

      for (auto i = m_entities.begin(); i != m_entities.end(); ++i) {
         (*i)->update();
         (*i)->draw();
      }

      m_ePoly->rotate(1.2, m);
      m_ePlatform0->rotate(-1.2);

//      m_ePlatform1->setRotation(20.f * sin(DEG_TO_RAD(a)));
//      a += 1.f;

      m_eFloor->update();
      m_ePoly->update();
      m_ePlatform0->update();
      m_ePlatform1->update();

      m_eFloor->draw();
      m_ePoly->draw();
      m_ePlatform0->draw();
      m_ePlatform1->draw();

      m_worldSpace.dbg_draw(Colour(0.f, 0.f, 1.f, 1.f), 1, 7.f);

      renderer.tick(Colour(0.4, 0.4, 0.4, 1.0));

      LOOP_END
   }

   return EXIT_SUCCESS;
}
