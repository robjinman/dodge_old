#include "TestCase1.hpp"


using namespace std;
using namespace Dodge;


// An entity added and removed (press space) from another entity.


void TestCase1::setup() {
   m_state = 0;

   m_entities.push_back(pEntity_t(new Entity(internString("BigShape0"), internString("Polygon"))));
   m_entities.back()->setShape(unique_ptr<Shape>(new Quad(Vec2f(0.4, 0.3))));
   m_entities.back()->setFillColour(Colour(0.0, 0.0, 1.0, 1.0));
   m_entities.back()->setLineColour(Colour(0.0, 1.0, 0.0, 1.0));
   m_entities.back()->setLineWidth(3);
   m_entities.back()->setTranslation(0.4, 0.1);
   m_entities.back()->setZ(1);
   m_entities.back()->setRotation(45.f);
   m_entities.back()->addToWorld();

   m_entities.push_back(pEntity_t(new Entity(internString("BigShape1"), internString("Polygon"))));
   m_entities.back()->setShape(unique_ptr<Shape>(new Quad(Vec2f(0.15, 0.075))));
   m_entities.back()->setFillColour(Colour(1.0, 0.0, 1.0, 1.0));
   m_entities.back()->setLineColour(Colour(0.0, 1.0, 0.0, 1.0));
   m_entities.back()->setLineWidth(3);
   m_entities.back()->setTranslation(0.5, 0.4);
   m_entities.back()->setZ(2);
   m_entities.back()->rotate(-90.f);
   m_entities.back()->addToWorld();
}

void TestCase1::onKeyDown(int key) {
   switch (key) {
      case WinIO::KEY_SPACE:
         if (m_state == 0) {
            m_entities[0]->addChild(m_entities[1]);
            m_state = 1;
         }
         else {
            m_entities[0]->removeChild(m_entities[1]);
            m_state = 0;
         }
      break;
   }
}

void TestCase1::update() {
   for (unsigned int i = 0; i < m_entities.size(); ++i)
      m_entities[i]->update();

   for (unsigned int i = 0; i < m_entities.size(); ++i) {
      const Range& r = m_entities[i]->getBoundary();
      r.dbg_draw(Colour(), Colour(1.0, 0.0, 0.0, 0.5), 1, 5.f);

      m_entities[i]->draw();
   }
}

void TestCase1::terminate() {
   m_entities.clear();
}

TestCase1::~TestCase1() {
   terminate();
}
