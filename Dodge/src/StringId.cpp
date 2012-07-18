/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2011
 */

#include <map>
#include <StringId.hpp>


using namespace std;


namespace Dodge {


//===========================================
// getTable
//===========================================
map<long, string>& getTable() {
   static map<long, string> stringIds = map<long, string>();
   return stringIds;
}

//===========================================
// hash
//===========================================
long hash(const char *str) {
   long hash = 5381;
   int c;

   while ((c = *str++))
      hash = ((hash << 5) + hash) + c;

   return hash;
}

//===========================================
// internString
//===========================================
long internString(const string& str) {
   long id = hash(str.data());
   getTable()[id] = str;

   return id;
}

//===========================================
// getInternedString
//===========================================
string getInternedString(long id) {
   map<long, string>::iterator it = getTable().find(id);
   return it != getTable().end() ? it->second : string();
}


}
