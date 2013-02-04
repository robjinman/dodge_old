/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <renderer/ogl/RenderMode.hpp>

#include <renderer/ogl/NonTexturedAlphaMode.hpp>
#include <renderer/ogl/TexturedAlphaMode.hpp>
#ifdef WIN32
#include <renderer/ogl/FixedFunctionMode.hpp>
#endif
#include <renderer/GL_CHECK.hpp>


namespace Dodge {


//===========================================
// RenderMode::create
//===========================================
RenderMode* RenderMode::create(Renderer::mode_t kind) {
   switch (kind) {
      case Renderer::NONTEXTURED_ALPHA: return new NonTexturedAlphaMode();
      case Renderer::TEXTURED_ALPHA:    return new TexturedAlphaMode();
#ifdef WIN32
      case Renderer::FIXED_FUNCTION:    return new FixedFunctionMode();
#endif
      default:
         throw RendererException("Error constructing shader program; Unrecognised type", __FILE__, __LINE__);
   };
}

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


}
