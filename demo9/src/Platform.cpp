#include <memory>
#include <dodge/math/math.hpp>
#include <dodge/EEntityCollision.hpp>
#include "Platform.hpp"


using namespace std;
using namespace Dodge;
using namespace boost;


//===========================================
// Platform::Platform
//===========================================
Platform::Platform(float32_t w, float32_t h)
   : Asset(internString("Platform")),
     Entity(internString("platform")),
     PhysicalEntity(internString("platform"),
        unique_ptr<Shape>(new Quad(Vec2f(w, h))),
        EntityPhysics::options_t(false, false, 1.f, 1.f)) {}

//===========================================
// Platform::update
//===========================================
void Platform::update() {
   PhysicalEntity::update();

   for (auto i = m_pendingAddition.begin(); i != m_pendingAddition.end(); ++i) {
      if (m_contacts.find(*i) != m_contacts.end()) continue;

      Vec2f targetTransl = (*i)->getTranslation_abs();
      float32_t targetRot = (*i)->getRotation_abs();

      addChild(*i);

      (*i)->setRotation_abs(targetRot);
      (*i)->setTranslation_abs(targetTransl);

      m_contacts.insert(*i);
   }

   for (auto i = m_pendingRemoval.begin(); i != m_pendingRemoval.end(); ++i) {
      if (m_contacts.find(*i) == m_contacts.end()) continue;

      Vec2f targetTransl = (*i)->getTranslation_abs();
      float32_t targetRot = (*i)->getRotation_abs();

      removeChild(*i);

      (*i)->setRotation_abs(targetRot);
      (*i)->setTranslation_abs(targetTransl);

      m_contacts.erase(*i);
   }

   m_pendingAddition.clear();
   m_pendingRemoval.clear();
}

//===========================================
// Platform::onEvent
//===========================================
void Platform::onEvent(const EEvent* event) {
   PhysicalEntity::onEvent(event);

   static long entityCollisionStr = internString("entityCollision");
   if (event->getType() != entityCollisionStr) return;

   const EEntityCollision* e = static_cast<const EEntityCollision*>(event);

   // We're in the middle of Box2D's step() function, so
   // can't move anything yet.

   assert(e->entityA);

   assert(e->entityA == getSharedPtr());
   assert(e->entityB != getSharedPtr());

   assert(e->entityB.use_count() > 1);

   if (e->incoming) {
      if (m_contacts.find(e->entityB) == m_contacts.end()) {
         m_pendingAddition.insert(e->entityB);
      }
   }
   else {
      m_pendingRemoval.insert(e->entityB);
   }
}

//===========================================
// Platform::~Platform
//===========================================
Platform::~Platform() {
   m_pendingAddition.clear();
   m_pendingRemoval.clear();
}
