/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __SHADER_PROGRAM_HPP__
#define __SHADER_PROGRAM_HPP__


#include "Renderer.hpp"
#include "../Model.hpp"


namespace Dodge {


class ShaderProgram {
   public:
      virtual void setActive() = 0;
      virtual void sendData(const IModel* model, const cml::matrix44f_c& projMat) = 0;

      virtual ~ShaderProgram() {}

      static ShaderProgram* create(Renderer::mode_t kind);
      static void newShaderFromSource(const char** shaderSrc, GLint type, GLint prog);

   protected:
      inline Renderer::modelHandle_t model_getHandle(const IModel& model) const;
      inline void model_setHandle(IModel& model, Renderer::modelHandle_t handle);
      inline bool model_containsPerVertexColourData(const IModel& model) const;
      inline size_t model_vertexDataSize(const IModel& model) const;
      inline const void* model_getVertexData(const IModel& model) const;
      inline const Renderer::matrixElement_t* model_getMatrix(const IModel& model) const;
};

//===========================================
// Relay calls to model object. This is to allow derived classes to
// access Model's private members.
//===========================================
inline Renderer::modelHandle_t ShaderProgram::model_getHandle(const IModel& model) const { return model.m_handle; }
inline void ShaderProgram::model_setHandle(IModel& model, Renderer::modelHandle_t handle) { model.m_handle = handle; }
inline bool ShaderProgram::model_containsPerVertexColourData(const IModel& model) const { return model.m_colData; }
inline size_t ShaderProgram::model_vertexDataSize(const IModel& model) const { return model.vertexDataSize(); }
inline const void* ShaderProgram::model_getVertexData(const IModel& model) const { return model.getVertexData(); }
inline const Renderer::matrixElement_t* ShaderProgram::model_getMatrix(const IModel& model) const { return model.m_matrix; }
//===========================================


}


#endif
