#include "TestCase0.hpp"


using namespace std;
using namespace Dodge;


// An entity is given a shape, rotated, and drawn along with its bounding box.


void TestCase0::setup() {
   m_entity0 = pEntity_t(new Entity(internString("BigShape0"), internString("Polygon")));
   m_entity0->setShape(unique_ptr<Shape>(new Quad(Vec2f(0.4, 0.3))));
   m_entity0->setFillColour(Colour(0.0, 0.0, 1.0, 1.0));
   m_entity0->setLineColour(Colour(0.0, 1.0, 0.0, 1.0));
   m_entity0->setLineWidth(3);
   m_entity0->setTranslation(0.4, 0.1);
   m_entity0->setRotation(45.f);

   m_entity0->addToWorld();
}

void TestCase0::update() {
   m_entity0->update();
   m_entity0->draw();

   const Range& r = m_entity0->getBoundary();
   r.dbg_draw(Colour(), Colour(1.0, 0.0, 0.0, 0.5), 1, 5.f);
}

void TestCase0::terminate() {
   m_entity0.reset();
}

TestCase0::~TestCase0() {
   terminate();
}
