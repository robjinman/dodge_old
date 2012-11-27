#include <sstream>
#include <iostream>
#include "EPendingDeletion.hpp"
#include "Application.hpp"
#include "Soil.hpp"
#include "CPhysicalEntity.hpp"
#include "CPhysicalSprite.hpp"
#include "CShape.hpp"
#include "CSprite.hpp"


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
      case WinIO::KEY_F: cout << "Frame rate: " << m_frameRate << "fps\n"; break;
      case WinIO::KEY_P: m_player->dbg_print(cout); break;
      case WinIO::KEY_1: m_player->dbg_flags ^= Player::DBG_DRAW_SENSORS; break;
      case WinIO::KEY_2: m_player->dbg_flags ^= Player::DBG_DRAW_SHAPE; break;
      case WinIO::KEY_3: dbg_flags ^= DBG_DRAW_WORLDSPACE; break;
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
// Application::loadMapSettings
//===========================================
void Application::loadMapSettings(const XmlNode data) {
   try {
      XML_NODE_CHECK(data, settings);

      XmlNode node = data.firstChild();
      XML_NODE_CHECK(node, bgColour);
      m_bgColour = Colour(node.firstChild());

      node = node.nextSibling();
      XML_NODE_CHECK(node, dimensions);
      m_mapSize = Vec2f(node.firstChild());

      node = node.nextSibling();
      XML_NODE_CHECK(node, fillerTile);

      XmlAttribute attr = node.firstAttribute();
      XML_ATTR_CHECK(attr, id);
      m_fillerTileId = attr.getLong();
   }
   catch (XmlException& e) {
      e.prepend("Error loading map settings; ");
      throw;
   }
}

//===========================================
// Application::constructItem
//===========================================
boost::shared_ptr<Item> Application::constructItem(const XmlNode data) const {
   if (data.name() == "Player") return pItem_t(new Player(data));
   if (data.name() == "Soil") return pItem_t(new Soil(data));
   if (data.name() == "CShape") return pItem_t(new CShape(data));
   if (data.name() == "CSprite") return pItem_t(new CSprite(data));
   if (data.name() == "CPhysicalEntity") return pItem_t(new CPhysicalEntity(data));
   if (data.name() == "CPhysicalSprite") return pItem_t(new CPhysicalSprite(data));

   throw Exception("Unrecognised item type", __FILE__, __LINE__);
}

//===========================================
// Application::constructAsset
//
// If proto = -1 asset is *not* constructed from prototype
//===========================================
boost::shared_ptr<Asset> Application::constructAsset(const XmlNode data, long proto, bool addToWorld) {
   if (data.isNull())
      throw XmlException("Error constructing asset; XML node is empty", __FILE__, __LINE__);

   boost::shared_ptr<Asset> asset;

   // Construct non-Item assets
   if (data.name() == "Texture") {
      asset = boost::shared_ptr<Asset>(new Texture(data));
   }
   // Construct Items
   else {
      pItem_t item;

      if (proto == -1) {
         asset = item = constructItem(data);
      }
      else {
         asset = item = pItem_t(dynamic_cast<Item*>(m_assetManager.cloneAsset(proto)));

         if (!item)
            throw XmlException("Error constructing asset; Bad prototype id", __FILE__, __LINE__);

         item->assignData(data);
      }

      if (addToWorld) {
         item->addToWorld();
         m_worldSpace.insertAndTrackEntity(item);

         if (data.name() == "Player")
            m_player = boost::dynamic_pointer_cast<Player>(item);
         else
            m_items[item->getName()] = item;
      }
   }

   return asset;
}

//===========================================
// Application::loadAssets_r
//===========================================
void Application::loadAssets_r(const string& file, int depth) {
   try {
      XmlDocument doc;

      XmlNode decl = doc.parse(file);
      if (decl.isNull())
         throw XmlException("Expected XML declaration", __FILE__, __LINE__);

      XmlNode node = decl.nextSibling();
      XML_NODE_CHECK(node, ASSETFILE);

      node = node.firstChild();
      if (node.isNull())
         throw XmlException("Expected 'using', 'settings', or 'assets' tag", __FILE__, __LINE__);

      if (node.name() == "using") {
         XmlNode node_ = node.firstChild();
         while (!node_.isNull() && node_.name() == "file") {
            string path = string("data/xml/").append(node_.getString());
            loadAssets_r(path, depth + 1);

            node_ = node_.nextSibling();
         }

         node = node.nextSibling();
      }

      if (!node.isNull() && node.name() == "settings") {
         loadMapSettings(node);
         node = node.nextSibling();
      }

      XML_NODE_CHECK(node, assets);

      node = node.firstChild();
      while (!node.isNull() && node.name() == "asset") {
         XmlAttribute attr = node.firstAttribute();
         XML_ATTR_CHECK(attr, id);
         long id = attr.getLong();

         long proto = -1;
         attr = attr.nextAttribute();
         if (!attr.isNull() && attr.name() == "proto")
            proto = attr.getLong();

         boost::shared_ptr<Asset> asset = constructAsset(node.firstChild(), proto, depth == 0 ? true : false);
         m_assetManager.addAsset(id, asset);

         node = node.nextSibling();
      }
   }
   catch (XmlException& e) {
      e.prepend("Error loading assets from XML file; ");
      throw;
   }
}

//===========================================
// Application::computeFrameRate
//===========================================
void Application::computeFrameRate() {
   static Timer timer;
   static long i = 0;
   i++;

   if (i % 100 == 0) {
      m_frameRate = 100.0 / timer.getTime();
      timer.reset();
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
// Application::draw
//===========================================
void Application::draw() {
   Renderer renderer;
   renderer.clear();

#ifdef DEBUG
   if (dbg_flags & DBG_DRAW_WORLDSPACE) {
      m_graphics2d.setLineWidth(2);
      m_graphics2d.setLineColour(Colour(1.f, 0.f, 0.f, 1.f));
      m_worldSpace.dbg_draw(5);
   }
#endif

   for (map<long, pItem_t>::iterator i = m_items.begin(); i != m_items.end(); ++i)
      i->second->draw();

   m_player->draw();
}

//===========================================
// Application::update
//===========================================
void Application::update() {
   m_graphics2d.clear(Colour(0.5, 0.6, 0.8, 1.0));

   for (map<long, pItem_t>::iterator i = m_items.begin(); i != m_items.end(); ++i)
      i->second->update();

   m_player->update();

   pCamera_t cam = m_graphics2d.getCamera();
   cam->setTranslation(m_player->getTranslation_abs() - cam->getViewSize() / 2.f);
}

//===========================================
// Application::onWindowResize
//===========================================
void Application::onWindowResize(int w, int h) {
   Renderer renderer;
   renderer.onWindowResize(w, h);
   m_graphics2d.getCamera()->setProjection(static_cast<float32_t>(w) / static_cast<float32_t>(h), 1.f);
}

//===========================================
// Application::loadMap
//===========================================
void Application::loadMap() {
   static long stopBlockStr = internString("stopBlock");

   stringstream strMap;
   strMap << "data/xml/map" << m_currentMap << ".xml";
   loadAssets_r(strMap.str());

   pItem_t filler = boost::dynamic_pointer_cast<Item>(m_assetManager.getAssetPointer(m_fillerTileId));

   float32_t w = filler->getBoundary().getSize().x;
   float32_t h = filler->getBoundary().getSize().y;

   unique_ptr<Primitive> shape(filler->getShape().clone());
   shape->scale(Vec2f(0.9f, 0.9f));

   for (float32_t x = 0.f; x < m_mapSize.x; x += w) {
      for (float32_t y = 0.f; y < m_mapSize.y; y += h) {

         vector<pEntity_t> vec;
         m_worldSpace.getEntities(filler->getBoundary(), vec);
         bool clear = true;
         for (uint_t i = 0; i < vec.size(); ++i) {
            if (!vec[i]->hasShape()) continue;
            if (vec[i]->getTypeName() == stopBlockStr) continue;

            if (Math::overlap(*shape, Vec2f(x + 0.005, y + 0.005), vec[i]->getShape(), vec[i]->getTranslation_abs())) {
               clear = false;
               break;
            }
         }

         if (clear) {
            pItem_t item(filler->clone());
            item->setTranslation(x, y);

            item->addToWorld();
            m_worldSpace.trackEntity(item);
            m_items[item->getName()] = item;
         }
      }
   }
}

//===========================================
// Application::begin
//===========================================
void Application::begin() {
   m_currentMap = 0;

   m_win.init("Shit Game", 640, 480, false);
   m_win.registerCallback(WinIO::EVENT_WINCLOSE, Functor<void, TYPELIST_0()>(this, &Application::quit));
   m_win.registerCallback(WinIO::EVENT_KEYDOWN, Functor<void, TYPELIST_1(int)>(this, &Application::keyDown));
   m_win.registerCallback(WinIO::EVENT_KEYUP, Functor<void, TYPELIST_1(int)>(this, &Application::keyUp));
   m_win.registerCallback(WinIO::EVENT_WINRESIZE, Functor<void, TYPELIST_2(int, int)>(this, &Application::onWindowResize));

   m_eventManager.registerCallback(internString("pendingDeletion"),
      Functor<void, TYPELIST_1(EEvent*)>(this, &Application::deletePending));

   m_worldSpace.init(unique_ptr<Quadtree<pEntity_t> >(new Quadtree<pEntity_t>(1, Range(-1.f, -1.f, 4.f, 4.f))));

   m_graphics2d.init(640, 480);

   Box2dPhysics::loadSettings("data/physics.conf");

   loadMap();

   Renderer renderer;
   while (1) {
      m_win.doEvents();
      keyboard();
      update();
      Box2dPhysics::update();
      m_eventManager.doEvents();

      draw();

      renderer.render();
      m_win.swapBuffers();

      computeFrameRate();
   }
}
