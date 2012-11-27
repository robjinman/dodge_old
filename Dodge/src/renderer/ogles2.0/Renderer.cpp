/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <fstream>
#include <cstdlib>
#include <cassert>
#include <Exception.hpp>
#include <renderer/ogles2.0/Renderer.hpp>
#include <globals.hpp>


using namespace cml;
using namespace std;


namespace Dodge {


map<Renderer::mode_t, GLint> Renderer::m_shaderProgIds = map<Renderer::mode_t, GLint>();
Renderer::mode_t Renderer::m_mode = Renderer::UNDEFINED;
bool Renderer::m_init = false;
Renderer::SceneGraph Renderer::m_sceneGraph = Renderer::SceneGraph();
boost::shared_ptr<Camera> Renderer::m_camera = boost::shared_ptr<Camera>(new Camera(1.f, 1.f));
boost::shared_ptr<RenderBrush> Renderer::m_brush = boost::shared_ptr<RenderBrush>(new RenderBrush);
GLint Renderer::m_locPosition = -1;
GLint Renderer::m_locColour = -1;
GLint Renderer::m_locBUniColour = -1;
GLint Renderer::m_locUniColour = -1;
GLint Renderer::m_locTexCoord = -1;
GLint Renderer::m_locMV = -1;
GLint Renderer::m_locP = -1;


//===========================================
// Renderer::init
//===========================================
void Renderer::init(const char* optsFile) {
   if (m_init) return;
   m_init = true;

   try {
      GL_CHECK(glFrontFace(GL_CCW));
      GL_CHECK(glEnable(GL_CULL_FACE));
      GL_CHECK(glEnable(GL_DEPTH_TEST));
      GL_CHECK(glEnable(GL_STENCIL_TEST));

      constructShaderProgs();
      setMode(TEXTURED_ALPHA);
   }
   catch (...) {
      m_init = false;
      throw;
   }
}

//===========================================
// Renderer::onWindowResize
//===========================================
void Renderer::onWindowResize(int_t w, int_t h) {
   GL_CHECK(glViewport(0, 0, w, h));
}

//===========================================
// Renderer::setMode
//===========================================
void Renderer::setMode(mode_t mode) {
   if (!m_init)
      throw Exception("Error setting render mode; Renderer not initialised", __FILE__, __LINE__);

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
         throw Exception("Could not set rendering mode; Mode not supported", __FILE__, __LINE__);
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
      throw Exception("Could not process shader; Error in source", __FILE__, __LINE__);

   GL_CHECK(glAttachShader(prog, shader));
}

//===========================================
// Renderer::newTexture
//===========================================
Renderer::textureHandle_t Renderer::newTexture(const textureData_t* texture, int_t width, int_t height) {
   if (!m_init)
      throw Exception("Error creating texture; Renderer not initialised", __FILE__, __LINE__);

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
   if (!m_init)
      throw Exception("Error staging model for rendering; Renderer not initialised", __FILE__, __LINE__);

   model->lineWidth = m_brush->getLineWidth();
   if (!model->colData) {
      if (model->primitiveType == LINES)
         model->colour = m_brush->getLineColour();
      else
         model->colour = m_brush->getFillColour();
   }

   m_sceneGraph.insert(model);
}

//===========================================
// Renderer::bufferModel
//===========================================
void Renderer::bufferModel(pModel_t model) {
   if (!m_init)
      throw Exception("Error constructing VBO; Renderer not initialised", __FILE__, __LINE__);

   GL_CHECK(glGenBuffers(1, &model->handle));
   GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, model->handle));

   size_t vertSize = 0;
   if (model->colData) {
      switch (model->renderMode) {
         case TEXTURED_ALPHA:       vertSize = sizeof(vvvttcccc_t);  break;
         case NONTEXTURED_ALPHA:    vertSize = sizeof(vvvcccc_t);    break;
         default:
            throw Exception("Error constructing VBO; Render mode not supported", __FILE__, __LINE__);
      }
   }
   else {
      switch (model->renderMode) {
         case TEXTURED_ALPHA:       vertSize = sizeof(vvvtt_t);  break;
         case NONTEXTURED_ALPHA:    vertSize = sizeof(vvv_t);    break;
         default:
            throw Exception("Error constructing VBO; Render mode not supported", __FILE__, __LINE__);
      }
   }

   GL_CHECK(glBufferData(GL_ARRAY_BUFFER, model->n * vertSize, model->verts, GL_STATIC_DRAW));
}

//===========================================
// Renderer::freeBufferedModel
//===========================================
void Renderer::freeBufferedModel(pModel_t model) {
   GL_CHECK(glDeleteBuffers(1, &model->handle));
   model->handle = 0;
}

//===========================================
// Renderer::render
//===========================================
void Renderer::render() {
   if (!m_init)
      throw Exception("Error rendering geometry; Renderer not initialised", __FILE__, __LINE__);

   for (SceneGraph::iterator i = m_sceneGraph.begin(); i != m_sceneGraph.end(); ++i) {
      pModel_t model = *i;

      if (model->verts == NULL) continue;

      setMode(model->renderMode);

      GL_CHECK(glUniformMatrix4fv(m_locMV, 1, GL_FALSE, model->matrix));
      GL_CHECK(glUniformMatrix4fv(m_locP, 1, GL_FALSE, m_camera->getMatrix().data()));


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
            throw Exception("Error rendering geometry; Render mode not supported", __FILE__, __LINE__);
      }

      GL_CHECK(glDrawArrays(primitiveToGLType(model->primitiveType), 0, model->n));
   }

   m_sceneGraph.clear();
}

//===========================================
// Renderer::clear
//===========================================
void Renderer::clear() {
   if (!m_init)
      throw Exception("Error clearing rendering surface; Renderer not initialised", __FILE__, __LINE__);

   GL_CHECK(glClearColor(m_brush->getFillColour().r, m_brush->getFillColour().g, m_brush->getFillColour().b, m_brush->getFillColour().a));
   GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}


}
