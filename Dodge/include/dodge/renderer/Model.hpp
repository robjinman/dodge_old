#ifndef __MODEL_HPP__
#define __MODEL_HPP__


#include "Renderer.hpp"


namespace Dodge {


class Model {
   friend class Renderer;

   public:
      Model(Renderer::mode_t kind, bool perVertexColours);
      Model(const Model& cpy);

      Model& operator=(const Model& rhs);

      void setVertices(Renderer::primitive_t primitiveType, const void* verts, Renderer::int_t n, size_t vertSz);
      void getVertexData(void* dest, int startByte, int endByte) const;
      Renderer::int_t getNumVertices() const;
      void setMatrix(const Renderer::matrixElement_t* matrix);
      void setMatrixElement(int idx, Renderer::matrixElement_t val);
      Renderer::matrixElement_t getMatrixElement(int idx) const;
      void getMatrix(Renderer::matrixElement_t* matrix) const;
      void setColour(const Colour& colour);
      Colour getColour() const;
      void setLineWidth(Renderer::int_t lineWidth);
      Renderer::int_t getLineWidth() const;
      void setTextureHandle(Renderer::textureHandle_t texHandle);
      Renderer::textureHandle_t getTextureHandle() const;
      Renderer::mode_t getRenderMode() const;

      ~Model();

   private:
      Renderer::matrixElement_t m_matrix[16];
      Renderer::textureHandle_t m_texHandle;
      Colour m_colour;
      Renderer::int_t m_lineWidth;

      Renderer::primitive_t m_primitiveType;
      void* m_verts;
      Renderer::int_t m_n;
      size_t m_bytes;
      Renderer::modelHandle_t m_handle;
      Renderer::mode_t m_renderMode;
      bool m_colData;

      mutable std::mutex m_mutex;

      void copyData(const Model& cpy);
      void deleteVertices();
};


}


#endif
