/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <cstring>
#include <cml/cml.h>
#include "renderer/Renderer.hpp"
#include "EntityAnimations.hpp"
#include "StringId.hpp"
#include "EAnimFinished.hpp"
#include "EventManager.hpp"
#include "AssetManager.hpp"


using namespace cml;
using namespace std;


namespace Dodge {


#ifdef DEBUG
//===========================================
// EntityAnimations::dbg_print
//===========================================
void EntityAnimations::dbg_print(ostream& out, int tab) const {
   for (int i = 0; i < tab; i++) out << "\t";
   out << "EntityAnimations\n";

   for (int i = 0; i < tab + 1; i++) out << "\t";
   out << "texSection:\n";
   m_texSection.dbg_print(out, tab + 1);

   for (int i = 0; i < tab + 1; i++) out << "\t";
   out << "onScreenSize: (" << m_onScreenSize.x << ", " << m_onScreenSize.y << ")\n";

   for (map<long, pAnimation_t>::const_iterator it = m_animations.begin(); it != m_animations.end(); ++it)
      it->second->dbg_print(out, tab + 1);
}
#endif

//===========================================
// EntityAnimations::EntityAnimations
//===========================================
EntityAnimations::EntityAnimations(Entity* entity, pTexture_t texture)
   : m_entity(entity),
     m_texture(texture),
     m_texSection(),
     m_activeAnim(),
     m_model(Renderer::TRIANGLES),
     m_renderer(Renderer::getInstance()) {}

//===========================================
// EntityAnimations::EntityAnimations
//
// Construct deep copy.
//===========================================
EntityAnimations::EntityAnimations(const EntityAnimations& copy, Entity* entity)
   : m_entity(entity),
     m_activeAnim(),
     m_model(Renderer::TRIANGLES),
     m_renderer(Renderer::getInstance()) {

   m_onScreenSize = copy.m_onScreenSize;
   m_texSection = copy.m_texSection;
   m_texture = copy.m_texture;

   map<long, pAnimation_t>::const_iterator it = copy.m_animations.begin();
   while (it != copy.m_animations.end()) {
      pAnimation_t anim(new Animation(*(it->second)));
      m_animations[it->first] = anim;
      ++it;
   }
}

//===========================================
// EntityAnimations::EntityAnimations
//===========================================
EntityAnimations::EntityAnimations(Entity* entity, const XmlNode data)
   : m_entity(entity),
     m_activeAnim(),
     m_model(Renderer::TRIANGLES),
     m_renderer(Renderer::getInstance()) {

   AssetManager assetManager;

   try {
      XML_NODE_CHECK(data, EntityAnimations);

      XmlNode node = data.firstChild();
      XML_NODE_CHECK(node, texture);

      XmlAttribute attr = node.firstAttribute();
      if (!attr.isNull() && attr.name() == "ptr") {
         long id = attr.getLong();
         m_texture = boost::dynamic_pointer_cast<Texture>(assetManager.getAssetPointer(id));

         if (!m_texture)
            throw XmlException("Bad texture asset id", __FILE__, __LINE__);
      }
      else {
         m_texture = pTexture_t(new Texture(node.firstChild()));
      }

      node = node.nextSibling();
      XML_NODE_CHECK(node, textureSection);
      m_texSection = Range(node.firstChild());

      node = node.nextSibling();
      XML_NODE_CHECK(node, onScreenSize);
      m_onScreenSize = Vec2f(node.firstChild());

      node = node.nextSibling();
      while (!node.isNull() && node.name() == "animation") {
         XmlAttribute attr = node.firstAttribute();
         if (!attr.isNull() && attr.name() == "proto") {
            long id = attr.getLong();

            pAnimation_t anim(dynamic_cast<Animation*>(assetManager.cloneAsset(id)));

            if (!anim)
               throw XmlException("Bad entity asset id", __FILE__, __LINE__);

            m_animations[anim->getName()] = anim;
         }
         else {
            pAnimation_t anim(new Animation(node.firstChild()));
            m_animations[anim->getName()] = anim;
         }

         node = node.nextSibling();
      }
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class EntityAnimations; ");
      throw;
   }
}

//===========================================
// EntityAnimations::assignData
//===========================================
void EntityAnimations::assignData(const XmlNode data) {
   if (data.isNull() || data.name() != "EntityAnimations") return;

   AssetManager assetManager;

   try {
      XmlNode node = data.firstChild();
      if (!node.isNull() && node.name() == "texture") {

         XmlAttribute attr = node.firstAttribute();
         if (!attr.isNull() && attr.name() == "id") {
            long id = attr.getLong();
            m_texture = boost::dynamic_pointer_cast<Texture>(assetManager.getAssetPointer(id));

            if (!m_texture)
               throw XmlException("Bad texture asset id", __FILE__, __LINE__);
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

      if (!node.isNull() && node.name() == "onScreenSize") {
         m_onScreenSize = Vec2f(node.firstChild());
         node = node.nextSibling();
      }

      while (!node.isNull() && node.name() == "animation") {
         XmlAttribute attr = node.firstAttribute();
         if (!attr.isNull() && attr.name() == "proto") {
            long id = attr.getLong();

            pAnimation_t anim(dynamic_cast<Animation*>(assetManager.cloneAsset(id)));

            if (!anim)
               throw XmlException("Bad entity asset id", __FILE__, __LINE__);

            m_animations[anim->getName()] = anim;
         }
         else {
            pAnimation_t anim(new Animation(node.firstChild()));
            m_animations[anim->getName()] = anim;
         }

         node = node.nextSibling();
      }
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class EntityAnimations; ");
      throw;
   }
}

//===========================================
// EntityAnimations::draw
//===========================================
void EntityAnimations::draw() const {
   m_renderer.draw(&m_model);
}

//===========================================
// EntityAnimations::playAnimation
//
// Plays an animation if it's not already playing.
//===========================================
bool EntityAnimations::playAnimation(long name, bool repeat) {
   if (getAnimState() == Animation::PLAYING) return false;

   map<long, pAnimation_t>::iterator it = m_animations.find(name);
   if (it == m_animations.end()) return false;

   m_activeAnim = it->second;
   return m_activeAnim->play(repeat);
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
// EntityAnimations::updateModel
//===========================================
void EntityAnimations::updateModel() {
   Vec2f pos = m_entity->getTranslation_abs();

   float32_t x = pos.x;
   float32_t y = pos.y;
   int z = m_entity->getZ();

   float32_t angle = m_entity->getRotation_abs();

   matrix44f_c rotation;
   matrix44f_c translation;
   matrix44f_c mv;

   float32_t rads = DEG_TO_RAD(angle);
   matrix_rotation_euler(rotation, 0.f, 0.f, rads, euler_order_xyz);
   matrix_translation(translation, x, y, 0.f);
   mv = translation * rotation;

   float32_t w = m_onScreenSize.x * m_entity->getScale().x;
   float32_t h = m_onScreenSize.y * m_entity->getScale().y;
   float32_t fZ = static_cast<float32_t>(z);

   float32_t imgW = m_texture->getWidth();
   float32_t imgH = m_texture->getHeight();

   float32_t tX1 = m_texSection.getPosition().x / imgW;
   float32_t tX2 = (m_texSection.getPosition().x + m_texSection.getSize().x) / imgW;
   float32_t tY1 = m_texSection.getPosition().y / imgH;
   float32_t tY2 = (m_texSection.getPosition().y + m_texSection.getSize().y) / imgH;

   // Flip texture vertically
   tY1 = 1.f - tY1;
   tY2 = 1.f - tY2;

   vvvtt_t verts[] = {
      {w,   0.0,  fZ,    tX2, tY1},
      {w,   h,    fZ,    tX2, tY2},
      {0.0, 0.0,  fZ,    tX1, tY1},
      {w,   h,    fZ,    tX2, tY2},
      {0.0, h,    fZ,    tX1, tY2},
      {0.0, 0.0,  fZ,    tX1, tY1}
   };

   m_model.setVertices(0, verts, 6);
   m_model.setMatrix(mv.data());
   m_model.setColour(m_entity->getFillColour());
   m_model.setTextureHandle(m_texture->getHandle());
}

//===========================================
// EntityAnimations::onEvent
//===========================================
void EntityAnimations::onEvent(const EEvent* event) {
   static long entityRotationStr = internString("entityRotation");
   static long entityTranslationStr = internString("entityTranslation");

   if (event->getType() == entityRotationStr
      || event->getType() == entityTranslationStr) {

      Vec2f pos = m_entity->getTranslation_abs();

      float32_t x = pos.x;
      float32_t y = pos.y;

      float32_t angle = m_entity->getRotation_abs();

      matrix44f_c rotation;
      matrix44f_c translation;
      matrix44f_c mv;

      float32_t rads = DEG_TO_RAD(angle);
      matrix_rotation_euler(rotation, 0.f, 0.f, rads, euler_order_xyz);
      matrix_translation(translation, x, y, 0.f);
      mv = translation * rotation;
      m_model.setMatrix(mv.data());
   }
}

//===========================================
// EntityAnimations::addToWorld
//===========================================
void EntityAnimations::addToWorld() {
   updateModel();
}

//===========================================
// EntityAnimations::update
//===========================================
void EntityAnimations::update() {
   map<long, pAnimation_t>::iterator it = m_animations.begin();
   while (it != m_animations.end()) {
      bool justFinished = false;
      it->second->update(&justFinished);

      const AnimFrame* frame = it->second->getCurrentFrame();
      if (frame) {
         setTextureSection(frame->pos.x, frame->pos.y, frame->dim.x, frame->dim.y);
         m_entity->setFillColour(frame->col);
         if (frame->shape) m_entity->setShape(unique_ptr<Shape>(frame->shape->clone())); // TODO: ShapeDelta
         updateModel();
      }

      if (justFinished) {
         try {
            EventManager eventManager;

            EAnimFinished* event = new EAnimFinished(m_entity->getSharedPtr(), it->second);
            m_entity->onEvent(event);
            eventManager.queueEvent(event);
         }
         catch (bad_alloc& e) {
            Exception ex("Error updating EntityAnimations; ", __FILE__, __LINE__);
            ex.append(e.what());
            throw ex;
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
