/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <definitions.hpp>
#include <Graphics2d.hpp>
#include <Texture.hpp>
#include <CompoundPoly.hpp>
#include <Font.hpp>


using namespace cml;


namespace Dodge {


Renderer Graphics2d::m_renderer = Renderer();
matrix44f_c Graphics2d::m_orthographic = matrix44f_c();
Vec2f Graphics2d::m_pixSz = Vec2f(0.f, 0.f);


//===========================================
// Graphics2d::init
//===========================================
void Graphics2d::init(int w, int h) {
   m_renderer.init();

   float32_t ratio = static_cast<float32_t>(w) / static_cast<float32_t>(h);
   matrix_orthographic_RH(m_orthographic, ratio, 1.f, 0.01f, 100.f, cml::z_clip_neg_one);

   m_pixSz = Vec2f(ratio / static_cast<float32_t>(w), 1.f / static_cast<float32_t>(h));

   setViewPos(0.f, 0.f);
}

//===========================================
// Graphics2d::setViewPos
//===========================================
void Graphics2d::setViewPos(float32_t x, float32_t y) {
   m_orthographic.data()[12] = x - 1.0;
   m_orthographic.data()[13] = y - 1.0;

   // Move the camera back slightly (any small negative number will do as
   // we're using an orthographic projection).
   m_orthographic.data()[14] = -0.01f;
}

//===========================================
// Graphics2d::drawImage
//===========================================
void Graphics2d::drawImage(const Texture& image, float32_t x, float32_t y, int z, float32_t w, float32_t h,
   float32_t srcX, float32_t srcY, float32_t srcW, float32_t srcH, float32_t a, const Colour& col) const {

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

   Renderer::colourElement_t colours[] = {
      col.r, col.g, col.b, col.a,
      col.r, col.g, col.b, col.a,
      col.r, col.g, col.b, col.a,
      col.r, col.g, col.b, col.a,
      col.r, col.g, col.b, col.a,
      col.r, col.g, col.b, col.a
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

   float32_t rads = DEG_TO_RAD(a);
   matrix_rotation_euler(rotation, 0.f, 0.f, rads, euler_order_xyz);
   matrix_translation(translation, x, y, 0.f);
   modelView = translation * rotation;

   matrix44f_c mvp = m_orthographic * modelView;

   m_renderer.setMatrix(mvp.data());
   m_renderer.setActiveTexture(image.getId());
   m_renderer.setGeometry(verts, 6);
   m_renderer.setColours(colours, 6);
   m_renderer.setTextureCoords(texCoords, 6);
   m_renderer.render();
}

//===========================================
// Graphics2d::drawImage
//===========================================
void Graphics2d::drawImage(const Texture& image, float32_t destX, float32_t destY, int destZ,
   float32_t srcX, float32_t srcY, float32_t w, float32_t h, float32_t a, const Colour& col) const {

   drawImage(image, destX, destY, destZ, w, h, srcX, srcY, image.getWidth(), image.getHeight(), a, col);
}

//===========================================
// Graphics2d::drawPlainAlphaQuad
//===========================================
void Graphics2d::drawPlainAlphaQuad(float32_t x, float32_t y, int z, float32_t w, float32_t h,
   float32_t a, Colour col) const {

   m_renderer.setMode(Renderer::NONTEXTURED_ALPHA);

   float32_t fZ = static_cast<float32_t>(z);

   Renderer::vertexElement_t verts[] = {
      w, 0.f, fZ,      // Bottom right
      w, h, fZ,        // Top right
      0.f, 0.f, fZ,    // Bottom left
      w, h, fZ,        // Top right
      0.f, h, fZ,      // Top left
      0.f, 0.f, fZ     // Bottom left
   };
   Renderer::colourElement_t colours[] = {
      col.r, col.g, col.b, col.a,
      col.r, col.g, col.b, col.a,
      col.r, col.g, col.b, col.a,
      col.r, col.g, col.b, col.a,
      col.r, col.g, col.b, col.a,
      col.r, col.g, col.b, col.a
   };

   matrix44f_c rotation;
   matrix44f_c translation;
   matrix44f_c modelView;

   float32_t rads = DEG_TO_RAD(a);
   matrix_rotation_euler(rotation, 0.f, 0.f, rads, euler_order_xyz);
   matrix_translation(translation, x, y, 0.f);
   modelView = translation * rotation;

   matrix44f_c mvp = m_orthographic * modelView;

   m_renderer.setMatrix(mvp.data());
   m_renderer.setGeometry(verts, 6);
   m_renderer.setColours(colours, 6);
   m_renderer.render();
}

//===========================================
// Graphics2d::drawPlainAlphaPoly
//
// Polygon must be convex
//===========================================
void Graphics2d::drawPlainAlphaPoly(const Poly& poly, float32_t x, float32_t y, int z, float32_t a,
   const Colour& colour) const {

   m_renderer.setMode(Renderer::NONTEXTURED_ALPHA);
   int n = poly.getNumVertices();

   float32_t fZ = static_cast<float32_t>(z);


   // Construct triangle fan

   // The number of vertices in a triangle fan is 3n-6,
   // where n is the number of vertices in the polygon.
   Renderer::vertexElement_t verts[3 * 3 * Poly::MAX_VERTS - 6];
   Renderer::colourElement_t colours[4 * 3 * Poly::MAX_VERTS - 6];

   int i = 0;
   for (int v = 1; v < n - 1; ++v) {
      verts[3 * i + 0] = poly.getVertex(0).x;
      verts[3 * i + 1] = poly.getVertex(0).y;
      verts[3 * i + 2] = fZ;
      colours[4 * i + 0] = colour.r;
      colours[4 * i + 1] = colour.g;
      colours[4 * i + 2] = colour.b;
      colours[4 * i + 3] = colour.a;
      i++;

      verts[3 * i + 0] = poly.getVertex(v).x;
      verts[3 * i + 1] = poly.getVertex(v).y;
      verts[3 * i + 2] = fZ;
      colours[4 * i + 0] = colour.r;
      colours[4 * i + 1] = colour.g;
      colours[4 * i + 2] = colour.b;
      colours[4 * i + 3] = colour.a;
      i++;

      verts[3 * i + 0] = poly.getVertex(v + 1).x;
      verts[3 * i + 1] = poly.getVertex(v + 1).y;
      verts[3 * i + 2] = fZ;
      colours[4 * i + 0] = colour.r;
      colours[4 * i + 1] = colour.g;
      colours[4 * i + 2] = colour.b;
      colours[4 * i + 3] = colour.a;
      i++;
   }

   matrix44f_c rotation;
   matrix44f_c translation;
   matrix44f_c modelView;

   float32_t rads = DEG_TO_RAD(a);
   matrix_rotation_euler(rotation, 0.f, 0.f, rads, euler_order_xyz);
   matrix_translation(translation, x, y, 0.f);
   modelView = translation * rotation;

   matrix44f_c mvp = m_orthographic * modelView;

   m_renderer.setMatrix(mvp.data());
   m_renderer.setGeometry(verts, 3 * n - 6);
   m_renderer.setColours(colours, 3 * n - 6);
   m_renderer.render();
}

//===========================================
// Graphics2d::drawPlainAlphaCompoundPoly
//
// Comprising polygons must be convex
//===========================================
void Graphics2d::drawPlainAlphaCompoundPoly(const CompoundPoly& poly, float32_t x, float32_t y,
   int z, float32_t a, const Colour& colour) const {

   for (int i = 0; i < poly.getNumPolys(); ++i)
      drawPlainAlphaPoly(poly.getPoly(i), x, y, z, a, colour);
}

//===========================================
// Graphics2d::drawText
//===========================================
void Graphics2d::drawText(const Font& font, const std::string& text, float32_t x, float32_t y,
   int z, float32_t a, const Vec2f& pivot, const Vec2f& scale, const Colour& col) const {

   float32_t texSectionX1 = font.getTextureSectionPosition().x;
   float32_t texSectionY1 = font.getTextureSectionPosition().y;
   float32_t texSectionX2 = font.getTextureSectionDimensions().x;

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
      matrix_rotation_euler(rot, 0.f, 0.f, DEG_TO_RAD(a), euler_order_xyz);
      pos = piv + (rot * (pos - piv));

      drawImage(*font.getTexture(), pos[0], pos[1], z, chW, chH, srcX, srcY, pxChW, pxChH, a, col);
   }
}

//===========================================
// Graphics2d::drawText
//===========================================
void Graphics2d::drawText(const Font& font, const std::string& text, float32_t x, float32_t y,
   int z, const Vec2f& scale, const Colour& col) const {

   drawText(font, text, x, y, z, 0.f, Vec2f(0.f, 0.f), scale, col);
}

//===========================================
// Graphics2d::clear
//===========================================
void Graphics2d::clear() const {
   m_renderer.clear();
}

//===========================================
// Graphics2d::clear
//===========================================
void Graphics2d::clear(const Colour& colour) const {
   Renderer::colourElement_t col[] = { colour.r, colour.g, colour.b, colour.a };
   m_renderer.clear(col);
}


}
