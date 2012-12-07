/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <renderer/ogles2.0/ShaderProgram.hpp>
#include <renderer/ogles2.0/NonTexturedAlphaShader.hpp>
#include <renderer/ogles2.0/TexturedAlphaShader.hpp>
#include <renderer/Model.hpp>
#include <renderer/GL_CHECK.hpp>


namespace Dodge {


//===========================================
// ShaderProgram::create
//===========================================
ShaderProgram* ShaderProgram::create(Renderer::mode_t kind) {
   switch (kind) {
      case Renderer::NONTEXTURED_ALPHA: return new NonTexturedAlphaShader();
      case Renderer::TEXTURED_ALPHA:    return new TexturedAlphaShader();
      default:
         throw RendererException("Error constructing shader program; Unrecognised type", __FILE__, __LINE__);
   };
}

//===========================================
// ShaderProgram::newShaderFromSource
//===========================================
void ShaderProgram::newShaderFromSource(const char** shaderSrc, GLint type, GLint prog) {
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
// Relay calls to model object. This is to allow derived classes to
// access Model's private members.
//===========================================
void ShaderProgram::model_lock(const IModel& model) const { model.lock(); }
void ShaderProgram::model_unlock(const IModel& model) const { model.unlock(); }
Renderer::modelHandle_t ShaderProgram::model_getHandle(const IModel& model) const { return model.getHandle(); }
void ShaderProgram::model_setHandle(IModel& model, Renderer::modelHandle_t handle) { model.setHandle(handle); }
bool ShaderProgram::model_containsPerVertexColourData(const IModel& model) const { return model.containsPerVertexColourData(); }
size_t ShaderProgram::model_vertexDataSize(const IModel& model) const { return model.vertexDataSize(); }
const void* ShaderProgram::model_getVertexData(const IModel& model) const { return model.getVertexData(); }
const Renderer::matrixElement_t* ShaderProgram::model_getMatrix(const IModel& model) const { return model.getMatrix(); }
//===========================================


}
