/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <renderer/ogles2.0/TexturedAlphaShader.hpp>
#include <renderer/Model.hpp>
#include <renderer/GL_CHECK.hpp>
#include <cml/matrix.h>
#include <globals.hpp>


using namespace std;
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
// TexturedAlphaShader::hashByteArray
//===========================================
long TexturedAlphaShader::hashByteArray(const byte_t* array, size_t len) const {
   long hash = 5381;
   int c;

   for (uint_t i = 0; i < len; ++i) {
      c = *(array + i);
      hash = ((hash << 5) + hash) + c;
   }

   return hash;
}

//===========================================
// TexturedAlphaShader::hashModel
//===========================================
long TexturedAlphaShader::hashModel(const IModel* model) const {
   long hashes[1];
   size_t sz = 1 * sizeof(long);

   hashes[0] = hashByteArray(reinterpret_cast<const byte_t*>(model_getVertexData(*model)), model_vertexDataSize(*model));

   return hashByteArray(reinterpret_cast<const byte_t*>(hashes), sz);
}

//===========================================
// TexturedAlphaShader::hashPendingModels
//===========================================
long TexturedAlphaShader::hashPendingModels() const {
   StackAllocator& stack = gGetMemStack();
   StackAllocator::marker_t marker = stack.getMarker();

   size_t sz = sizeof(long) * m_pending.size();
   long* hashes = reinterpret_cast<long*>(stack.alloc(sz));

   for (uint_t i = 0; i < m_pending.size(); ++i) {
      hashes[i] = hashModel(m_pending[i]);
   }

   long hash = hashByteArray(reinterpret_cast<byte_t*>(hashes), sz);

   stack.freeToMarker(marker);

   return hash;
}

//===========================================
// TexturedAlphaShader::renderPendingModels
//===========================================
void TexturedAlphaShader::renderPendingModels() {
   for (uint_t i = 0; i < m_pending.size(); ++i)
      renderModel(m_pending[i], *m_P);
}

//===========================================
// TexturedAlphaShader::transformVertex
//===========================================
void TexturedAlphaShader::transformVertex(GLfloat* vertex, const GLfloat* m) const {
   GLfloat v[4] = { vertex[0], vertex[1], vertex[2], 1.0 };

   vertex[0] = v[0] * m[0] + v[1] * m[4] + v[2] * m[8] + v[3] * m[12];
   vertex[1] = v[0] * m[1] + v[1] * m[5] + v[2] * m[9] + v[3] * m[13];
   vertex[2] = v[0] * m[2] + v[1] * m[6] + v[2] * m[10] + v[3] * m[14];
}

//===========================================
// TexturedAlphaShader::batchAndRenderPendingModels
//===========================================
void TexturedAlphaShader::batchAndRenderPendingModels() {
   uint_t geoCursor = 0;
   uint_t totalVerts = 0;
   uint_t nModels = m_pending.size();
   size_t vertSz = 0;

   for (uint_t i = 0; i < m_pending.size(); ++i) {
      const IModel* model = m_pending[i];

      uint_t nVerts = model->getNumVertices();
      vertSz = model_getVertexSize(*model);
      size_t vertDataSz = model_vertexDataSize(*model);
      const GLfloat* matrix = model_getMatrix(*model);

      while (m_geometry.size() < geoCursor + vertDataSz)
         m_geometry.resize(m_geometry.size() * 2 + 1);

      model->getVertices(m_geometry.data() + geoCursor, 0, nVerts);

      for (uint_t v = 0; v < nVerts; ++v)
         transformVertex(reinterpret_cast<GLfloat*>(m_geometry.data() + geoCursor + v * vertSz), matrix);

      geoCursor += vertDataSz;

      totalVerts += nVerts;
   }

   if (nModels > 0) {
      GLuint vbo;

      GL_CHECK(glGenBuffers(1, &vbo));
      GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vbo));
      GL_CHECK(glBufferData(GL_ARRAY_BUFFER, geoCursor, m_geometry.data(), GL_STATIC_DRAW));

      batch_t batch = { vbo, totalVerts, true };

      long hash = hashPendingModels();
      m_VBOs[hash] = batch;

      renderBatch(batch);
   }
}

//===========================================
// TexturedAlphaShader::renderBatch
//===========================================
void TexturedAlphaShader::renderBatch(const batch_t& batch) {
   bool colData = model_containsPerVertexColourData(*m_pending.back());
   Renderer::primitive_t primType = m_pending.back()->getPrimitiveType();
   const Colour& colour = m_pending.back()->getColour();
   GLuint texHandle = m_pending.back()->getTextureHandle();
   GLint lineWidth = m_pending.back()->getLineWidth();
   size_t vertSz = model_getVertexSize(*m_pending.back());

   GLfloat mat[] = { 1.f, 0.f, 0.f, 0.f,
                     0.f, 1.f, 0.f, 0.f,
                     0.f, 0.f, 1.f, 0.f,
                     0.f, 0.f, 0.f, 1.f };

   GL_CHECK(glUniformMatrix4fv(m_locMV, 1, GL_FALSE, mat));
   GL_CHECK(glUniformMatrix4fv(m_locP, 1, GL_FALSE, m_P->data()));

   if (primType == Renderer::LINES) {
      if (lineWidth != 0)
         GL_CHECK(glLineWidth(lineWidth));
   }

   if (colData) {
      GL_CHECK(glUniform1i(m_locBUniColour, 0));
      GL_CHECK(glEnableVertexAttribArray(m_locColour));
   }
   else {
      GL_CHECK(glDisableVertexAttribArray(m_locColour));
      GL_CHECK(glUniform1i(m_locBUniColour, 1));
      GL_CHECK(glUniform4f(m_locUniColour, colour.r, colour.g, colour.b, colour.a));
   }

   GL_CHECK(glBindTexture(GL_TEXTURE_2D, texHandle));

   GLint stride = vertSz;

   GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, batch.vbo));

   GLuint offset = 0;

   GL_CHECK(glVertexAttribPointer(m_locPosition, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<const void*>(offset)));
   offset += 3 * sizeof(GLfloat);

   GL_CHECK(glVertexAttribPointer(m_locTexCoord, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<const void*>(offset)));
   offset += 2 * sizeof(GLfloat);

   if (colData) {
      GL_CHECK(glVertexAttribPointer(m_locColour, 4, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<const void*>(offset)));
      offset += 4 * sizeof(GLfloat);
   }

   GL_CHECK(glDrawArrays(primitiveToGLType(primType), 0, batch.nVerts));
}

//===========================================
// TexturedAlphaShader::sendData
//===========================================
void TexturedAlphaShader::sendData(const IModel* model, const matrix44f_c& projMat) {
   // Queue model for batching

   if (isCompatibleWithPending(model)) {

      // Add to batch
      m_pending.push_back(model);
      m_P = &projMat;

      if (m_pending.size() >= MAX_BATCH_SIZE)
         processPending();
   }
   else {
      // Flush current batch and start a new one

      processPending();

      m_pending.push_back(model);
      m_P = &projMat;
   }
}

//===========================================
// TexturedAlphaShader::processPending
//===========================================
void TexturedAlphaShader::processPending() {
   if (m_pending.size() >= MIN_BATCH_SIZE) {

      // Find out if batch has already been sent to video card
      long hash = hashPendingModels();
      auto i = m_VBOs.find(hash);

      if (i != m_VBOs.end()) {
         renderBatch(i->second);
         i->second.rendered = true;
      }
      else
         batchAndRenderPendingModels();
   }
   else
      renderPendingModels();

   m_pending.clear();
}

//===========================================
// TexturedAlphaShader::flush
//
// Called at end of frame.
//===========================================
void TexturedAlphaShader::flush() {
   processPending();

   // Delete VBOs that were not used this frame
   for (auto i = m_VBOs.begin(); i != m_VBOs.end();) {
      if (i->second.rendered == false) {
         GL_CHECK(glDeleteBuffers(1, &i->second.vbo));
         m_VBOs.erase(i++);
      }
      else {
         i->second.rendered = false;
         ++i;
      }
   }
}

//===========================================
// TexturedAlphaShader::renderModel
//===========================================
void TexturedAlphaShader::renderModel(const IModel* model, const matrix44f_c& projMat) {
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

      const Colour& colour = model->getColour();
      GL_CHECK(glUniform4f(m_locUniColour, colour.r, colour.g, colour.b, colour.a));
   }

   GL_CHECK(glBindTexture(GL_TEXTURE_2D, model->getTextureHandle()));

   const vvvttcccc_t* verts = reinterpret_cast<const vvvttcccc_t*>(model_getVertexData(*model));
   GLint stride = model_containsPerVertexColourData(*model) ? sizeof(vvvttcccc_t) : sizeof(vvvtt_t);

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
      offset += 3 * sizeof(GLfloat);

      GL_CHECK(glVertexAttribPointer(m_locTexCoord, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<const void*>(offset)));
      offset += 2 * sizeof(GLfloat);

      if (model_containsPerVertexColourData(*model))
         GL_CHECK(glVertexAttribPointer(m_locColour, 4, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<const void*>(offset)));
   }

   GL_CHECK(glDrawArrays(primitiveToGLType(model->getPrimitiveType()), 0, model->getNumVertices()));
}


}
