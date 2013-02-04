/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <cstdlib>
#include <cassert>
#include <renderer/ogl/Renderer.hpp>
#include <renderer/RendererException.hpp>
#include <renderer/Model.hpp>
#include <renderer/SceneGraph.hpp>
#include <renderer/ogl/RenderMode.hpp>
#include <renderer/GL_CHECK.hpp>
#include <Timer.hpp>


using namespace cml;
using namespace std;


namespace Dodge {


Renderer* Renderer::m_instance = NULL;


//===========================================
// Renderer::Renderer
//===========================================
Renderer::Renderer()
   : m_activeRenderMode(NULL),
     m_mode(UNDEFINED),
     m_init(false),
     m_camera(new Camera(1.f, 1.f)),
     m_running(false),
     m_thread(NULL),
     m_msgQueueEmpty(true),
     m_exception(UNKNOWN_EXCEPTION, NULL),
     m_errorPending(false)
#ifdef DEBUG
   , m_frameRate(0)
#endif
     {

   m_state[0].sceneGraph = unique_ptr<SceneGraph>(new SceneGraph);
   m_state[1].sceneGraph = unique_ptr<SceneGraph>(new SceneGraph);
   m_state[2].sceneGraph = unique_ptr<SceneGraph>(new SceneGraph);

   m_stateChangeMutex.lock();

   m_state[0].status = renderState_t::IS_BEING_RENDERED;
   m_state[1].status = renderState_t::IS_IDLE;
   m_state[2].status = renderState_t::IS_BEING_UPDATED;

   m_idxRender = 0;
   m_idxLatest = 0;
   m_idxUpdate = 2;

   m_stateChangeMutex.unlock();
}

//===========================================
// Renderer::init
//===========================================
void Renderer::init() {
   WinIO win;
   win.createGLContext();

   m_fixedPipeline = !win.isSupportedGLVersion(WinIO::GL_2_0);

   GL_CHECK(glEnable(GL_CULL_FACE));
   GL_CHECK(glFrontFace(GL_CCW));

   if (m_fixedPipeline) {
      m_activeRenderMode = RenderMode::create(FIXED_FUNCTION);
      m_activeRenderMode->setActive();
   }
   else {
      constructRenderModes();
      setMode(TEXTURED_ALPHA);
   }
}

//===========================================
// Renderer::start
//===========================================
void Renderer::start() {
   if (!m_thread) {
      m_running = true;
      m_thread = new thread(&Renderer::renderLoop, this);
   }
}

//===========================================
// Renderer::stop
//===========================================
void Renderer::stop() {
   if (m_running) {
      m_running = false;
      m_thread->join();
      delete m_thread;
      m_thread = NULL;
   }
}

//===========================================
// Renderer::tick
//
// This is called by the main thread every frame, after all
// draw calls, to notify the renderer that the next
// frame is complete and ready for rendering.
//===========================================
void Renderer::tick(const Colour& bgColour) {
   checkForErrors();

   m_stateChangeMutex.lock();

   for (int i = 0; i < 3; ++i) {
      if (m_state[i].status == renderState_t::IS_PENDING_RENDER)
         m_state[i].status = renderState_t::IS_IDLE;
   }

   m_idxLatest = m_idxUpdate;
   m_state[m_idxLatest].status = renderState_t::IS_PENDING_RENDER;
   m_camera->getMatrix(m_state[m_idxLatest].P);

   m_idxUpdate = -1;
   for (int i = 0; i < 3; ++i)
      if (m_state[i].status == renderState_t::IS_IDLE) m_idxUpdate = i;

   assert(m_idxUpdate != -1);

   m_state[m_idxUpdate].sceneGraph->clear();
   m_state[m_idxUpdate].bgColour = bgColour;
   m_state[m_idxUpdate].status = renderState_t::IS_BEING_UPDATED;

   m_stateChangeMutex.unlock();
}

//===========================================
// Renderer::checkForErrors
//===========================================
void Renderer::checkForErrors() {
   if (m_errorPending) {
      switch (m_exception.type) {
         case RENDERER_EXCEPTION: {
            RendererException* ptr = reinterpret_cast<RendererException*>(m_exception.data);
            throw *ptr;
         }
         break;
         case EXCEPTION: {
            Exception* ptr = reinterpret_cast<Exception*>(m_exception.data);
            throw *ptr;
         }
         break;
         default:
            throw RendererException("An unknown error occured", __FILE__, __LINE__);
      }
   }
}

//===========================================
// Renderer::primitiveToGLType
//===========================================
GLint Renderer::primitiveToGLType(primitive_t primitiveType) const {
   switch (primitiveType) {
      case TRIANGLES:      return GL_TRIANGLES;
      case LINES:          return GL_LINES;
      case TRIANGLE_STRIP: return GL_TRIANGLE_STRIP;
      default:
         throw RendererException("Primitive type not supported", __FILE__, __LINE__);
   }
}

//===========================================
// Renderer::setMode
//===========================================
void Renderer::setMode(mode_t mode) {
   if (m_fixedPipeline) return;
   if (mode == m_mode) return;

   map<mode_t, RenderMode*>::iterator it = m_renderModes.find(mode);
   assert(it != m_renderModes.end());

   m_activeRenderMode = it->second;
   m_activeRenderMode->setActive();

   m_mode = mode;
}

//===========================================
// Renderer::constructRenderModes
//===========================================
void Renderer::constructRenderModes() {
   if (m_fixedPipeline) return;

   RenderMode* mode = RenderMode::create(NONTEXTURED_ALPHA);
   m_renderModes[NONTEXTURED_ALPHA] = mode;

   mode = RenderMode::create(TEXTURED_ALPHA);
   m_renderModes[TEXTURED_ALPHA] = mode;
}

//===========================================
// Renderer::loadGLTexture
//===========================================
Renderer::textureHandle_t Renderer::loadGLTexture(const textureData_t* texture, int_t width, int_t height) {
   GLuint texId;

   GL_CHECK(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
   GL_CHECK(glGenTextures(1, &texId));

   GL_CHECK(glBindTexture(GL_TEXTURE_2D, texId));

   GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture));

   GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
   GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    
   GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
   GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

   return texId;
}

//===========================================
// Renderer::draw
//===========================================
void Renderer::draw(const IModel* model) {
   m_state[m_idxUpdate].sceneGraph->insert(model);
}

//===========================================
// Renderer::constructVBO
//===========================================
void Renderer::constructVBO(IModel* model) {
   modelHandle_t handle;

   if (model->m_handle == 0) {
      GL_CHECK(glGenBuffers(1, &handle));
      GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, handle));
      GL_CHECK(glBufferData(GL_ARRAY_BUFFER, model->vertexDataSize(), model->getVertexData(), GL_STATIC_DRAW));

      model->m_handle = handle;
   }
   else {
      freeBufferedModel(model);
   }
}

//===========================================
// Renderer::freeBufferedModel
//===========================================
void Renderer::freeBufferedModel(IModel* model) {
   GL_CHECK(glDeleteBuffers(1, &model->m_handle));
   model->m_handle = 0;
}

//===========================================
// Renderer::queueMsg
//===========================================
void Renderer::queueMsg(Message msg) {
   m_msgQueueMutex.lock();

   m_msgQueueEmpty = false;
   m_msgQueue.push_back(msg);

   m_msgQueueMutex.unlock();
}

//===========================================
// Renderer::loadTexture
//===========================================
void Renderer::loadTexture(const textureData_t* texture, int_t width, int_t height, textureHandle_t* handle) {
   msgTexHandleReq_t data = { texture, width, height, handle };
   queueMsg(Message(MSG_TEX_HANDLE_REQ, data));

   // Hang until message is processed
   while (!m_msgQueueEmpty) { if (m_errorPending) break; }
}

//===========================================
// Renderer::unloadTexture
//===========================================
void Renderer::unloadTexture(textureHandle_t handle) {
   msgTexUnloadReq_t data = { handle };
   queueMsg(Message(MSG_TEX_UNLOAD_REQ, data));
}

//===========================================
// Renderer::bufferModel
//===========================================
void Renderer::bufferModel(IModel* model) {
   msgConstructVbo_t data = { model };
   queueMsg(Message(MSG_CONSTRUCT_VBO, data));
}

//===========================================
// Renderer::onWindowResize
//===========================================
void Renderer::onWindowResize(int_t x, int_t y) {
   msgVpResizeReq_t data = { x, y };
   queueMsg(Message(MSG_VP_RESIZE_REQ, data));
}

//===========================================
// Renderer::processMessage
//===========================================
void Renderer::processMessage(const Message& msg) {
   try {
      switch (msg.type) {
         case MSG_TEX_HANDLE_REQ: {
            msgTexHandleReq_t dat = boost::get<msgTexHandleReq_t>(msg.data);
            *dat.retVal = loadGLTexture(dat.texData, dat.w, dat.h);
         }
         break;
         case MSG_TEX_UNLOAD_REQ: {
            msgTexUnloadReq_t dat = boost::get<msgTexUnloadReq_t>(msg.data);
            GL_CHECK(glDeleteTextures(1, &dat.handle));
         }
         break;
         case MSG_VP_RESIZE_REQ: {
            msgVpResizeReq_t dat = boost::get<msgVpResizeReq_t>(msg.data);
            GL_CHECK(glViewport(0, 0, dat.w, dat.h));
         }
         break;
         case MSG_CONSTRUCT_VBO: {
            msgConstructVbo_t dat = boost::get<msgConstructVbo_t>(msg.data);
            constructVBO(dat.model);
         }
         break;
      }
   }
   catch (boost::bad_get& e) {
      RendererException ex("Error processing request; ", __FILE__, __LINE__);
      ex.append(e.what());
      throw ex;
   }
}

#ifdef DEBUG
//===========================================
// Renderer::computeFrameRate
//===========================================
void Renderer::computeFrameRate() {
   static Timer timer;
   static long i = 0;
   i++;

   if (i % 10 == 0) {
      m_frameRate = 10.0 / timer.getTime();
      timer.reset();
   }
}
#endif

//===========================================
// Renderer::processMessages
//===========================================
void Renderer::processMessages() {
   m_msgQueueMutex.lock();

   for (auto i = m_msgQueue.begin(); i != m_msgQueue.end(); ++i) {
      processMessage(*i);
   }

   m_msgQueue.clear();
   m_msgQueueEmpty = true;

   m_msgQueueMutex.unlock();
}

//===========================================
// Renderer::renderLoop
//===========================================
void Renderer::renderLoop() {
   try {
      WinIO win;
      init();

      while (m_running) {
         processMessages();

         clear();

         for (auto i = m_state[m_idxRender].sceneGraph->begin(); i != m_state[m_idxRender].sceneGraph->end(); ++i) {
            const IModel* model = *i;

            if (model->getNumVertices() == 0) continue;

            if (!m_fixedPipeline) {
               mode_t mode = model->getRenderMode();

               for (auto j = m_renderModes.begin(); j != m_renderModes.end(); ++j) {
                  if (j->first != mode && j->second->hasPending()) {
                     setMode(j->first);
                     j->second->flush();
                  }
               }

               setMode(mode);
            }

            m_activeRenderMode->sendData(model, m_state[m_idxRender].P);
         }

         m_activeRenderMode->flush();

         win.swapBuffers();

         m_stateChangeMutex.lock();

         m_state[m_idxRender].status = renderState_t::IS_IDLE;
         m_idxRender = m_idxLatest;
         m_state[m_idxRender].status = renderState_t::IS_BEING_RENDERED;

         m_stateChangeMutex.unlock();
#ifdef DEBUG
         computeFrameRate();
#endif
      }
   }
   catch (RendererException& e) {
      e.prepend("Exception caught in render loop; ");
      m_exception = e.constructWrapper();
      m_errorPending = true;

      // Await imminent death
      while (m_running) {}
   }
   catch (Exception& e) {
      e.prepend("Exception caught in render loop; ");
      m_exception = exceptionWrapper_t(EXCEPTION, new Exception(e));
      m_errorPending = true;

      while (m_running) {}
   }
   catch (...) {
      m_exception = exceptionWrapper_t(UNKNOWN_EXCEPTION, NULL);
      m_errorPending = true;

      while (m_running) {}
   }
}

//===========================================
// Renderer::clear
//===========================================
void Renderer::clear() {
   const Colour& col = m_state[m_idxRender].bgColour;

   GL_CHECK(glClearColor(col.r, col.g, col.b, col.a));
   GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));
}


}
