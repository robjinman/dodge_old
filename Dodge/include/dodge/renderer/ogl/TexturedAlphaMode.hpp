/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __TEXTURED_ALPHA_MODE_HPP__
#define __TEXTURED_ALPHA_MODE_HPP__


#include <memory>
#include "RenderMode.hpp"
#include "../../definitions.hpp"


namespace Dodge {


class TexturedAlphaMode : public RenderMode {
   public:
      TexturedAlphaMode();

      virtual bool hasPending() const;
      virtual void setActive();
      virtual void sendData(const IModel* model, const cml::matrix44f_c& projMat);
      virtual void flush();

      virtual ~TexturedAlphaMode() {}

   private:
      struct batch_t {
         GLuint vbo;
         GLint nVerts;
         bool rendered;
      };

      static const uint_t MIN_BATCH_SIZE = 24;
      static const uint_t MAX_BATCH_SIZE = 512;

      void processPending();
      void batchAndRenderPendingModels();
      void renderBatch(const batch_t& batch);
      void renderPendingModels();
      void renderModel(const IModel* model, const cml::matrix44f_c& projMat);
      inline bool isCompatibleWithPending(const IModel* A) const;
      long hashByteArray(const byte_t* array, size_t len) const;
      long hashModel(const IModel* model) const;
      long hashPendingModels() const;
      void transformVertex(GLfloat* vertex, const GLfloat* matrix) const;
      bool isSupported(const IModel* model) const;

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

      std::vector<const IModel*> m_pending;
      const cml::matrix44f_c* m_P;
};

//===========================================
// TexturedAlphaMode::isCompatibleWithPending
//===========================================
inline bool TexturedAlphaMode::isCompatibleWithPending(const IModel* A) const {
   if (m_pending.size() == 0) return true;

   const IModel* B = m_pending.back();

   return A->getVertexLayout() == B->getVertexLayout()
      && A->getPrimitiveType() == B->getPrimitiveType()
      && A->getTextureHandle() == B->getTextureHandle()
      && A->getColour() == B->getColour()
      && A->getLineWidth() == B->getLineWidth();
}


}


#endif
