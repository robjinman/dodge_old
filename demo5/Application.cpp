#include <sstream>
#include <iostream>
#include <fstream>
#include <cstring>
#include <memory>
#include <dodge/rapidxml/rapidxml.hpp>
#include "EPendingDeletion.hpp"
#include "Application.hpp"
#include "Soil.hpp"
#include "StopBlock.hpp"


using namespace std;
using namespace rapidxml;
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
   m_eventManager.clearAll();
   m_grid->removeAll();
   m_player.reset();
   m_win.destroyWindow();

   m_onExit();
}

//===========================================
// Application::keyDown
//===========================================
void Application::keyDown(int key) {
   switch (key) {
      case KEY_ESCAPE: quit(); break;
      case KEY_F: cout << "Frame rate: " << m_frameRate << "fps\n" << flush; break;
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
   if (m_keyState[KEY_LEFT]) {
      m_player->moveLeft();
   }
   if (m_keyState[KEY_UP]) {
      m_player->moveUp();
   }
   if (m_keyState[KEY_RIGHT]) {
      m_player->moveRight();
   }
   if (m_keyState[KEY_DOWN]) {
      m_player->moveDown();
   }
}

//===========================================
// Application::parseItemFile
//===========================================
pItem_t Application::parseItemFile(const string& file) {
   pItem_t item;

   ifstream fin(file);
   if (!fin.good())
      throw Exception("Error parsing xml file; bad file", __FILE__, __LINE__);

   // Get file length
   fin.seekg (0, ios::end);
   int len = fin.tellg();
   fin.seekg (0, ios::beg);

   // Load data and add null byte
   unique_ptr<char[]> buf(new char[len + 1]);
   buf[len] = '\0';
   fin.read(buf.get(), len);

   xml_document<char> doc;
   try {
      doc.parse<parse_full ^ parse_comment_nodes>(buf.get());
   }
   catch (parse_error& e) {
      stringstream msg;
      msg << "Error parsing xml file: " << e.what() << " at byte "
         << e.where<char>() - buf.get() << " '" << string(e.where<char>(), 50) << "'";

      throw Exception(msg.str(), __FILE__, __LINE__);
   }

   // First node is XML declaration
   xml_node<>* decl = doc.first_node();
   xml_node<>* node = NULL;

   if (decl) node = decl->next_sibling();
   if (node) {

      // Call the correct constructor
      if (strcmp(node->name(), "Player") == 0)
         item = pItem_t(new Player(m_grid, m_zeroGRegion));
      else if (strcmp(node->name(), "Soil") == 0)
         item = pItem_t(new Soil);
      else if (strcmp(node->name(), "StopBlock") == 0)
         item = pItem_t(new StopBlock);
      else
         throw Exception("Error parsing xml file; unknown class name", __FILE__, __LINE__);

      item->assignData(node);
   }
   else
      throw Exception("Error parsing xml file", __FILE__, __LINE__);

   return item;
}

//===========================================
// Application::mapSetup
//===========================================
void Application::mapSetup() {
   try {
      map<long, pItem_t> prototypes;

      stringstream strMap;
      strMap << "data/xml/map" << m_currentMap << ".xml";

      ifstream fin(strMap.str());
      if (!fin.good())
         throw Exception("Error parsing xml file; bad file", __FILE__, __LINE__);

      // Get file length
      fin.seekg (0, ios::end);
      int len = fin.tellg();
      fin.seekg (0, ios::beg);

      // Load data and add null byte
      unique_ptr<char[]> buf(new char[len + 1]);
      buf[len] = '\0';
      fin.read(buf.get(), len);

      xml_document<char> doc;
      try {
         doc.parse<parse_full ^ parse_comment_nodes>(buf.get());
      }
      catch (parse_error& e) {
         stringstream msg;
         msg << "Error parsing xml file: " << e.what() << " at byte "
            << e.where<char>() - buf.get() << " '" << string(e.where<char>(), 50) << "'";

         throw Exception(msg.str(), __FILE__, __LINE__);
      }

      // First node is XML declaration
      xml_node<>* decl = doc.first_node();
      xml_node<>* node0 = NULL;

      if (decl) node0 = decl->next_sibling();
      if (!node0 || strcmp(node0->name(), "gameMap") != 0)
         throw Exception("Error parsing xml file; expected 'gameMap'", __FILE__, __LINE__);

      xml_node<>* node1 = node0->first_node();
      if (!node1 || strcmp(node1->name(), "using") != 0)
         throw Exception("Error parsing xml file; expected 'using'", __FILE__, __LINE__);

      // Load item prototypes
      xml_node<>* node2 = node1->first_node();
      while (node2) {
         if (strcmp(node2->name(), "file") == 0) {
            string path = string("data/xml/").append(node2->value());

            pItem_t proto = parseItemFile(path);
            prototypes[proto->getDefName()] = proto;
         }

         node2 = node2->next_sibling();
      }

      node1 = node1->next_sibling();
      if (!node1 || strcmp(node1->name(), "zeroGRegion") != 0)
         throw Exception("Error parsing xml file; expected 'zeroGRegion'", __FILE__, __LINE__);

      // Load zeroGRegion
      node2 = node1->first_node();
      if (node2) m_zeroGRegion.assignData(node2);

      node1 = node1->next_sibling();
      if (!node1 || strcmp(node1->name(), "entities") != 0)
         throw Exception("Error parsing xml file; expected 'entities'", __FILE__, __LINE__);

      // Spawn instances from prototypes
      node2 = node1->first_node();
      while (node2) {
         if (strcmp(node2->name(), "Player") == 0) {
            pPlayer_t tmp(new Player(m_grid, m_zeroGRegion));

            // Assign data and extract defName (name of prototype)
            tmp->assignData(node2);

            map<long, pItem_t>::iterator it = prototypes.find(tmp->getDefName());
            if (it == prototypes.end())
               throw Exception("Error parsing xml file; prototype not defined", __FILE__, __LINE__);

            pPlayer_t proto = boost::static_pointer_cast<Player>(it->second);
            m_player = pPlayer_t(new Player(*proto, m_grid, m_zeroGRegion));

            // Assign data again to set differences from prototype
            m_player->assignData(node2);

            m_player->addToWorld();
            m_grid->trackEntity(m_player);
         }
         else {
            pItem_t item;

            if (strcmp(node2->name(), "Soil") == 0) {
               pSoil_t tmp(new Soil);
               tmp->assignData(node2);

               map<long, pItem_t>::iterator it = prototypes.find(tmp->getDefName());
               if (it == prototypes.end())
                  throw Exception("Error parsing xml file; prototype not defined", __FILE__, __LINE__);

               pSoil_t proto = boost::static_pointer_cast<Soil>(it->second);
               item = pItem_t(new Soil(*proto));
            }
            else if (strcmp(node2->name(), "StopBlock") == 0) {
               pStopBlock_t tmp(new StopBlock);
               tmp->assignData(node2);

               map<long, pItem_t>::iterator it = prototypes.find(tmp->getDefName());
               if (it == prototypes.end())
                  throw Exception("Error parsing xml file; prototype not defined", __FILE__, __LINE__);

               pStopBlock_t proto = boost::static_pointer_cast<StopBlock>(it->second);
               item = pItem_t(new StopBlock(*proto));
            }

            item->assignData(node2);
            m_items[item->getName()] = item;
            item->addToWorld();
            m_grid->addEntity(item);
            m_grid->trackEntity(item);
         }

         node2 = node2->next_sibling();
      }
   }
   catch (bad_alloc& e) {
      Exception ex("Error loading map; ", __FILE__, __LINE__);
      ex.append(e.what());
      throw ex;
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
void Application::deletePending(pEEvent_t event) {
   static long pendingDeletionStr = internString("pendingDeletion");

   if (event->getType() == pendingDeletionStr) {
      pEPendingDeletion_t e = boost::static_pointer_cast<EPendingDeletion>(event);
      pItem_t item = e->getItem();

      m_grid->removeEntity(item, item->getPosition(), item->getBoundingPoly());
      m_grid->unTrackEntity(item);

      item->removeFromWorld();

      m_items.erase(item->getName());
   }
}

//===========================================
// Application::drawAndUpdateItems
//===========================================
void Application::drawAndUpdateItems() {
   for (map<long, pItem_t>::iterator i = m_items.begin(); i != m_items.end(); ++i) {
      i->second->update();
      i->second->draw(Vec2f(0, 0));
   }

   m_player->update();
   m_player->draw(Vec2f(0, 0));
}

//===========================================
// Application::begin
//===========================================
void Application::begin() {
   m_currentMap = 0;

   initialiseAll("Shit Game", 320, 240, false);
   m_win.registerCallback(GFXEVENT_WINCLOSE, Functor<void, TYPELIST_0()>(this, &Application::quit));
   m_win.registerCallback(GFXEVENT_KEYDOWN, Functor<void, TYPELIST_1(int)>(this, &Application::keyDown));
   m_win.registerCallback(GFXEVENT_KEYUP, Functor<void, TYPELIST_1(int)>(this, &Application::keyUp));

   m_eventManager.registerCallback(internString("pendingDeletion"),
      Functor<void, TYPELIST_1(pEEvent_t)>(this, &Application::deletePending));

   Renderer renderer;

   try {
      m_grid = pGrid_t(new Grid(320 / 16, 240 / 16, 16, 16));
   }
   catch (bad_alloc& e) {
      Exception ex("Error allocating Grid object; ", __FILE__, __LINE__);
      ex.append(e.what());
      throw ex;
   }

   EntityPhysics::loadSettings("data/physics.conf");

   mapSetup();

   while (1) {
      m_win.doEvents();
      renderer.clear();
      m_eventManager.doEvents();
      renderer.drawPlainAlphaPoly(Vec2f(0, 0), m_zeroGRegion, Colour(0.3, 0.3, 0.2, 1.0), 0);
      EntityPhysics::update();
      keyboard();
      drawAndUpdateItems();
      computeFrameRate();
      m_win.swapBuffers();
   }
}
