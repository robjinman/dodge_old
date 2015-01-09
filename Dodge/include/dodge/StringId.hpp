/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2011
 */

#ifndef __STRING_ID_HPP__
#define __STRING_ID_HPP__


#include <string>


namespace Dodge {


long internString(const std::string& str);
std::string getInternedString(long id);


}


#endif /*!__STRING_ID_HPP__*/
