#include "TestCase5.hpp"


using namespace std;
using namespace Dodge;


// Tests events propagated by Entity


void TestCase5::setup() {
   m_state = 0;

   m_eventManager.registerCallback(internString("entityTranslation"), Functor<void, TYPELIST_1(const EEvent*)>(this, &TestCase5::onEvent));
   m_eventManager.registerCallback(internString("entityRotation"), Functor<void, TYPELIST_1(const EEvent*)>(this, &TestCase5::onEvent));
   m_eventManager.registerCallback(internString("entityShape"), Functor<void, TYPELIST_1(const EEvent*)>(this, &TestCase5::onEvent));
   m_eventManager.registerCallback(internString("entityBoundingBox"), Functor<void, TYPELIST_1(const EEvent*)>(this, &TestCase5::onEvent));

   m_entities.push_back(pEntity_t(new Entity(internString("Polygon"))));
   m_entities.back()->setShape(unique_ptr<Shape>(new Quad(Vec2f(0.1, 0.1))));
   m_entities.back()->setFillColour(Colour(1.0, 1.0, 0.0, 1.0));
   m_entities.back()->setLineColour(Colour(0.0, 0.0, 0.0, 1.0));
   m_entities.back()->setLineWidth(3);
   m_entities.back()->setTranslation(0.1, 0.1);
   m_entities.back()->setZ(1);
   m_entities.back()->addToWorld();

   m_entities.push_back(pEntity_t(new Entity(internString("Polygon"))));
   m_entities.back()->setShape(unique_ptr<Shape>(new Quad(Vec2f(0.1, 0.1))));
   m_entities.back()->setFillColour(Colour(0.0, 0.0, 1.0, 1.0));
   m_entities.back()->setLineColour(Colour(0.0, 0.0, 0.0, 1.0));
   m_entities.back()->setLineWidth(3);
   m_entities.back()->setTranslation(0.3, 0.1);
   m_entities.back()->setZ(3);
   m_entities.back()->addToWorld();

   m_entities.push_back(pEntity_t(new Entity(internString("Polygon"))));
   m_entities.back()->setShape(unique_ptr<Shape>(new Quad(Vec2f(0.1, 0.1))));
   m_entities.back()->setFillColour(Colour(0.0, 1.0, 0.0, 1.0));
   m_entities.back()->setLineColour(Colour(0.0, 0.0, 0.0, 1.0));
   m_entities.back()->setLineWidth(3);
   m_entities.back()->setTranslation(0.5, 0.1);
   m_entities.back()->setZ(3);
   m_entities.back()->addToWorld();

   m_entities.push_back(pEntity_t(new Entity(internString("Polygon"))));
   m_entities.back()->setShape(unique_ptr<Shape>(new Quad(Vec2f(0.1, 0.1))));
   m_entities.back()->setFillColour(Colour(0.0, 0.0, 0.0, 0.0));
   m_entities.back()->setLineColour(Colour(1.0, 0.0, 0.0, 1.0));
   m_entities.back()->setLineWidth(5);
   m_entities.back()->setZ(5);
   m_entities.back()->addToWorld();
}

void TestCase5::onEvent(const EEvent* event) {
   static long entityTranslationStr = internString("entityTranslation");
   static long entityRotationStr = internString("entityRotation");
   static long entityBoundingBoxStr = internString("entityBoundingBox");
   static long entityShapeStr = internString("entityShape");

   if (event->getType() == entityTranslationStr) {
      const EEntityTranslation* e = static_cast<const EEntityTranslation*>(event);
      if (e->entity != m_entities[2]) return;

      Vec2f v = e->newTransl_abs - e->oldTransl_abs;

      cout << "Translating by (" << v.x << ", " << v.y << ")\n";
      m_entities[3]->translate(v);
   }
   else if (event->getType() == entityRotationStr) {
      const EEntityRotation* e = static_cast<const EEntityRotation*>(event);
      if (e->entity != m_entities[2]) return;

      float32_t r = e->newRotation_abs - e->oldRotation_abs;

      cout << "Rotating by " << r << "\n";
      m_entities[3]->rotate(r);
   }
   else if (event->getType() == entityBoundingBoxStr) {
      const EEntityBoundingBox* e = static_cast<const EEntityBoundingBox*>(event);
      if (e->entity != m_entities[2]) return;

      cout << "Bounding box changed\n";
   }
   else if (event->getType() == entityShapeStr) {
      const EEntityShape* e = static_cast<const EEntityShape*>(event);
      if (e->entity != m_entities[2]) return;

      cout << "Shape changed\n";
   }
}

void TestCase5::setupState1() {
   cout << "STATE 1\n";

   m_entities[2]->translate(0.1f, 0.2f);
   m_entities[2]->rotate(30.f);
}

void TestCase5::setupState2() {
   cout << "STATE 2\n";

   m_entities[1]->addChild(m_entities[2]);
   m_entities[0]->addChild(m_entities[1]);
}

void TestCase5::setupState3() {
   cout << "STATE 3\n";

   m_entities[0]->rotate(20.f);
   m_entities[1]->rotate(45.f);
}

void TestCase5::setupState4() {
   cout << "STATE 4\n";

   m_entities[1]->removeChild(m_entities[2]);
   m_entities[0]->removeChild(m_entities[1]);
}

void TestCase5::setupState5() {
   cout << "STATE 5\n";

   m_entities[1]->addChild(m_entities[2]);
   m_entities[0]->addChild(m_entities[1]);
}

void TestCase5::setupState6() {
   cout << "STATE 6\n";

   m_entities[2]->setTranslation_abs(0.5f, 0.5f);
   m_entities[2]->setRotation_abs(0.f);
}

void TestCase5::setupState7() {
   cout << "STATE 7\n";

   m_entities[0]->setTranslation_abs(0.3f, 0.5f);
   m_entities[0]->setRotation_abs(0.f);
}

void TestCase5::setupState8() {
   cout << "STATE 8\n";

   m_entities[1]->setTranslation_abs(0.1f, 0.5f);
   m_entities[1]->setRotation_abs(0.f);
}

void TestCase5::onKeyDown(int key) {
   switch (key) {
      case WinIO::KEY_SPACE: {
         switch (m_state) {
            case 0:  setupState1(); break;
            case 1:  setupState2(); break;
            case 2:  setupState3(); break;
            case 3:  setupState4(); break;
            case 4:  setupState5(); break;
            case 5:  setupState6(); break;
            case 6:  setupState7(); break;
            case 7:  setupState8(); break;
            default: assert(false); break;
         }
         ++m_state;
      }
      break;
   }
}

void TestCase5::update() {
   for (unsigned int i = 0; i < m_entities.size(); ++i)
      m_entities[i]->update();

   for (unsigned int i = 0; i < m_entities.size(); ++i) {
      const Range& r = m_entities[i]->getBoundary();
      r.dbg_draw(Colour(), Colour(1.0, 0.0, 0.0, 0.5), 1, 5.f);

      m_entities[i]->draw();
   }
}

void TestCase5::terminate() {
   m_entities.clear();
   m_state = 0;

   m_eventManager.unregisterCallback(internString("entityTranslation"), Functor<void, TYPELIST_1(const EEvent*)>(this, &TestCase5::onEvent));
   m_eventManager.unregisterCallback(internString("entityRotation"), Functor<void, TYPELIST_1(const EEvent*)>(this, &TestCase5::onEvent));
   m_eventManager.unregisterCallback(internString("entityShape"), Functor<void, TYPELIST_1(const EEvent*)>(this, &TestCase5::onEvent));
   m_eventManager.unregisterCallback(internString("entityBoundingBox"), Functor<void, TYPELIST_1(const EEvent*)>(this, &TestCase5::onEvent));
}

TestCase5::~TestCase5() {
   terminate();
}
