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
void FixedFunctionMode::sendData(const IModel* model, const matrix44f_c& projMat, GLuint vbo) {
   static long vvv = internString("vvv");
   static long vvvcccc = internString("vvvcccc");
   static long vvvtt = internString("vvvtt");
   static long vvvttcccc = internString("vvvttcccc");

   if (!isSupported(model))
      throw RendererException("Model type not supported by FixedFunctionMode", __FILE__, __LINE__);

   uint_t nVerts = model->getNumVertices();
   long vertLayout = model->getVertexLayout();

   glMatrixMode(GL_PROJECTION);
   glLoadMatrixf(projMat.data());

   glMatrixMode(GL_MODELVIEW);
   glLoadMatrixf(model_getMatrix(*model));

   const Colour& col = model->getColour();

   GLint stride = 0;

   if (vertLayout == vvv) {
      stride = sizeof(vvv_t);
      GL_CHECK(glEnableClientState(GL_VERTEX_ARRAY));
      GL_CHECK(glDisableClientState(GL_COLOR_ARRAY));
      GL_CHECK(glDisableClientState(GL_TEXTURE_COORD_ARRAY));
      GL_CHECK(glColor4f(col.r, col.g, col.b, col.a));
      GL_CHECK(glDisable(GL_TEXTURE_2D));
   }
   else if (vertLayout == vvvcccc) {
      stride = sizeof(vvvcccc_t);
      GL_CHECK(glEnableClientState(GL_VERTEX_ARRAY));
      GL_CHECK(glEnableClientState(GL_COLOR_ARRAY));
      GL_CHECK(glDisableClientState(GL_TEXTURE_COORD_ARRAY));
      GL_CHECK(glDisable(GL_TEXTURE_2D));
   }
   else if (vertLayout == vvvtt) {
      stride = sizeof(vvvtt_t);
      GL_CHECK(glEnableClientState(GL_VERTEX_ARRAY));
      GL_CHECK(glDisableClientState(GL_COLOR_ARRAY));
      GL_CHECK(glEnableClientState(GL_TEXTURE_COORD_ARRAY));
      GL_CHECK(glColor4f(col.r, col.g, col.b, col.a));
      GL_CHECK(glEnable(GL_TEXTURE_2D));
      GL_CHECK(glBindTexture(GL_TEXTURE_2D, model->getTextureHandle()));
   }
   else if (vertLayout == vvvttcccc) {
      stride = sizeof(vvvttcccc);
      GL_CHECK(glEnableClientState(GL_VERTEX_ARRAY));
      GL_CHECK(glEnableClientState(GL_COLOR_ARRAY));
      GL_CHECK(glEnableClientState(GL_TEXTURE_COORD_ARRAY));
      GL_CHECK(glEnable(GL_TEXTURE_2D));
      GL_CHECK(glBindTexture(GL_TEXTURE_2D, model->getTextureHandle()));
   }

   assert(stride > 0);

   const vvvttcccc_t* verts = reinterpret_cast<const vvvttcccc_t*>(model_getVertexData(*model));

   if (vbo == 0) {
      GL_CHECK(gl_bindBuffer(GL_ARRAY_BUFFER, 0));

      GL_CHECK(glVertexPointer(3, GL_FLOAT, stride, verts));

      if (vertLayout == vvvtt || vertLayout == vvvttcccc)
         GL_CHECK(glTexCoordPointer(2, GL_FLOAT, stride, &verts[0].t1));

      if (vertLayout == vvvttcccc || vertLayout == vvvcccc)
         GL_CHECK(glColorPointer(4, GL_FLOAT, stride, &verts[0].c1));
   }
   else {
      GL_CHECK(gl_bindBuffer(GL_ARRAY_BUFFER, vbo));

      GLuint offset = 0;

      GL_CHECK(glVertexPointer(3, GL_FLOAT, stride, reinterpret_cast<const void*>(offset)));
      offset += 3 * sizeof(GLfloat);

      if (vertLayout == vvvtt || vertLayout == vvvttcccc) {
         GL_CHECK(glTexCoordPointer(2, GL_FLOAT, stride, reinterpret_cast<const void*>(offset)));
         offset += 2 * sizeof(GLfloat);
      }

      if (vertLayout == vvvttcccc || vertLayout == vvvcccc)
         GL_CHECK(glColorPointer(4, GL_FLOAT, stride, reinterpret_cast<const void*>(offset)));
   }

   GL_CHECK(glDrawArrays(primitiveToGLType(model->getPrimitiveType()), 0, nVerts));
}


}
