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
#include <KvpParser.hpp>


using namespace cml;
using namespace std;


namespace Dodge {


Renderer* Renderer::m_instance = NULL;


//===========================================
// Renderer::Renderer
//===========================================
Renderer::Renderer()
   : m_swapBuffers(dummySwapFunc),
     m_makeGLContext(dummyMakeGLContextFunc),
     m_activeRenderMode(NULL),
     m_mode(UNDEFINED),
     m_init(false),
     m_frameNumber(0),
     m_camera(new Camera(1.f, 1.f)),
     m_running(false),
     m_thread(NULL),
     m_scratchSpace(1024),
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

   lock_guard<mutex> lock(m_stateChangeMutex);

   m_state[0].status = renderState_t::IS_BEING_RENDERED;
   m_state[1].status = renderState_t::IS_IDLE;
   m_state[2].status = renderState_t::IS_BEING_UPDATED;

   m_idxRender = 0;
   m_idxLatest = 0;
   m_idxUpdate = 2;
}

//===========================================
// Renderer::loadSettingsFromFile
//===========================================
void Renderer::loadSettingsFromFile(const string& file) {
   try {
      assert(!m_running);

      KvpParser parser;

      parser.parseFile(file);
      string impl = parser.getMetaData(0);

      if (impl != "OpenGL")
         throw RendererException("Error loading renderer settings; File not for OpenGL implementation.", __FILE__, __LINE__);

      string fixed = parser.getValue("fixed_pipeline");
      if (fixed == "true") {
         m_usrReqSettings.fixedPipeline = true;
      }
      else if (fixed == "false") {
         m_usrReqSettings.fixedPipeline = false;
      }
      else if (fixed == "") {}
      else
         throw RendererException("Error loading renderer settings; Invalid value '"
            + fixed + "' received for 'fixed_pipeline' option.", __FILE__, __LINE__);

      string vbos = parser.getValue("VBOs");
      if (vbos == "true") {
         m_usrReqSettings.VBOs = true;
      }
      else if (vbos == "false") {
         m_usrReqSettings.VBOs = false;
      }
      else if (vbos == "") {}
      else
         throw RendererException("Error loading renderer settings; Invalid value '"
            + vbos + "' received for 'VBOs' option.", __FILE__, __LINE__);
   }
   catch (Exception& e) {
      RendererException ex("Error loading renderer settings; Bad file; ", __FILE__, __LINE__);
      ex.append(e.what());
      throw ex;
   }
   catch (...) {
      throw RendererException("Error loading renderer settings; Bad file", __FILE__, __LINE__);
   }
}

//===========================================
// Renderer::start
//===========================================
void Renderer::start(Functor<void, TYPELIST_0()> makeGLContextFunc, Functor<void, TYPELIST_0()> swapFunc) {
   if (!m_thread) {
      m_makeGLContext = makeGLContextFunc;
      m_swapBuffers = swapFunc;

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
// This is called by the main thread every frame after all
// draw calls to notify the renderer that the next
// frame is complete and ready for rendering.
//===========================================
void Renderer::tick(const Colour& bgColour) {
   checkForErrors();

   lock_guard<mutex> lock(m_stateChangeMutex);

   // Any states that were pending render are now out of date, and will
   // not be rendered.
   for (int i = 0; i < 3; ++i) {
      if (m_state[i].status == renderState_t::IS_PENDING_RENDER)
         m_state[i].status = renderState_t::IS_IDLE;
   }

   // The state that has just finished being updated is now the latest,
   // and is pending render.
   m_idxLatest = m_idxUpdate;
   m_state[m_idxLatest].status = renderState_t::IS_PENDING_RENDER;

   m_camera->getMatrix(m_state[m_idxLatest].P);

   // Choose a new state for updating.
   m_idxUpdate = -1;
   for (int i = 0; i < 3; ++i)
      if (m_state[i].status == renderState_t::IS_IDLE) m_idxUpdate = i;

   assert(m_idxUpdate != -1);

   m_state[m_idxUpdate].sceneGraph->clear();
   m_state[m_idxUpdate].bgColour = bgColour;
   m_state[m_idxUpdate].status = renderState_t::IS_BEING_UPDATED;
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
// Renderer::draw
//===========================================
void Renderer::draw(const IModel* model) {
   m_state[m_idxUpdate].sceneGraph->insert(model);
}

//===========================================
// Renderer::bufferModel
//===========================================
void Renderer::bufferModel(IModel* model) {
   if (!m_oglSupport.VBOs.available) return;

   lock_guard<mutex> lock(m_msgQueueMutex);

   size_t totalSize = model->getTotalSize();

   IModel* ptr = reinterpret_cast<IModel*>(m_scratchSpace.alloc(totalSize));
   model->copyTo(ptr);

   msgConstructVbo_t data = { ptr };
   queueMsg(Message(MSG_CONSTRUCT_VBO, data));
}

//===========================================
// Renderer::freeBufferedModel
//===========================================
void Renderer::freeBufferedModel(IModel* model) {
   if (!m_oglSupport.VBOs.available) return;

   lock_guard<mutex> lock1(m_msgQueueMutex);
   {
      lock_guard<mutex> lock2(m_vboMapMutex);

      auto i = m_vboMap.find(model->m_id);
      if (i != m_vboMap.end() && i->second != 0) {
         msgDestroyVbo_t data = { i->second };
         queueMsg(Message(MSG_DESTROY_VBO, data));

         i->second = 0;
      }
   }
}

//===========================================
// Renderer::queueMsg
//===========================================
void Renderer::queueMsg(Message msg) {
   m_msgQueueEmpty = false;
   m_msgQueue.push_back(msg);
}

//===========================================
// Renderer::loadTexture
//===========================================
void Renderer::loadTexture(const textureData_t* texture, int_t width, int_t height, textureHandle_t* handle) {
   msgTexHandleReq_t data = { texture, width, height, handle };

   {
      lock_guard<mutex> lock(m_msgQueueMutex);
      queueMsg(Message(MSG_TEX_HANDLE_REQ, data));
   }

   // Hang until message is processed
   while (!m_msgQueueEmpty) { if (m_errorPending) break; }
}

//===========================================
// Renderer::unloadTexture
//===========================================
void Renderer::unloadTexture(textureHandle_t handle) {
   msgTexUnloadReq_t data = { handle };

   lock_guard<mutex> lock(m_msgQueueMutex);
   queueMsg(Message(MSG_TEX_UNLOAD_REQ, data));
}

//===========================================
// Renderer::onWindowResize
//===========================================
void Renderer::onWindowResize(int_t x, int_t y) {
   msgVpResizeReq_t data = { x, y };

   lock_guard<mutex> lock(m_msgQueueMutex);
   queueMsg(Message(MSG_VP_RESIZE_REQ, data));
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
// Renderer::init
//===========================================
void Renderer::init() {
   m_makeGLContext();

   glGetError();

#if defined GLES_1_1
   m_oglSupport.shaders = oglFeature_t(false);
   m_oglSupport.VBOs = oglFeature_t(true, CORE);
#elif defined GLES_2_0
   m_oglSupport.shaders = oglFeature_t(true, CORE);
   m_oglSupport.VBOs = oglFeature_t(true, CORE);
#elif defined GLEW
   if (!GLEW_VERSION_1_3)
      throw RendererException("Program requires OpenGL 1.3 or later", __FILE__, __LINE__);

   if (m_usrReqSettings.VBOs && GLEW_VERSION_1_5) {
      m_oglSupport.VBOs = oglFeature_t(true, CORE);
   }
   else {
      if (m_usrReqSettings.VBOs && GLEW_ARB_vertex_buffer_object)
         m_oglSupport.VBOs = oglFeature_t(true, ARB);
      else
         m_oglSupport.VBOs = oglFeature_t(false);
   }

   if (!m_usrReqSettings.fixedPipeline && GLEW_VERSION_2_0) {
      m_oglSupport.shaders = oglFeature_t(true, CORE);
   }
   else {
      if (!m_usrReqSettings.fixedPipeline
         && GLEW_ARB_fragment_program
         && GLEW_ARB_fragment_shader
         && GLEW_ARB_shader_objects
         && GLEW_ARB_vertex_program
         && GLEW_ARB_vertex_shader
         && GLEW_ARB_shading_language_100) {

         m_oglSupport.shaders = oglFeature_t(true, ARB);
      }
      else
         m_oglSupport.shaders = oglFeature_t(false);
   }
#endif

#ifdef GL_FIXED_PIPELINE
   m_oglSupport.shaders = oglFeature_t(false);
#endif

   GL_CHECK(glEnable(GL_CULL_FACE));
   GL_CHECK(glFrontFace(GL_CCW));

   m_gl.initialise(m_oglSupport);

   if (!m_oglSupport.shaders.available) {
      m_activeRenderMode = RenderMode::create(FIXED_FUNCTION);
      m_activeRenderMode->setActive();
   }
   else {
      constructRenderModes();
      setMode(TEXTURED_ALPHA);
   }
}

//===========================================
// Renderer::setMode
//===========================================
void Renderer::setMode(mode_t mode) {
   if (!m_oglSupport.shaders.available) return;
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
   if (!m_oglSupport.shaders.available) return;

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

   GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
   GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

   return texId;
}

//===========================================
// Renderer::constructVbo
//===========================================
void Renderer::constructVbo(IModel* model) {
   lock_guard<mutex> lock(m_vboMapMutex);

   auto i = m_vboMap.find(model->m_id);

   if (i != m_vboMap.end() && i->second != 0)
      destroyVbo(i->second);

   GLuint handle;

   GL_CHECK(m_gl.genBuffers(1, &handle));
   GL_CHECK(m_gl.bindBuffer(GL_ARRAY_BUFFER, handle));
   GL_CHECK(m_gl.bufferData(GL_ARRAY_BUFFER, model->vertexDataSize(), model->getVertexData(), GL_STATIC_DRAW));

   m_vboMap[model->m_id] = handle;
}

//===========================================
// Renderer::destroyVbo
//===========================================
void Renderer::destroyVbo(GLuint handle) {
   if (handle != 0)
      GL_CHECK(m_gl.deleteBuffers(1, &handle));
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
            constructVbo(dat.model);
         }
         break;
         case MSG_DESTROY_VBO: {
            msgDestroyVbo_t dat = boost::get<msgDestroyVbo_t>(msg.data);
            destroyVbo(dat.handle);
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
      m_frameRate = static_cast<long>(10.0 / timer.getTime());
      timer.reset();
   }
}
#endif

//===========================================
// Renderer::processMessages
//===========================================
void Renderer::processMessages() {
   lock_guard<mutex> lock(m_msgQueueMutex);

   for (auto i = m_msgQueue.begin(); i != m_msgQueue.end(); ++i) {
      processMessage(*i);
   }

   m_msgQueue.clear();
   m_scratchSpace.clear();
   m_msgQueueEmpty = true;
}

//===========================================
// Renderer::renderLoop
//===========================================
void Renderer::renderLoop() {
   try {
      init();

      while (m_running) {
         processMessages();

         clear();

         for (auto i = m_state[m_idxRender].sceneGraph->begin(); i != m_state[m_idxRender].sceneGraph->end(); ++i) {
            const IModel* model = *i;

            if (model->getNumVertices() == 0) continue;

            if (m_oglSupport.shaders.available) {
               mode_t mode = model->getRenderMode();
               setMode(mode);
            }

            GLuint vbo = 0;
            if (m_oglSupport.VBOs.available) {
               lock_guard<mutex> lock(m_vboMapMutex);

               auto j = m_vboMap.find(model->m_id);
               if (j != m_vboMap.end()) vbo = j->second;
            }

            m_activeRenderMode->sendData(model, m_state[m_idxRender].P, vbo);
         }

         m_swapBuffers();

         {
            lock_guard<mutex> lock(m_stateChangeMutex);

            m_state[m_idxRender].status = renderState_t::IS_IDLE;
            m_idxRender = m_idxLatest;
            m_state[m_idxRender].status = renderState_t::IS_BEING_RENDERED;
         }

         ++m_frameNumber;
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
