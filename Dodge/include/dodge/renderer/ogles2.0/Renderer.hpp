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
#include <mutex>
#include <thread>
#include <atomic>
#include <boost/shared_ptr.hpp>
#include <boost/variant.hpp>
#include "Colour.hpp"
#include "../../WinIO.hpp"
#include "../RenderBrush.hpp"
#include "../Camera.hpp"
#include "../../Asset.hpp"
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
         // ...
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

            void unlock() const { m_mutex.unlock(); }
            void lock() const { m_mutex.lock(); }

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

            mutable std::mutex m_mutex;
      };

      typedef boost::shared_ptr<Model> pModel_t;

      void setBgColour(const Colour& col);

      inline void attachCamera(boost::shared_ptr<Camera> camera);
      inline Camera& getCamera() const;

      inline void attachBrush(boost::shared_ptr<RenderBrush> brush);
      inline RenderBrush& getBrush() const;

      void onWindowResize(int_t w, int_t h);
      void newTexture(const textureData_t* texture, int_t width, int_t height, textureHandle_t* handle);

      void bufferModel(pModel_t model);
      void freeBufferedModel(pModel_t model);

      void stageModel(const pModel_t model);
      void unStageModel(const pModel_t model);
#ifdef DEBUG
      inline long getFrameRate() const;
#endif
      void start();
      void stop();

      void checkForErrors();

      enum exceptionType_t {
         UNKNOWN_EXCEPTION,
         RENDERER_EXCEPTION
         // ...
      };

      struct exceptionWrapper_t {
         exceptionType_t type;
         void* data;
      };

   private:

      // Stores model pointers in order; first by render mode and then by texture handle.
      class SceneGraph {
         friend class iterator;

         private:
            typedef std::pair<mode_t, textureHandle_t> key_t;
            typedef std::pair<key_t, pModel_t> entry_t;
            typedef std::set<entry_t> container_t;

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

            void remove(pModel_t model) {
               if (m_container.erase(entry_t(key_t(model->renderMode, model->texHandle), model)) == 0) {

                  for (auto i = m_container.begin(); i != m_container.end(); ++i) {
                     if (i->second == model) {
                        m_container.erase(i);
                        break;
                     }
                  }
               }
            }

            void clear() { m_container.clear(); }
            iterator begin() { return iterator(this, m_container.begin()); }
            iterator end() { return iterator(this, m_container.end()); }

         private:
            container_t m_container;
      };

      typedef enum {
         MSG_TEX_HANDLE_REQ,
         MSG_VP_RESIZE_REQ,
         MSG_CONSTRUCT_VBO
         // ...
      } msgType_t;

      struct msgTexHandleReq_t {
         const textureData_t* texData;
         int_t w;
         int_t h;

         textureHandle_t* retVal;
      };

      struct msgVpResizeReq_t {
         int_t w;
         int_t h;
      };

      struct msgConstructVbo_t {
         pModel_t model;
      };

      typedef boost::variant<
         msgTexHandleReq_t,
         msgVpResizeReq_t,
         msgConstructVbo_t
         // ...
      > msgData_t;

      struct Message {
         Message(msgType_t type_, msgData_t data_)
            : type(type_), data(data_) {}

         msgType_t type;
         msgData_t data;
      };

      static void renderLoop();

      void init();
      void clear();
      void setMode(mode_t mode);
      void constructShaderProgs();
      void constructTexturedShaderProg();
      void constructNonTexturedShaderProg();
      inline bool isSupportedPrimitive(primitive_t primitiveType) const;
      GLint primitiveToGLType(primitive_t primitiveType) const;
      void newShaderFromSource(const char** shaderSrc, GLint type, GLint prog);
      void processMsg(const Message& msg);
      textureHandle_t loadTexture(const textureData_t* texture, int_t w, int_t h);
      void constructVBO(pModel_t model);
      void queueMsg(Message msg);
#ifdef DEBUG
      void computeFrameRate();
#endif

      static std::map<mode_t, GLint> m_shaderProgIds;

      static GLint m_locPosition;
      static GLint m_locColour;
      static GLint m_locBUniColour;
      static GLint m_locUniColour;
      static GLint m_locTexCoord;
      static GLint m_locMV;
      static GLint m_locP;

      static std::atomic<bool> m_init;
      static mode_t m_mode;

      static SceneGraph m_sceneGraph;
      static std::mutex m_sceneGraphMutex;

      static boost::shared_ptr<RenderBrush> m_brush;
      static std::mutex m_brushMutex;

      static boost::shared_ptr<Camera> m_camera;
      static std::mutex m_cameraMutex;

      static std::atomic<bool> m_running;
      static std::thread* m_thread;

      static std::vector<Message> m_msgQueue;
      static std::mutex m_msgQueueMutex;

      static std::atomic<bool> m_msgQueueEmpty;

      static Colour m_bgColour;
      static std::mutex m_bgColourMutex;

      static exceptionWrapper_t m_exception;
      static std::atomic<bool> m_errorPending;

#ifdef DEBUG
      static std::atomic<long> m_frameRate;
#endif
};

#ifdef DEBUG
//===========================================
// Renderer::getFrameRate
//===========================================
inline long Renderer::getFrameRate() const {
   return m_frameRate;
}
#endif

//===========================================
// Renderer::isSupportedPrimitive
//===========================================
inline bool Renderer::isSupportedPrimitive(primitive_t primitiveType) const {
   return primitiveType == TRIANGLES || primitiveType == LINES;
}

//===========================================
// Renderer::attachCamera
//===========================================
inline void Renderer::attachCamera(boost::shared_ptr<Camera> camera) {
   m_cameraMutex.lock();
   m_camera = camera;
   m_cameraMutex.unlock();
}

//===========================================
// Renderer::getCamera
//===========================================
inline Camera& Renderer::getCamera() const {
   m_cameraMutex.lock();
   Camera& cpy = *m_camera;
   m_cameraMutex.unlock();

   return cpy;
}

//===========================================
// Renderer::attachBrush
//===========================================
inline void Renderer::attachBrush(boost::shared_ptr<RenderBrush> brush) {
   m_brushMutex.lock();
   m_brush = brush;
   m_brushMutex.unlock();
}

//===========================================
// Renderer::getBrush
//===========================================
inline RenderBrush& Renderer::getBrush() const {
   m_brushMutex.lock();
   RenderBrush& cpy = *m_brush;
   m_brushMutex.unlock();

   return cpy;
}


}


#endif /*!__RENDERER_HPP__*/
