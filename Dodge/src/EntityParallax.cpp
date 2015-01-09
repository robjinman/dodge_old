#include "renderer/Renderer.hpp"
#include "EntityParallax.hpp"


namespace Dodge {


//===========================================
// EntityParallax::EntityParallax
//===========================================
EntityParallax::EntityParallax(Entity* entity, const XmlNode data)
   : m_entity(entity) {

   try {
      XML_NODE_CHECK(data, EntityParallax);

      XmlNode node = data.firstChild();
      XML_NODE_CHECK(node, range);
      m_range = Vec2f(node.firstChild());
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class EntityParallax; ");
      throw;
   }

   init();
}

//===========================================
// EntityParallax::EntityParallax
//===========================================
EntityParallax::EntityParallax(Entity* entity, const Vec2f& range)
   : m_entity(entity), m_range(range) {

   init();
}

//===========================================
// EntityParallax::EntityParallax
//===========================================
EntityParallax::EntityParallax(Entity* entity, const EntityParallax& copy)
   : m_entity(entity), m_range(copy.m_range) {

   init();
}

//===========================================
// EntityParallax::assignData
//===========================================
void EntityParallax::assignData(const XmlNode data) {
   try {
      if (data.isNull() || data.name() != "EntityParallax") return;

      XmlNode node = data.firstChild();
      if (!node.isNull() && node.name() == "range") {
         m_range = Vec2f(node.firstChild());
      }
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class EntityParallax; ");
      throw;
   }

   init();
}

//===========================================
// EntityParallax::getSize
//===========================================
size_t EntityParallax::getSize() const {
   return sizeof(EntityParallax);
}

//===========================================
// EntityParallax::init
//===========================================
void EntityParallax::init() {
   m_pos = m_entity->getTranslation_abs();
}

//===========================================
// EntityParallax::update
//===========================================
void EntityParallax::update() {
   Camera& cam = Renderer::getInstance().getCamera();
   const Range& boundary = m_entity->getBoundary();

   Vec2f viewCentre = cam.getTranslation() + cam.getViewSize() / 2.0;
   Vec2f maxDist = cam.getViewSize() / 2.0;
   Vec2f centre = boundary.getPosition() + (boundary.getSize() / 2.0);
   Vec2f dist = viewCentre - centre;
   Vec2f norm(dist.x / maxDist.x, dist.y / maxDist.y);

   Vec2f transl(norm.x * m_range.x, norm.y * m_range.y);

   // TODO: use setTranslation_abs()
   m_entity->setTranslation(m_pos + transl);
}

#ifdef DEBUG
//===========================================
// EntityParallax::dbg_print
//===========================================
void EntityParallax::dbg_print(std::ostream& out, int tab) const {
   // TODO
}
#endif


}
