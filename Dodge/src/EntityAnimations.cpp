/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <cstring>
#include "Texture.hpp"
#include "EntityAnimations.hpp"
#include "StringId.hpp"


using namespace std;
using namespace rapidxml;


namespace Dodge {


Graphics2d EntityAnimations::m_graphics2d = Graphics2d();


#ifdef DEBUG
//===========================================
// EntityAnimations::dbg_print
//===========================================
void EntityAnimations::dbg_print(std::ostream& out, int tab) const {
   for (int i = 0; i < tab; i++) out << "\t";
   out << "EntityAnimations\n";

   for (map<long, Animation*>::const_iterator it = m_animations.begin(); it != m_animations.end(); ++it)
      it->second->dbg_print(out, tab + 1);
}
#endif

//===========================================
// EntityAnimations::EntityAnimations
//
// Construct deep copy.
//===========================================
EntityAnimations::EntityAnimations(const EntityAnimations& copy, Entity* entity)
   : m_entity(entity), m_activeAnim() {

   m_texSection = copy.m_texSection;
   m_texture = copy.m_texture;

   try {
      map<long, Animation*>::const_iterator it = copy.m_animations.begin();
      while (it != copy.m_animations.end()) {
         Animation* anim(new Animation(*(it->second)));
         m_animations[it->first] = anim;
         ++it;
      }
   }
   catch (bad_alloc& e) {
      Exception ex("Error constructing EntityAnimations; ", __FILE__, __LINE__);
      ex.append(e.what());
      throw ex;
   }
}

//===========================================
// EntityAnimations::assignData
//===========================================
void EntityAnimations::assignData(const xml_node<>* data) {
   if (strcmp(data->name(), "EntityAnimations") != 0)
      throw Exception("Error parsing XML for instance of class EntityAnimations", __FILE__, __LINE__);

   const xml_node<>* node = data->first_node();
   if (node && strcmp(node->name(), "Image") == 0) {
      xml_attribute<>* attr = node->first_attribute();

      string path;
      int w, h;

      if (attr && strcmp(attr->name(), "path") == 0) {
         path.assign(attr->value());
         attr = attr->next_attribute();

         if (attr && strcmp(attr->name(), "w") == 0) {
            sscanf(attr->value(), "%d", &w);
            attr = attr->next_attribute();

            if (attr && strcmp(attr->name(), "h") == 0) {
               sscanf(attr->value(), "%d", &h);

               m_texture = pTexture_t(new Texture(path.data()));
            }
         }
      }
      node = node->next_sibling();
   }
   if (node && strcmp(node->name(), "textureSection") == 0) {
      const xml_node<>* child = node->first_node();
      if (child) m_texSection.assignData(child);
   }
   while (node) {
      if (strcmp(node->name(), "Animation") == 0) {
         Animation* anim(new Animation());
         anim->assignData(node);
         m_animations[anim->getName()] = anim;
      }

      node = node->next_sibling();
   }
}

//===========================================
// EntityAnimations::draw
//===========================================
void EntityAnimations::draw(const Vec2f& at) const {
   float32_t x = m_entity->getMatrix()(0, 3) - at.x;
   float32_t y = m_entity->getMatrix()(1, 3) - at.y;
   float32_t z = m_entity->getMatrix()(2, 3);
   float32_t w = m_texSection.getSize().x * m_entity->getScale().x;
   float32_t h = m_texSection.getSize().y * m_entity->getScale().y;
   float32_t tx = m_texSection.getPosition().x;
   float32_t ty = m_texSection.getPosition().y;
   float32_t tw = m_texSection.getSize().x;
   float32_t th = m_texSection.getSize().y;
   float32_t a = 0.f; // TODO

   m_graphics2d.drawImage(*m_texture, x, y, z, w, h, tx, ty, tw, th, a);
}

//===========================================
// EntityAnimations::playAnimation
//
// Plays an animation if it's not already playing.
//===========================================
bool EntityAnimations::playAnimation(long name) {
   if (getAnimState() == Animation::PLAYING) return false;

   map<long, Animation*>::iterator it = m_animations.find(name);
   if (it == m_animations.end()) return false;

   m_activeAnim = it->second;
   return m_activeAnim->play();
}

//===========================================
// EntityAnimations::update
//===========================================
void EntityAnimations::update() {
   map<long, Animation*>::iterator it = m_animations.begin();
   while (it != m_animations.end()) {
      it->second->update();

      const AnimFrame* frame = it->second->getCurrentFrame();
      if (frame) {
         setTextureSection(frame->pos.x, frame->pos.y, frame->dim.x, frame->dim.y);
         if (frame->hasPoly) m_entity->setBoundingPoly(frame->poly);
/*
         if (it->second->getCurrentFrameIndex() == it->second->getNumFrames()) {
            try {
               pEAnimFinished_t event(new EAnimFinished(m_entity->getSharedPtr(), it->second));
               m_entity->onEvent(event);
            }
            catch (bad_alloc& e) {
               Exception ex("Error updating EntityAnimations; ", __FILE__, __LINE__);
               ex.append(e.what());
               throw ex;
            }
         }
*/
      }

      ++it;
   }
}


}
