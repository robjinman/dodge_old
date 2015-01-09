/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __UTILS_HPP__
#define __UTILS_HPP__


#include <windows.h>
#include "Timer.hpp"


namespace Dodge {


inline void sleepThread(double seconds) {
   Timer timer;
   Sleep(static_cast<DWORD>(floor(seconds * 1000.0)));
   while (timer.getTime () < seconds) {}
}


}


#endif
