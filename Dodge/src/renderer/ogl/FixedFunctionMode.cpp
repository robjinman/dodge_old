/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <renderer/ogl/FixedFunctionMode.hpp>
#include <renderer/Model.hpp>
#include <renderer/GL_CHECK.hpp>


using namespace cml;


namespace Dodge {


//===========================================
// FixedFunctionMode::FixedFunctionMode
//===========================================
FixedFunctionMode::FixedFunctionMode() {}

//===========================================
// FixedFunctionMode::setActive
//===========================================
void FixedFunctionMode::setActive() {
   GL_CHECK(glEnable(GL_TEXTURE_2D));
   GL_CHECK(glEnable(GL_BLEND));
   GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

//===========================================
// FixedFunctionMode::isSupported
//===========================================
bool FixedFunctionMode::isSupported(const IModel* model) const {
   static long vvv = internString("vvv");
   static long vvvcccc = internString("vvvcccc");
   static long vvvtt = internString("vvvtt");
   static long vvvttcccc = internString("vvvttcccc");

   long layout = model->getVertexLayout();

   return layout == vvv
      || layout == vvvcccc
      || layout == vvvtt
      || layout == vvvttcccc;
}

//===========================================
// FixedFunctionMode::sendData
//===========================================
void FixedFunctionMode::sendData(const IModel* model, const matrix44f_c& projMat) {
   static long vvv = internString("vvv");
   static long vvvcccc = internString("vvvcccc");
   static long vvvtt = internString("vvvtt");
   static long vvvttcccc = internString("vvvttcccc");

   if (!isSupported(model))
      throw RendererException("Model type not supported by FixedFunctionMode", __FILE__, __LINE__);


   uint_t nVerts = model->getNumVertices();
   long vertLayout = model->getVertexLayout();

   if (vertLayout == vvvtt || vertLayout == vvvttcccc) {
      GL_CHECK(glEnable(GL_TEXTURE_2D));
      GL_CHECK(glBindTexture(GL_TEXTURE_2D, model->getTextureHandle()));
   }
   else {
      GL_CHECK(glDisable(GL_TEXTURE_2D));
   }

   glMatrixMode(GL_PROJECTION);
   glLoadMatrixf(projMat.data());

   glMatrixMode(GL_MODELVIEW);
   glLoadMatrixf(model_getMatrix(*model));

   const Colour& col = model->getColour();

   glBegin(primitiveToGLType(model->getPrimitiveType()));
   if (vertLayout == vvv) {
      const vvv_t* verts = reinterpret_cast<const vvv_t*>(model_getVertexData(*model));

      for (uint_t i = 0; i < nVerts; ++i) {
         glColor4f(col.r, col.g, col.b, col.a);
         glVertex3f(verts[i].v1, verts[i].v2, verts[i].v3);
      }
   }
   else if (vertLayout == vvvcccc) {
      const vvvcccc_t* verts = reinterpret_cast<const vvvcccc_t*>(model_getVertexData(*model));

      for (uint_t i = 0; i < nVerts; ++i) {
         glColor4f(verts[i].c1, verts[i].c2, verts[i].c3, verts[i].c4);
         glVertex3f(verts[i].v1, verts[i].v2, verts[i].v3);
      }
   }
   else if (vertLayout == vvvtt) {
      const vvvtt_t* verts = reinterpret_cast<const vvvtt_t*>(model_getVertexData(*model));

      for (uint_t i = 0; i < nVerts; ++i) {
         glTexCoord2f(verts[i].t1, verts[i].t2);
         glColor4f(col.r, col.g, col.b, col.a);
         glVertex3f(verts[i].v1, verts[i].v2, verts[i].v3);
      }
   }
   else if (vertLayout == vvvttcccc) {
      const vvvttcccc_t* verts = reinterpret_cast<const vvvttcccc_t*>(model_getVertexData(*model));

      for (uint_t i = 0; i < nVerts; ++i) {
         glTexCoord2f(verts[i].t1, verts[i].t2);
         glColor4f(verts[i].c1, verts[i].c2, verts[i].c3, verts[i].c4);
         glVertex3f(verts[i].v1, verts[i].v3, verts[i].v3);
      }
   }
   glEnd();
}


}
