/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <cstdlib>
#include <cassert>
#include <renderer/ogles2.0/Renderer.hpp>
#include <renderer/RendererException.hpp>
#include <renderer/GL_CHECK.hpp>
#include <Timer.hpp>


using namespace cml;
using namespace std;


namespace Dodge {


atomic<bool> Renderer::m_running(false);
map<Renderer::mode_t, GLint> Renderer::m_shaderProgIds;
Renderer::mode_t Renderer::m_mode = Renderer::UNDEFINED;
Renderer::SceneGraph Renderer::m_sceneGraph;
mutex Renderer::m_sceneGraphMutex;
boost::shared_ptr<Camera> Renderer::m_camera = boost::shared_ptr<Camera>(new Camera(1.f, 1.f));
mutex Renderer::m_cameraMutex;
GLint Renderer::m_locPosition = -1;
GLint Renderer::m_locColour = -1;
GLint Renderer::m_locBUniColour = -1;
GLint Renderer::m_locUniColour = -1;
GLint Renderer::m_locTexCoord = -1;
GLint Renderer::m_locMV = -1;
GLint Renderer::m_locP = -1;
thread* Renderer::m_thread = NULL;
std::vector<Renderer::Message> Renderer::m_msgQueue;
mutex Renderer::m_msgQueueMutex;
atomic<bool> Renderer::m_msgQueueEmpty(true);
Colour Renderer::m_bgColour;
mutex Renderer::m_bgColourMutex;
Renderer::exceptionWrapper_t Renderer::m_exception = { UNKNOWN_EXCEPTION, NULL };
atomic<bool> Renderer::m_errorPending(false);
#ifdef DEBUG
atomic<long> Renderer::m_frameRate(0);
#endif


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
      m_thread = new thread(Renderer::renderLoop);
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

   switch (mode) {
      case TEXTURED_ALPHA: {
         map<mode_t, GLint>::iterator it = m_shaderProgIds.find(TEXTURED_ALPHA);
         assert(it != m_shaderProgIds.end());

         GL_CHECK(glUseProgram(it->second));

         m_locPosition = GL_CHECK(glGetAttribLocation(it->second, "av4position"));
         m_locColour = GL_CHECK(glGetAttribLocation(it->second, "av4colour"));
         m_locTexCoord = GL_CHECK(glGetAttribLocation(it->second, "av2texcoord"));

         m_locBUniColour = GL_CHECK(glGetUniformLocation(it->second, "bUniColour"));
         m_locUniColour = GL_CHECK(glGetUniformLocation(it->second, "uniColour"));
         m_locMV = GL_CHECK(glGetUniformLocation(it->second, "mv"));
         m_locP = GL_CHECK(glGetUniformLocation(it->second, "p"));

         GL_CHECK(glEnableVertexAttribArray(m_locPosition));
         GL_CHECK(glEnableVertexAttribArray(m_locColour));
         GL_CHECK(glEnableVertexAttribArray(m_locTexCoord));

         GL_CHECK(glEnable(GL_BLEND));
         GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
      }
      break;
      case NONTEXTURED_ALPHA: {
         map<mode_t, GLint>::iterator it = m_shaderProgIds.find(NONTEXTURED_ALPHA);
         assert(it != m_shaderProgIds.end());

         GL_CHECK(glUseProgram(it->second));

         m_locPosition = GL_CHECK(glGetAttribLocation(it->second, "av4position"));
         m_locColour = GL_CHECK(glGetAttribLocation(it->second, "av4colour"));

         m_locBUniColour = GL_CHECK(glGetUniformLocation(it->second, "bUniColour"));
         m_locUniColour = GL_CHECK(glGetUniformLocation(it->second, "uniColour"));
         m_locMV = GL_CHECK(glGetUniformLocation(it->second, "mv"));
         m_locP = GL_CHECK(glGetUniformLocation(it->second, "p"));

         GL_CHECK(glEnableVertexAttribArray(m_locPosition));
         GL_CHECK(glEnableVertexAttribArray(m_locColour));

         GL_CHECK(glEnable(GL_BLEND));
         GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
      }
      break;
      default:
         throw RendererException("Could not set rendering mode; Mode not supported", __FILE__, __LINE__);
   }

   m_mode = mode;
}

//===========================================
// Renderer::constructShaderProgs
//===========================================
void Renderer::constructShaderProgs() {
   constructTexturedShaderProg();
   constructNonTexturedShaderProg();
}

//===========================================
// Renderer::constructTexturedShaderProg
//===========================================
void Renderer::constructTexturedShaderProg() {
   GLint prog = GL_CHECK(glCreateProgram());

   const char* vertShader[1] = { NULL };
   vertShader[0] =
      "precision mediump float;                          \n"
      "attribute vec4 av4position;                       \n"
      "attribute vec4 av4colour;                         \n"
      "attribute vec2 av2texcoord;                       \n"
      "uniform bool bUniColour;                          \n"
      "uniform vec4 uniColour;                           \n"
      "uniform mat4 mv;                                  \n"
      "uniform mat4 p;                                   \n"
      "varying vec4 vv4colour;                           \n"
      "varying vec2 vv2texcoord;                         \n"
      "void main() {                                     \n"
      "   if (bUniColour) {                              \n"
      "      vv4colour = uniColour;                      \n"
      "   }                                              \n"
      "   else {                                         \n"
      "      vv4colour = av4colour;                      \n"
      "   }                                              \n"
      "   gl_Position = p * mv * av4position;            \n"
      "   vv2texcoord = av2texcoord;                     \n"
      "}                                                 \n";

   newShaderFromSource(vertShader, GL_VERTEX_SHADER, prog);

   const char* fragShader[1] = { NULL };
   fragShader[0] =
      "precision mediump float;                          \n"
      "varying vec4 vv4colour;                           \n"
      "varying vec2 vv2texcoord;                         \n"
      "uniform sampler2D stexture;                       \n"
      "void main() {                                     \n"
      "   vec4 tex = texture2D(stexture, vv2texcoord);   \n"
      "   gl_FragColor = vv4colour * tex;                \n"
      "}                                                 \n";

   newShaderFromSource(fragShader, GL_FRAGMENT_SHADER, prog);

   GL_CHECK(glLinkProgram(prog));
   m_shaderProgIds[TEXTURED_ALPHA] = prog;
}

//===========================================
// Renderer::constructNonTexturedShaderProg
//===========================================
void Renderer::constructNonTexturedShaderProg() {
   GLint prog = GL_CHECK(glCreateProgram());

   const char* vertShader[1] = { NULL };
   vertShader[0] =
      "precision mediump float;                          \n"
      "attribute vec4 av4position;                       \n"
      "attribute vec4 av4colour;                         \n"
      "uniform bool bUniColour;                          \n"
      "uniform vec4 uniColour;                           \n"
      "uniform mat4 mv;                                  \n"
      "uniform mat4 p;                                   \n"
      "varying vec4 vv4colour;                           \n"
      "void main() {                                     \n"
      "   if (bUniColour) {                              \n"
      "      vv4colour = uniColour;                      \n"
      "   }                                              \n"
      "   else {                                         \n"
      "      vv4colour = av4colour;                      \n"
      "   }                                              \n"
      "   gl_Position = p * mv * av4position;            \n"
      "}                                                 \n";

   newShaderFromSource(vertShader, GL_VERTEX_SHADER, prog);

   const char* fragShader[1] = { NULL };
   fragShader[0] =
      "precision mediump float;                          \n"
      "varying vec4 vv4colour;                           \n"
      "void main() {                                     \n"
      "   gl_FragColor = vv4colour;                      \n"
      "}                                                 \n";

   newShaderFromSource(fragShader, GL_FRAGMENT_SHADER, prog);

   GL_CHECK(glLinkProgram(prog));
   m_shaderProgIds[NONTEXTURED_ALPHA] = prog;
}

//===========================================
// Renderer::newShaderFromSource
//===========================================
void Renderer::newShaderFromSource(const char** shaderSrc, GLint type, GLint prog) {
   GLint shader;
   GLint success = GL_FALSE;

   // Create shader and load into GL
   shader = GL_CHECK(glCreateShader(type));

   GL_CHECK(glShaderSource(shader, 1, shaderSrc, NULL));

   // Compile the shader
   GL_CHECK(glCompileShader(shader));
   GL_CHECK(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));

   if(success != GL_TRUE)
      throw RendererException("Could not process shader; Error in source", __FILE__, __LINE__);

   GL_CHECK(glAttachShader(prog, shader));
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
void Renderer::stageModel(pModel_t model) {
   m_sceneGraphMutex.lock();
   m_sceneGraph.insert(model);
   m_sceneGraphMutex.unlock();
}

//===========================================
// Renderer::unstageModel
//===========================================
void Renderer::unstageModel(pModel_t model) {
   m_sceneGraphMutex.lock();
   m_sceneGraph.remove(model);
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
void Renderer::constructVBO(pModel_t model) {
   model->lock();

   GL_CHECK(glGenBuffers(1, &model->handle));
   GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, model->handle));

   size_t vertSize = 0;
   if (model->colData) {
      switch (model->renderMode) {
         case TEXTURED_ALPHA:       vertSize = sizeof(vvvttcccc_t);  break;
         case NONTEXTURED_ALPHA:    vertSize = sizeof(vvvcccc_t);    break;
         default:
            throw RendererException("Error constructing VBO; Render mode not supported", __FILE__, __LINE__);
      }
   }
   else {
      switch (model->renderMode) {
         case TEXTURED_ALPHA:       vertSize = sizeof(vvvtt_t);  break;
         case NONTEXTURED_ALPHA:    vertSize = sizeof(vvv_t);    break;
         default:
            throw RendererException("Error constructing VBO; Render mode not supported", __FILE__, __LINE__);
      }
   }

   GL_CHECK(glBufferData(GL_ARRAY_BUFFER, model->n * vertSize, model->verts, GL_STATIC_DRAW));

   model->unlock();
}

//===========================================
// Renderer::freeBufferedModel
//===========================================
void Renderer::freeBufferedModel(pModel_t model) {
   model->lock();

   GL_CHECK(glDeleteBuffers(1, &model->handle));
   model->handle = 0;

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
void Renderer::bufferModel(pModel_t model) {
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
// Renderer::processMsg
//===========================================
void Renderer::processMsg(const Message& msg) {
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
// Renderer::renderLoop
//===========================================
void Renderer::renderLoop() {
   Renderer renderer;

   try {
      WinIO win;
      win.createGLContext();

      renderer.init();

      while (m_running) {
         renderer.clear();

         // Check for pending messages
         m_msgQueueMutex.lock();
         for (auto i = m_msgQueue.begin(); i != m_msgQueue.end(); ++i) {
            renderer.processMsg(*i);
         }
         m_msgQueue.clear();
         m_msgQueueEmpty = true;
         m_msgQueueMutex.unlock();


         m_sceneGraphMutex.lock();
         for (auto i = m_sceneGraph.begin(); i != m_sceneGraph.end(); ++i) {
            pModel_t model = *i;
            model->lock();

            if (model->verts == NULL) {
               model->unlock();
               continue;
            }

            renderer.setMode(model->renderMode);

            GL_CHECK(glUniformMatrix4fv(m_locMV, 1, GL_FALSE, model->matrix));

            cml::matrix44f_c P;

            m_cameraMutex.lock();
            m_camera->getMatrix(P);
            m_cameraMutex.unlock();

            GL_CHECK(glUniformMatrix4fv(m_locP, 1, GL_FALSE, P.data()));


            // If model contains per-vertex colour data
            if (model->colData) {
               GL_CHECK(glUniform1i(m_locBUniColour, 0));
               GL_CHECK(glEnableVertexAttribArray(m_locColour));
            }
            else {
               GL_CHECK(glDisableVertexAttribArray(m_locColour));
               GL_CHECK(glUniform1i(m_locBUniColour, 1));
               GL_CHECK(glUniform4f(m_locUniColour, model->colour.r, model->colour.g, model->colour.b, model->colour.a));
            }


            if (model->primitiveType == LINES) {
               if (model->lineWidth != 0)
                  GL_CHECK(glLineWidth(model->lineWidth));
            }


            switch (model->renderMode) {
               case TEXTURED_ALPHA: {
                  GL_CHECK(glBindTexture(GL_TEXTURE_2D, model->texHandle));

                  vvvttcccc_t* verts = reinterpret_cast<vvvttcccc_t*>(model->verts);
                  int_t stride = model->colData ? sizeof(vvvttcccc_t) : sizeof(vvvtt_t);

                  if (model->handle == 0) {
                     GL_CHECK(glVertexAttribPointer(m_locPosition, 3, GL_FLOAT, GL_FALSE, stride, verts));
                     GL_CHECK(glVertexAttribPointer(m_locTexCoord, 2, GL_FLOAT, GL_FALSE, stride, &verts[0].t1));

                     if (model->colData)
                        GL_CHECK(glVertexAttribPointer(m_locColour, 4, GL_FLOAT, GL_FALSE, stride, &verts[0].c1));
                  }
                  else {
                     GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, model->handle));

                     GLuint offset = 0;

                     GL_CHECK(glVertexAttribPointer(m_locPosition, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<const void*>(offset)));
                     offset += 3 * sizeof(vertexElement_t);

                     GL_CHECK(glVertexAttribPointer(m_locTexCoord, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<const void*>(offset)));
                     offset += 2 * sizeof(texCoordElement_t);

                     if (model->colData)
                        GL_CHECK(glVertexAttribPointer(m_locColour, 4, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<const void*>(offset)));
                  }
               }
               break;
               case NONTEXTURED_ALPHA: {
                  vvvcccc_t* verts = reinterpret_cast<vvvcccc_t*>(model->verts);
                  int_t stride = model->colData ? sizeof(vvvcccc_t) : sizeof(vvv_t);

                  if (model->handle == 0) {
                     GL_CHECK(glVertexAttribPointer(m_locPosition, 3, GL_FLOAT, GL_FALSE, stride, verts));

                     if (model->colData)
                        GL_CHECK(glVertexAttribPointer(m_locColour, 4, GL_FLOAT, GL_FALSE, stride, &verts[0].c1));
                  }
                  else {
                     GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, model->handle));

                     GLuint offset = 0;

                     GL_CHECK(glVertexAttribPointer(m_locPosition, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<const void*>(offset)));
                     offset += 3 * sizeof(vertexElement_t);

                     if (model->colData)
                        GL_CHECK(glVertexAttribPointer(m_locColour, 4, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<const void*>(offset)));
                  }
               }
               break;
               default:
                  throw RendererException("Error rendering geometry; Render mode not supported", __FILE__, __LINE__);
            }

            GL_CHECK(glDrawArrays(renderer.primitiveToGLType(model->primitiveType), 0, model->n));

            model->unlock();
         }
         m_sceneGraphMutex.unlock();

         win.swapBuffers();

#ifdef DEBUG
         renderer.computeFrameRate();
#endif
      }
   }
   catch (RendererException& e) {
      e.prepend("Exception caught in render loop; ");
      renderer.m_exception = e.constructWrapper();
      renderer.m_errorPending = true;

      // Await imminent death
      while (m_running) {}
   }
   catch (...) {
      renderer.m_exception = { UNKNOWN_EXCEPTION, NULL };
      renderer.m_errorPending = true;
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
