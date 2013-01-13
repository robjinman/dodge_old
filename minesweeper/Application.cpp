/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2013
 */

#include <sstream>
#include <iostream>
#include "EPendingDeletion.hpp"
#include "Application.hpp"
#include "Tile.hpp"


#define TARGET_MEM_USAGE 115000


using namespace std;
using namespace Dodge;


//===========================================
// Application::Application
//===========================================
Application::Application()
   : m_onExit(Functor<void, TYPELIST_0()>(this, &Application::exitDefault)),
     m_renderer(Dodge::Renderer::getInstance()),
     m_frameRate(60.0) {

   for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
      m_mouseState[i] = false;
}

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
   m_mapLoader.freeAllAssets();
   m_win.destroyWindow();

   m_onExit();
}

//===========================================
// Application::keyDown
//===========================================
void Application::keyDown(int key) {

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
#endif
   }

   m_keyState[key] = true;
}

//===========================================
// Application::keyUp
//===========================================
void Application::keyUp(int key) {
   m_keyState[key] = false;
}

//===========================================
// Application::mouseLeftClick
//===========================================
void Application::mouseLeftClick(int x, int y) {
   m_mouseState[MOUSE_LEFT_BUTTON] = true;
}

//===========================================
// Application::mouseRightClick
//===========================================
void Application::mouseRightClick(int x, int y) {
   m_mouseState[MOUSE_RIGHT_BUTTON] = true;
}

//===========================================
// Application::mouseLeftRelease
//===========================================
void Application::mouseLeftRelease(int x, int y) {
   m_mouseState[MOUSE_LEFT_BUTTON] = false;
}

//===========================================
// Application::mouseRightRelease
//===========================================
void Application::mouseRightRelease(int x, int y) {
   m_mouseState[MOUSE_RIGHT_BUTTON] = false;
}

//===========================================
// Application::mouseMove
//===========================================
void Application::mouseMove(int x, int y) {

}

//===========================================
// Application::keyboard
//===========================================
void Application::keyboard() {

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
      XML_NODE_CHECK(node, windowSize);
      Vec2i winSz(node.firstChild());

      node = node.nextSibling();
      XML_NODE_CHECK(node, bgColour);
      m_bgColour = Colour(node.firstChild());

      m_win.init("Window Title", winSz.x, winSz.y, false);
      m_renderer.start();

      pCamera_t camera(new Camera(static_cast<float32_t>(winSz.x) / static_cast<float32_t>(winSz.y), 1.f));
      m_renderer.attachCamera(camera);

      const Range& mb = m_mapLoader.getMapBoundary();
      Range boundary(mb.getPosition() - Vec2f(0.1, 0.1), mb.getSize() + Vec2f(0.2, 0.2));
      m_worldSpace.init(unique_ptr<Quadtree<pEntity_t> >(new Quadtree<pEntity_t>(1, boundary)));
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
   long proto = -1;
   bool addToWorld = false;

   XmlAttribute attr = data.firstAttribute();
   attr = attr.nextAttribute();

   if (!attr.isNull() && attr.name() == "proto") {
      proto = attr.getLong();
      attr = attr.nextAttribute();
   }

   if (!attr.isNull() && attr.name() == "addToWorld") {
      addToWorld = attr.getBool();
   }

   XmlNode node = data.firstChild();


   // Construct non-Item assets

   if (node.name() == "Texture") return pAsset_t(new Texture(node));
   // ...


   // Construct Items

   pItem_t item;
   if (proto != -1) {
      item = pItem_t(dynamic_cast<Item*>(m_assetManager.cloneAsset(proto)));
      item->assignData(node);
   }
   else {
      if (node.name() == "Item") item = pItem_t(new Item(node));
      if (node.name() == "Tile") item = pItem_t(new Tile(node));
      // ...
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
   for (auto i = m_items.begin(); i != m_items.end(); ++i)
      i->second->update();
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
/*
   vector<pEntity_t> visibleEnts;

   m_worldSpace.getEntities(m_viewArea, visibleEnts);

   for (uint_t i = 0; i < visibleEnts.size(); ++i)
      visibleEnts[i]->draw();
*/

   for (auto i = m_items.begin(); i != m_items.end(); ++i)
      i->second->draw();

#ifdef DEBUG
   if (dbg_worldSpaceVisible)
      m_worldSpace.dbg_draw(Colour(1.f, 1.f, 1.f, 1.f), 2, 9);
#endif
}

//===========================================
// Application::populateMap
//===========================================
void Application::populateMap() {
   long tileProtoId = 1;

   const Range& mb = m_mapLoader.getMapBoundary();
   const Vec2f& pos = mb.getPosition();
   const Vec2f& sz = mb.getSize();

   pTile_t proto = boost::dynamic_pointer_cast<Tile>(m_assetManager.getAssetPointer(tileProtoId));
   if (!proto)
      throw Exception("Error populating map; No tileProto item defined", __FILE__, __LINE__);

   for (float32_t i = pos.x; i < pos.x + sz.x; i += proto->getOnScreenSize().x) {
      for (float32_t j = pos.y; j < pos.y + sz.y; j += proto->getOnScreenSize().y) {
         pTile_t tile(dynamic_cast<Tile*>(m_assetManager.cloneAsset(tileProtoId)));

         tile->setTranslation(i, j);

         tile->addToWorld();
         m_worldSpace.insertAndTrackEntity(tile);
         m_items[tile->getName()] = tile;
      }
   }
}

//===========================================
// Application::launch
//===========================================
void Application::launch(int argc, char** argv) {
#ifdef DEBUG
   dbg_worldSpaceVisible = false;

   if (argc > 0) {
      for (int i = 0; i < argc; ++i) {
         if (strcmp(argv[i], "-novsync") == 0)
            WinIO::dbg_flags |= WinIO::DBG_NO_VSYNC;
      }
   }
#endif
   gInitialise();

   m_mapLoader.initialise(Functor<void, TYPELIST_1(const Dodge::XmlNode)>(this, &Application::setMapSettings),
      Functor<Dodge::pAsset_t, TYPELIST_1(const Dodge::XmlNode)>(this, &Application::constructAsset),
      Functor<void, TYPELIST_1(Dodge::pAsset_t)>(this, &Application::deleteAsset),
      TARGET_MEM_USAGE);

   m_mapLoader.parseMapFile("data/xml/map0.xml");

   m_win.registerCallback(WinIO::EVENT_WINCLOSE, Functor<void, TYPELIST_0()>(this, &Application::quit));
   m_win.registerCallback(WinIO::EVENT_KEYDOWN, Functor<void, TYPELIST_1(int)>(this, &Application::keyDown));
   m_win.registerCallback(WinIO::EVENT_KEYUP, Functor<void, TYPELIST_1(int)>(this, &Application::keyUp));
   m_win.registerCallback(WinIO::EVENT_MOUSEMOVE, Functor<void, TYPELIST_2(int, int)>(this, &Application::mouseMove));
   m_win.registerCallback(WinIO::EVENT_BTN1PRESS, Functor<void, TYPELIST_2(int, int)>(this, &Application::mouseLeftClick));
   m_win.registerCallback(WinIO::EVENT_BTN3PRESS, Functor<void, TYPELIST_2(int, int)>(this, &Application::mouseRightClick));
   m_win.registerCallback(WinIO::EVENT_BTN1RELEASE, Functor<void, TYPELIST_2(int, int)>(this, &Application::mouseLeftRelease));
   m_win.registerCallback(WinIO::EVENT_BTN3RELEASE, Functor<void, TYPELIST_2(int, int)>(this, &Application::mouseRightRelease));
   m_win.registerCallback(WinIO::EVENT_WINRESIZE, Functor<void, TYPELIST_2(int, int)>(this, &Application::onWindowResize));

   m_eventManager.registerCallback(internString("pendingDeletion"),
      Functor<void, TYPELIST_1(EEvent*)>(this, &Application::deletePending));

   Box2dPhysics::loadSettings("data/physics.conf");

   m_mapLoader.update(m_renderer.getCamera().getTranslation());

//   m_gameState = ST_START_MENU;
   m_gameState = ST_RUNNING;
   populateMap();

   while (1) {
      LOOP_START

      switch (m_gameState) {
         case ST_START_MENU:
            computeFrameRate();
            m_win.doEvents();
            keyboard();
            update();
            m_eventManager.doEvents();
            draw();
            m_renderer.tick(m_bgColour);
            m_win.swapBuffers();
         break;
         case ST_RUNNING:
            computeFrameRate();
            m_win.doEvents();
            keyboard();
            update();
            Box2dPhysics::update();
            m_eventManager.doEvents();
            draw();
            m_renderer.tick(m_bgColour);
            m_win.swapBuffers();
         break;
      }

      LOOP_END
   }
}
