/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <renderer/ogles2.0/TexturedAlphaShader.hpp>
#include <renderer/Model.hpp>
#include <renderer/GL_CHECK.hpp>


using namespace cml;


namespace Dodge {


//===========================================
// TexturedAlphaShader::TexturedAlphaShader
//===========================================
TexturedAlphaShader::TexturedAlphaShader() {
   m_id = GL_CHECK(glCreateProgram());

   const char* vertShader[1] = { NULL };
   vertShader[0] =
      "precision mediump float;                          \n"
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

   ShaderProgram::newShaderFromSource(vertShader, GL_VERTEX_SHADER, m_id);

   const char* fragShader[1] = { NULL };
   fragShader[0] =
      "precision mediump float;                          \n"
      "varying vec4 vv4colour;                           \n"
      "varying vec2 vv2texcoord;                         \n"
      "uniform sampler2D stexture;                       \n"
      "void main() {                                     \n"
      "   vec4 tex = texture2D(stexture, vv2texcoord);   \n"
      "   gl_FragColor = vv4colour * tex;                \n"
      "}                                                 \n";

   ShaderProgram::newShaderFromSource(fragShader, GL_FRAGMENT_SHADER, m_id);

   GL_CHECK(glLinkProgram(m_id));
}

//===========================================
// TexturedAlphaShader::setActive
//===========================================
void TexturedAlphaShader::setActive() {
   GL_CHECK(glUseProgram(m_id));

   m_locPosition = GL_CHECK(glGetAttribLocation(m_id, "av4position"));
   m_locColour = GL_CHECK(glGetAttribLocation(m_id, "av4colour"));
   m_locTexCoord = GL_CHECK(glGetAttribLocation(m_id, "av2texcoord"));

   m_locBUniColour = GL_CHECK(glGetUniformLocation(m_id, "bUniColour"));
   m_locUniColour = GL_CHECK(glGetUniformLocation(m_id, "uniColour"));
   m_locMV = GL_CHECK(glGetUniformLocation(m_id, "mv"));
   m_locP = GL_CHECK(glGetUniformLocation(m_id, "p"));

   GL_CHECK(glEnableVertexAttribArray(m_locPosition));
   GL_CHECK(glEnableVertexAttribArray(m_locColour));
   GL_CHECK(glEnableVertexAttribArray(m_locTexCoord));

   GL_CHECK(glEnable(GL_BLEND));
   GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

//===========================================
// TexturedAlphaShader::sendData
//===========================================
void TexturedAlphaShader::sendData(const IModel* model, const matrix44f_c& projMat) {
   model_lock(*model);

   GL_CHECK(glUniformMatrix4fv(m_locMV, 1, GL_FALSE, model_getMatrix(*model)));
   GL_CHECK(glUniformMatrix4fv(m_locP, 1, GL_FALSE, projMat.data()));

   if (model->getPrimitiveType() == Renderer::LINES) {
      if (model->getLineWidth() != 0)
         GL_CHECK(glLineWidth(model->getLineWidth()));
   }

   // If model contains per-vertex colour data
   if (model_containsPerVertexColourData(*model)) {
      GL_CHECK(glUniform1i(m_locBUniColour, 0));
      GL_CHECK(glEnableVertexAttribArray(m_locColour));
   }
   else {
      GL_CHECK(glDisableVertexAttribArray(m_locColour));
      GL_CHECK(glUniform1i(m_locBUniColour, 1));

      Colour colour = model->getColour();
      GL_CHECK(glUniform4f(m_locUniColour, colour.r, colour.g, colour.b, colour.a));
   }

   GL_CHECK(glBindTexture(GL_TEXTURE_2D, model->getTextureHandle()));

   const vvvttcccc_t* verts = reinterpret_cast<const vvvttcccc_t*>(model_getVertexData(*model));
   Renderer::int_t stride = model_containsPerVertexColourData(*model) ? sizeof(vvvttcccc_t) : sizeof(vvvtt_t);

   if (model_getHandle(*model) == 0) {
      GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));

      GL_CHECK(glVertexAttribPointer(m_locPosition, 3, GL_FLOAT, GL_FALSE, stride, verts));
      GL_CHECK(glVertexAttribPointer(m_locTexCoord, 2, GL_FLOAT, GL_FALSE, stride, &verts[0].t1));

      if (model_containsPerVertexColourData(*model))
         GL_CHECK(glVertexAttribPointer(m_locColour, 4, GL_FLOAT, GL_FALSE, stride, &verts[0].c1));
   }
   else {
      GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, model_getHandle(*model)));

      GLuint offset = 0;

      GL_CHECK(glVertexAttribPointer(m_locPosition, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<const void*>(offset)));
      offset += 3 * sizeof(Renderer::vertexElement_t);

      GL_CHECK(glVertexAttribPointer(m_locTexCoord, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<const void*>(offset)));
      offset += 2 * sizeof(Renderer::texCoordElement_t);

      if (model_containsPerVertexColourData(*model))
         GL_CHECK(glVertexAttribPointer(m_locColour, 4, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<const void*>(offset)));
   }

   model_unlock(*model);
}


}
