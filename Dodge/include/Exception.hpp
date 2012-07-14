/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2011
 */

#ifndef __EXCEPTION_HPP__
#define __EXCEPTION_HPP__


#include <stdexcept>
#include <string>


namespace Dodge {


class Exception : public std::runtime_error {
   public:
      Exception(const std::string& msg, const char* file, unsigned int line);

      virtual const char* what() const throw();
      inline void append(const std::string& text) throw();
      inline void prepend(const std::string& text) throw();

      ~Exception() throw() {}

   private:
      void constructMsg() throw();

      std::string m_msg;
      std::string m_file;
      unsigned int m_line;

      std::string m_outMsg;
};

//===========================================
// Exception::append
//===========================================
inline void Exception::append(const std::string& text) throw() {
   m_msg.append(text);
   constructMsg();
}

//===========================================
// Exception::prepend
//===========================================
inline void Exception::prepend(const std::string& text) throw() {
   m_msg.insert(0, text);
   constructMsg();
}


}


#endif /*!__EXCEPTION_HPP__*/
