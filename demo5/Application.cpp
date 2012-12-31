/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <sstream>
#include <iostream>
#include "EPendingDeletion.hpp"
#include "Application.hpp"
#include "Soil.hpp"
#include "CSprite.hpp"
#include "CParallaxSprite.hpp"
#include "CPhysicalEntity.hpp"
#include "CPhysicalSprite.hpp"


using namespace std;
using namespace Dodge;


//===========================================
// Application::onExit
//===========================================
void Application::onExit(Functor<void, TYPELIST_0()> callBack) {
   m_onExit = callBack;
}

//===========================================
// Application::exitDefault
//===========================================
void Application::exitDefault() {
   exit(0);
}

//===========================================
// Application::quit
//===========================================
void Application::quit() {
   m_renderer.stop();

   m_gameMap.items.clear();
   m_eventManager.clear();
   m_gameMap.worldSpace.removeAll();
   m_gameMap.assetManager.freeAllAssets();
   m_player.reset();
   m_win.destroyWindow();

   m_onExit();
}

//===========================================
// Application::keyDown
//===========================================
void Application::keyDown(int key) {
   if (key == WinIO::KEY_LEFT || key == WinIO::KEY_RIGHT
      || key == WinIO::KEY_UP || key == WinIO::KEY_DOWN) {

      m_dirKeyStack.push_back(key);
   }

   switch (key) {
      case WinIO::KEY_ESCAPE: quit(); break;
#ifdef DEBUG
      case WinIO::KEY_F:
         cout << "Frame rate (main thread): " << m_frameRate << "fps\n";
         cout << "Frame rate (renderer): " << m_renderer.getFrameRate() << "fps\n";
      break;
      case WinIO::KEY_P: m_player->dbg_print(cout); break;
      case WinIO::KEY_1: m_player->dbg_flags ^= Player::DBG_DRAW_SENSORS; break;
      case WinIO::KEY_2: m_player->dbg_flags ^= Player::DBG_DRAW_SHAPE; break;
      case WinIO::KEY_3: dbg_worldSpaceVisible = !dbg_worldSpaceVisible; break;
#endif
   }

   m_keyState[key] = true;
}

//===========================================
// Application::keyUp
//===========================================
void Application::keyUp(int key) {
   m_keyState[key] = false;

   // dirKeyStack is the list of currently depressed directional
   // keys in the order they were pressed.
   for (uint_t i = 0; i < m_dirKeyStack.size(); ++i) {
      if (m_keyState[m_dirKeyStack[i]] == false) {
         m_dirKeyStack.erase(m_dirKeyStack.begin() + i);
         --i;
      }
   }
}

//===========================================
// Application::keyboard
//===========================================
void Application::keyboard() {
   static int missedKeypress = -1;

   // Attempt to move in direction of missed keypress
   if (missedKeypress != -1) {
      switch (missedKeypress) {
         case WinIO::KEY_RIGHT:  if (m_player->moveRight()) missedKeypress = -1; break;
         case WinIO::KEY_LEFT:   if (m_player->moveLeft())  missedKeypress = -1; break;
         case WinIO::KEY_UP:     if (m_player->moveUp())    missedKeypress = -1; break;
         case WinIO::KEY_DOWN:   if (m_player->moveDown())  missedKeypress = -1; break;
      }
   }
   else {
      // Move in the direction of most recently pressed key
      if (m_dirKeyStack.size() > 0) {
         switch (m_dirKeyStack.back()) {
            case WinIO::KEY_RIGHT:
               if (!m_player->moveRight()) {                       // If the player is already moving remember this keypress
                  if (m_player->facingDir() != Player::RIGHT)
                     missedKeypress = WinIO::KEY_RIGHT;
               }
               break;
            case WinIO::KEY_LEFT:
               if (!m_player->moveLeft()) {
                  if (m_player->facingDir() != Player::LEFT)
                     missedKeypress = WinIO::KEY_LEFT;
               }
               break;
            case WinIO::KEY_UP:
               if (!m_player->moveUp()) {
                  if (m_player->facingDir() != Player::UP)
                     missedKeypress = WinIO::KEY_UP;
               }
               break;
            case WinIO::KEY_DOWN:
               if (!m_player->moveDown()) {
                  if (m_player->facingDir() != Player::DOWN)
                     missedKeypress = WinIO::KEY_DOWN;
               }
               break;
         }
      }
   }
}

//===========================================
// Application::computeFrameRate
//===========================================
void Application::computeFrameRate() {
   static Timer timer;

   m_frameRate = 1.0 / timer.getTime();
   timer.reset();
}

//===========================================
// Application::deletePending
//===========================================
void Application::deletePending(EEvent* event) {
   static long pendingDeletionStr = internString("pendingDeletion");

   if (event->getType() == pendingDeletionStr) {
      EPendingDeletion* e = static_cast<EPendingDeletion*>(event);

      m_gameMap.worldSpace.removeAndUntrackEntity(e->item);
      e->item->removeFromWorld();
      m_gameMap.items.erase(e->item->getName());
      m_gameMap.assetManager.freeAsset(e->item->getAssetId());
   }
}

//===========================================
// Application::update
//===========================================
void Application::update() {
   for (map<long, pItem_t>::iterator i = m_gameMap.items.begin(); i != m_gameMap.items.end(); ++i)
      i->second->update();

   m_player->update();

   updateViewArea();
}

//===========================================
// Application::onWindowResize
//===========================================
void Application::onWindowResize(int w, int h) {
   m_renderer.onWindowResize(w, h);
   m_renderer.getCamera().setProjection(static_cast<float32_t>(w) / static_cast<float32_t>(h), 1.f);
}

//===========================================
// Application::draw
//===========================================
void Application::draw() const {
   vector<pEntity_t> visibleEnts;

   m_gameMap.worldSpace.getEntities(m_viewArea, visibleEnts);

   for (uint_t i = 0; i < visibleEnts.size(); ++i)
      visibleEnts[i]->draw();

   m_player->draw();

#ifdef DEBUG
   if (dbg_worldSpaceVisible)
      m_gameMap.worldSpace.dbg_draw(Colour(1.f, 1.f, 1.f, 1.f), 2, 9);
#endif
}

//===========================================
// Application::updateViewArea
//===========================================
void Application::updateViewArea() {
   Camera& cam = m_renderer.getCamera();
   Vec2f viewPos = m_player->getTranslation_abs() - cam.getViewSize() / 2.f;

   if (viewPos.x < 0.f) viewPos.x = m_gameMap.mapBoundary.getPosition().x;
   if (viewPos.y < 0.f) viewPos.y = m_gameMap.mapBoundary.getPosition().y;

   if (viewPos.x + cam.getViewSize().x > m_gameMap.mapBoundary.getPosition().x + m_gameMap.mapBoundary.getSize().x)
      viewPos.x = m_gameMap.mapBoundary.getPosition().x + m_gameMap.mapBoundary.getSize().x - cam.getViewSize().x;

   if (viewPos.y + cam.getViewSize().y > m_gameMap.mapBoundary.getPosition().y + m_gameMap.mapBoundary.getSize().y)
      viewPos.y = m_gameMap.mapBoundary.getPosition().y + m_gameMap.mapBoundary.getSize().y - cam.getViewSize().y;

   cam.setTranslation(viewPos);
   m_viewArea.setPosition(viewPos);
   m_viewArea.setSize(cam.getViewSize());

   m_mapLoader.update(viewPos);
}

//===========================================
// Application::begin
//===========================================
void Application::begin(int argc, char** argv) {
   m_currentMap = 0;

#ifdef DEBUG
   dbg_worldSpaceVisible = false;

   if (argc > 0) {
      for (int i = 0; i < argc; ++i) {
         if (strcmp(argv[i], "-novsync") == 0)
            WinIO::dbg_flags |= WinIO::DBG_NO_VSYNC;
      }
   }
#endif
   m_win.init("Shit Game", 640, 480, false);
   gInitialise();

   m_win.registerCallback(WinIO::EVENT_WINCLOSE, Functor<void, TYPELIST_0()>(this, &Application::quit));
   m_win.registerCallback(WinIO::EVENT_KEYDOWN, Functor<void, TYPELIST_1(int)>(this, &Application::keyDown));
   m_win.registerCallback(WinIO::EVENT_KEYUP, Functor<void, TYPELIST_1(int)>(this, &Application::keyUp));
   m_win.registerCallback(WinIO::EVENT_WINRESIZE, Functor<void, TYPELIST_2(int, int)>(this, &Application::onWindowResize));

   m_renderer.start();

   m_eventManager.registerCallback(internString("pendingDeletion"),
      Functor<void, TYPELIST_1(EEvent*)>(this, &Application::deletePending));

   m_gameMap.worldSpace.init(unique_ptr<Quadtree<pEntity_t> >(new Quadtree<pEntity_t>(1, Range(-1.f, -1.f, 4.f, 4.f))));

   pCamera_t camera(new Camera(640.0 / 480.0, 1.f));
   m_renderer.attachCamera(camera);

   Box2dPhysics::loadSettings("data/physics.conf");

   stringstream str;
   str << "data/xml/map" << m_currentMap << ".xml";
   m_mapLoader.parseMapFile(str.str(), &m_gameMap);

   m_mapLoader.update(camera->getTranslation() + Vec2f(0.1, 0.1));

   for (auto i = m_gameMap.items.begin(); i != m_gameMap.items.end(); ++i) {
      if (i->first == internString("player")) {
         m_player = boost::static_pointer_cast<Player>(i->second);
         m_gameMap.items.erase(i);
         break;
      }
   }

   if (!m_player) throw Exception("m_player is NULL", __FILE__, __LINE__);

   while (1) {
      LOOP_START

      computeFrameRate();
      m_win.doEvents();
      keyboard();
      update();
      Box2dPhysics::update();
      m_eventManager.doEvents();
      draw();
      m_renderer.tick(m_gameMap.bgColour);
      m_win.swapBuffers();

      LOOP_END
   }
}
