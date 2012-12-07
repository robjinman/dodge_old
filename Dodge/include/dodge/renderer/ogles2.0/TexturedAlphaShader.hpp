/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __TEXTURED_ALPHA_SHADER_HPP__
#define __TEXTURED_ALPHA_SHADER_HPP__


#include "ShaderProgram.hpp"


namespace Dodge {


class TexturedAlphaShader : public ShaderProgram {
   public:
      TexturedAlphaShader();

      virtual void setActive();
      virtual void sendData(const IModel* model, const cml::matrix44f_c& projMat);

      virtual ~TexturedAlphaShader() {}

   private:
      GLint m_id;

      GLint m_locPosition;
      GLint m_locColour;
      GLint m_locBUniColour;
      GLint m_locUniColour;
      GLint m_locTexCoord;
      GLint m_locMV;
      GLint m_locP;
};


}


#endif
