/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __NONTEXTURED_ALPHA_MODE_HPP__
#define __NONTEXTURED_ALPHA_MODE_HPP__


#include "RenderMode.hpp"


namespace Dodge {


class NonTexturedAlphaMode : public RenderMode {
   public:
      NonTexturedAlphaMode();

      virtual bool hasPending() const { return false; }
      virtual void setActive();
      virtual void sendData(const IModel* model, const cml::matrix44f_c& projMat);
      virtual void flush() {}

      virtual ~NonTexturedAlphaMode() {}

   private:
      bool isSupported(const IModel* model) const;

      GLint m_id;

      GLint m_locPosition;
      GLint m_locColour;
      GLint m_locBUniColour;
      GLint m_locUniColour;
      GLint m_locMV;
      GLint m_locP;
};


}


#endif
