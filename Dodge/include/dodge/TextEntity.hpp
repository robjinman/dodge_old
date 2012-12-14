#ifndef __TEXT_ENTITY_HPP__
#define __TEXT_ENTITY_HPP__


#include "Entity.hpp"
#include "renderer/Font.hpp"
#include "renderer/Model.hpp"


namespace Dodge {


class TextEntity : public Entity {
   public:
      explicit TextEntity(const XmlNode data);
      explicit TextEntity(long type, const pFont_t font, const std::string& text, const Vec2f& size);
      TextEntity(long name, long type, const pFont_t font, const std::string& text, const Vec2f& size);
      TextEntity(const TextEntity& copy);
      TextEntity(const TextEntity& copy, long name);

      virtual TextEntity* clone() const;
      virtual void assignData(const XmlNode data);

      virtual void onEvent(const EEvent* event);

      virtual void setFillColour(const Colour& colour);

      virtual void render() const;
      virtual void unrender() const;

      virtual void update();

#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif

      void setText(const std::string& text);
      inline const std::string& getText() const;

      inline void setTextSize(const Vec2f& size);
      void setTextSize(float32_t x, float32_t y);
      inline const Vec2f& getTextSize() const;

      virtual ~TextEntity();

   private:
      Renderer& m_renderer;

      const pFont_t m_font;
      std::string m_text;
      Vec2f m_size;

      mutable PlainTexturedAlphaModel m_model;

      void updateModel() const;
};

typedef boost::shared_ptr<TextEntity> pTextEntity_t;

//===========================================
// TextEntity::getText
//===========================================
inline const std::string& TextEntity::getText() const {
   return m_text;
}

//===========================================
// TextEntity::setTextSize
//===========================================
inline void TextEntity::setTextSize(const Vec2f& size) {
   setTextSize(size.x, size.y);
}

//===========================================
// TextEntity::getTextSize
//===========================================
inline const Vec2f& TextEntity::getTextSize() const {
   return m_size;
}


}


#endif
