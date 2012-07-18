#ifndef __FACTORY_HPP__
#define __FACTORY_HPP__


#include <map>
#include "DefaultFactoryError.hpp"
#include "Functor.hpp"
#include "TypeList.hpp"


template <
   class TBase,
   class TPtrBase,
   typename TObjectCreator,
   typename TTypeIdentifier = long,
   template<typename, class> // Must define onCreationOfUnknownType()
      class TFactoryErrorPolicy = DefaultFactoryError
>
class Factory;

template <
   class TBase,
   class TPtrBase,
   class TTList,
   typename TTypeIdentifier,
   template<typename, class> class TFactoryErrorPolicy
>
class Factory<TBase, TPtrBase, Functor<TPtrBase, TTList>, TTypeIdentifier, TFactoryErrorPolicy>
   : public TFactoryErrorPolicy<TTypeIdentifier, TPtrBase> {

   protected:
      typedef typename TL::TypeAtNonStrict<TTList, 0, EmptyType>::Result P1;
      typedef typename TL::TypeAtNonStrict<TTList, 1, EmptyType>::Result P2;
      typedef typename TL::TypeAtNonStrict<TTList, 2, EmptyType>::Result P3;
      typedef typename TL::TypeAtNonStrict<TTList, 3, EmptyType>::Result P4;
      typedef typename TL::TypeAtNonStrict<TTList, 4, EmptyType>::Result P5;
      typedef typename TL::TypeAtNonStrict<TTList, 5, EmptyType>::Result P6;
      typedef typename TL::TypeAtNonStrict<TTList, 6, EmptyType>::Result P7;
      typedef typename TL::TypeAtNonStrict<TTList, 7, EmptyType>::Result P8;
      typedef typename TL::TypeAtNonStrict<TTList, 8, EmptyType>::Result P9;
      typedef typename TL::TypeAtNonStrict<TTList, 9, EmptyType>::Result P10;
      typedef typename TL::TypeAtNonStrict<TTList, 10, EmptyType>::Result P11;
      typedef typename TL::TypeAtNonStrict<TTList, 11, EmptyType>::Result P12;
      typedef typename TL::TypeAtNonStrict<TTList, 12, EmptyType>::Result P13;
      typedef typename TL::TypeAtNonStrict<TTList, 13, EmptyType>::Result P14;
      typedef typename TL::TypeAtNonStrict<TTList, 14, EmptyType>::Result P15;

   public:
      void registerClass(TTypeIdentifier id, const Functor<TPtrBase, TTList>& creator) {
         m_creators.insert(typename AssocMap_t::value_type(id, creator));
      }

      void unregisterClass(TTypeIdentifier id) { m_creators.erase(id); }

      TPtrBase createObject(TTypeIdentifier id) const {
         typename AssocMap_t::const_iterator i = m_creators.find(id);
         return i == m_creators.end() ? onCreationOfUnknownType(id) : i->second();
      }
      TPtrBase createObject(TTypeIdentifier id, P1 p1) const {
         typename AssocMap_t::const_iterator i = m_creators.find(id);
         return i == m_creators.end() ? onCreationOfUnknownType(id) : i->second(p1);
      }
      TPtrBase createObject(TTypeIdentifier id, P1 p1, P2 p2) const {
         typename AssocMap_t::const_iterator i = m_creators.find(id);
         return i == m_creators.end() ? onCreationOfUnknownType(id) : i->second(p1, p2);
      }
      TPtrBase createObject(TTypeIdentifier id, P1 p1, P2 p2, P3 p3) const {
         typename AssocMap_t::const_iterator i = m_creators.find(id);
         return i == m_creators.end() ? onCreationOfUnknownType(id) : i->second(p1, p2, p3);
      }
      TPtrBase createObject(TTypeIdentifier id, P1 p1, P2 p2, P3 p3, P4 p4) const {
         typename AssocMap_t::const_iterator i = m_creators.find(id);
         return i == m_creators.end() ? onCreationOfUnknownType(id) : i->second(p1, p2, p3, p4);
      }
      TPtrBase createObject(TTypeIdentifier id, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) const {
         typename AssocMap_t::const_iterator i = m_creators.find(id);
         return i == m_creators.end() ? onCreationOfUnknownType(id) : i->second(p1, p2, p3, p4, p5);
      }
      TPtrBase createObject(TTypeIdentifier id, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6) const {
         typename AssocMap_t::const_iterator i = m_creators.find(id);
         return i == m_creators.end() ? onCreationOfUnknownType(id) : i->second(p1, p2, p3, p4, p5, p6);
      }
      TPtrBase createObject(TTypeIdentifier id, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7) const {
         typename AssocMap_t::const_iterator i = m_creators.find(id);
         return i == m_creators.end() ? onCreationOfUnknownType(id) : i->second(p1, p2, p3, p4, p5, p6, p7);
      }
      TPtrBase createObject(TTypeIdentifier id, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8) const {
         typename AssocMap_t::const_iterator i = m_creators.find(id);
         return i == m_creators.end() ? onCreationOfUnknownType(id) : i->second(p1, p2, p3, p4, p5, p6, p7, p8);
      }
      TPtrBase createObject(TTypeIdentifier id, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8,
         P9 p9) const {

         typename AssocMap_t::const_iterator i = m_creators.find(id);
         return i == m_creators.end() ? onCreationOfUnknownType(id) : i->second(p1, p2, p3, p4, p5, p6, p7, p8, p9);
      }
      TPtrBase createObject(TTypeIdentifier id, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8,
         P9 p9, P10 p10) const {

         typename AssocMap_t::const_iterator i = m_creators.find(id);
         return i == m_creators.end() ? onCreationOfUnknownType(id) : i->second(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
      }
      TPtrBase createObject(TTypeIdentifier id, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8,
         P9 p9, P10 p10, P11 p11) const {

         typename AssocMap_t::const_iterator i = m_creators.find(id);
         return i == m_creators.end() ? onCreationOfUnknownType(id) : i->second(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
      }
      TPtrBase createObject(TTypeIdentifier id, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8,
         P9 p9, P10 p10, P11 p11, P12 p12) const {

         typename AssocMap_t::const_iterator i = m_creators.find(id);
         return i == m_creators.end() ? onCreationOfUnknownType(id) : i->second(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
      }
      TPtrBase createObject(TTypeIdentifier id, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8,
         P9 p9, P10 p10, P11 p11, P12 p12, P13 p13) const {

         typename AssocMap_t::const_iterator i = m_creators.find(id);
         return i == m_creators.end() ? onCreationOfUnknownType(id) : i->second(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13);
      }
      TPtrBase createObject(TTypeIdentifier id, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8,
         P9 p9, P10 p10, P11 p11, P12 p12, P13 p13, P14 p14) const {

         typename AssocMap_t::const_iterator i = m_creators.find(id);
         return i == m_creators.end() ? onCreationOfUnknownType(id) : i->second(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14);
      }
      TPtrBase createObject(TTypeIdentifier id, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8,
         P9 p9, P10 p10, P11 p11, P12 p12, P13 p13, P14 p14, P15 p15) const {

         typename AssocMap_t::const_iterator i = m_creators.find(id);
         return i == m_creators.end() ? onCreationOfUnknownType(id) : i->second(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15);
      }

   private:
      typedef std::map<TTypeIdentifier, Functor<TPtrBase, TTList> > AssocMap_t;
      AssocMap_t m_creators;
};


#endif
