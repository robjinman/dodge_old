/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __MODEL_HPP__
#define __MODEL_HPP__


#include "../StringId.hpp"
#include "Renderer.hpp"


namespace Dodge {


class IModel {
   friend class SceneGraph;
   friend class RenderMode;
   friend class Renderer;

   public:
      //===========================================
      // IModel::IModel
      //===========================================
      IModel()
         : m_primitiveType(Renderer::TRIANGLES),
           m_renderMode(Renderer::UNDEFINED),
           m_id(nextId) {

         ++nextId;

         memset(m_matrix, 0, 16 * sizeof(Renderer::matrixElement_t));
}

      //===========================================
      // IModel::IModel
      //===========================================
      IModel(Renderer::mode_t renderMode, Renderer::primitive_t primitiveType)
         : m_primitiveType(primitiveType),
           m_renderMode(renderMode),
           m_id(nextId) {

         ++nextId;
         m_matrix[0] = 1.0; m_matrix[4] = 0.0; m_matrix[8]  = 0.0; m_matrix[12] = 0.0;
         m_matrix[1] = 0.0; m_matrix[5] = 1.0; m_matrix[9]  = 0.0; m_matrix[13] = 0.0;
         m_matrix[2] = 0.0; m_matrix[6] = 0.0; m_matrix[10] = 1.0; m_matrix[14] = 0.0;
         m_matrix[3] = 0.0; m_matrix[7] = 0.0; m_matrix[11] = 0.0; m_matrix[15] = 1.0;
      }

      //===========================================
      // IModel::getPrimitiveType
      //===========================================
      inline Renderer::primitive_t getPrimitiveType() const {
         return m_primitiveType;
      }

      //===========================================
      // IModel::setMatrix
      //===========================================
      void setMatrix(const Renderer::matrixElement_t* matrix) {
         memcpy(m_matrix, matrix, 16 * sizeof(Renderer::matrixElement_t));
      }

      //===========================================
      // IModel::setMatrixElement
      //===========================================
      void setMatrixElement(uint_t idx, Renderer::matrixElement_t val) {
         m_matrix[idx] = val;
      }

      //===========================================
      // IModel::getMatrixElement
      //===========================================
      Renderer::matrixElement_t getMatrixElement(uint_t idx) const {
         return m_matrix[idx];
      }

      //===========================================
      // IModel::getMatrix
      //===========================================
      void getMatrix(Renderer::matrixElement_t* matrix) const {
         memcpy(matrix, m_matrix, 16 * sizeof(Renderer::matrixElement_t));
      }

      //===========================================
      // IModel::getRenderMode
      //===========================================
      Renderer::mode_t getRenderMode() const {
         return m_renderMode;
      }

      virtual uint_t getNumVertices() const = 0;
      virtual void setColour(const Colour& colour) = 0;
      virtual Colour getColour() const = 0;
      virtual void setLineWidth(Renderer::int_t lineWidth) = 0;
      virtual Renderer::int_t getLineWidth() const = 0;
      virtual void setTextureHandle(Renderer::textureHandle_t texHandle) = 0;
      virtual Renderer::textureHandle_t getTextureHandle() const = 0;
      virtual size_t getTotalSize() const = 0;
      virtual void getVertices(void* dest, uint_t startIdx, uint_t endIdx) const = 0;
      virtual long getVertexLayout() const = 0;

#ifdef DEBUG
      //===========================================
      // IModel::dbg_print
      //===========================================
      virtual void dbg_print(std::ostream& out, int tab = 0) const {
         for (int t = 0; t < tab; ++t) out << "\t";
         out << "IModel\n";

         for (int t = 0; t < tab + 1; ++t) out << "\t";
         out << "matrix:\n";

         for (int i = 0; i < 4; ++i) {
            for (int t = 0; t < tab + 2; ++t) out << "\t";

            for (int j = 0; j < 4; ++j)
               out << m_matrix[i * 4 + j] << " ";

            out << "\n";
         }

         for (int t = 0; t < tab + 1; ++t) out << "\t";
         out << "primitiveType: " << m_primitiveType << "\n";

         for (int t = 0; t < tab + 1; ++t) out << "\t";
         out << "renderMode: " << m_renderMode << "\n";
      }
#endif

      virtual ~IModel() {}

   protected:
      void shallowCopy(const IModel& cpy) {
         memcpy(m_matrix, cpy.m_matrix, 16 * sizeof(Renderer::matrixElement_t));
         m_primitiveType = cpy.m_primitiveType;
         m_renderMode = cpy.m_renderMode;
      }

      // Returns transformed z-coord of first vertex
      virtual float32_t getDepth() const = 0;

      virtual size_t getSizeOf() const = 0;
      virtual size_t vertexDataSize() const = 0;
      virtual const void* getVertexData() const = 0;
      virtual void copyTo(void* ptr) const = 0;

      Renderer::matrixElement_t m_matrix[16];
      Renderer::primitive_t m_primitiveType;
      Renderer::mode_t m_renderMode;
      long m_id;

   private:
      static long nextId;
};


template <class T>
class Model : public IModel {

   friend class RenderMode;
   friend class Renderer;

   public:
      //===========================================
      // Model::Model
      //===========================================
      Model(long vertLayout, Renderer::mode_t renderMode, Renderer::primitive_t primitiveType)
         : IModel(renderMode, primitiveType),
           m_vertLayout(vertLayout), m_verts(NULL), m_n(0), m_texHandle(0), m_lineWidth(0) {}

      //===========================================
      // Model::Model
      //===========================================
      Model(const Model& cpy) : m_n(0) {
         deepCopy(cpy);
      }

      //===========================================
      // Model::operator=
      //===========================================
      Model& operator=(const Model& rhs) {
         deepCopy(rhs);
         return *this;
      }

      //===========================================
      // Model::getNumVertices
      //===========================================
      virtual uint_t getNumVertices() const {
         return m_n;
      }

      //===========================================
      // Model::setColour
      //===========================================
      virtual void setColour(const Colour& colour) {
         m_colour = colour;
      }

      //===========================================
      // Model::getColour
      //===========================================
      virtual Colour getColour() const {
         return m_colour;
      }

      //===========================================
      // Model::setLineWidth
      //===========================================
      virtual void setLineWidth(Renderer::int_t lineWidth) {
         m_lineWidth = lineWidth;
      }

      //===========================================
      // Model::getLineWidth
      //===========================================
      virtual Renderer::int_t getLineWidth() const {
         return m_lineWidth;
      }

      //===========================================
      // Model::setTextureHandle
      //===========================================
      virtual void setTextureHandle(Renderer::textureHandle_t texHandle) {
         m_texHandle = texHandle;
      }

      //===========================================
      // Model::getTextureHandle
      //===========================================
      virtual Renderer::textureHandle_t getTextureHandle() const {
         return m_texHandle;
      }

      //===========================================
      // Model::setVertices
      //===========================================
      void setVertices(uint_t idx, const T* verts, uint_t num) {
         if (idx + num > m_n) {
            T* tmp = new T[idx + num];
            memcpy(tmp, m_verts, sizeof(T) * idx);

            delete[] m_verts;
            m_verts = tmp;

            m_n = idx + num;
         }

         memcpy(m_verts + idx, verts, sizeof(T) * num);
      }

      //===========================================
      // Model::setVertex
      //===========================================
      void setVertex(uint_t idx, const T& vert) {
         m_verts[idx] = vert;
      }

      //===========================================
      // Model::getVertices
      //===========================================
      virtual void getVertices(void* dest, uint_t startIdx, uint_t endIdx) const {
         memcpy(dest, m_verts + startIdx, sizeof(T) * (endIdx - startIdx));
      }

      //===========================================
      // Model::getVertex
      //===========================================
      T getVertex(uint_t idx) const {
         return m_verts[idx];
      }

      //===========================================
      // Model::eraseVertices
      //===========================================
      void eraseVertices() {
         delete[] m_verts;
         m_verts = NULL;
         m_n = 0;
      }

      //===========================================
      // Model::getTotalSize
      //===========================================
      virtual size_t getTotalSize() const {
         return getSizeOf() + sizeof(T) * m_n;
      }

      //===========================================
      // Model::getVertexLayout
      //===========================================
      virtual long getVertexLayout() const {
         return m_vertLayout;
      }

      //===========================================
      // Model::~Model
      //===========================================
      virtual ~Model() {
         delete[] m_verts;
      }

#ifdef DEBUG
      //===========================================
      // Model::dbg_print
      //===========================================
      virtual void dbg_print(std::ostream& out, int tab = 0) const {
         for (int t = 0; t < tab; ++t) out << "\t";
         out << "Model\n";

         IModel::dbg_print(out, tab + 1);

         for (int t = 0; t < tab + 1; ++t) out << "\t";
         out << "vertex layout: " << getInternedString(m_vertLayout) << "\n";

         for (int t = 0; t < tab + 1; ++t) out << "\t";
         out << "textureHandle: " << m_texHandle << "\n";

         for (int t = 0; t < tab + 1; ++t) out << "\t";
         out << "colour: (" << m_colour.r << " " << m_colour.g << " " << m_colour.b << " " << m_colour.a << ")\n";

         for (int t = 0; t < tab + 1; ++t) out << "\t";
         out << "lineWidth: " << m_lineWidth << "\n";

         for (int t = 0; t < tab + 1; ++t) out << "\t";
         out << "vertices (" << m_n << "):\n";

         for (uint_t i = 0; i < m_n; ++i)
            m_verts[i].dbg_print(out, tab + 2);
      }
#endif

   private:
      //===========================================
      // Model::Model
      //===========================================
      Model()
         : IModel(),
           m_vertLayout(0),
           m_verts(NULL),
           m_n(0),
           m_texHandle(0),
           m_lineWidth(0) {}

      //===========================================
      // Model::deepCopy
      //===========================================
      void deepCopy(const Model& cpy) {
         shallowCopy(cpy);
         setVertices(0, cpy.m_verts, cpy.m_n);
      }

      //===========================================
      // Model::shallowCopy
      //===========================================
      void shallowCopy(const Model& cpy) {
         IModel::shallowCopy(cpy);

         m_vertLayout = cpy.m_vertLayout;
         m_texHandle = cpy.m_texHandle;
         m_colour = cpy.m_colour;
         m_lineWidth = cpy.m_lineWidth;
      }

      //===========================================
      // Model::vertexDataSize
      //===========================================
      virtual size_t vertexDataSize() const {
         return m_n * sizeof(T);
      }

      //===========================================
      // Model::getVertexData
      //===========================================
      virtual const void* getVertexData() const {
         return m_verts;
      }

      //===========================================
      // Model::getSizeOf
      //===========================================
      virtual size_t getSizeOf() const {
         return sizeof(Model<T>);
      }

      //===========================================
      // Model::getDepth
      //===========================================
      virtual float32_t getDepth() const {
         float32_t z = m_n > 0 ? m_verts[0].v3 : 0;
         return z + m_matrix[14];
      }

      //===========================================
      // Model::copyTo
      //===========================================
      virtual void copyTo(void* ptr) const {
         Model<T>* pModel = new(ptr) Model<T>();
         pModel->shallowCopy(*this);
         pModel->m_id = m_id;
         pModel->m_n = m_n;

         byte_t* p = reinterpret_cast<byte_t*>(ptr);
         byte_t* verts = p + sizeof(Model<T>);

         memcpy(verts, m_verts, m_n * sizeof(T));

         pModel->m_verts = reinterpret_cast<T*>(verts);
      }

      long m_vertLayout;
      T* m_verts;
      uint_t m_n;
      Renderer::textureHandle_t m_texHandle;
      Colour m_colour;
      Renderer::int_t m_lineWidth;
};


struct vvvcccc_t {
   vvvcccc_t() {}
   vvvcccc_t(Renderer::vertexElement_t v1_, Renderer::vertexElement_t v2_, Renderer::vertexElement_t v3_,
      Renderer::colourElement_t c1_, Renderer::colourElement_t c2_, Renderer::colourElement_t c3_,
      Renderer::colourElement_t c4_)
         : v1(v1_), v2(v2_), v3(v3_), c1(c1_), c2(c2_), c3(c3_), c4(c4_) {}

   Renderer::vertexElement_t v1;
   Renderer::vertexElement_t v2;
   Renderer::vertexElement_t v3;
   Renderer::colourElement_t c1;
   Renderer::colourElement_t c2;
   Renderer::colourElement_t c3;
   Renderer::colourElement_t c4;
#ifdef DEBUG
   void dbg_print(std::ostream& out, int tab = 0) const {
      for (int t = 0; t < tab; ++t) out << "\t";
      out << "(" << v1 << " " << v2 << " " << v3 << ") (" << c1 << " " << c2 << " " << c3 << " " << c4 << ")\n";
   }
#endif
};

struct vvv_t {
   vvv_t() {}
   vvv_t(Renderer::vertexElement_t v1_, Renderer::vertexElement_t v2_, Renderer::vertexElement_t v3_)
      : v1(v1_), v2(v2_), v3(v3_) {}

   Renderer::vertexElement_t v1;
   Renderer::vertexElement_t v2;
   Renderer::vertexElement_t v3;
#ifdef DEBUG
   void dbg_print(std::ostream& out, int tab = 0) const {
      for (int t = 0; t < tab; ++t) out << "\t";
      out << "(" << v1 << " " << v2 << " " << v3 << ")\n";
   }
#endif
};

struct vvvttcccc_t {
   vvvttcccc_t() {}
   vvvttcccc_t(Renderer::vertexElement_t v1_, Renderer::vertexElement_t v2_, Renderer::vertexElement_t v3_,
      Renderer::texCoordElement_t t1_, Renderer::texCoordElement_t t2_, Renderer::colourElement_t c1_,
      Renderer::colourElement_t c2_, Renderer::colourElement_t c3_, Renderer::colourElement_t c4_)
         : v1(v1_), v2(v2_), v3(v3_), t1(t1_), t2(t2_), c1(c1_), c2(c2_), c3(c3_), c4(c4_) {}

   Renderer::vertexElement_t v1;
   Renderer::vertexElement_t v2;
   Renderer::vertexElement_t v3;
   Renderer::texCoordElement_t t1;
   Renderer::texCoordElement_t t2;
   Renderer::colourElement_t c1;
   Renderer::colourElement_t c2;
   Renderer::colourElement_t c3;
   Renderer::colourElement_t c4;
#ifdef DEBUG
   void dbg_print(std::ostream& out, int tab = 0) const {
      for (int t = 0; t < tab; ++t) out << "\t";
      out << "(" << v1 << " " << v2 << " " << v3 << ") (" << t1 << " " << t2 << ") (" << c1 << " " << c2 << " " << c3 << " " << c4 << ")\n";
   }
#endif
};

struct vvvtt_t {
   vvvtt_t() {}
   vvvtt_t(Renderer::vertexElement_t v1_, Renderer::vertexElement_t v2_, Renderer::vertexElement_t v3_,
      Renderer::texCoordElement_t t1_, Renderer::texCoordElement_t t2_)
         : v1(v1_), v2(v2_), v3(v3_), t1(t1_), t2(t2_) {}

   Renderer::vertexElement_t v1;
   Renderer::vertexElement_t v2;
   Renderer::vertexElement_t v3;
   Renderer::texCoordElement_t t1;
   Renderer::texCoordElement_t t2;
#ifdef DEBUG
   void dbg_print(std::ostream& out, int tab = 0) const {
      for (int t = 0; t < tab; ++t) out << "\t";
      out << "(" << v1 << " " << v2 << " " << v3 << ") (" << t1 << " " << t2 << ")\n";
   }
#endif
};


// DO NOT STORE DATA IN THESE CLASSES.
class ColouredNonTexturedAlphaModel : public Model<vvvcccc_t> {
   public:
      ColouredNonTexturedAlphaModel(Renderer::primitive_t primitiveType)
         : Model(internString("vvvcccc"), Renderer::NONTEXTURED_ALPHA, primitiveType) {}
};

class PlainNonTexturedAlphaModel : public Model<vvv_t> {
   public:
      PlainNonTexturedAlphaModel(Renderer::primitive_t primitiveType)
         : Model(internString("vvv"), Renderer::NONTEXTURED_ALPHA, primitiveType) {}
};

class ColouredTexturedAlphaModel : public Model<vvvttcccc_t> {
   public:
      ColouredTexturedAlphaModel(Renderer::primitive_t primitiveType)
         : Model(internString("vvvttcccc"), Renderer::TEXTURED_ALPHA, primitiveType) {}
};

class PlainTexturedAlphaModel : public Model<vvvtt_t> {
   public:
      PlainTexturedAlphaModel(Renderer::primitive_t primitiveType)
         : Model(internString("vvvtt"), Renderer::TEXTURED_ALPHA, primitiveType) {}
};


}


#endif
