/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <renderer/ogl/RenderMode.hpp>

#ifdef GLEW
   #include <renderer/ogl/FixedFunctionMode.hpp>
   #include <renderer/ogl/NonTexturedAlphaMode.hpp>
   #include <renderer/ogl/TexturedAlphaMode.hpp>
#else
   #ifdef GL_FIXED_PIPELINE
      #include <renderer/ogl/FixedFunctionMode.hpp>
   #else
      #include <renderer/ogl/NonTexturedAlphaMode.hpp>
      #include <renderer/ogl/TexturedAlphaMode.hpp>
   #endif
#endif
#include <renderer/GL_CHECK.hpp>
#include <renderer/ogl/OglWrapper.hpp>


namespace Dodge {


//===========================================
// RenderMode::create
//===========================================
RenderMode* RenderMode::create(Renderer::mode_t kind) {
   switch (kind) {
#ifdef GLEW
      case Renderer::FIXED_FUNCTION:    return new FixedFunctionMode();
      case Renderer::NONTEXTURED_ALPHA: return new NonTexturedAlphaMode();
      case Renderer::TEXTURED_ALPHA:    return new TexturedAlphaMode();
#else
   #ifdef GL_FIXED_PIPELINE
      case Renderer::FIXED_FUNCTION:    return new FixedFunctionMode();
   #else
      case Renderer::NONTEXTURED_ALPHA: return new NonTexturedAlphaMode();
      case Renderer::TEXTURED_ALPHA:    return new TexturedAlphaMode();
   #endif
#endif
      default:
         throw RendererException("Error constructing shader program; Unrecognised type", __FILE__, __LINE__);
   };
}

#ifndef GL_FIXED_PIPELINE
//===========================================
// RenderMode::newShaderFromSource
//===========================================
void RenderMode::newShaderFromSource(const char** shaderSrc, GLint type, GLint prog) {
   OglWrapper gl;

   GLuint shader = GL_CHECK(gl.createShader(type));
   GL_CHECK(gl.shaderSource(shader, 1, shaderSrc, NULL));
   GL_CHECK(gl.compileShader(shader));

   GLint success = GL_FALSE;
   GL_CHECK(gl.getShaderiv(shader, GL_COMPILE_STATUS, &success));

   if (success != GL_TRUE) {
      char log[512];
      int len = 0;
      GL_CHECK(gl.getShaderInfoLog(shader, 512, &len, log));

      std::string strLog(log);
      if (strLog.length() > 0 && *(strLog.end() - 1) == '\n')
         strLog.erase(strLog.end() - 1);

      throw RendererException("Could not process shader; Error in source: " + strLog, __FILE__, __LINE__);
   }

   GL_CHECK(gl.attachShader(prog, shader));
}
#endif


}
