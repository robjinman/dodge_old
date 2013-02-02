/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2011
 */

#include <windows.h>
#include <windows/Timer.hpp>


namespace Dodge {


//===========================================
// Timer::reset
//===========================================
void Timer::reset() {
   LARGE_INTEGER count;
   LARGE_INTEGER freq;
   QueryPerformanceCounter(&count);
   QueryPerformanceFrequency(&freq);
   m_seconds = static_cast<double>(count.QuadPart) / static_cast<double>(freq.QuadPart);
}

//===========================================
// Timer::getTime
//===========================================
double Timer::getTime() const {
   LARGE_INTEGER count;
   LARGE_INTEGER freq;
   QueryPerformanceCounter(&count);
   QueryPerformanceFrequency(&freq);
   return static_cast<double>(count.QuadPart) / static_cast<double>(freq.QuadPart) - m_seconds;
}


}
