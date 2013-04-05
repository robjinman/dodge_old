/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <renderer/ogl/NonTexturedAlphaMode.hpp>
#include <renderer/Model.hpp>
#include <renderer/GL_CHECK.hpp>


using namespace cml;


namespace Dodge {


//===========================================
// NonTexturedAlphaMode::NonTexturedAlphaMode
//===========================================
NonTexturedAlphaMode::NonTexturedAlphaMode() {
   m_id = GL_CHECK(glCreateProgram());

   const char* vertShader[1] = { NULL };
   vertShader[0] =
      "precision mediump float;                          \n"
      "attribute vec4 av4position;                       \n"
      "attribute vec4 av4colour;                         \n"
      "uniform bool bUniColour;                          \n"
      "uniform vec4 uniColour;                           \n"
      "uniform mat4 mv;                                  \n"
      "uniform mat4 p;                                   \n"
      "varying vec4 vv4colour;                           \n"
      "void main() {                                     \n"
      "   if (bUniColour) {                              \n"
      "      vv4colour = uniColour;                      \n"
      "   }                                              \n"
      "   else {                                         \n"
      "      vv4colour = av4colour;                      \n"
      "   }                                              \n"
      "   gl_Position = p * mv * av4position;            \n"
      "}                                                 \n";

   RenderMode::newShaderFromSource(vertShader, GL_VERTEX_SHADER, m_id);

   const char* fragShader[1] = { NULL };
   fragShader[0] =
      "precision mediump float;                          \n"
      "varying vec4 vv4colour;                           \n"
      "void main() {                                     \n"
      "   gl_FragColor = vv4colour;                      \n"
      "}                                                 \n";

   RenderMode::newShaderFromSource(fragShader, GL_FRAGMENT_SHADER, m_id);

   GL_CHECK(glLinkProgram(m_id));
}

//===========================================
// NonTexturedAlphaMode::setActive
//===========================================
void NonTexturedAlphaMode::setActive() {
   GL_CHECK(glUseProgram(m_id));

   m_locPosition = GL_CHECK(glGetAttribLocation(m_id, "av4position"));
   m_locColour = GL_CHECK(glGetAttribLocation(m_id, "av4colour"));

   m_locBUniColour = GL_CHECK(glGetUniformLocation(m_id, "bUniColour"));
   m_locUniColour = GL_CHECK(glGetUniformLocation(m_id, "uniColour"));
   m_locMV = GL_CHECK(glGetUniformLocation(m_id, "mv"));
   m_locP = GL_CHECK(glGetUniformLocation(m_id, "p"));

   GL_CHECK(glEnableVertexAttribArray(m_locPosition));
   GL_CHECK(glEnableVertexAttribArray(m_locColour));

   GL_CHECK(glEnable(GL_BLEND));
   GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

//===========================================
// NonTexturedAlphaMode::isSupported
//===========================================
bool NonTexturedAlphaMode::isSupported(const IModel* model) const {
   static long vvv = internString("vvv");
   static long vvvcccc = internString("vvvcccc");

   long layout = model->getVertexLayout();

   return layout == vvv
      || layout == vvvcccc;
}

//===========================================
// NonTexturedAlphaMode::sendData
//===========================================
void NonTexturedAlphaMode::sendData(const IModel* model, const matrix44f_c& projMat, GLuint vbo) {
   if (!isSupported(model))
      throw RendererException("Model type not supported by NonTexturedAlphaMode", __FILE__, __LINE__);

   static long vvvcccc = internString("vvvcccc");

   long vertLayout = model->getVertexLayout();

   GL_CHECK(glUniformMatrix4fv(m_locMV, 1, GL_FALSE, model_getMatrix(*model)));
   GL_CHECK(glUniformMatrix4fv(m_locP, 1, GL_FALSE, projMat.data()));

   if (model->getPrimitiveType() == Renderer::LINES) {
      if (model->getLineWidth() != 0)
         GL_CHECK(glLineWidth(static_cast<GLfloat>(model->getLineWidth())));
   }

   // If model contains per-vertex colour data
   if (vertLayout == vvvcccc) {
      GL_CHECK(glUniform1i(m_locBUniColour, 0));
      GL_CHECK(glEnableVertexAttribArray(m_locColour));
   }
   else {
      GL_CHECK(glDisableVertexAttribArray(m_locColour));
      GL_CHECK(glUniform1i(m_locBUniColour, 1));

      Colour colour = model->getColour();
      GL_CHECK(glUniform4f(m_locUniColour, colour.r, colour.g, colour.b, colour.a));
   }

   const vvvcccc_t* verts = reinterpret_cast<const vvvcccc_t*>(model_getVertexData(*model));
   Renderer::int_t stride = vertLayout == vvvcccc ? sizeof(vvvcccc_t) : sizeof(vvv_t);

   if (vbo == 0) {
      GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));

      GL_CHECK(glVertexAttribPointer(m_locPosition, 3, GL_FLOAT, GL_FALSE, stride, verts));

      if (vertLayout == vvvcccc)
         GL_CHECK(glVertexAttribPointer(m_locColour, 4, GL_FLOAT, GL_FALSE, stride, &verts[0].c1));
   }
   else {
      GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vbo));

      GLuint offset = 0;

      GL_CHECK(glVertexAttribPointer(m_locPosition, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<const void*>(offset)));
      offset += 3 * sizeof(Renderer::vertexElement_t);

      if (vertLayout == vvvcccc)
         GL_CHECK(glVertexAttribPointer(m_locColour, 4, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<const void*>(offset)));
   }

   GL_CHECK(glDrawArrays(primitiveToGLType(model->getPrimitiveType()), 0, model->getNumVertices()));
}


}
