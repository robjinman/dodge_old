/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2013
 */

#include <sstream>
#include <iostream>
#include "EPendingDeletion.hpp"
#include "Application.hpp"
#include "Soil.hpp"
#include "Item.hpp"


#define TARGET_MEM_USAGE 115000


using namespace std;
using namespace Dodge;


//===========================================
// Application::Application
//===========================================
Application::Application()
   : m_onExit(Functor<void, TYPELIST_0()>(this, &Application::exitDefault)),
     m_renderer(Dodge::Renderer::getInstance()),
     m_frameRate(60.0),
     m_mapLoader(MapLoader::getInstance()) {}

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

   m_entities.clear();
   m_eventManager.clear();
   m_worldSpace.removeAll();
   m_mapLoader.freeAllAssets();
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
      case WinIO::KEY_ADD: {
         m_zoomLevel *= 0.9;
         float32_t w = m_win.getWindowWidth();
         float32_t h = m_win.getWindowHeight();
         float32_t ratio = w / h;
         m_renderer.getCamera().setProjection(ratio * m_zoomLevel, m_zoomLevel);
      }
      break;
      case WinIO::KEY_SUBTRACT: {
         m_zoomLevel *= 1.1;
         float32_t w = m_win.getWindowWidth();
         float32_t h = m_win.getWindowHeight();
         float32_t ratio = w / h;
         m_renderer.getCamera().setProjection(ratio * m_zoomLevel, m_zoomLevel);
      }
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

      m_worldSpace.removeAndUntrackEntity(e->entity);
      e->entity->removeFromWorld();
      m_entities.erase(e->entity->getName());
      m_assetManager.freeAsset(e->entity->getAssetId());
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
      m_entities.erase(entity->getName());
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


   // Construct non-Entity assets

   if (node.name() == "Texture") return pAsset_t(new Texture(node));
   // ...


   // Construct Entities

   pEntity_t entity;

   // Does the XML description reference a prototype?
   if (proto != -1) {
      entity = pEntity_t(dynamic_cast<Entity*>(m_assetManager.cloneAsset(proto)));

      if (!entity)
         throw Exception("Prototype not found", __FILE__, __LINE__);

      // If the prototype has an Item object (aux data), set it to point at the entity.
      IAuxData* p = entity->getAuxDataPtr();
      if (p) {
         Item* item = dynamic_cast<Item*>(p);
         assert(item);

         item->setEntity(entity.get());
      }

      // If this XML node contains the entity along with an Item
      bool hasAuxData = false;
      if (node.name() == "ExtEntity") {
         node = node.firstChild();
         hasAuxData = true;
      }

      // The Entity node comes before the Item node
      entity->assignData(node);

      // Now we construct the Item
      if (hasAuxData) {
         node = node.nextSibling();

         if (node.name() != "Item")
            throw Exception("Expected Item node", __FILE__, __LINE__);

         Item* item = new Item(node);
         item->setEntity(entity.get());

         entity->attachAuxData(unique_ptr<Item>(item));
      }
   }
   // If XML description does not reference a prototype
   else {
      bool hasAuxData = false;
      if (node.name() == "ExtEntity") {
         node = node.firstChild();
         hasAuxData = true;
      }

      if (node.name() == "Player") entity = pEntity_t(new Player(node));
      if (node.name() == "Soil") entity = pEntity_t(new Soil(node));
      if (node.name() == "ParallaxSprite") entity = pEntity_t(new ParallaxSprite(node));
      if (node.name() == "Entity") entity = pEntity_t(new Entity(node));
      if (node.name() == "Sprite") entity = pEntity_t(new Sprite(node));
      if (node.name() == "PhysicalEntity") entity = pEntity_t(new PhysicalEntity<Box2dPhysics>(node));
      if (node.name() == "PhysicalSprite") entity = pEntity_t(new PhysicalSprite<Box2dPhysics>(node));

      if (!entity) {
         Exception ex("Unrecognised entity type '", __FILE__, __LINE__);
         ex.append(node.name());
         ex.append("'");
         throw ex;
      }

      if (hasAuxData) {
         node = node.nextSibling();

         if (node.name() != "Item")
            throw Exception("Expected Item node", __FILE__, __LINE__);

         Item* item = new Item(node);
         item->setEntity(entity.get());

         entity->attachAuxData(unique_ptr<Item>(item));
      }
   }

   if (addToWorld) {
      entity->addToWorld();
      m_worldSpace.insertAndTrackEntity(entity);
      m_entities[entity->getName()] = entity;
   }

   return entity;
}

//===========================================
// Application::update
//===========================================
void Application::update() {
   for (auto i = m_entities.begin(); i != m_entities.end(); ++i)
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
   Vec2f viewSz = cam.getViewSize();

   const Vec2f& mapPos = m_mapLoader.getMapBoundary().getPosition();
   const Vec2f& mapSz = m_mapLoader.getMapBoundary().getSize();

   if (viewPos.x < 0.f) viewPos.x = mapPos.x;
   if (viewPos.y < 0.f) viewPos.y = mapPos.y;

   if (viewPos.x + cam.getViewSize().x > mapPos.x + mapSz.x)
      viewPos.x = mapPos.x + mapSz.x - cam.getViewSize().x;

   if (viewPos.y + cam.getViewSize().y > mapPos.y + mapSz.y)
      viewPos.y = mapPos.y + mapSz.y - cam.getViewSize().y;

   cam.setTranslation(viewPos / m_zoomLevel);
   m_viewArea.setPosition(viewPos);
   m_viewArea.setSize(viewSz);

   m_mapLoader.update(viewPos + viewSz / 2.f);
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
   m_win.init("Terraform", 640, 480, false);

   m_win.registerCallback(WinIO::EVENT_WINCLOSE, Functor<void, TYPELIST_0()>(this, &Application::quit));
   m_win.registerCallback(WinIO::EVENT_KEYDOWN, Functor<void, TYPELIST_1(int)>(this, &Application::keyDown));
   m_win.registerCallback(WinIO::EVENT_KEYUP, Functor<void, TYPELIST_1(int)>(this, &Application::keyUp));
   m_win.registerCallback(WinIO::EVENT_WINRESIZE, Functor<void, TYPELIST_2(int, int)>(this, &Application::onWindowResize));

   m_mapLoader.initialise(Functor<void, TYPELIST_1(const Dodge::XmlNode)>(this, &Application::setMapSettings),
      Functor<Dodge::pAsset_t, TYPELIST_1(const Dodge::XmlNode)>(this, &Application::constructAsset),
      Functor<void, TYPELIST_1(Dodge::pAsset_t)>(this, &Application::deleteAsset),
      TARGET_MEM_USAGE);

   Functor<void, TYPELIST_0()> fMakeContext(&m_win, &WinIO::createGLContext);
   Functor<void, TYPELIST_0()> fSwap(&m_win, &WinIO::swapBuffers);

   m_renderer.start(fMakeContext, fSwap);

   m_eventManager.registerCallback(internString("pendingDeletion"),
      Functor<void, TYPELIST_1(EEvent*)>(this, &Application::deletePending));

   m_zoomLevel = 1.0;
   pCamera_t camera(new Camera(640.0 / 480.0, 1.f));
   m_renderer.attachCamera(camera);

   Box2dPhysics::loadSettings("data/physics.conf");

   stringstream str;
   str << "data/xml/map" << m_currentMap << ".xml";
   m_mapLoader.parseMapFile(str.str());

   m_mapLoader.update(camera->getTranslation());

   for (auto i = m_entities.begin(); i != m_entities.end(); ++i) {
      if (i->first == internString("player")) {
         m_player = boost::dynamic_pointer_cast<Player>(i->second);
         m_entities.erase(i);
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
      Box2dPhysics::step();
      m_eventManager.doEvents();
      draw();
      m_renderer.tick(m_bgColour);

      LOOP_END
   }
}
