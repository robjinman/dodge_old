/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2011
 */

#ifdef WIN32
#include <windows/Timer.hpp>
#else
#include <linux/Timer.hpp>
#endif


namespace Dodge {


//===========================================
// Timer::reset
//===========================================
void Timer::reset() {
   timeval time;
   gettimeofday(&time, 0);

   m_seconds = static_cast<double>(time.tv_sec)
      + (static_cast<double>(time.tv_usec) / 1000000.0);
}

//===========================================
// Timer::getTime
//===========================================
double Timer::getTime() const {
   timeval time;
   gettimeofday(&time, 0);

   return (static_cast<double>(time.tv_sec)
      + (static_cast<double>(time.tv_usec) / 1000000.0)) - m_seconds;
}


}
