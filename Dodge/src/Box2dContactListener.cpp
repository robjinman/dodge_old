#include <Box2dContactListener.hpp>
#include <EEntityCollision.hpp>
#include <Entity.hpp>


namespace Dodge {


//===========================================
// Box2dContactListener::BeginContact
//===========================================
void Box2dContactListener::BeginContact(b2Contact* contact) {
   void* udA = contact->GetFixtureA()->GetBody()->GetUserData();
   void* udB = contact->GetFixtureB()->GetBody()->GetUserData();

   if (udA && udB) {
      Entity* entA = reinterpret_cast<Entity*>(udA);
      Entity* entB = reinterpret_cast<Entity*>(udB);

      EEvent* event1 = new EEntityCollision(true, entA->getSharedPtr(), entB->getSharedPtr());
      EEvent* event2 = new EEntityCollision(true, entB->getSharedPtr(), entA->getSharedPtr());

      entA->onEvent(event1);
      entB->onEvent(event2);

      m_eventManager.queueEvent(event1);
   }
}

//===========================================
// Box2dContactListener::EndContact
//===========================================
void Box2dContactListener::EndContact(b2Contact* contact) {
   void* udA = contact->GetFixtureA()->GetBody()->GetUserData();
   void* udB = contact->GetFixtureB()->GetBody()->GetUserData();

   if (udA && udB) {
      Entity* entA = reinterpret_cast<Entity*>(udA);
      Entity* entB = reinterpret_cast<Entity*>(udB);

      EEvent* event1 = new EEntityCollision(false, entA->getSharedPtr(), entB->getSharedPtr());
      EEvent* event2 = new EEntityCollision(false, entB->getSharedPtr(), entA->getSharedPtr());

      entA->onEvent(event1);
      entB->onEvent(event2);

      m_eventManager.queueEvent(event1);
   }
}


}
