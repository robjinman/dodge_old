/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__


#include <GLES2/gl2.h>
#include <cml/cml.h>
#include <map>
#include <queue>
#include <cstring>
#include <mutex>
#include <thread>
#include <atomic>
#include <boost/variant.hpp>
#include "Colour.hpp"
#include "../Camera.hpp"
#include "../../WinIO.hpp"
#include "../../definitions.hpp"


namespace Dodge {


class IModel;
class ShaderProgram;
class SceneGraph;

// OpenGL ES 2.0 implementation
class Renderer {
   public:
      static Renderer& getInstance() {
         if (!m_instance) m_instance = new Renderer;
         return *m_instance;
      }

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

      enum exceptionType_t {
         UNKNOWN_EXCEPTION,
         EXCEPTION,
         RENDERER_EXCEPTION
         // ...
      };

      struct exceptionWrapper_t {
         exceptionType_t type;
         void* data;
      };

      inline void attachCamera(pCamera_t camera);
      inline Camera& getCamera() const;

      void onWindowResize(int_t w, int_t h);
      void newTexture(const textureData_t* texture, int_t width, int_t height, textureHandle_t* handle);

      void bufferModel(IModel* model);
      void freeBufferedModel(IModel* model);

      void draw(const IModel* model);
#ifdef DEBUG
      inline long getFrameRate() const;
#endif
      void start();
      void stop();
      void tick(const Colour& bgColour = Colour(0.f, 0.f, 0.f, 1.f));

   protected:
      Renderer();

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
         IModel* model;
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

      struct renderState_t {
         enum status_t {
            IS_IDLE,
            IS_PENDING_RENDER,
            IS_BEING_RENDERED,
            IS_BEING_UPDATED
         };

         status_t status;
         std::unique_ptr<SceneGraph> sceneGraph;
         cml::matrix44f_c P;
         Colour bgColour;
      };

   private:
      static Renderer* m_instance;

      void renderLoop();
      void processMessages();

      void init();
      void clear();
      void checkForErrors();
      void setMode(mode_t mode);
      void constructShaderProgs();
      GLint primitiveToGLType(primitive_t primitiveType) const;
      void processMessage(const Message& msg);
      textureHandle_t loadTexture(const textureData_t* texture, int_t w, int_t h);
      void constructVBO(IModel* model);
      void queueMsg(Message msg);
#ifdef DEBUG
      void computeFrameRate();
#endif

      std::map<mode_t, ShaderProgram*> m_shaderProgs;
      ShaderProgram* m_activeShaderProg;
      mode_t m_mode;

      std::atomic<bool> m_init;

      renderState_t m_state[3];
      int m_idxLatest;
      int m_idxRender;
      int m_idxUpdate;
      mutable std::mutex m_stateChangeMutex;

      pCamera_t m_camera;

      std::atomic<bool> m_running;
      std::thread* m_thread;

      std::vector<Message> m_msgQueue;
      mutable std::mutex m_msgQueueMutex;

      std::atomic<bool> m_msgQueueEmpty;

      exceptionWrapper_t m_exception;
      std::atomic<bool> m_errorPending;

#ifdef DEBUG
      std::atomic<long> m_frameRate;
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
// Renderer::attachCamera
//===========================================
inline void Renderer::attachCamera(pCamera_t camera) {
   m_camera = camera;
}

//===========================================
// Renderer::getCamera
//===========================================
inline Camera& Renderer::getCamera() const {
   return *m_camera;
}


}


#endif /*!__RENDERER_HPP__*/
