/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <cstdlib>
#include <cassert>
#include <renderer/ogles2.0/Renderer.hpp>
#include <renderer/RendererException.hpp>
#include <renderer/Model.hpp>
#include <renderer/SceneGraph.hpp>
#include <renderer/ogles2.0/ShaderProgram.hpp>
#include <renderer/GL_CHECK.hpp>
#include <Timer.hpp>


using namespace cml;
using namespace std;


namespace Dodge {


Renderer* Renderer::m_instance = NULL;


Renderer::Renderer()
   : m_activeShaderProg(NULL),
     m_mode(UNDEFINED),
     m_init(false),
     m_sceneGraph(new SceneGraph),
     m_camera(new Camera(1.f, 1.f)),
     m_running(false),
     m_thread(NULL),
     m_msgQueueEmpty(true),
     m_exception({UNKNOWN_EXCEPTION, NULL}),
     m_errorPending(false)
#ifdef DEBUG
   , m_frameRate(0)
#endif
     {}

//===========================================
// Renderer::init
//===========================================
void Renderer::init() {
   GL_CHECK(glFrontFace(GL_CCW));
   GL_CHECK(glEnable(GL_CULL_FACE));
   GL_CHECK(glEnable(GL_DEPTH_TEST));
   GL_CHECK(glEnable(GL_STENCIL_TEST));

   constructShaderProgs();
   setMode(TEXTURED_ALPHA);
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
   m_running = false;
   m_thread->join();
   delete m_thread;
   m_thread = NULL;
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
      case TRIANGLES: return GL_TRIANGLES;
      case LINES: return GL_LINES;
      default:
         throw RendererException("Primitive type not supported", __FILE__, __LINE__);
   }
}

//===========================================
// Renderer::setMode
//===========================================
void Renderer::setMode(mode_t mode) {
   if (mode == m_mode) return;

   map<mode_t, ShaderProgram*>::iterator it = m_shaderProgs.find(mode);
   assert(it != m_shaderProgs.end());

   m_activeShaderProg = it->second;
   m_activeShaderProg->setActive();

   m_mode = mode;
}

//===========================================
// Renderer::constructShaderProgs
//===========================================
void Renderer::constructShaderProgs() {
   ShaderProgram* prog = ShaderProgram::create(NONTEXTURED_ALPHA);
   m_shaderProgs[NONTEXTURED_ALPHA] = prog;

   prog = ShaderProgram::create(TEXTURED_ALPHA);
   m_shaderProgs[TEXTURED_ALPHA] = prog;
}

//===========================================
// Renderer::loadTexture
//===========================================
Renderer::textureHandle_t Renderer::loadTexture(const textureData_t* texture, int_t width, int_t height) {
   GLuint texId;

   GL_CHECK(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
   GL_CHECK(glGenTextures(1, &texId));

   GL_CHECK(glBindTexture(GL_TEXTURE_2D, texId));

   GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture));

   GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
   GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

   return texId;
}

//===========================================
// Renderer::stageModel
//===========================================
void Renderer::stageModel(const IModel* model) {
   m_sceneGraphMutex.lock();
   m_sceneGraph->insert(model);
   m_sceneGraphMutex.unlock();
}

//===========================================
// Renderer::unstageModel
//===========================================
void Renderer::unstageModel(const IModel* model) {
   m_sceneGraphMutex.lock();
   m_sceneGraph->remove(model);
   m_sceneGraphMutex.unlock();
}

//===========================================
// Renderer::setBgColour
//===========================================
void Renderer::setBgColour(const Colour& col) {
   m_bgColourMutex.lock();
   m_bgColour = col;
   m_bgColourMutex.unlock();
}

//===========================================
// Renderer::constructVBO
//===========================================
void Renderer::constructVBO(IModel* model) {
   model->lock();

   modelHandle_t handle;

   GL_CHECK(glGenBuffers(1, &handle));
   GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, handle));
   GL_CHECK(glBufferData(GL_ARRAY_BUFFER, model->vertexDataSize(), model->getVertexData(), GL_STATIC_DRAW));

   model->setHandle(handle);

   model->unlock();
}

//===========================================
// Renderer::freeBufferedModel
//===========================================
void Renderer::freeBufferedModel(IModel* model) {
   model->lock();

   modelHandle_t handle = model->getHandle();

   GL_CHECK(glDeleteBuffers(1, &handle));
   model->setHandle(0);

   model->unlock();
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
// Renderer::newTexture
//===========================================
void Renderer::newTexture(const textureData_t* texture, int_t width, int_t height, textureHandle_t* handle) {
   msgTexHandleReq_t data = { texture, width, height, handle };
   queueMsg(Message(MSG_TEX_HANDLE_REQ, data));

   // Hang until message is processed
   while (!m_msgQueueEmpty) { if (m_errorPending) break; }
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
            *dat.retVal = loadTexture(dat.texData, dat.w, dat.h);
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
      win.createGLContext();

      init();

      while (m_running) {
         processMessages();

         clear();

         cml::matrix44f_c P;
         m_cameraMutex.lock();
         m_camera->getMatrix(P);
         m_cameraMutex.unlock();

         m_sceneGraphMutex.lock();
         for (auto i = m_sceneGraph->begin(); i != m_sceneGraph->end(); ++i) {
            const IModel* model = *i;

            if (model->getNumVertices() == 0) continue;

            setMode(model->getRenderMode());

            m_activeShaderProg->sendData(model, P);

            GL_CHECK(glDrawArrays(primitiveToGLType(model->getPrimitiveType()), 0, model->getNumVertices()));
         }
         m_sceneGraphMutex.unlock();

         win.swapBuffers();

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
      m_exception = { EXCEPTION, new Exception(e) };
      m_errorPending = true;

      while (m_running) {}
   }
   catch (...) {
      m_exception = { UNKNOWN_EXCEPTION, NULL };
      m_errorPending = true;

      while (m_running) {}
   }
}

//===========================================
// Renderer::clear
//===========================================
void Renderer::clear() {
   m_bgColourMutex.lock();
   GL_CHECK(glClearColor(m_bgColour.r, m_bgColour.g, m_bgColour.b, m_bgColour.a));
   m_bgColourMutex.unlock();

   GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}


}
