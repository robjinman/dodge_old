/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2013
 */

#ifndef __AUDIO_EXCEPTION_HPP__
#define __AUDIO_EXCEPTION_HPP__


#include <string>
#include "../Exception.hpp"


namespace Dodge {


class AudioException : public Exception {
   public:
      AudioException(const std::string& msg, const char* file, unsigned int line)
         : Exception(msg, file, line) {}

      virtual ~AudioException() throw() {}
};


}


#endif
