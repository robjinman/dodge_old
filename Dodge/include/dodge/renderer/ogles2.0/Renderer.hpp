/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__


#include <GLES2/gl2.h>
#include <cml/cml.h>
#include <set>
#include <map>
#include <cstring>
#include <boost/shared_ptr.hpp>
#include "Colour.hpp"
#include "../RenderBrush.hpp"
#include "../Camera.hpp"
#include "../../Asset.hpp"
#include "../../GL_CHECK.hpp"
#include "../../xml/xml.hpp"
#include "../../definitions.hpp"


namespace Dodge {


// OpenGL ES 2.0 implementation
class Renderer {
   public:
      typedef GLint int_t;
      typedef GLfloat float_t;
      typedef GLfloat vertexElement_t;
      typedef GLfloat matrixElement_t;
      typedef GLfloat colourElement_t;
      typedef GLfloat texCoordElement_t;
      typedef byte_t textureData_t;
      typedef GLuint textureHandle_t;
      typedef GLuint modelHandle_t;

      enum mode_t {
         UNDEFINED,
         TEXTURED_ALPHA,
         NONTEXTURED_ALPHA
      };

      enum primitive_t {
         TRIANGLES,
         LINES,
         QUADS,
         TRIANGLE_STRIP
      };

      struct vvvtt_t {
         vertexElement_t v1;
         vertexElement_t v2;
         vertexElement_t v3;
         texCoordElement_t t1;
         texCoordElement_t t2;
      };

      struct vvv_t {
         vertexElement_t v1;
         vertexElement_t v2;
         vertexElement_t v3;
      };

      struct vvvttcccc_t {
         vertexElement_t v1;
         vertexElement_t v2;
         vertexElement_t v3;
         texCoordElement_t t1;
         texCoordElement_t t2;
         colourElement_t c1;
         colourElement_t c2;
         colourElement_t c3;
         colourElement_t c4;
      };

      struct vvvcccc_t {
         vertexElement_t v1;
         vertexElement_t v2;
         vertexElement_t v3;
         colourElement_t c1;
         colourElement_t c2;
         colourElement_t c3;
         colourElement_t c4;
      };

      class Model {
         friend class Renderer;

         public:
            Model(mode_t kind, bool perVertexColours)
               : primitiveType(TRIANGLES),
                 verts(NULL),
                 n(0),
                 matrix({1.f, 0.f, 0.f, 0.f,
                         0.f, 1.f, 0.f, 0.f,
                         0.f, 0.f, 1.f, 0.f,
                         0.f, 0.f, 0.f, 1.f}),
                 texHandle(0),
                 handle(0),
                 renderMode(kind),
                 colData(perVertexColours) {}

            primitive_t primitiveType;
            void* verts;
            int_t n;
            matrixElement_t matrix[16];
            textureHandle_t texHandle;

            ~Model() {
               if (colData) {
                  switch (renderMode) {
                     case TEXTURED_ALPHA:    delete[] reinterpret_cast<vvvttcccc_t*>(verts); break;
                     case NONTEXTURED_ALPHA: delete[] reinterpret_cast<vvvcccc_t*>(verts);   break;
                     default: break;
                  }
               }
               else {
                  switch (renderMode) {
                     case TEXTURED_ALPHA:    delete[] reinterpret_cast<vvvtt_t*>(verts); break;
                     case NONTEXTURED_ALPHA: delete[] reinterpret_cast<vvv_t*>(verts);   break;
                     default: break;
                  }
               }
            }

         private:
            modelHandle_t handle;
            mode_t renderMode;
            bool colData;

            Colour colour;
            int_t lineWidth;
      };

      typedef boost::shared_ptr<Model> pModel_t;

      void init(const char* optsFile = NULL);

      textureHandle_t newTexture(const textureData_t* texture, int_t width, int_t height);

      inline void attachCamera(boost::shared_ptr<Camera> camera);
      inline const Camera& getCamera() const;

      inline void attachBrush(boost::shared_ptr<RenderBrush> brush);
      inline const RenderBrush& getBrush() const;

      void onWindowResize(int_t w, int_t h);

      void bufferModel(pModel_t model);
      void freeBufferedModel(pModel_t model);

      void stageModel(pModel_t model);
      void render();

      void clear();

   private:

      // Stores model pointers in order; first by render mode and then by texture handle.
      class SceneGraph {
         friend class iterator;

         private:
            typedef std::pair<mode_t, textureHandle_t> key_t;
            typedef std::pair<key_t, pModel_t> entry_t;
            typedef std::multiset<entry_t> container_t;

         public:
            class iterator {
               friend class SceneGraph;

               public:
                  pModel_t operator*() { return m_i->second; }
                  pModel_t operator->() { return m_i->second; }
                  bool operator!=(const iterator& rhs) const { return !(*this == rhs); }
                  bool operator==(const iterator& rhs) const { return m_i == rhs.m_i; }

                  iterator& operator++() {
                     ++m_i;
                     return *this;
                  }

               private:
                  iterator(SceneGraph* sg, SceneGraph::container_t::iterator i)
                     : m_sg(sg), m_i(i) {}

                  SceneGraph* m_sg;
                  SceneGraph::container_t::iterator m_i;
            };

            void insert(pModel_t model) {
               m_container.insert(entry_t(key_t(model->renderMode, model->texHandle), model));
            }

            void clear() { m_container.clear(); }
            iterator begin() { return iterator(this, m_container.begin()); }
            iterator end() { return iterator(this, m_container.end()); }

         private:
            container_t m_container;
      };

      void setMode(mode_t mode);
      void constructShaderProgs();
      void constructTexturedShaderProg();
      void constructNonTexturedShaderProg();
      inline bool isSupportedPrimitive(primitive_t primitiveType) const;
      inline GLint primitiveToGLType(primitive_t primitiveType) const;

      void newShaderFromSource(const char** shaderSrc, GLint type, GLint prog);

      static std::map<mode_t, GLint> m_shaderProgIds;

      static GLint m_locPosition;
      static GLint m_locColour;
      static GLint m_locBUniColour;
      static GLint m_locUniColour;
      static GLint m_locTexCoord;
      static GLint m_locMV;
      static GLint m_locP;

      static bool m_init;
      static mode_t m_mode;

      static SceneGraph m_sceneGraph;
      static boost::shared_ptr<RenderBrush> m_brush;
      static boost::shared_ptr<Camera> m_camera;
};

//===========================================
// Renderer::isSupportedPrimitive
//===========================================
inline bool Renderer::isSupportedPrimitive(primitive_t primitiveType) const {
   return primitiveType == TRIANGLES || primitiveType == LINES;
}

//===========================================
// Renderer::primitiveToGLType
//===========================================
inline GLint Renderer::primitiveToGLType(primitive_t primitiveType) const {
   switch (primitiveType) {
      case TRIANGLES: return GL_TRIANGLES;
      case LINES: return GL_LINES;
      default:
         throw Exception("Primitive not supported", __FILE__, __LINE__);
   }
}

//===========================================
// Renderer::attachCamera
//===========================================
inline void Renderer::attachCamera(boost::shared_ptr<Camera> camera) {
   m_camera = camera;
}

//===========================================
// Renderer::getCamera
//===========================================
inline const Camera& Renderer::getCamera() const {
   return *m_camera;
}

//===========================================
// Renderer::attachBrush
//===========================================
inline void Renderer::attachBrush(boost::shared_ptr<RenderBrush> brush) {
   m_brush = brush;
}

//===========================================
// Renderer::getBrush
//===========================================
inline const RenderBrush& Renderer::getBrush() const {
   return *m_brush;
}


}


#endif /*!__RENDERER_HPP__*/
