/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <renderer/ogl/RenderMode.hpp>

#ifdef WIN32
   #include <renderer/ogl/FixedFunctionMode.hpp>
   #include <renderer/ogl/NonTexturedAlphaMode.hpp>
   #include <renderer/ogl/TexturedAlphaMode.hpp>
#elif defined LINUX
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


namespace Dodge {


//===========================================
// RenderMode::create
//===========================================
RenderMode* RenderMode::create(Renderer::mode_t kind) {
   switch (kind) {
#ifdef WIN32
      case Renderer::FIXED_FUNCTION:    return new FixedFunctionMode();
      case Renderer::NONTEXTURED_ALPHA: return new NonTexturedAlphaMode();
      case Renderer::TEXTURED_ALPHA:    return new TexturedAlphaMode();
#elif defined LINUX
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
   GLint shader;
   GLint success = GL_FALSE;

   // Create shader and load into GL
   shader = GL_CHECK(glCreateShader(type));

   GL_CHECK(glShaderSource(shader, 1, shaderSrc, NULL));

   // Compile the shader
   GL_CHECK(glCompileShader(shader));
   GL_CHECK(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));

   if (success != GL_TRUE)
      throw RendererException("Could not process shader; Error in source", __FILE__, __LINE__);

   GL_CHECK(glAttachShader(prog, shader));
}
#endif


}
