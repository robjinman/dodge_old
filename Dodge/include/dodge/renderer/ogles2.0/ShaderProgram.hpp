/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __SHADER_PROGRAM_HPP__
#define __SHADER_PROGRAM_HPP__


#include "Renderer.hpp"


namespace Dodge {


class ShaderProgram {
   public:
      virtual void setActive() = 0;
      virtual void sendData(const IModel* model, const cml::matrix44f_c& projMat) = 0;

      virtual ~ShaderProgram() {}

      static ShaderProgram* create(Renderer::mode_t kind);
      static void newShaderFromSource(const char** shaderSrc, GLint type, GLint prog);

   protected:
      Renderer::modelHandle_t model_getHandle(const IModel& model) const;
      void model_setHandle(IModel& model, Renderer::modelHandle_t handle);
      bool model_containsPerVertexColourData(const IModel& model) const;
      size_t model_vertexDataSize(const IModel& model) const;
      const void* model_getVertexData(const IModel& model) const;
      const Renderer::matrixElement_t* model_getMatrix(const IModel& model) const;
};


}


#endif
