/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <cstring>
#include "renderer/Texture.hpp"
#include "EntityAnimations.hpp"
#include "StringId.hpp"
#include "EAnimFinished.hpp"
#include "AssetManager.hpp"


using namespace std;
using namespace rapidxml;


namespace Dodge {


Graphics2d EntityAnimations::m_graphics2d = Graphics2d();


#ifdef DEBUG
//===========================================
// EntityAnimations::dbg_print
//===========================================
void EntityAnimations::dbg_print(ostream& out, int tab) const {
   for (int i = 0; i < tab; i++) out << "\t";
   out << "EntityAnimations\n";

   for (map<long, pAnimation_t>::const_iterator it = m_animations.begin(); it != m_animations.end(); ++it)
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
      map<long, pAnimation_t>::const_iterator it = copy.m_animations.begin();
      while (it != copy.m_animations.end()) {
         pAnimation_t anim(new Animation(*(it->second)));
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
// EntityAnimations::EntityAnimations
//===========================================
EntityAnimations::EntityAnimations(Entity* entity, const XmlNode data)
   : m_entity(entity), m_activeAnim() {

   if (data.isNull() || data.name() != "EntityAnimations")
      throw XmlException("Error parsing XML for instance of class EntityAnimations; Expected 'EntityAnimations' tag", __FILE__, __LINE__);

   AssetManager assetManager;

   XmlNode node = data.firstChild();
   if (node.isNull() || node.name() != "texture")
      throw XmlException("Error parsing XML for instance of class EntityAnimations; Expected 'texture' tag", __FILE__, __LINE__);

   XmlAttribute attr = node.firstAttribute();
   if (!attr.isNull() && attr.name() == "ptr") {
      long id = 0;
      sscanf(attr.value().data(), "%ld", &id);
      m_texture = boost::dynamic_pointer_cast<Texture>(assetManager.getAssetPointer(id));

      if (!m_texture)
         throw XmlException("Error parsing XML for instance of class EntityAnimations; Bad texture asset id", __FILE__, __LINE__);
   }
   else {
      m_texture = pTexture_t(new Texture(node.firstChild()));
   }

   node = node.nextSibling();

   if (node.isNull() || node.name() != "textureSection")
      throw XmlException("Error parsing XML for instance of class EntityAnimations; Expected 'textureSection' tag", __FILE__, __LINE__);

   m_texSection = Range(node.firstChild());
   node = node.nextSibling();

   while (!node.isNull() && node.name() == "animation") {
      XmlAttribute attr = node.firstAttribute();
      if (!attr.isNull() && attr.name() == "proto") {
         long id = 0;
         sscanf(attr.value().data(), "%ld", &id);

         pAnimation_t anim(dynamic_cast<Animation*>(assetManager.cloneAsset(id)));
         m_animations[anim->getName()] = anim;
      }
      else {
         pAnimation_t anim(new Animation(node.firstChild()));
         m_animations[anim->getName()] = anim;
      }

      node = node.nextSibling();
   }
}

//===========================================
// EntityAnimations::assignData
//===========================================
void EntityAnimations::assignData(const XmlNode data) {
   if (data.isNull() || data.name() != "EntityAnimations") return;

   AssetManager assetManager;

   XmlNode node = data.firstChild();
   if (!node.isNull() && node.name() == "texture") {

      XmlAttribute attr = node.firstAttribute();
      if (!attr.isNull() && attr.name() == "id") {
         long id = 0;
         sscanf(attr.value().data(), "%ld", &id);
         m_texture = boost::dynamic_pointer_cast<Texture>(assetManager.getAssetPointer(id));

         if (!m_texture)
            throw XmlException("Error parsing XML for instance of class EntityAnimations; Bad texture asset id", __FILE__, __LINE__);
      }
      else {
         m_texture = pTexture_t(new Texture(node.firstChild()));
      }

      node = node.nextSibling();
   }

   if (!node.isNull() && node.name() == "textureSection") {
      m_texSection = Range(node.firstChild());
      node = node.nextSibling();
   }

   while (!node.isNull() && node.name() == "animation") {
      XmlAttribute attr = node.firstAttribute();
      if (!attr.isNull() && attr.name() == "proto") {
         long id = 0;
         sscanf(attr.value().data(), "%ld", &id);

         pAnimation_t anim(dynamic_cast<Animation*>(assetManager.cloneAsset(id)));
         m_animations[anim->getName()] = anim;
      }
      else {
         pAnimation_t anim(new Animation(node.firstChild()));
         m_animations[anim->getName()] = anim;
      }

      node = node.nextSibling();
   }
}

//===========================================
// EntityAnimations::draw
//===========================================
void EntityAnimations::draw() const {
   Vec2f pos = m_entity->getTranslation_abs();

   float32_t x = pos.x;
   float32_t y = pos.y;
   int z = m_entity->getZ();

   float32_t tx = m_texSection.getPosition().x;
   float32_t ty = m_texSection.getPosition().y;
   float32_t tw = m_texSection.getSize().x;
   float32_t th = m_texSection.getSize().y;
   float32_t a = m_entity->getRotation_abs();

   m_graphics2d.setFillColour(Colour(m_entity->getRenderBrush()->getFillColour()));
   m_graphics2d.drawImage(*m_texture, tx, ty, tw, th, x, y, z, a, Vec2f(0.f, 0.f), m_entity->getScale());
}

//===========================================
// EntityAnimations::playAnimation
//
// Plays an animation if it's not already playing.
//===========================================
bool EntityAnimations::playAnimation(long name) {
   if (getAnimState() == Animation::PLAYING) return false;

   map<long, pAnimation_t>::iterator it = m_animations.find(name);
   if (it == m_animations.end()) return false;

   m_activeAnim = it->second;
   return m_activeAnim->play();
}

//===========================================
// EntityAnimations::addAnimation
//===========================================
void EntityAnimations::addAnimation(pAnimation_t anim) {
   m_animations[anim->getName()] = anim;

   // If this is the first animation, set default texSection to first frame
   if (m_animations.size() == 1) {
      const AnimFrame* frame = anim->getFrame(0);

      setTextureSection(frame->pos.x, frame->pos.y, frame->dim.x, frame->dim.y);
   }
}

//===========================================
// EntityAnimations::removeAnimation
//===========================================
void EntityAnimations::removeAnimation(long anim) {
   map<long, pAnimation_t>::iterator i = m_animations.find(anim);

   if (i != m_animations.end()) {
      if (i->second == m_activeAnim) m_activeAnim = pAnimation_t();

      m_animations.erase(i);
   }
}

//===========================================
// EntityAnimations::update
//===========================================
void EntityAnimations::update() {
   map<long, pAnimation_t>::iterator it = m_animations.begin();
   while (it != m_animations.end()) {
      it->second->update();

      const AnimFrame* frame = it->second->getCurrentFrame();
      if (frame) {
         setTextureSection(frame->pos.x, frame->pos.y, frame->dim.x, frame->dim.y);
         if (frame->shape) m_entity->setShape(unique_ptr<Primitive>(frame->shape->clone())); // TODO: PrimitiveDelta

         if (it->second->getCurrentFrameIndex() == it->second->getNumFrames()) {
            try {
               EAnimFinished* event = new EAnimFinished(m_entity->getSharedPtr(), it->second);
               m_entity->onEvent(event);
            }
            catch (bad_alloc& e) {
               Exception ex("Error updating EntityAnimations; ", __FILE__, __LINE__);
               ex.append(e.what());
               throw ex;
            }
         }
      }

      ++it;
   }
}

//===========================================
// EntityAnimations::~EntityAnimations
//===========================================
EntityAnimations::~EntityAnimations() {}


}
