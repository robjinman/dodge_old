#ifndef __UTILS_HPP__
#define __UTILS_HPP__


#include <unistd.h>


namespace Dodge {


inline void sleepThread(double seconds) {
   usleep(seconds * 1000000.0);
}


}


#endif
