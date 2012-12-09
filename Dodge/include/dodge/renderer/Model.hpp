/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __MODEL_HPP__
#define __MODEL_HPP__


#include "Renderer.hpp"


namespace Dodge {


class IModel {
   friend class ShaderProgram;
   friend class Renderer;

   public:
      virtual Renderer::primitive_t getPrimitiveType() const = 0;
      virtual uint_t getNumVertices() const = 0;
      virtual void setMatrix(const Renderer::matrixElement_t* matrix) = 0;
      virtual void setMatrixElement(uint_t idx, Renderer::matrixElement_t val) = 0;
      virtual Renderer::matrixElement_t getMatrixElement(uint_t idx) const = 0;
      virtual void getMatrix(Renderer::matrixElement_t* matrix) const = 0;
      virtual void setColour(const Colour& colour) = 0;
      virtual Colour getColour() const = 0;
      virtual void setLineWidth(Renderer::int_t lineWidth) = 0;
      virtual Renderer::int_t getLineWidth() const = 0;
      virtual void setTextureHandle(Renderer::textureHandle_t texHandle) = 0;
      virtual Renderer::textureHandle_t getTextureHandle() const = 0;
      virtual Renderer::mode_t getRenderMode() const = 0;

      virtual ~IModel() {}

   private:
      virtual void lock() const = 0;
      virtual void unlock() const = 0;

      virtual Renderer::modelHandle_t getHandle() const = 0;
      virtual void setHandle(Renderer::modelHandle_t handle) = 0;
      virtual bool containsPerVertexColourData() const = 0;
      virtual size_t vertexDataSize() const = 0;
      virtual const void* getVertexData() const = 0;
      virtual const Renderer::matrixElement_t* getMatrix() const = 0;
};


template <class T>
class Model : public IModel {
   friend class ShaderProgram;
   friend class Renderer;

   public:
      //===========================================
      // Model::Model
      //===========================================
      Model(Renderer::mode_t renderMode, Renderer::primitive_t primitiveType, bool perVertexColourData)
         : m_matrix({1.f, 0.f, 0.f, 0.f,
                     0.f, 1.f, 0.f, 0.f,
                     0.f, 0.f, 1.f, 0.f,
                     0.f, 0.f, 0.f, 1.f}),
           m_verts(NULL),
           m_n(0),
           m_texHandle(0),
           m_lineWidth(1),
           m_primitiveType(primitiveType),
           m_colData(perVertexColourData),
           m_renderMode(renderMode),
           m_handle(0) {}

      //===========================================
      // Model::Model
      //===========================================
      Model(const Model& cpy) : m_n(0) {
         m_mutex.lock();
         deepCopy(cpy);
         m_mutex.unlock();
      }

      //===========================================
      // Model::operator=
      //===========================================
      Model& operator=(const Model& rhs) {
         m_mutex.lock();
         deepCopy(rhs);
         m_mutex.unlock();

         return *this;
      }

      //===========================================
      // Model::getPrimitiveType
      //===========================================
      virtual Renderer::primitive_t getPrimitiveType() const {
         m_mutex.lock();
         Renderer::primitive_t cpy = m_primitiveType;
         m_mutex.unlock();

         return cpy;
      }

      //===========================================
      // Model::getNumVertices
      //===========================================
      virtual uint_t getNumVertices() const {
         m_mutex.lock();
         uint_t cpy = m_n;
         m_mutex.unlock();

         return cpy;
      }

      //===========================================
      // Model::setMatrix
      //===========================================
      virtual void setMatrix(const Renderer::matrixElement_t* matrix) {
         m_mutex.lock();
         memcpy(m_matrix, matrix, 16 * sizeof(Renderer::matrixElement_t));
         m_mutex.unlock();
      }

      //===========================================
      // Model::setMatrixElement
      //===========================================
      virtual void setMatrixElement(uint_t idx, Renderer::matrixElement_t val) {
         m_mutex.lock();
         m_matrix[idx] = val;
         m_mutex.unlock();
      }

      //===========================================
      // Model::getMatrixElement
      //===========================================
      virtual Renderer::matrixElement_t getMatrixElement(uint_t idx) const {
         m_mutex.lock();
         Renderer::matrixElement_t m = m_matrix[idx];
         m_mutex.unlock();

         return m;
      }

      //===========================================
      // Model::getMatrix
      //===========================================
      virtual void getMatrix(Renderer::matrixElement_t* matrix) const {
         m_mutex.lock();
         memcpy(matrix, m_matrix, 16 * sizeof(Renderer::matrixElement_t));
         m_mutex.unlock();
      }

      //===========================================
      // Model::setColour
      //===========================================
      virtual void setColour(const Colour& colour) {
         m_mutex.lock();
         m_colour = colour;
         m_mutex.unlock();
      }

      //===========================================
      // Model::getColour
      //===========================================
      virtual Colour getColour() const {
         m_mutex.lock();
         Colour cpy = m_colour;
         m_mutex.unlock();

         return cpy;
      }

      //===========================================
      // Model::setLineWidth
      //===========================================
      virtual void setLineWidth(Renderer::int_t lineWidth) {
         m_mutex.lock();
         m_lineWidth = lineWidth;
         m_mutex.unlock();
      }

      //===========================================
      // Model::getLineWidth
      //===========================================
      virtual Renderer::int_t getLineWidth() const {
         m_mutex.lock();
         Renderer::int_t cpy = m_lineWidth;
         m_mutex.unlock();

         return cpy;
      }

      //===========================================
      // Model::setTextureHandle
      //===========================================
      virtual void setTextureHandle(Renderer::textureHandle_t texHandle) {
         m_mutex.lock();
         m_texHandle = texHandle;
         m_mutex.unlock();
      }

      //===========================================
      // Model::getTextureHandle
      //===========================================
      virtual Renderer::textureHandle_t getTextureHandle() const {
         m_mutex.lock();
         Renderer::textureHandle_t cpy = m_texHandle;
         m_mutex.unlock();

         return cpy;
      }

      //===========================================
      // Model::getRenderMode
      //===========================================
      virtual Renderer::mode_t getRenderMode() const {
         m_mutex.lock();
         Renderer::mode_t cpy = m_renderMode;
         m_mutex.unlock();

         return cpy;
      }

      //===========================================
      // Model::setVertices
      //===========================================
      void setVertices(uint_t idx, const T* verts, uint_t num) {
         m_mutex.lock();

         if (idx + num > m_n) {
            T* tmp = new T[idx + num];
            memcpy(tmp, m_verts, sizeof(T) * idx);

            delete[] m_verts;
            m_verts = tmp;

            m_n = idx + num;
         }

         memcpy(m_verts + idx, verts, sizeof(T) * num);

         m_mutex.unlock();
      }

      //===========================================
      // Model::setVertex
      //===========================================
      void setVertex(uint_t idx, const T& vert) {
         m_mutex.lock();
         m_verts[idx] = vert;
         m_mutex.unlock();
      }

      //===========================================
      // Model::getVertices
      //===========================================
      void getVertices(T* dest, uint_t startIdx, uint_t endIdx) const {
         m_mutex.lock();
         memcpy(dest, m_verts + startIdx, sizeof(T) * (endIdx - startIdx));
         m_mutex.unlock();
      }

      //===========================================
      // Model::getVertex
      //===========================================
      T getVertex(uint_t idx) const {
         m_mutex.lock();
         T cpy = m_verts[idx];
         m_mutex.unlock();

         return cpy;
      }

      //===========================================
      // Model::~Model
      //===========================================
      virtual ~Model() {
         delete[] m_verts;
      }

   private:
      //===========================================
      // Model::deepCopy
      //===========================================
      void deepCopy(const Model& cpy) {
         cpy.m_mutex.lock();

         memcpy(m_matrix, cpy.m_matrix, 16 * sizeof(Renderer::matrixElement_t));
         setVertices(0, cpy.m_verts, cpy.m_n);
         m_texHandle = cpy.m_texHandle;
         m_colour = cpy.m_colour;
         m_lineWidth = cpy.m_lineWidth;
         m_primitiveType = cpy.m_primitiveType;
         m_colData = cpy.m_colData;
         m_renderMode = cpy.m_renderMode;
         m_handle = 0;

         cpy.m_mutex.unlock();
      }

      mutable std::recursive_mutex m_mutex;

      Renderer::matrixElement_t m_matrix[16];
      T* m_verts;
      uint_t m_n;
      Renderer::textureHandle_t m_texHandle;
      Colour m_colour;
      Renderer::int_t m_lineWidth;
      Renderer::primitive_t m_primitiveType;
      bool m_colData;

      Renderer::mode_t m_renderMode;
      Renderer::modelHandle_t m_handle;

      virtual void lock() const { m_mutex.lock(); }
      virtual void unlock() const { m_mutex.unlock(); }

      virtual Renderer::modelHandle_t getHandle() const {
         return m_handle;
      }

      virtual void setHandle(Renderer::modelHandle_t handle) {
         m_handle = handle;
      }

      virtual bool containsPerVertexColourData() const {
         return m_colData;
      }

      virtual size_t vertexDataSize() const {
         return m_n * sizeof(T);
      }

      virtual const void* getVertexData() const {
         return m_verts;
      }

      virtual const Renderer::matrixElement_t* getMatrix() const {
         return m_matrix;
      }
};


struct vvvcccc_t {
   Renderer::vertexElement_t v1;
   Renderer::vertexElement_t v2;
   Renderer::vertexElement_t v3;
   Renderer::colourElement_t c1;
   Renderer::colourElement_t c2;
   Renderer::colourElement_t c3;
   Renderer::colourElement_t c4;
};

struct vvv_t {
   Renderer::vertexElement_t v1;
   Renderer::vertexElement_t v2;
   Renderer::vertexElement_t v3;
};

struct vvvttcccc_t {
   Renderer::vertexElement_t v1;
   Renderer::vertexElement_t v2;
   Renderer::vertexElement_t v3;
   Renderer::texCoordElement_t t1;
   Renderer::texCoordElement_t t2;
   Renderer::colourElement_t c1;
   Renderer::colourElement_t c2;
   Renderer::colourElement_t c3;
   Renderer::colourElement_t c4;
};

struct vvvtt_t {
   Renderer::vertexElement_t v1;
   Renderer::vertexElement_t v2;
   Renderer::vertexElement_t v3;
   Renderer::texCoordElement_t t1;
   Renderer::texCoordElement_t t2;
};


class ColouredNonTexturedAlphaModel : public Model<vvvcccc_t> {
   public:
      ColouredNonTexturedAlphaModel(Renderer::primitive_t primitiveType)
         : Model(Renderer::NONTEXTURED_ALPHA, primitiveType, true) {}
};

class PlainNonTexturedAlphaModel : public Model<vvv_t> {
   public:
      PlainNonTexturedAlphaModel(Renderer::primitive_t primitiveType)
         : Model(Renderer::NONTEXTURED_ALPHA, primitiveType, false) {}
};

class ColouredTexturedAlphaModel : public Model<vvvttcccc_t> {
   public:
      ColouredTexturedAlphaModel(Renderer::primitive_t primitiveType)
         : Model(Renderer::TEXTURED_ALPHA, primitiveType, true) {}
};

class PlainTexturedAlphaModel : public Model<vvvtt_t> {
   public:
      PlainTexturedAlphaModel(Renderer::primitive_t primitiveType)
         : Model(Renderer::TEXTURED_ALPHA, primitiveType, false) {}
};


}


#endif
