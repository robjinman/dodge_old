/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2011
 */

#include <sstream>
#include "Exception.hpp"


namespace Dodge {


//===========================================
// Exception::Exception
//===========================================
Exception::Exception(const std::string& msg, const char* file, unsigned int line)
   : runtime_error(msg), m_msg(msg), m_file(file), m_line(line) {

   constructMsg();
}

//===========================================
// Exception::constructMsg
//===========================================
void Exception::constructMsg() throw() {
   std::stringstream str;

   str << m_msg << " (FILE: " << m_file << ", LINE: " << m_line << ")";
   m_outMsg = str.str();
}

//===========================================
// Exception::what
//===========================================
const char* Exception::what() const throw() { return m_outMsg.data(); }


}
