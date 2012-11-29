/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <cml/cml.h>
#include <renderer/renderer.hpp>
#include <definitions.hpp>
#include <Graphics2d.hpp>
#include <globals.hpp>
#include <Range.hpp>


using namespace cml;


namespace Dodge {


bool Graphics2d::m_init = false;
Renderer Graphics2d::m_renderer = Renderer();
pCamera_t Graphics2d::m_camera = pCamera_t();
boost::shared_ptr<RenderBrush> Graphics2d::m_renderBrush = boost::shared_ptr<RenderBrush>(new RenderBrush);


//===========================================
// Graphics2d::init
//===========================================
void Graphics2d::init(int w, int h) {
   m_init = true;

   m_camera = pCamera_t(new Camera(static_cast<float32_t>(w) / static_cast<float32_t>(h), 1.f));

   m_renderer.attachCamera(m_camera);
}

//===========================================
// Graphics2d::drawImage
//
// src:  Source rectangle in pixel coords
// dest: Destination rectangle in world coords
//===========================================
void Graphics2d::drawImage(const Texture& image, const Range& src, const Range& dest, int z,
   float32_t angle, const Vec2f& pivot) const {

   if (!m_init)
      throw Exception("Error drawing image; Graphics2d not initialised", __FILE__, __LINE__);

   float32_t w = dest.getSize().x;
   float32_t h = dest.getSize().y;
   float32_t fZ = static_cast<float32_t>(z);
   float32_t imgW = image.getWidth();
   float32_t imgH = image.getHeight();

   float32_t tX1 = src.getPosition().x / imgW;
   float32_t tX2 = (src.getPosition().x + src.getSize().x) / imgW;
   float32_t tY1 = src.getPosition().y / imgH;
   float32_t tY2 = (src.getPosition().y + src.getSize().y) / imgH;

   // Flip texture vertically
   tY1 = 1.f - tY1;
   tY2 = 1.f - tY2;

   Renderer::vvvtt_t verts[] = {
      {w,   0.0,  fZ,    tX2, tY1},
      {w,   h,    fZ,    tX2, tY2},
      {0.0, 0.0,  fZ,    tX1, tY1},
      {w,   h,    fZ,    tX2, tY2},
      {0.0, h,    fZ,    tX1, tY2},
      {0.0, 0.0,  fZ,    tX1, tY1}
   };

   boost::shared_ptr<Renderer::Model> model(new Renderer::Model(Renderer::TEXTURED_ALPHA, false));

   model->primitiveType = Renderer::TRIANGLES;
   model->texHandle = image.getHandle();
   model->verts = new Renderer::vvvtt_t[6];
   model->n = 6;

   memcpy(model->verts, verts, 6 * sizeof(Renderer::vvvtt_t));

   matrix44f_c rotation;
   matrix44f_c translation;
   matrix44f_c mv;

   // TODO: use pivot
   float32_t rads = DEG_TO_RAD(angle);
   matrix_rotation_euler(rotation, 0.f, 0.f, rads, euler_order_xyz);
   matrix_translation(translation, dest.getPosition().x, dest.getPosition().y, 0.f);
   mv = translation * rotation;

   memcpy(model->matrix, mv.data(), 16 * sizeof(Renderer::matrixElement_t));

   m_renderer.stageModel(model);
}

//===========================================
// Graphics2d::drawText
//===========================================
void Graphics2d::drawText(const Font& font, const Vec2f& size, const std::string& text, float32_t x,
   float32_t y, int z, float32_t angle, const Vec2f& pivot) const {

   if (!m_init)
      throw Exception("Error drawing text; Graphics2d not initialised", __FILE__, __LINE__);

   float32_t texSectionX1 = font.getTextureSection().getPosition().x;
   float32_t texSectionY1 = font.getTextureSection().getPosition().y;
   float32_t texSectionX2 = font.getTextureSection().getSize().x;

   float32_t pxChW = font.getCharWidth();    // Char dimensions in pixels
   float32_t pxChH = font.getCharHeight();

   int rowLen = (static_cast<float32_t>(texSectionX2 - texSectionX1) / static_cast<float32_t>(pxChW)) + 0.5;

   for (uint_t i = 0; i < text.length(); ++i) {
      // Character world coords (pre-transformation)
      float32_t chX = x + static_cast<float32_t>(i) * size.x;
      float32_t chY = y;

      float32_t srcX = texSectionX1 + pxChW * (static_cast<float32_t>((text[i] - ' ') % rowLen));
      float32_t srcY = texSectionY1 + pxChH * static_cast<float32_t>((text[i] - ' ') / rowLen);

      srcY = font.getTexture()->getHeight() - srcY - pxChH; // Flip

      vector4f pos(chX, chY, 0.f, 1.f);
      vector4f piv(pivot.x, pivot.y, 0.f, 1.f);

      matrix44f_c rot;
      matrix_rotation_euler(rot, 0.f, 0.f, DEG_TO_RAD(angle), euler_order_xyz);
      pos = piv + (rot * (pos - piv));

      drawImage(*font.getTexture(), Range(srcX, srcY, pxChW, pxChH), Range(pos[0], pos[1], size.x, size.y), z, angle, Vec2f(pos[0], pos[1]));
   }
}

//===========================================
// Graphics2d::clear
//===========================================
void Graphics2d::clear(const Colour& col) const {
   if (!m_init)
      throw Exception("Error clearing screen; Graphics2d not initialised", __FILE__, __LINE__);

//   setFillColour(col);
//   m_renderer.attachBrush(m_renderBrush);
//   m_renderer.clear();
}


}
