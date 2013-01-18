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
#include "renderer/Model.hpp"
#include "Range.hpp"


namespace Dodge {


class EntityAnimations {
   public:
      EntityAnimations(Entity* entity, const XmlNode data);
      EntityAnimations(Entity* entity, pTexture_t texture);
      EntityAnimations(const EntityAnimations& copy, Entity* entity);

      virtual size_t getSize() const;
      virtual void assignData(const XmlNode data);

      void setTexture(pTexture_t texture);
      void setTextureSection(float32_t x, float32_t y, float32_t w, float32_t h);
      void setOnScreenSize(float32_t w, float32_t h);
      void setAnimationDuration(float32_t duration);

      void addAnimation(pAnimation_t anim);
      void removeAnimation(long anim);

      const Range& getTextureSection() const;
      const Vec2f& getOnScreenSize() const;
      const Colour& getColour() const;
      Animation::state_t getAnimState() const;
      long getAnimName() const;
      bool hasAnimation(long name) const;

      bool playAnimation(long name, bool repeat = false);
      bool playAnimation(bool repeat = false);
      void stepAnimation();
      void pauseAnimation();
      void stopAnimation();

      virtual void onEvent(const EEvent* event);

      virtual void addToWorld();
      virtual void removeFromWorld() {}

      virtual void draw() const;

      virtual void update();
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif

      virtual ~EntityAnimations() = 0;

   protected:
      pTexture_t getTexture() const;
      std::map<long, pAnimation_t>& getAnimations();

   private:
      void updateModel();

      Entity* m_entity;
      pTexture_t m_texture;
      Range m_texSection;
      Vec2f m_onScreenSize;
      std::map<long, pAnimation_t> m_animations;
      pAnimation_t m_activeAnim;

      mutable PlainTexturedAlphaModel m_model;
      Renderer& m_renderer;
};


}


#endif /*!__ENTITY_ANIMATIONS_HPP__*/
