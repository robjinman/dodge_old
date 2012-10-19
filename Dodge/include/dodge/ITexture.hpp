/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __I_TEXTURE_HPP__
#define __I_TEXTURE_HPP__


#include "definitions.hpp"


namespace Dodge {


template <typename T_INTEGER, typename T_HANDLE>
class ITexture { 
   public:
      ITexture(const char* file) {}

      //===========================================
      // ITexture::getWidth
      //===========================================
      T_INTEGER getWidth() const {
         return m_width;
      }

      //===========================================
      // ITexture::getHeight
      //===========================================
      T_INTEGER getHeight() const {
         return m_height;
      }

      //===========================================
      // ITexture::getData
      //===========================================
      const byte_t* getData() const {
         return m_data;
      }

      //===========================================
      // ITexture::getHandle
      //===========================================
      const T_HANDLE& getHandle() const {
         return m_handle;
      }

   protected:
      byte_t* m_data;
      T_INTEGER m_width;
      T_INTEGER m_height;
      T_HANDLE m_handle;
};


}


#endif
