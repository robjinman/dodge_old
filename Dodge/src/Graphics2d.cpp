/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <definitions.hpp>
#include <Graphics2d.hpp>
#include <Texture.hpp>
#include <Font.hpp>


using namespace cml;


namespace Dodge {


bool Graphics2d::m_init = false;
Renderer Graphics2d::m_renderer = Renderer();
matrix44f_c Graphics2d::m_projectionMatrix = matrix44f_c();
Vec2f Graphics2d::m_pixSz = Vec2f(0.f, 0.f);

// Brush may not actually exist yet, so must be set again in init()
Renderer::Brush& Graphics2d::m_renderBrush = m_renderer.getBrush();


//===========================================
// Graphics2d::init
//===========================================
void Graphics2d::init(int w, int h) {
   m_init = true;

   m_renderer.init();
   m_renderBrush = m_renderer.getBrush();

   float32_t ratio = static_cast<float32_t>(w) / static_cast<float32_t>(h);
   matrix_orthographic_RH(m_projectionMatrix, ratio, 1.f, 0.01f, 100.f, cml::z_clip_neg_one);

   m_pixSz = Vec2f(ratio / static_cast<float32_t>(w), 1.f / static_cast<float32_t>(h));

   setViewPos(0.f, 0.f);
}

//===========================================
// Graphics2d::setViewPos
//===========================================
void Graphics2d::setViewPos(float32_t x, float32_t y) {
   m_projectionMatrix.data()[12] = x - 1.0;
   m_projectionMatrix.data()[13] = y - 1.0;

   // Move the camera back slightly (any small negative number will do as
   // we're using an orthographic projection).
   m_projectionMatrix.data()[14] = -0.01f;

   // TODO: this is inefficient
   m_renderer.setP(m_projectionMatrix.data());
}

//===========================================
// Graphics2d::drawImage
//===========================================
void Graphics2d::drawImage(const Texture& image, float32_t srcX, float32_t srcY, float32_t srcW, float32_t srcH,
   float32_t x, float32_t y, int z, float32_t angle, const Vec2f& pivot, const Vec2f& scale) const {

   float32_t w = srcW * m_pixSz.x * scale.x;
   float32_t h = srcH * m_pixSz.y * scale.y;

   m_renderer.setMode(Renderer::TEXTURED_ALPHA);

   float32_t fZ = static_cast<float32_t>(z);
   float32_t imgW = image.getWidth();
   float32_t imgH = image.getHeight();

   Renderer::vertexElement_t verts[] = {
      w, 0.f, fZ,      // Bottom right
      w, h, fZ,        // Top right
      0.f, 0.f, fZ,    // Bottom left
      w, h, fZ,        // Top right
      0.f, h, fZ,      // Top left
      0.f, 0.f, fZ     // Bottom left
   };

   float32_t tX1 = srcX / imgW;
   float32_t tX2 = (srcX + srcW) / imgW;
   float32_t tY1 = srcY / imgH;
   float32_t tY2 = (srcY + srcH) / imgH;

   // Flip texture vertically
   tY1 = 1.f - tY1;
   tY2 = 1.f - tY2;

   Renderer::texCoordElement_t texCoords[] = {
      tX2, tY1,  // Bottom right
      tX2, tY2,  // Top right
      tX1, tY1,  // Bottom left
      tX2, tY2,  // Top right
      tX1, tY2,  // Top left
      tX1, tY1   // Bottom left
   };

   matrix44f_c rotation;
   matrix44f_c translation;
   matrix44f_c modelView;

   // TODO: use pivot
   float32_t rads = DEG_TO_RAD(angle);
   matrix_rotation_euler(rotation, 0.f, 0.f, rads, euler_order_xyz);
   matrix_translation(translation, x, y, 0.f);
   modelView = translation * rotation;

   m_renderer.setMatrix(modelView.data());
   m_renderer.setActiveTexture(image.getId());
   m_renderer.setGeometry(verts, Renderer::TRIANGLES, 6);
   m_renderer.setTextureCoords(texCoords, 6);

   setFillColour(Colour(1.f, 1.f, 1.f, 1.f));
   m_renderer.render();
}

//===========================================
// Graphics2d::drawText
//===========================================
void Graphics2d::drawText(const Font& font, const std::string& text, float32_t x, float32_t y, int z,
   float32_t angle, const Vec2f& pivot, const Vec2f& scale) const {

   float32_t texSectionX1 = font.getTextureSection().getPosition().x;
   float32_t texSectionY1 = font.getTextureSection().getPosition().y;
   float32_t texSectionX2 = font.getTextureSection().getSize().x;

   float32_t pxChW = font.getCharWidth();    // Char dimensions in pixels (not scaled)
   float32_t pxChH = font.getCharHeight();

   int rowLen = (static_cast<float32_t>(texSectionX2 - texSectionX1) / static_cast<float32_t>(pxChW)) + 0.5;

   for (unsigned int i = 0; i < text.length(); ++i) {
      float32_t pxOffset = scale.x * static_cast<float32_t>(i) * pxChW;
      float32_t chX = x + pxOffset * m_pixSz.x;
      float32_t chY = y;
      float32_t chW = pxChW * scale.x * m_pixSz.x;
      float32_t chH = pxChH * scale.y * m_pixSz.y;
      float32_t srcX = texSectionX1 + pxChW * (static_cast<float32_t>((text[i] - ' ') % rowLen));
      float32_t srcY = texSectionY1 + pxChH * static_cast<float32_t>((text[i] - ' ') / rowLen);
      srcY = font.getTexture()->getHeight() - srcY - pxChH; // Flip

      vector4f pos(chX, chY, 0.f, 1.f);
      vector4f piv(pivot.x, pivot.y, 0.f, 1.f);

      matrix44f_c rot;
      matrix_rotation_euler(rot, 0.f, 0.f, DEG_TO_RAD(angle), euler_order_xyz);
      pos = piv + (rot * (pos - piv));

      drawImage(*font.getTexture(), srcX, srcY, chW, chH, pos[0], pos[1], z, angle, Vec2f(pos[0], pos[1]), scale);
   }
}

//===========================================
// Graphics2d::clear
//===========================================
void Graphics2d::clear(const Colour& col) const {
   setFillColour(col);
   m_renderer.clear();
}


}
