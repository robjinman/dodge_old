/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __TEXTURED_ALPHA_MODE_HPP__
#define __TEXTURED_ALPHA_MODE_HPP__


#include "RenderMode.hpp"
#include "../../definitions.hpp"


namespace Dodge {


class TexturedAlphaMode : public RenderMode {
   public:
      TexturedAlphaMode();

      virtual void setActive();
      virtual void sendData(const IModel* model, const cml::matrix44f_c& projMat, GLuint vbo);

      virtual ~TexturedAlphaMode() {}

   private:
      bool isSupported(const IModel* model) const;

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
