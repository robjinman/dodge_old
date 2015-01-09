/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <renderer/ogl/TexturedAlphaMode.hpp>
#include <renderer/Model.hpp>
#include <renderer/GL_CHECK.hpp>
#include <globals.hpp>


using namespace std;
using namespace cml;


namespace Dodge {


//===========================================
// TexturedAlphaMode::TexturedAlphaMode
//===========================================
TexturedAlphaMode::TexturedAlphaMode() {

   m_id = GL_CHECK(m_gl.createProgram());

   const char* vertShader[1] = { NULL };
   vertShader[0] =
      "attribute vec4 av4position;                       \n"
      "attribute vec4 av4colour;                         \n"
      "attribute vec2 av2texcoord;                       \n"

      "uniform bool bUniColour;                          \n"
      "uniform vec4 uniColour;                           \n"
      "uniform mat4 mv;                                  \n"
      "uniform mat4 p;                                   \n"

      "varying vec4 vv4colour;                           \n"
      "varying vec2 vv2texcoord;                         \n"

      "void main() {                                     \n"
      "   if (bUniColour) {                              \n"
      "      vv4colour = uniColour;                      \n"
      "   }                                              \n"
      "   else {                                         \n"
      "      vv4colour = av4colour;                      \n"
      "   }                                              \n"

      "   gl_Position = p * mv * av4position;            \n"
      "   vv2texcoord = av2texcoord;                     \n"
      "}                                                 \n";

   RenderMode::newShaderFromSource(vertShader, GL_VERTEX_SHADER, m_id);

   const char* fragShader[1] = { NULL };
   fragShader[0] =
      "varying vec4 vv4colour;                           \n"
      "varying vec2 vv2texcoord;                         \n"

      "uniform sampler2D stexture;                       \n"

      "void main() {                                     \n"
      "   vec4 tex = texture2D(stexture, vv2texcoord);   \n"
      "   gl_FragColor = vv4colour * tex;                \n"
      "}                                                 \n";

   RenderMode::newShaderFromSource(fragShader, GL_FRAGMENT_SHADER, m_id);

   GL_CHECK(m_gl.linkProgram(m_id));
}

//===========================================
// TexturedAlphaMode::setActive
//===========================================
void TexturedAlphaMode::setActive() {
   GL_CHECK(m_gl.useProgram(m_id));

   m_locPosition = GL_CHECK(m_gl.getAttribLocation(m_id, "av4position"));
   m_locColour = GL_CHECK(m_gl.getAttribLocation(m_id, "av4colour"));
   m_locTexCoord = GL_CHECK(m_gl.getAttribLocation(m_id, "av2texcoord"));

   m_locBUniColour = GL_CHECK(m_gl.getUniformLocation(m_id, "bUniColour"));
   m_locUniColour = GL_CHECK(m_gl.getUniformLocation(m_id, "uniColour"));
   m_locMV = GL_CHECK(m_gl.getUniformLocation(m_id, "mv"));
   m_locP = GL_CHECK(m_gl.getUniformLocation(m_id, "p"));

   GL_CHECK(m_gl.enableVertexAttribArray(m_locPosition));
   GL_CHECK(m_gl.enableVertexAttribArray(m_locColour));
   GL_CHECK(m_gl.enableVertexAttribArray(m_locTexCoord));

   GL_CHECK(glEnable(GL_BLEND));
   GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

//===========================================
// TexturedAlphaMode::isSupported
//===========================================
bool TexturedAlphaMode::isSupported(const IModel* model) const {
   static long vvvtt = internString("vvvtt");
   static long vvvttcccc = internString("vvvttcccc");

   long layout = model->getVertexLayout();

   return layout == vvvtt
      || layout == vvvttcccc;
}

//===========================================
// TexturedAlphaMode::sendData
//===========================================
void TexturedAlphaMode::sendData(const IModel* model, const matrix44f_c& projMat, GLuint vbo) {
   static long vvvttcccc = internString("vvvttcccc");

   if (!isSupported(model))
      throw RendererException("Model type not supported by TexturedAlphaMode", __FILE__, __LINE__);

   long vertLayout = model->getVertexLayout();

   GL_CHECK(m_gl.uniformMatrix4fv(m_locMV, 1, GL_FALSE, model_getMatrix(*model)));
   GL_CHECK(m_gl.uniformMatrix4fv(m_locP, 1, GL_FALSE, projMat.data()));

   if (model->getPrimitiveType() == Renderer::LINES) {
      if (model->getLineWidth() != 0)
         GL_CHECK(glLineWidth(static_cast<GLfloat>(model->getLineWidth())));
   }

   // If model contains per-vertex colour data
   if (vertLayout == vvvttcccc) {
      GL_CHECK(m_gl.uniform1i(m_locBUniColour, 0));
      GL_CHECK(m_gl.enableVertexAttribArray(m_locColour));
   }
   else {
      GL_CHECK(m_gl.disableVertexAttribArray(m_locColour));
      GL_CHECK(m_gl.uniform1i(m_locBUniColour, 1));

      const Colour& colour = model->getColour();
      GL_CHECK(m_gl.uniform4f(m_locUniColour, colour.r, colour.g, colour.b, colour.a));
   }

   GL_CHECK(glBindTexture(GL_TEXTURE_2D, model->getTextureHandle()));

   const vvvttcccc_t* verts = reinterpret_cast<const vvvttcccc_t*>(model_getVertexData(*model));
   GLint stride = vertLayout == vvvttcccc ? sizeof(vvvttcccc_t) : sizeof(vvvtt_t);

   if (vbo == 0) {
      if (m_gl.getSupportedFeatures().VBOs.available)
         GL_CHECK(m_gl.bindBuffer(GL_ARRAY_BUFFER, 0));

      GL_CHECK(m_gl.vertexAttribPointer(m_locPosition, 3, GL_FLOAT, GL_FALSE, stride, verts));
      GL_CHECK(m_gl.vertexAttribPointer(m_locTexCoord, 2, GL_FLOAT, GL_FALSE, stride, &verts[0].t1));

      if (vertLayout == vvvttcccc)
         GL_CHECK(m_gl.vertexAttribPointer(m_locColour, 4, GL_FLOAT, GL_FALSE, stride, &verts[0].c1));
   }
   else {
      GL_CHECK(m_gl.bindBuffer(GL_ARRAY_BUFFER, vbo));

      GLuint offset = 0;

      GL_CHECK(m_gl.vertexAttribPointer(m_locPosition, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<const void*>(offset)));
      offset += 3 * sizeof(GLfloat);

      GL_CHECK(m_gl.vertexAttribPointer(m_locTexCoord, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<const void*>(offset)));
      offset += 2 * sizeof(GLfloat);

      if (vertLayout == vvvttcccc)
         GL_CHECK(m_gl.vertexAttribPointer(m_locColour, 4, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<const void*>(offset)));
   }

   GL_CHECK(glDrawArrays(primitiveToGLType(model->getPrimitiveType()), 0, model->getNumVertices()));
}


}
