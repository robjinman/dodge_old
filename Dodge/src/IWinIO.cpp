/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include "IWinIO.hpp"


namespace Dodge {


IWinIO::callbackMap_t IWinIO::m_callbacks = IWinIO::callbackMap_t();
int IWinIO::m_width = 0;
int IWinIO::m_height = 0;


//===========================================
// IWinIO::getWindowWidth
//===========================================
int IWinIO::getWindowWidth() const {
   return m_width;
}

//===========================================
// IWinIO::unregisterCallback
//===========================================
void IWinIO::unregisterCallback(winEvent_t event, callback_t func) {
   callbackMap_t::iterator i = m_callbacks.find(event);

   if (i != m_callbacks.end()) {

     for (unsigned int f = 0; f < i->second.size(); f++) {
        if (i->second[f] == func) {
           i->second.erase(i->second.begin() + f);
           --f;
        }
     }

   }
}


}
