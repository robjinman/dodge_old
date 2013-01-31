/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __SHADER_PROGRAM_HPP__
#define __SHADER_PROGRAM_HPP__


#include "Renderer.hpp"
#include "../Model.hpp"
#include "../RendererException.hpp"


namespace Dodge {


class ShaderProgram {
   public:
      virtual bool hasPending() const = 0;
      virtual void setActive() = 0;
      virtual void sendData(const IModel* model, const cml::matrix44f_c& projMat) = 0;
      virtual void flush() = 0;

      virtual ~ShaderProgram() {}

      static ShaderProgram* create(Renderer::mode_t kind);
      static void newShaderFromSource(const char** shaderSrc, GLint type, GLint prog);

   protected:
      inline GLint primitiveToGLType(Renderer::primitive_t primitiveType) const;

      inline Renderer::modelHandle_t model_getHandle(const IModel& model) const;
      inline void model_setHandle(IModel& model, Renderer::modelHandle_t handle);
      inline const void* model_getVertexData(const IModel& model) const;
      inline const Renderer::matrixElement_t* model_getMatrix(const IModel& model) const;
};

//===========================================
// ShaderProgram::primitiveToGLType
//===========================================
inline GLint ShaderProgram::primitiveToGLType(Renderer::primitive_t primitiveType) const {
   switch (primitiveType) {
      case Renderer::TRIANGLES:      return GL_TRIANGLES;
      case Renderer::LINES:          return GL_LINES;
      case Renderer::TRIANGLE_STRIP: return GL_TRIANGLE_STRIP;
      default:
         throw RendererException("Primitive type not supported", __FILE__, __LINE__);
   }
}

//===========================================
// Relay calls to model object. This is to allow derived classes to
// access Model's private members.
//===========================================
inline Renderer::modelHandle_t ShaderProgram::model_getHandle(const IModel& model) const { return model.m_handle; }
inline void ShaderProgram::model_setHandle(IModel& model, Renderer::modelHandle_t handle) { model.m_handle = handle; }
inline const void* ShaderProgram::model_getVertexData(const IModel& model) const { return model.getVertexData(); }
inline const Renderer::matrixElement_t* ShaderProgram::model_getMatrix(const IModel& model) const { return model.m_matrix; }
//===========================================


}


#endif
