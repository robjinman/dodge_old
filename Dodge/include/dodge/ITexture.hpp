/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __I_TEXTURE_HPP__
#define __I_TEXTURE_HPP__


namespace Dodge {


template <typename T_ID>
class ITexture { 
   public:
      ITexture(const char* file) {}

      //===========================================
      // ITexture::getWidth
      //===========================================
      int getWidth() const {
         return m_width;
      }

      //===========================================
      // ITexture::getHeight
      //===========================================
      int getHeight() const {
         return m_height;
      }

      //===========================================
      // ITexture::getData
      //===========================================
      const unsigned char* getData() const {
         return m_data;
      }

      //===========================================
      // ITexture::getId
      //===========================================
      const T_ID& getId() const {
         return m_id;
      }

   protected:
      unsigned char* m_data;
      int m_width;
      int m_height;
      T_ID m_id;
};


}


#endif
