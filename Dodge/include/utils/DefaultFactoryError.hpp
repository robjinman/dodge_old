#ifndef __DEFAULT_FACTORY_ERROR_HPP__
#define __DEFAULT_FACTORY_ERROR_HPP__


#include <stdexcept>


template <
   typename TTypeIdentifier,
   class TPtrBase
>
class DefaultFactoryError {
   public:
      class UnknownTypeException : public std::runtime_error {
         public:
            UnknownTypeException(const TTypeIdentifier& id)
               : std::runtime_error("Unknown object type passed to factory"), m_id(id) {}
            TTypeIdentifier getId() const throw() { return m_id; }
            ~UnknownTypeException() throw() {}
         private:
            TTypeIdentifier m_id;
      };
   protected:
      static TPtrBase onCreationOfUnknownType(const TTypeIdentifier& id) { throw UnknownTypeException(id); }
};


#endif
