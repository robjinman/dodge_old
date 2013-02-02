#include <TextEntity.hpp>
#include <renderer/Renderer.hpp>
#include <globals.hpp>
#include <AssetManager.hpp>


namespace Dodge {


//===========================================
// TextEntity::TextEntity
//===========================================
TextEntity::TextEntity(const XmlNode data)
   : Asset(internString("TextEntity")),
     Entity(data.firstChild()),
     m_renderer(Renderer::getInstance()),
     m_model(Renderer::TRIANGLES) {

   try {
      XML_NODE_CHECK(data, TextEntity);

      XmlNode node = data.nthChild(1);
      XML_NODE_CHECK(node, font);

      XmlAttribute attr = node.firstAttribute();
      XML_ATTR_CHECK(attr, ptr);
      long fontId = attr.getLong();

      AssetManager assetManager;
      pFont_t font = boost::dynamic_pointer_cast<Dodge::Font>(assetManager.getAssetPointer(fontId));

      if (!font)
         throw XmlException("Bad asset id", __FILE__, __LINE__);

      m_font = font;

      node = node.nextSibling();
      XML_NODE_CHECK(node, textSize);
      m_size = Vec2f(node.firstChild());

      node = node.nextSibling();
      XML_NODE_CHECK(node, text);
      setText(node.getString());
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class TextEntity; ");
      throw;
   }
}

//===========================================
// TextEntity::TextEntity
//===========================================
TextEntity::TextEntity(long type, const pFont_t font, const std::string& text, const Vec2f& size)
   : Asset(internString("TextEntity")),
     Entity(type),
     m_renderer(Renderer::getInstance()),
     m_font(font),
     m_size(size),
     m_model(Renderer::TRIANGLES) {

   setText(text);
}

//===========================================
// TextEntity::TextEntity
//===========================================
TextEntity::TextEntity(long name, long type, const pFont_t font, const std::string& text, const Vec2f& size)
   : Asset(internString("TextEntity")),
     Entity(name, type),
     m_renderer(Renderer::getInstance()),
     m_font(font),
     m_size(size),
     m_model(Renderer::TRIANGLES) {

   setText(text);
}

//===========================================
// TextEntity::TextEntity
//===========================================
TextEntity::TextEntity(const TextEntity& copy)
   : Asset(internString("TextEntity")),
     Entity(copy),
     m_renderer(Renderer::getInstance()),
     m_model(Renderer::TRIANGLES) {

   // TODO
}

//===========================================
// TextEntity::TextEntity
//===========================================
TextEntity::TextEntity(const TextEntity& copy, long name)
   : Asset(internString("TextEntity")),
     Entity(copy, name),
     m_renderer(Renderer::getInstance()),
     m_model(Renderer::TRIANGLES) {

   // TODO
}

//===========================================
// TextEntity::clone
//===========================================
TextEntity* TextEntity::clone() const {
   return new TextEntity(*this);
}

//===========================================
// TextEntity::getSize
//===========================================
size_t TextEntity::getSize() const {
   return sizeof(TextEntity)
      - sizeof(Entity)
      - sizeof(PlainTexturedAlphaModel)
      + Entity::getSize()
      + m_model.getTotalSize();
}

//===========================================
// TextEntity::assignData
//===========================================
void TextEntity::assignData(const XmlNode data) {
   try {
      if (data.isNull() || data.name() != "TextEntity") return;

      XmlNode node = data.nthChild(1);
      if (!node.isNull() && node.name() == "font") {

         XmlAttribute attr = node.firstAttribute();
         if (!attr.isNull() && attr.name() == "ptr") {
            long fontId = attr.getLong();

            AssetManager assetManager;
            pFont_t font = boost::dynamic_pointer_cast<Dodge::Font>(assetManager.getAssetPointer(fontId));

            if (!font)
               throw XmlException("Bad asset id", __FILE__, __LINE__);

            m_font = font;
         }

         node = node.nextSibling();
      }

      if (!node.isNull() && node.name() == "textSize") {
         m_size = Vec2f(node.firstChild());
         node = node.nextSibling();
      }

      XML_NODE_CHECK(node, text);
      setText(node.getString());
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class TextEntity; ");
      throw;
   }
}

//===========================================
// TextEntity::draw
//===========================================
void TextEntity::draw() const {
   m_renderer.draw(&m_model);
}

//===========================================
// TextEntity::update
//===========================================
void TextEntity::update() {
   // TODO
}

#ifdef DEBUG
//===========================================
// TextEntity::dbg_print
//===========================================
void TextEntity::dbg_print(std::ostream& out, int tab) const {
   // TODO
}
#endif

//===========================================
// TextEntity::onEvent
//===========================================
void TextEntity::onEvent(const EEvent* event) {
   static long entityRotationStr = internString("entityRotation");
   static long entityTranslationStr = internString("entityTranslation");

   if (event->getType() == entityRotationStr
      || event->getType() == entityTranslationStr) {

      updateModel();
   }
}

//===========================================
// TextEntity::setZ
//===========================================
void TextEntity::setZ(int z) {
   Entity::setZ(z);
   m_model.setMatrixElement(14, z);
}

//===========================================
// TextEntity::updateModel
//===========================================
void TextEntity::updateModel() const {
   float32_t x = getTranslation_abs().x;
   float32_t y = getTranslation_abs().y;
   float32_t z = getZ();

   float32_t texSectionX1 = m_font->getTextureSection().getPosition().x;
   float32_t texSectionY1 = m_font->getTextureSection().getPosition().y;
   float32_t texSectionX2 = texSectionX1 + m_font->getTextureSection().getSize().x;
   float32_t texSectionY2 = texSectionY1 + m_font->getTextureSection().getSize().y;

   float32_t texW = m_font->getTexture()->getWidth();
   float32_t texH = m_font->getTexture()->getHeight();

   float32_t pxChW = m_font->getCharWidth();    // Char dimensions in pixels
   float32_t pxChH = m_font->getCharHeight();

   int rowLen = static_cast<float32_t>(texSectionX2 - texSectionX1) / static_cast<float32_t>(pxChW);

   StackAllocator::marker_t marker = gGetMemStack().getMarker();
   vvvtt_t* verts = reinterpret_cast<vvvtt_t*>(gGetMemStack().alloc(m_text.size() * 6 * sizeof(vvvtt_t)));

   int v = 0;
   for (uint_t i = 0; i < m_text.length(); ++i) {
      // Character world coords (pre-transformation)
      float32_t chX = x + static_cast<float32_t>(i) * m_size.x;
      float32_t chY = y;

      float32_t srcX = texSectionX1 + pxChW * static_cast<float32_t>((m_text[i] - ' ') % rowLen);
      float32_t srcY = texSectionY2 - pxChH * static_cast<float32_t>((m_text[i] - ' ') / rowLen + 1);

      float32_t tY1 = srcY / texH;
      float32_t tY2 = (srcY + pxChH) / texH;
      tY1 = 1.f - tY1;
      tY2 = 1.f - tY2;

      verts[v] = vvvtt_t(chX + m_size.x, chY,            z,    (srcX + pxChW) / texW, tY1);    ++v;
      verts[v] = vvvtt_t(chX + m_size.x, chY + m_size.y, z,    (srcX + pxChW) / texW, tY2);    ++v;
      verts[v] = vvvtt_t(chX,            chY,            z,    srcX / texW,           tY1);    ++v;
      verts[v] = vvvtt_t(chX + m_size.x, chY + m_size.y, z,    (srcX + pxChW) / texW, tY2);    ++v;
      verts[v] = vvvtt_t(chX,            chY + m_size.y, z,    srcX / texW,           tY2);    ++v;
      verts[v] = vvvtt_t(chX,            chY,            z,    srcX / texW,           tY1);    ++v;
   }

   m_model.eraseVertices();
   m_model.setVertices(0, verts, 6 * m_text.size());
   m_model.setColour(getFillColour());
   m_model.setLineWidth(0);
   m_model.setTextureHandle(m_font->getTexture()->getHandle());

   m_renderer.freeBufferedModel(&m_model);
   m_renderer.bufferModel(&m_model);

   gGetMemStack().freeToMarker(marker);
}

//===========================================
// TextEntity::setText
//===========================================
void TextEntity::setText(const std::string& text) {
   m_text = text;
   updateModel();
}

//===========================================
// TextEntity::setFillColour
//===========================================
void TextEntity::setFillColour(const Colour& colour) {
   Entity::setFillColour(colour);
   m_model.setColour(getFillColour());
}

//===========================================
// TextEntity::~TextEntity
//===========================================
TextEntity::~TextEntity() {
   m_renderer.freeBufferedModel(&m_model);
}


}
