/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <fstream>
#include <cstdlib>
#include <cassert>
#include <Exception.hpp>
#include <Renderer.hpp>
#include <globals.hpp>


using namespace cml;
using namespace std;


namespace Dodge {


map<Renderer::mode_t, GLint> Renderer::m_shaderProgIds = map<Renderer::mode_t, GLint>();
Renderer::mode_t Renderer::m_mode = Renderer::UNDEFINED;
matrix44f_c Renderer::m_mv = matrix44f_c();
GLint Renderer::m_locPosition = -1;
GLint Renderer::m_locColour = -1;
GLint Renderer::m_locTexCoord = -1;
GLint Renderer::m_locMVP = -1;
bool Renderer::m_init = false;
bool Renderer::m_geometrySet = false;
bool Renderer::m_coloursSet = false;
bool Renderer::m_texCoordsSet = false;
int Renderer::m_vertCount = -1;
int Renderer::m_colCount = -1;
int Renderer::m_texCoordCount = -1;
GLint Renderer::m_primitiveType = GL_TRIANGLES;


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
// Renderer::setMode
//===========================================
void Renderer::setMode(mode_t mode) {
   if (!m_init)
      throw Exception("Error setting render mode; renderer not initialised", __FILE__, __LINE__);

   if (mode == m_mode) return;

   switch (mode) {
      case TEXTURED_ALPHA: {
         map<mode_t, GLint>::iterator it = m_shaderProgIds.find(TEXTURED_ALPHA);
         assert(it != m_shaderProgIds.end());

         m_locPosition = GL_CHECK(glGetAttribLocation(it->second, "av4position"));
         m_locColour = GL_CHECK(glGetAttribLocation(it->second, "av4colour"));
         m_locTexCoord = GL_CHECK(glGetAttribLocation(it->second, "av2texcoord"));
         m_locMVP = GL_CHECK(glGetUniformLocation(it->second, "mvp"));

         GL_CHECK(glUseProgram(it->second));

         GL_CHECK(glEnable(GL_BLEND));
         GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
      }
      break;
      case NONTEXTURED_ALPHA: {
         map<mode_t, GLint>::iterator it = m_shaderProgIds.find(NONTEXTURED_ALPHA);
         assert(it != m_shaderProgIds.end());

         m_locPosition = GL_CHECK(glGetAttribLocation(it->second, "av4position"));
         m_locColour = GL_CHECK(glGetAttribLocation(it->second, "av4colour"));
         m_locTexCoord = -1;
         m_locMVP = GL_CHECK(glGetUniformLocation(it->second, "mvp"));

         GL_CHECK(glUseProgram(it->second));

         GL_CHECK(glEnable(GL_BLEND));
         GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
      }
      break;
      default:
         throw Exception("Could not set rendering mode; mode not supported", __FILE__, __LINE__);
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
      "uniform mat4 mvp;                                 \n"
      "varying vec4 vv4colour;                           \n"
      "varying vec2 vv2texcoord;                         \n"
      "void main() {                                     \n"
	   "   vv4colour = av4colour;                         \n"
	   "   gl_Position = mvp * av4position;               \n"
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
      "uniform mat4 mvp;                                 \n"
      "varying vec4 vv4colour;                           \n"
      "void main() {                                     \n"
	   "   vv4colour = av4colour;                         \n"
	   "   gl_Position = mvp * av4position;               \n"
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

   m_locPosition = GL_CHECK(glGetAttribLocation(prog, "av4position"));
   m_locColour = GL_CHECK(glGetAttribLocation(prog, "av4colour"));
   m_locTexCoord = -1;
   m_locMVP = GL_CHECK(glGetUniformLocation(prog, "mvp"));
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
      throw Exception("Could not process shader; error in source", __FILE__, __LINE__);

   GL_CHECK(glAttachShader(prog, shader));
}

//===========================================
// Renderer::setMatrix
//===========================================
void Renderer::setMatrix(const matrixElement_t* modelView) {
   if (!m_init)
      throw Exception("Error setting render matrix; renderer not initialised", __FILE__, __LINE__);

   matrixf_c mv(modelView, 4, 4);
   mv.transpose();
   m_mv = mv;
}

//===========================================
// Renderer::newTexture
//===========================================
Renderer::textureId_t Renderer::newTexture(const textureData_t* texture, int width, int height) {
   if (!m_init)
      throw Exception("Error creating texture; renderer not initialised", __FILE__, __LINE__);

   GLuint texId;

   GL_CHECK(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
   GL_CHECK(glGenTextures(1, &texId));

   GL_CHECK(glBindTexture(GL_TEXTURE_2D, texId));

   GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture));

   GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
   GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

   return static_cast<textureId_t>(texId);
}

//===========================================
// Renderer::setActiveTexture
//===========================================
void Renderer::setActiveTexture(textureId_t texId) {
   if (!m_init)
      throw Exception("Error setting active texture; renderer not initialised", __FILE__, __LINE__);

   glBindTexture(GL_TEXTURE_2D, static_cast<textureId_t>(texId));
}

//===========================================
// Renderer::setGeometry
//===========================================
void Renderer::setGeometry(const vertexElement_t* verts, primitive_t primitiveType, int count) {
   if (!m_init)
      throw Exception("Error setting render geometry; renderer not initialised", __FILE__, __LINE__);

   if (isSupportedPrimitive(primitiveType))
      m_primitiveType = primitiveToGLType(primitiveType);
   else
      throw Exception("Error setting renderable geometry; primitive not supported", __FILE__, __LINE__);

   GL_CHECK(glEnableVertexAttribArray(m_locPosition));
   GL_CHECK(glVertexAttribPointer(m_locPosition, 3, GL_FLOAT, GL_FALSE, 0, verts));

   m_vertCount = count;
   m_geometrySet = true;
}

//===========================================
// Renderer::setColours
//===========================================
void Renderer::setColours(const colourElement_t* colours, int count) {
   if (!m_init)
      throw Exception("Error setting render colours; renderer not initialised", __FILE__, __LINE__);

   GL_CHECK(glEnableVertexAttribArray(m_locColour));
   GL_CHECK(glVertexAttribPointer(m_locColour, 4, GL_FLOAT, GL_FALSE, 0, colours));

   m_colCount = count;
   m_coloursSet = true;
}

//===========================================
// Renderer::setTextureCoords
//===========================================
void Renderer::setTextureCoords(const texCoordElement_t* texCoords, int count) {
   if (!m_init)
      throw Exception("Error setting texture coords; renderer not initialised", __FILE__, __LINE__);

   GL_CHECK(glEnableVertexAttribArray(m_locTexCoord));
   GL_CHECK(glVertexAttribPointer(m_locTexCoord, 2, GL_FLOAT, GL_FALSE, 0, texCoords));

   m_texCoordCount = count;
   m_texCoordsSet = true;
}

//===========================================
// Renderer::render
//===========================================
void Renderer::render() {
   if (!m_init)
      throw Exception("Error rendering geometry; renderer not initialised", __FILE__, __LINE__);

   if (!m_geometrySet)
      throw Exception("Error rendering geometry; geometry not set", __FILE__, __LINE__);

   if (m_coloursSet) {
      if (m_colCount != m_vertCount)
         throw Exception("Error rendering geometry; mismatch in array sizes", __FILE__, __LINE__);
   }
   else {
      // Use colours from m_brush

      StackAllocator::marker_t marker = gMemStack.getMarker();
      GLfloat* colours = static_cast<GLfloat*>(gMemStack.alloc(4 * m_vertCount * sizeof(GLfloat)));

      GLfloat* col;
      if (m_primitiveType == LINES)
         col = m_brush->lineColour;
      else
         col = m_brush->fillColour;

      for (int i = 0; i < m_vertCount; ++i) {
         colours[i * 4 + 0] = col[0];
         colours[i * 4 + 1] = col[1];
         colours[i * 4 + 2] = col[2];
         colours[i * 4 + 3] = col[3];
      }

      setColours(colours, m_vertCount); // TODO: I assume the colour array is copied ??

      gMemStack.freeToMarker(marker);
   }

   if (m_texCoordsSet && (m_texCoordCount != m_vertCount))
      throw Exception("Error rendering geometry; mismatch in array sizes", __FILE__, __LINE__);

   if (m_primitiveType == LINES && m_brush->lineWidth == 0) return;

   // Compute MVP matrix
   matrixf_c P(m_projMat, 4, 4);
   P.transpose();
   matrix44f_c mvp = P * m_mv;

   GL_CHECK(glUniformMatrix4fv(m_locMVP, 1, GL_FALSE, mvp.data()));

   if (m_brush->lineWidth != 0)
      GL_CHECK(glLineWidth(m_brush->lineWidth));

   GL_CHECK(glDrawArrays(m_primitiveType, 0, m_vertCount));

   // Supplying colour information is optional in all shaders
//   GL_CHECK(glDisableVertexAttribArray(m_locColour));

   m_geometrySet = false;
   m_coloursSet = false;
   m_texCoordsSet = false;
}

//===========================================
// Renderer::clear
//===========================================
void Renderer::clear() {
   if (!m_init)
      throw Exception("Error clearing rendering surface; renderer not initialised", __FILE__, __LINE__);

   GL_CHECK(glClearColor(m_brush->fillColour[0], m_brush->fillColour[1], m_brush->fillColour[2], m_brush->fillColour[3]));
   GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}


}
