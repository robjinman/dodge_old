#ifndef __DEFAULT_MANAGER_ERROR_HPP__
#define __DEFAULT_MANAGER_ERROR_HPP__


#include <stdexcept>
#include <set>
#include "utils/DefaultFactoryError.hpp"


template <
   typename TTypeIdentifier,
   class TPtrBase
>
class DefaultManagerError : public DefaultFactoryError<TTypeIdentifier, TPtrBase> {
   public:
      class ObjectNotFoundException : public std::runtime_error {
         public:
            ObjectNotFoundException(const TTypeIdentifier& id)
               : std::runtime_error("Object not found"), m_id(id) {}
            TTypeIdentifier getId() const throw() { return m_id; }
            ~ObjectNotFoundException() throw() {}
         private:
            TTypeIdentifier m_id;
      };
   protected:
      static TPtrBase onObjectNotFound(const TTypeIdentifier& id) { throw ObjectNotFoundException(id); }
      static std::set<TPtrBase> onRetrievalOfUnknownType(const TTypeIdentifier& id) { return std::set<TPtrBase>(); }
};


#endif
