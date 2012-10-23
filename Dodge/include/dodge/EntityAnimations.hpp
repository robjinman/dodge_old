/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __ENTITY_ANIMATIONS_HPP__
#define __ENTITY_ANIMATIONS_HPP__


#ifdef DEBUG
#include <ostream>
#endif
#include "rapidxml/rapidxml.hpp"
#include "Animation.hpp"
#include "Entity.hpp"
#include "renderer/Texture.hpp"
#include "Graphics2d.hpp"
#include "Range.hpp"


namespace Dodge {


class EntityAnimations {
   public:
      EntityAnimations(Entity* entity, pTexture_t texture)
         : m_entity(entity),
           m_texture(texture),
           m_texSection(),
           m_activeAnim() {}

      EntityAnimations(const EntityAnimations& copy, Entity* entity);

      inline void setTexture(pTexture_t texture);
      inline void setTextureSection(float32_t x, float32_t y, float32_t w, float32_t h);

      void addAnimation(pAnimation_t anim);
      void removeAnimation(long anim);

      inline const Range& getTextureSection() const;
      inline const Colour& getColour() const;
      inline Animation::state_t getAnimState() const;
      inline long getAnimName() const;

      bool playAnimation(long name);
      inline bool playAnimation();
      inline void pauseAnimation();
      inline void stopAnimation();
      virtual void draw(const Vec2f& at) const;
      virtual void update();
      virtual void assignData(const rapidxml::xml_node<>* data);
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif

      virtual ~EntityAnimations() = 0;

   protected:
      inline pTexture_t getTexture() const;
      inline std::map<long, pAnimation_t>& getAnimations();

   private:
      Entity* m_entity;
      pTexture_t m_texture;
      Range m_texSection;
      std::map<long, pAnimation_t> m_animations;
      pAnimation_t m_activeAnim;

      static Graphics2d m_graphics2d;
};

//===========================================
// EntityAnimations::setTexture
//===========================================
inline void EntityAnimations::setTexture(pTexture_t texture) {
   m_texture = texture;
}

//===========================================
// EntityAnimations::setTextureSection
//===========================================
inline void EntityAnimations::setTextureSection(float32_t x, float32_t y, float32_t w, float32_t h) {
   m_texSection = Range(x, y, w, h);
}

//===========================================
// EntityAnimations::playAnimation
//===========================================
inline bool EntityAnimations::playAnimation() {
   if (m_activeAnim) return m_activeAnim->play();
}

//===========================================
// EntityAnimations::pauseAnimation
//===========================================
inline void EntityAnimations::pauseAnimation() {
   if (m_activeAnim) m_activeAnim->pause();
}

//===========================================
// EntityAnimations::stopAnimation
//===========================================
inline void EntityAnimations::stopAnimation() {
   if (m_activeAnim) {
      m_activeAnim->stop();
      m_activeAnim = pAnimation_t();
   }
}

//===========================================
// EntityAnimations::getTexture
//===========================================
inline pTexture_t EntityAnimations::getTexture() const {
   return m_texture;
}

//===========================================
// EntityAnimations::getTextureSection
//===========================================
inline const Range& EntityAnimations::getTextureSection() const {
   return m_texSection;
}

//===========================================
// EntityAnimations::getAnimState
//===========================================
inline Animation::state_t EntityAnimations::getAnimState() const {
   return m_activeAnim ? m_activeAnim->getState() : Animation::STOPPED;
}

//===========================================
// EntityAnimations::getAnimName
//===========================================
inline long EntityAnimations::getAnimName() const {
   return m_activeAnim ? m_activeAnim->getName() : 0;
}


}


#endif /*!__ENTITY_ANIMATIONS_HPP__*/
