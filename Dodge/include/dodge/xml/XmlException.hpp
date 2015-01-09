/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __XML_EXCEPTION_HPP__
#define __XML_EXCEPTION_HPP__


#include "../Exception.hpp"


namespace Dodge {


class XmlException : public Exception {
   public:
      XmlException(const std::string& msg, const char* srcFile, int srcLine)
         : Exception(msg, srcFile, srcLine) {}
};


}


#endif
