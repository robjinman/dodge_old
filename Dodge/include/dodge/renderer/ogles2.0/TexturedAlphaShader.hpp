/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __TEXTURED_ALPHA_SHADER_HPP__
#define __TEXTURED_ALPHA_SHADER_HPP__


#include <memory>
#include "ShaderProgram.hpp"
#include "../../definitions.hpp"


namespace Dodge {


class TexturedAlphaShader : public ShaderProgram {
   public:
      TexturedAlphaShader();

      virtual void setActive();
      virtual void sendData(const IModel* model, const cml::matrix44f_c& projMat);
      virtual void flush();

      virtual ~TexturedAlphaShader() {}

   private:
      struct batch_t {
         GLuint vbo;
         GLint nVerts;
         bool rendered;
      };

      static const uint_t MIN_BATCH_SIZE = 16;
      static const uint_t MAX_BATCH_SIZE = 512;

      void processPending();
      void batchAndRenderPendingModels();
      void renderBatch(const batch_t& batch);
      void renderPendingModels();
      void renderModel(const IModel* model, const cml::matrix44f_c& projMat);
      inline bool matEquals(const GLfloat* A, const GLfloat* B) const;
      inline bool isCompatibleWithPending(const IModel* A) const;
      long hashByteArray(const byte_t* array, size_t len) const;
      long hashModel(const IModel* model) const;
      long hashPendingModels() const;
      void transformVertex(GLfloat* vertex, const GLfloat* matrix) const;

      GLint m_id;

      GLint m_locPosition;
      GLint m_locColour;
      GLint m_locBUniColour;
      GLint m_locUniColour;
      GLint m_locTexCoord;
      GLint m_locMV;
      GLint m_locP;

      std::map<long, batch_t> m_VBOs;
      std::vector<byte_t> m_geometry;
//      std::vector<byte_t> m_mvData;

      std::vector<const IModel*> m_pending;
      const cml::matrix44f_c* m_P;
};

//===========================================
// TexturedAlphaShader::matEquals
//===========================================
inline bool TexturedAlphaShader::matEquals(const GLfloat* A, const GLfloat* B) const {
   return memcmp(A, B, 16 * sizeof(GLfloat));
}

//===========================================
// TexturedAlphaShader::isCompatibleWithPending
//===========================================
inline bool TexturedAlphaShader::isCompatibleWithPending(const IModel* A) const {
   if (m_pending.size() == 0) return true;

   const IModel* B = m_pending.back();

   return model_containsPerVertexColourData(*A) == model_containsPerVertexColourData(*B)
      && A->getPrimitiveType() == B->getPrimitiveType()
      && A->getTextureHandle() == B->getTextureHandle()
      && A->getColour() == B->getColour()
      && A->getLineWidth() == B->getLineWidth()
      && matEquals(model_getMatrix(*A), m_P->data());
}


}


#endif
