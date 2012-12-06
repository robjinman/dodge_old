/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __ENTITY_ANIMATIONS_HPP__
#define __ENTITY_ANIMATIONS_HPP__


#ifdef DEBUG
#include <ostream>
#endif
#include "xml/xml.hpp"
#include "Animation.hpp"
#include "Entity.hpp"
#include "renderer/Texture.hpp"
#include "renderer/Renderer.hpp"
#include "renderer/Model.hpp"
#include "Range.hpp"


namespace Dodge {


class EntityAnimations {
   public:
      EntityAnimations(Entity* entity, const XmlNode data);

      EntityAnimations(Entity* entity, pTexture_t texture)
         : m_entity(entity),
           m_texture(texture),
           m_texSection(),
           m_activeAnim(),
           m_model(Renderer::TRIANGLES) {}

      EntityAnimations(const EntityAnimations& copy, Entity* entity);

      virtual void assignData(const XmlNode data);

      inline void setTexture(pTexture_t texture);
      inline void setTextureSection(float32_t x, float32_t y, float32_t w, float32_t h);
      inline void setOnScreenSize(float32_t w, float32_t h);

      void addAnimation(pAnimation_t anim); // TODO: use unique_ptr instead
      void removeAnimation(long anim);

      inline const Range& getTextureSection() const;
      inline const Vec2f& getOnScreenSize() const;
      inline const Colour& getColour() const;
      inline Animation::state_t getAnimState() const;
      inline long getAnimName() const;

      bool playAnimation(long name);
      inline bool playAnimation();
      inline void pauseAnimation();
      inline void stopAnimation();

      virtual void render() const;
      virtual void unrender() const;

      virtual void update();
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif

      virtual ~EntityAnimations() = 0;

   protected:
      inline pTexture_t getTexture() const;
      inline std::map<long, pAnimation_t>& getAnimations();

   private:
      void updateModel();

      Entity* m_entity;
      pTexture_t m_texture;
      Range m_texSection;
      Vec2f m_onScreenSize;
      std::map<long, pAnimation_t> m_animations;
      pAnimation_t m_activeAnim;
      mutable PlainTexturedAlphaModel m_model;

      static Renderer m_renderer;
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
// EntityAnimations::setOnScreenSize
//===========================================
inline void EntityAnimations::setOnScreenSize(float32_t w, float32_t h) {
   m_onScreenSize.x = w;
   m_onScreenSize.y = h;
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
// EntityAnimations::getOnScreenSize
//===========================================
inline const Vec2f& EntityAnimations::getOnScreenSize() const {
   return m_onScreenSize;
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
