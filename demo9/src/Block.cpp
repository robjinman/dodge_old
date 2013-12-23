#include <memory>
#include <dodge/math/math.hpp>
#include <dodge/EEntityCollision.hpp>
#include "Block.hpp"


using namespace std;
using namespace Dodge;


//===========================================
// Block::Block
//===========================================
Block::Block()
   : Asset(internString("Block")),
     Entity(internString("block")),
     PhysicalEntity(internString("block"),
        unique_ptr<Shape>(new Quad(Vec2f(0.05f, 0.05f))),
        EntityPhysics::options_t(true, false, 1.f, 1.f)) {

   m_contacts = 0;

   setFillColour(Colour(0.4, 0.4, 1.0, 1.0));
   setLineColour(Colour(0.0, 1.0, 0.0, 1.0));
   setLineWidth(2);
}

//===========================================
// Block::onEvent
//===========================================
void Block::onEvent(const EEvent* event) {
   PhysicalEntity::onEvent(event);

   static long entityCollisionStr = internString("entityCollision");

   if (event->getType() != entityCollisionStr) return;

   const EEntityCollision* e = static_cast<const EEntityCollision*>(event);

   if (e->incoming)
      ++m_contacts;
   else
      --m_contacts;

   assert(m_contacts >= 0);

   if (m_contacts > 0)
      setLineColour(Colour(0.0, 0.0, 0.0, 1.0));
   else
      setLineColour(Colour(0.0, 1.0, 0.0, 1.0));
}
