/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __RENDER_MODE_HPP__
#define __RENDER_MODE_HPP__


#include "Renderer.hpp"
#include "../Model.hpp"
#include "../RendererException.hpp"


namespace Dodge {


class RenderMode {
   public:
      virtual bool hasPending() const = 0;
      virtual void setActive() = 0;
      virtual void sendData(const IModel* model, const cml::matrix44f_c& projMat) = 0;
      virtual void flush() = 0;

      virtual ~RenderMode() {}

      static RenderMode* create(Renderer::mode_t kind);
      static void newShaderFromSource(const char** shaderSrc, GLint type, GLint prog);

   protected:
      inline GLint primitiveToGLType(Renderer::primitive_t primitiveType) const;

      inline Renderer::modelHandle_t model_getHandle(const IModel& model) const;
      inline void model_setHandle(IModel& model, Renderer::modelHandle_t handle);
      inline const void* model_getVertexData(const IModel& model) const;
      inline const Renderer::matrixElement_t* model_getMatrix(const IModel& model) const;
};

//===========================================
// RenderMode::primitiveToGLType
//===========================================
inline GLint RenderMode::primitiveToGLType(Renderer::primitive_t primitiveType) const {
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
inline Renderer::modelHandle_t RenderMode::model_getHandle(const IModel& model) const { return model.m_handle; }
inline void RenderMode::model_setHandle(IModel& model, Renderer::modelHandle_t handle) { model.m_handle = handle; }
inline const void* RenderMode::model_getVertexData(const IModel& model) const { return model.getVertexData(); }
inline const Renderer::matrixElement_t* RenderMode::model_getMatrix(const IModel& model) const { return model.m_matrix; }
//===========================================


}


#endif
