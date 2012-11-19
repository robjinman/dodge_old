#include <sstream>
#include <iostream>
#include <fstream>
#include <cstring>
#include <memory>
#include "EPendingDeletion.hpp"
#include "Application.hpp"
#include "Soil.hpp"
#include "StopBlock.hpp"


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
   m_player.reset();
   m_win.destroyWindow();

   m_onExit();
}

//===========================================
// Application::keyDown
//===========================================
void Application::keyDown(int key) {
   switch (key) {
      case WinIO::KEY_ESCAPE: quit(); break;
      case WinIO::KEY_F: cout << "Frame rate: " << m_frameRate << "fps\n" << flush; break;
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
// Application::keyboard
//===========================================
void Application::keyboard() {
   if (m_keyState[WinIO::KEY_LEFT]) {
      m_player->moveLeft();
   }
   if (m_keyState[WinIO::KEY_UP]) {
      m_player->moveUp();
   }
   if (m_keyState[WinIO::KEY_RIGHT]) {
      m_player->moveRight();
   }
   if (m_keyState[WinIO::KEY_DOWN]) {
      m_player->moveDown();
   }
}

//===========================================
// Application::constructAsset
//
// If proto = -1 asset is *not* constructed from prototype
//===========================================
boost::shared_ptr<Asset> Application::constructAsset(const XmlNode data, long proto) {
   if (data.isNull())
      throw XmlException("Error constructing asset; XML node is empty", __FILE__, __LINE__);

   boost::shared_ptr<Asset> asset;

   if (data.name() == "Texture") {
      asset = boost::shared_ptr<Asset>(new Texture(data));
   }
   else if (data.name() == "Player") {
      if (proto == -1) {
         asset = pPlayer_t(new Player(data));
      }
      else {
         Player* player = dynamic_cast<Player*>(m_assetManager.cloneAsset(proto));

         if (!player)
            throw XmlException("Error constructing asset of type Player; Bad prototype id", __FILE__, __LINE__);

         m_player = pPlayer_t(player);

         m_player->assignData(data);
         m_player->addToWorld();
         m_worldSpace.trackEntity(m_player);

         asset = m_player;
      }
   }
   else if (data.name() == "Soil") {
      if (proto == -1) {
         asset = pSoil_t(new Soil(data));
      }
      else {
         pSoil_t soil(dynamic_cast<Soil*>(m_assetManager.cloneAsset(proto)));

         if (!soil)
            throw XmlException("Error constructing asset of type Soil; Bad prototype id", __FILE__, __LINE__);

         soil->assignData(data);
         soil->addToWorld();
         m_worldSpace.trackEntity(soil);
         m_items[soil->getName()] = soil;

         asset = soil;
      }
   }
   else
      throw XmlException("Error constructing entity; Unrecognised type", __FILE__, __LINE__);

   return asset;
}

//===========================================
// Application::loadAssets_r
//===========================================
void Application::loadAssets_r(const string& file) {
   try {
      XmlDocument doc;

      XmlNode decl = doc.parse(file);
      if (decl.isNull())
         throw XmlException("Expected XML declaration", __FILE__, __LINE__);

      XmlNode node = decl.nextSibling();
      XML_NODE_CHECK(node, ASSETFILE);

      node = node.firstChild();
      if (node.isNull())
         throw XmlException("Expected 'using' or 'assets' tag", __FILE__, __LINE__);

      if (node.name() == "using") {
         XmlNode node_ = node.firstChild();
         while (!node_.isNull() && node_.name() == "file") {
            string path = string("data/xml/").append(node_.getString());
            loadAssets_r(path);

            node_ = node_.nextSibling();
         }

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

         boost::shared_ptr<Asset> asset = constructAsset(node.firstChild(), proto);
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
      pItem_t item = e->getItem();

      m_worldSpace.removeAndUntrackEntity(item);
      item->removeFromWorld();
      m_items.erase(item->getName());
   }
}

//===========================================
// Application::draw
//===========================================
void Application::draw() {
   m_graphics2d.clear(Colour(0.5, 0.6, 0.8, 1.0));

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
// Application::begin
//===========================================
void Application::begin() {
   m_currentMap = 0;

   m_win.init("Shit Game", 640, 480, false);
   m_win.registerCallback(WinIO::EVENT_WINCLOSE, Functor<void, TYPELIST_0()>(this, &Application::quit));
   m_win.registerCallback(WinIO::EVENT_KEYDOWN, Functor<void, TYPELIST_1(int)>(this, &Application::keyDown));
   m_win.registerCallback(WinIO::EVENT_KEYUP, Functor<void, TYPELIST_1(int)>(this, &Application::keyUp));
   m_win.registerCallback(WinIO::EVENT_WINRESIZE, Functor<void, TYPELIST_2(int, int)>(this, &Application::onWindowResize));

   m_worldSpace.init(unique_ptr<Quadtree<pEntity_t> >(new Quadtree<pEntity_t>(1, Range(0.f, 0.f, 64.f / 48.f, 1.f))));

   m_graphics2d.init(640, 480);

   Box2dPhysics::loadSettings("data/physics.conf");

   stringstream strMap;
   strMap << "data/xml/map" << m_currentMap << ".xml";
   loadAssets_r(strMap.str());

   while (1) {
      m_win.doEvents();
      keyboard();
      update();
      Box2dPhysics::update();
      m_eventManager.doEvents();

      draw();
      m_win.swapBuffers();

      computeFrameRate();
   }
}
