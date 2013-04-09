#ifndef __OGL_WRAPPER_HPP__
#define __OGL_WRAPPER_HPP__


#include <cassert>
#ifdef GLEW
   #include <GLEW/glew.h>
#else
   #if defined GLES_1_1
      #include <GLES/gl.h>
      #define GL_FIXED_PIPELINE
   #elif defined GLES_2_0
      #include <GLES2/gl2.h>
   #endif
#endif


namespace Dodge {


enum nameScheme_t { CORE, ARB, EXT };

struct oglFeature_t {
   oglFeature_t(bool avail)
      : available(avail),
        nameScheme(CORE) {}

   oglFeature_t(bool avail, nameScheme_t naming)
      : available(avail),
        nameScheme(naming) {}

   bool available;
   nameScheme_t nameScheme;
};

struct oglSupport_t {
   oglSupport_t()
      : shaders(true, CORE),
        VBOs(true, CORE) {}

   oglFeature_t shaders;
   oglFeature_t VBOs;
};


class OglWrapper {
   public:
      void initialise(const oglSupport_t& supportedFeatures) {
         m_oglSupport = supportedFeatures;
      }

      inline const oglSupport_t& getSupportedFeatures() const;

      inline void bindBuffer(GLenum target, GLuint buf) const;
      inline void genBuffers(GLsizei n, GLuint* bufs) const;
      inline void bufferData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage) const;
      inline void deleteBuffers(GLsizei n, const GLuint* bufs) const;

#ifndef GL_FIXED_PIPELINE
      inline GLuint createProgram() const;
      inline void linkProgram(GLuint program) const;
      inline void useProgram(GLuint program) const;
      inline GLint getAttribLocation(GLuint program, const GLchar* name) const;
      inline GLint getUniformLocation(GLuint program, const GLchar* name) const;
      inline void enableVertexAttribArray(GLuint index) const;
      inline void disableVertexAttribArray(GLuint index) const;
      inline void uniform1i(GLint location, GLint v0) const;
      inline void uniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) const;
      inline void uniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) const;
      inline void vertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer) const;
      inline GLuint createShader(GLenum shaderType) const;
      inline void shaderSource(GLuint shader, GLsizei count, const GLchar** string, const GLint* length) const;
      inline void compileShader(GLuint shader) const;
      inline void getShaderiv(GLuint shader, GLenum pname, GLint* params) const;
      inline void attachShader(GLuint program, GLuint shader) const;
      inline void getShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei* length, GLchar* infoLog) const;
#endif

   private:
      static oglSupport_t m_oglSupport;
};

//===========================================
// OglWrapper::getSupportedFeatures
//===========================================
inline const oglSupport_t& OglWrapper::getSupportedFeatures() const {
   return m_oglSupport;
}

//===========================================
// OglWrapper::bindBuffer
//===========================================
inline void OglWrapper::bindBuffer(GLenum target, GLuint buf) const {
   assert(m_oglSupport.VBOs.available);

   switch (m_oglSupport.VBOs.nameScheme) {
      case CORE:  glBindBuffer(target, buf);       break;
#ifdef GLEW
      case ARB:   glBindBufferARB(target, buf);    break;
#endif
      default:
         assert(false);
   }
}

//===========================================
// OglWrapper::genBuffers
//===========================================
inline void OglWrapper::genBuffers(GLsizei n, GLuint* bufs) const {
   assert(m_oglSupport.VBOs.available);

   switch (m_oglSupport.VBOs.nameScheme) {
      case CORE:  glGenBuffers(n, bufs);       break;
#ifdef GLEW
      case ARB:   glGenBuffersARB(n, bufs);    break;
#endif
      default:
         assert(false);
   }
}

//===========================================
// OglWrapper::bufferData
//===========================================
inline void OglWrapper::bufferData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage) const {
   assert(m_oglSupport.VBOs.available);

   switch (m_oglSupport.VBOs.nameScheme) {
      case CORE:  glBufferData(target, size, data, usage);       break;
#ifdef GLEW
      case ARB:   glBufferDataARB(target, size, data, usage);    break;
#endif
      default:
         assert(false);
   }
}

//===========================================
// OglWrapper::deleteBuffers
//===========================================
inline void OglWrapper::deleteBuffers(GLsizei n, const GLuint* bufs) const {
   assert(m_oglSupport.VBOs.available);

   switch (m_oglSupport.VBOs.nameScheme) {
      case CORE:  glDeleteBuffers(n, bufs);       break;
#ifdef GLEW
      case ARB:   glDeleteBuffersARB(n, bufs);    break;
#endif
      default:
         assert(false);
   }
}

#ifndef GL_FIXED_PIPELINE
//===========================================
// OglWrapper::createProgram
//===========================================
inline GLuint OglWrapper::createProgram() const {
   assert(m_oglSupport.shaders.available);

   switch (m_oglSupport.shaders.nameScheme) {
      case CORE:  return glCreateProgram();             break;
#ifdef GLEW
      case ARB:   return glCreateProgramObjectARB();    break;
#endif
      default:
         assert(false);
   }
}

//===========================================
// OglWrapper::linkProgram
//===========================================
inline void OglWrapper::linkProgram(GLuint program) const {
   assert(m_oglSupport.shaders.available);

   switch (m_oglSupport.shaders.nameScheme) {
      case CORE:  glLinkProgram(program);       break;
#ifdef GLEW
      case ARB:   glLinkProgramARB(program);    break;
#endif
      default:
         assert(false);
   }
}

//===========================================
// OglWrapper::useProgram
//===========================================
inline void OglWrapper::useProgram(GLuint program) const {
   assert(m_oglSupport.shaders.available);

   switch (m_oglSupport.shaders.nameScheme) {
      case CORE:  glUseProgram(program);             break;
#ifdef GLEW
      case ARB:   glUseProgramObjectARB(program);    break;
#endif
      default:
         assert(false);
   }
}

//===========================================
// OglWrapper::getAttribLocation
//===========================================
inline GLint OglWrapper::getAttribLocation(GLuint program, const GLchar* name) const {
   assert(m_oglSupport.shaders.available);

   switch (m_oglSupport.shaders.nameScheme) {
      case CORE:  return glGetAttribLocation(program, name);       break;
#ifdef GLEW
      case ARB:   return glGetAttribLocationARB(program, name);    break;
#endif
      default:
         assert(false);
   }
}

//===========================================
// OglWrapper::getUniformLocation
//===========================================
inline GLint OglWrapper::getUniformLocation(GLuint program, const GLchar* name) const {
   assert(m_oglSupport.shaders.available);

   switch (m_oglSupport.shaders.nameScheme) {
      case CORE:  return glGetUniformLocation(program, name);       break;
#ifdef GLEW
      case ARB:   return glGetUniformLocationARB(program, name);    break;
#endif
      default:
         assert(false);
   }
}

//===========================================
// OglWrapper::enableVertexAttribArray
//===========================================
inline void OglWrapper::enableVertexAttribArray(GLuint index) const {
   assert(m_oglSupport.shaders.available);

   switch (m_oglSupport.shaders.nameScheme) {
      case CORE:  glEnableVertexAttribArray(index);       break;
#ifdef GLEW
      case ARB:   glEnableVertexAttribArrayARB(index);    break;
#endif
      default:
         assert(false);
   }
}

//===========================================
// OglWrapper::disableVertexAttribArray
//===========================================
inline void OglWrapper::disableVertexAttribArray(GLuint index) const {
   assert(m_oglSupport.shaders.available);

   switch (m_oglSupport.shaders.nameScheme) {
      case CORE:  glDisableVertexAttribArray(index);       break;
#ifdef GLEW
      case ARB:   glDisableVertexAttribArrayARB(index);    break;
#endif
      default:
         assert(false);
   }
}

//===========================================
// OglWrapper::uniform1i
//===========================================
inline void OglWrapper::uniform1i(GLint location, GLint v0) const {
   assert(m_oglSupport.shaders.available);

   switch (m_oglSupport.shaders.nameScheme) {
      case CORE:  glUniform1i(location, v0);       break;
#ifdef GLEW
      case ARB:   glUniform1iARB(location, v0);    break;
#endif
      default:
         assert(false);
   }
}

//===========================================
// OglWrapper::uniform1i
//===========================================
inline void OglWrapper::uniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) const {
   assert(m_oglSupport.shaders.available);

   switch (m_oglSupport.shaders.nameScheme) {
      case CORE:  glUniform4f(location, v0, v1, v2, v3);       break;
#ifdef GLEW
      case ARB:   glUniform4fARB(location, v0, v1, v2, v3);    break;
#endif
      default:
         assert(false);
   }
}

//===========================================
// OglWrapper::uniformMatrix4fv
//===========================================
inline void OglWrapper::uniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) const {
   assert(m_oglSupport.shaders.available);

   switch (m_oglSupport.shaders.nameScheme) {
      case CORE:  glUniformMatrix4fv(location, count, transpose, value);       break;
#ifdef GLEW
      case ARB:   glUniformMatrix4fvARB(location, count, transpose, value);    break;
#endif
      default:
         assert(false);
   }
}

//===========================================
// OglWrapper::vertexAttribPointer
//===========================================
inline void OglWrapper::vertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer) const {
   assert(m_oglSupport.shaders.available);

   switch (m_oglSupport.shaders.nameScheme) {
      case CORE:  glVertexAttribPointer(index, size, type, normalized, stride, pointer);       break;
#ifdef GLEW
      case ARB:   glVertexAttribPointerARB(index, size, type, normalized, stride, pointer);    break;
#endif
      default:
         assert(false);
   }
}

//===========================================
// OglWrapper::createShader
//===========================================
inline GLuint OglWrapper::createShader(GLenum shaderType) const {
   assert(m_oglSupport.shaders.available);

   switch (m_oglSupport.shaders.nameScheme) {
      case CORE:  return glCreateShader(shaderType);  break;
#ifdef GLEW
      case ARB: {
         GLenum arbType;
         switch (shaderType) {
            case GL_VERTEX_SHADER:     arbType = GL_VERTEX_SHADER_ARB;     break;
            case GL_FRAGMENT_SHADER:   arbType = GL_FRAGMENT_SHADER_ARB;   break;
            default:
               assert(false);
         };
         return glCreateShaderObjectARB(arbType);
      }
      break;
#endif
      default:
         assert(false);
   }
}

//===========================================
// OglWrapper::shaderSource
//===========================================
inline void OglWrapper::shaderSource(GLuint shader, GLsizei count, const GLchar** string, const GLint* length) const {
   assert(m_oglSupport.shaders.available);

   switch (m_oglSupport.shaders.nameScheme) {
      case CORE:  glShaderSource(shader, count, string, length);       break;
#ifdef GLEW
      case ARB:   glShaderSourceARB(shader, count, string, length);    break;
#endif
      default:
         assert(false);
   }
}

//===========================================
// OglWrapper::compileShader
//===========================================
inline void OglWrapper::compileShader(GLuint shader) const {
   assert(m_oglSupport.shaders.available);

   switch (m_oglSupport.shaders.nameScheme) {
      case CORE:  glCompileShader(shader);       break;
#ifdef GLEW
      case ARB:   glCompileShaderARB(shader);    break;
#endif
      default:
         assert(false);
   }
}

//===========================================
// OglWrapper::getShaderiv
//===========================================
inline void OglWrapper::getShaderiv(GLuint shader, GLenum pname, GLint* params) const {
   assert(m_oglSupport.shaders.available);

   switch (m_oglSupport.shaders.nameScheme) {
      case CORE:  glGetShaderiv(shader, pname, params);  break;
#ifdef GLEW
      case ARB: {
         GLenum arbName;
         switch (pname) {
            case GL_COMPILE_STATUS: arbName = GL_OBJECT_COMPILE_STATUS_ARB; break;
            // ...
            default:
               assert(false);
         }
         glGetObjectParameterivARB(shader, arbName, params);
      }
      break;
#endif
      default:
         assert(false);
   }
}

//===========================================
// OglWrapper::attachShader
//===========================================
inline void OglWrapper::attachShader(GLuint program, GLuint shader) const {
   assert(m_oglSupport.shaders.available);

   switch (m_oglSupport.shaders.nameScheme) {
      case CORE:  glAttachShader(program, shader);       break;
#ifdef GLEW
      case ARB:   glAttachObjectARB(program, shader);    break;
#endif
      default:
         assert(false);
   }
}

//===========================================
// OglWrapper::getShaderInfoLog
//===========================================
inline void OglWrapper::getShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei* length, GLchar* infoLog) const {
   assert(m_oglSupport.shaders.available);

   switch (m_oglSupport.shaders.nameScheme) {
      case CORE:  glGetShaderInfoLog(shader, maxLength, length, infoLog);  break;
#ifdef GLEW
      case ARB:   glGetInfoLogARB(shader, maxLength, length, infoLog);     break;
#endif
      default:
         assert(false);
   }
}
#endif


}


#endif
