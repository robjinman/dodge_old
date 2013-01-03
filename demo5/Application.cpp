/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2013
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


#define TARGET_MEM_USAGE 112000


using namespace std;
using namespace Dodge;


//===========================================
// Application::Application
//===========================================
Application::Application()
   : m_onExit(Functor<void, TYPELIST_0()>(this, &Application::exitDefault)),
     m_renderer(Dodge::Renderer::getInstance()),
     m_frameRate(60.0),
     m_mapLoader(m_assetManager,
        Functor<void, TYPELIST_1(const Dodge::XmlNode)>(this, &Application::setMapSettings),
        Functor<Dodge::pAsset_t, TYPELIST_1(const Dodge::XmlNode)>(this, &Application::constructAsset),
        Functor<void, TYPELIST_1(Dodge::pAsset_t)>(this, &Application::deleteAsset),
        TARGET_MEM_USAGE) {}

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

   m_items.clear();
   m_eventManager.clear();
   m_worldSpace.removeAll();
   m_assetManager.freeAllAssets();
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
      case WinIO::KEY_M:
         cout << "Memory usage: " << static_cast<float32_t>(m_mapLoader.dbg_getMemoryUsage()) / 1000.0 << "KB\n";
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
// Application::setMapSettings
//===========================================
void Application::setMapSettings(const XmlNode data) {
   try {
      XML_NODE_CHECK(data, customSettings);

      XmlNode node = data.firstChild();
      XML_NODE_CHECK(node, bgColour);
      m_bgColour = Colour(node.firstChild());
   }
   catch (XmlException& e) {
      e.prepend("Error loading map settings; ");
      throw;
   }
}

//===========================================
// Application::deletePending
//===========================================
void Application::deletePending(EEvent* event) {
   static long pendingDeletionStr = internString("pendingDeletion");

   if (event->getType() == pendingDeletionStr) {
      EPendingDeletion* e = static_cast<EPendingDeletion*>(event);

      m_worldSpace.removeAndUntrackEntity(e->item);
      e->item->removeFromWorld();
      m_items.erase(e->item->getName());
      m_assetManager.freeAsset(e->item->getAssetId());
   }
}

//===========================================
// Application::deleteAsset
//===========================================
void Application::deleteAsset(pAsset_t asset) {
   pEntity_t entity = dynamic_pointer_cast<Entity>(asset);

   if (entity) {
      m_worldSpace.removeAndUntrackEntity(entity);
      entity->removeFromWorld();
      m_items.erase(entity->getName());
   }

   m_assetManager.freeAsset(asset->getAssetId());
}

//===========================================
// Application::constructAsset
//===========================================
pAsset_t Application::constructAsset(const XmlNode data) {

   // Construct non-Item assets

   if (data.name() == "Texture") return pAsset_t(new Texture(data));
   // ...


   // Construct Items

   long proto = -1;
   bool addToWorld;

   XmlAttribute attr = data.firstAttribute();
   if (!attr.isNull() && attr.name() == "addToWorld") {
      addToWorld = attr.getBool();
      attr = attr.nextAttribute();
   }

   if (!attr.isNull() && attr.name() == "proto")
      proto = attr.getLong();

   pItem_t item;
   if (proto != -1) {
      item = pItem_t(dynamic_cast<Item*>(m_assetManager.cloneAsset(proto)));
      item->assignData(data);
   }
   else {
      if (data.name() == "Player") item = pItem_t(new Player(data));
      if (data.name() == "Soil") item = pItem_t(new Soil(data));
      if (data.name() == "Item") item = pItem_t(new Item(data));
      if (data.name() == "CParallaxSprite") item = pItem_t(new CParallaxSprite(data));
      if (data.name() == "CSprite") item = pItem_t(new CSprite(data));
      if (data.name() == "CPhysicalEntity") item = pItem_t(new CPhysicalEntity(data));
      if (data.name() == "CPhysicalSprite") item = pItem_t(new CPhysicalSprite(data));
   }

   if (!item)
      throw Exception("Unrecognised item type", __FILE__, __LINE__);

   if (addToWorld) {
      item->addToWorld();
      m_worldSpace.insertAndTrackEntity(item);
      m_items[item->getName()] = item;
   }

   return item;
}

//===========================================
// Application::update
//===========================================
void Application::update() {
   for (map<long, pItem_t>::iterator i = m_items.begin(); i != m_items.end(); ++i)
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

   m_worldSpace.getEntities(m_viewArea, visibleEnts);

   for (uint_t i = 0; i < visibleEnts.size(); ++i)
      visibleEnts[i]->draw();

   m_player->draw();

#ifdef DEBUG
   if (dbg_worldSpaceVisible)
      m_worldSpace.dbg_draw(Colour(1.f, 1.f, 1.f, 1.f), 2, 9);
#endif
}

//===========================================
// Application::updateViewArea
//===========================================
void Application::updateViewArea() {
   Camera& cam = m_renderer.getCamera();
   Vec2f viewPos = m_player->getTranslation_abs() - cam.getViewSize() / 2.f;

   const Vec2f& mapPos = m_mapLoader.getMapBoundary().getPosition();
   const Vec2f& mapSz = m_mapLoader.getMapBoundary().getSize();
/*
   if (viewPos.x < 0.f) viewPos.x = mapPos.x;
   if (viewPos.y < 0.f) viewPos.y = mapPos.y;

   if (viewPos.x + cam.getViewSize().x > mapPos.x + mapSz.x)
      viewPos.x = mapPos.x + mapSz.x - cam.getViewSize().x;

   if (viewPos.y + cam.getViewSize().y > mapPos.y + mapSz.y)
      viewPos.y = mapPos.y + mapSz.y - cam.getViewSize().y;
*/
   cam.setTranslation(viewPos);
   m_viewArea.setPosition(viewPos);
   m_viewArea.setSize(cam.getViewSize());

   m_mapLoader.update(viewPos + cam.getViewSize() / 2.f);
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

   m_worldSpace.init(unique_ptr<Quadtree<pEntity_t> >(new Quadtree<pEntity_t>(1, Range(-1.f, -1.f, 4.f, 4.f))));

   pCamera_t camera(new Camera(640.0 / 480.0, 1.f));
   m_renderer.attachCamera(camera);

   Box2dPhysics::loadSettings("data/physics.conf");

   stringstream str;
   str << "data/xml/map" << m_currentMap << ".xml";
   m_mapLoader.parseMapFile(str.str());

   m_mapLoader.update(camera->getTranslation());

   for (auto i = m_items.begin(); i != m_items.end(); ++i) {
      if (i->first == internString("player")) {
         m_player = boost::static_pointer_cast<Player>(i->second);
         m_items.erase(i);
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
      m_renderer.tick(m_bgColour);
      m_win.swapBuffers();

      LOOP_END
   }
}
