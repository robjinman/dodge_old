/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __TEXTURE_HPP__
#define __TEXTURE_HPP__


#include "definitions.hpp"
#include "ITexture.hpp"
#include "pnglite/pnglite.h"


namespace Dodge {


// PNG implementation of ITexture interface
class Texture : public ITexture {
   public:
      Texture(const char* file);

   private:
      png_t m_png;
};

typedef sharedPtr_t<Texture> pTexture_t;


}


#endif
