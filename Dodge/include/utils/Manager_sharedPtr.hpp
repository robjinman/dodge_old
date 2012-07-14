#ifndef __MANAGER_SHARED_PTR_HPP__
#define __MANAGER_SHARED_PTR_HPP__


#include <map>
#include <boost/shared_ptr.hpp>
#include <stdexcept>
#include "utils/TypeList.hpp"


namespace {

template <class TTList, typename TTypeIdentifier, typename TNameIdentifier>
class Accessor;

}


template <class TTList, typename TTypeIdentifier = long, typename TNameIdentifier = long>
class Manager_sharedPtr {
   friend class Accessor<TTList, TTypeIdentifier, TNameIdentifier>;

   private:
      typedef std::multimap<TTypeIdentifier, unsigned int> typesMap_t;

      template <class X> struct type_t {};
      template <unsigned int i> struct iType_t {};

      typedef typename TL::TypeAtNonStrict<TTList, 0, iType_t<0> >::Result T0;
      typedef typename TL::TypeAtNonStrict<TTList, 1, iType_t<1> >::Result T1;
      typedef typename TL::TypeAtNonStrict<TTList, 2, iType_t<2> >::Result T2;
      typedef typename TL::TypeAtNonStrict<TTList, 3, iType_t<3> >::Result T3;
      typedef typename TL::TypeAtNonStrict<TTList, 4, iType_t<4> >::Result T4;
      typedef typename TL::TypeAtNonStrict<TTList, 5, iType_t<5> >::Result T5;
      typedef typename TL::TypeAtNonStrict<TTList, 6, iType_t<6> >::Result T6;
      typedef typename TL::TypeAtNonStrict<TTList, 7, iType_t<7> >::Result T7;
      typedef typename TL::TypeAtNonStrict<TTList, 8, iType_t<8> >::Result T8;
      typedef typename TL::TypeAtNonStrict<TTList, 9, iType_t<9> >::Result T9;
      typedef typename TL::TypeAtNonStrict<TTList, 10, iType_t<10> >::Result T10;
      typedef typename TL::TypeAtNonStrict<TTList, 11, iType_t<11> >::Result T11;
      typedef typename TL::TypeAtNonStrict<TTList, 12, iType_t<12> >::Result T12;
      typedef typename TL::TypeAtNonStrict<TTList, 13, iType_t<13> >::Result T13;
      typedef typename TL::TypeAtNonStrict<TTList, 14, iType_t<14> >::Result T14;

      typedef boost::shared_ptr<T0> pT0;     typedef boost::shared_ptr<T1> pT1;     typedef boost::shared_ptr<T2> pT2;
      typedef boost::shared_ptr<T3> pT3;     typedef boost::shared_ptr<T4> pT4;     typedef boost::shared_ptr<T5> pT5;
      typedef boost::shared_ptr<T6> pT6;     typedef boost::shared_ptr<T7> pT7;     typedef boost::shared_ptr<T8> pT8;
      typedef boost::shared_ptr<T9> pT9;     typedef boost::shared_ptr<T10> pT10;   typedef boost::shared_ptr<T11> pT11;
      typedef boost::shared_ptr<T12> pT12;   typedef boost::shared_ptr<T13> pT13;   typedef boost::shared_ptr<T14> pT14;

   public:
      Manager_sharedPtr(const typesMap_t& types)
         :  m_types(types),
            m_pMap0(new map0_t()),
            m_pMap1(new map1_t()),
            m_pMap2(new map2_t()),
            m_pMap3(new map3_t()),
            m_pMap4(new map4_t()),
            m_pMap5(new map5_t()),
            m_pMap6(new map6_t()),
            m_pMap7(new map7_t()),
            m_pMap8(new map8_t()),
            m_pMap9(new map9_t()),
            m_pMap10(new map10_t()),
            m_pMap11(new map11_t()),
            m_pMap12(new map12_t()),
            m_pMap13(new map13_t()),
            m_pMap14(new map14_t()) {}

      template <class T>
      void addObject(const TTypeIdentifier& type, const TNameIdentifier& name, boost::shared_ptr<T> obj) {
         typedef boost::shared_ptr<T> pT;

         typename typesMap_t::const_iterator x = m_types.find(type);
         if (x == m_types.end()) err0();

         std::pair<typename typesMap_t::const_iterator, typename typesMap_t::const_iterator> r = m_types.equal_range(type);
         for (typename typesMap_t::const_iterator i = r.first; i != r.second; ++i) {
            switch (i->second) {
               case 0: {
                  if (!TL::Conversion<pT, pT0>::exists) {               // If pT is not implicitly convertible to pT0,
                     pT0 o = boost::dynamic_pointer_cast<T0>(obj);      // attempt a type-cast
                     if (o) addT0Object(name, o, 0); else err1();
                  } else addT0Object(name, obj, 0);
               } break;
               case 1: {
                  if (!TL::Conversion<pT, pT1>::exists) {
                     pT1 o = boost::dynamic_pointer_cast<T1>(obj);
                     if (o) addT1Object(name, o, 0); else err1();
                  } else addT1Object(name, obj, 0);
               } break;
               case 2: {
                  if (!TL::Conversion<pT, pT2>::exists) {
                     pT2 o = boost::dynamic_pointer_cast<T2>(obj);
                     if (o) addT2Object(name, o, 0); else err1();
                  } else addT2Object(name, obj, 0);
               } break;
               case 3: {
                  if (!TL::Conversion<pT, pT3>::exists) {
                     pT3 o = boost::dynamic_pointer_cast<T3>(obj);
                     if (o) addT3Object(name, o, 0); else err1();
                  } else addT3Object(name, obj, 0);
               } break;
               case 4: {
                  if (!TL::Conversion<pT, pT4>::exists) {
                     pT4 o = boost::dynamic_pointer_cast<T4>(obj);
                     if (o) addT4Object(name, o, 0); else err1();
                  } else addT4Object(name, obj, 0);
               } break;
               case 5: {
                  if (!TL::Conversion<pT, pT5>::exists) {
                     pT5 o = boost::dynamic_pointer_cast<T5>(obj);
                     if (o) addT5Object(name, o, 0); else err1();
                  } else addT5Object(name, obj, 0);
               } break;
               case 6: {
                  if (!TL::Conversion<pT, pT6>::exists) {
                     pT6 o = boost::dynamic_pointer_cast<T6>(obj);
                     if (o) addT6Object(name, o, 0); else err1();
                  } else addT6Object(name, obj, 0);
               } break;
               case 7: {
                  if (!TL::Conversion<pT, pT7>::exists) {
                     pT7 o = boost::dynamic_pointer_cast<T7>(obj);
                     if (o) addT7Object(name, o, 0); else err1();
                  } else addT7Object(name, obj, 0);
               } break;
               case 8: {
                  if (!TL::Conversion<pT, pT8>::exists) {
                     pT8 o = boost::dynamic_pointer_cast<T8>(obj);
                     if (o) addT8Object(name, o, 0); else err1();
                  } else addT8Object(name, obj, 0);
               } break;
               case 9: {
                  if (!TL::Conversion<pT, pT9>::exists) {
                     pT9 o = boost::dynamic_pointer_cast<T9>(obj);
                     if (o) addT9Object(name, o, 0); else err1();
                  } else addT9Object(name, obj, 0);
               } break;
               case 10: {
                  if (!TL::Conversion<pT, pT10>::exists) {
                     pT10 o = boost::dynamic_pointer_cast<T10>(obj);
                     if (o) addT10Object(name, o, 0); else err1();
                  } else addT10Object(name, obj, 0);
               } break;
               case 11: {
                  if (!TL::Conversion<pT, pT11>::exists) {
                     pT11 o = boost::dynamic_pointer_cast<T11>(obj);
                     if (o) addT11Object(name, o, 0); else err1();
                  } else addT11Object(name, obj, 0);
               } break;
               case 12: {
                  if (!TL::Conversion<pT, pT12>::exists) {
                     pT12 o = boost::dynamic_pointer_cast<T12>(obj);
                     if (o) addT12Object(name, o, 0); else err1();
                  } else addT12Object(name, obj, 0);
               } break;
               case 13: {
                  if (!TL::Conversion<pT, pT13>::exists) {
                     pT13 o = boost::dynamic_pointer_cast<T13>(obj);
                     if (o) addT13Object(name, o, 0); else err1();
                  } else addT13Object(name, obj, 0);
               } break;
               case 14: {
                  if (!TL::Conversion<pT, pT14>::exists) {
                     pT14 o = boost::dynamic_pointer_cast<T14>(obj);
                     if (o) addT14Object(name, o, 0); else err1();
                  } else addT14Object(name, obj, 0);
               } break;
            }
         }
      }

      void removeObject(const TNameIdentifier& name) {
         m_pMap0->erase(name);   m_pMap1->erase(name);   m_pMap2->erase(name);
         m_pMap3->erase(name);   m_pMap4->erase(name);   m_pMap5->erase(name);
         m_pMap6->erase(name);   m_pMap7->erase(name);   m_pMap8->erase(name);
         m_pMap9->erase(name);   m_pMap10->erase(name);  m_pMap11->erase(name);
         m_pMap12->erase(name);  m_pMap13->erase(name);  m_pMap14->erase(name);
      }

      template <class T>
      boost::shared_ptr<T> retrieveObject(const TNameIdentifier& name) const {
         const std::map<TNameIdentifier, boost::shared_ptr<T> >& objs = *retrieveObjects_(type_t<boost::shared_ptr<T> >());
         typename std::map<TNameIdentifier, boost::shared_ptr<T> >::const_iterator it = objs.find(name);
         return it != objs.end() ? it->second : boost::shared_ptr<T>();
      }

      template <class T>
      const std::map<TNameIdentifier, boost::shared_ptr<T> >& retrieveObjects() const {
         return *retrieveObjects_(type_t<boost::shared_ptr<T> >());
      }

      template <class TSubList>
      Manager_sharedPtr<TSubList, TTypeIdentifier, TNameIdentifier> spawnSubManager(const typesMap_t& types) const {
         typedef Manager_sharedPtr<TSubList, TTypeIdentifier, TNameIdentifier> manager_t;
         typedef Accessor<TSubList, TTypeIdentifier, TNameIdentifier> accessor_t;

         manager_t sub(types);
         accessor_t accessor(sub);

         accessor.getMap0() = retrObjsNonStrict<typename accessor_t::pT0>(type_t<boost::shared_ptr<typename TL::TypeAtNonStrict<TSubList, 0, EmptyType>::Result> >());
         accessor.getMap1() = retrObjsNonStrict<typename accessor_t::pT1>(type_t<boost::shared_ptr<typename TL::TypeAtNonStrict<TSubList, 1, EmptyType>::Result> >());
         accessor.getMap2() = retrObjsNonStrict<typename accessor_t::pT2>(type_t<boost::shared_ptr<typename TL::TypeAtNonStrict<TSubList, 2, EmptyType>::Result> >());
         accessor.getMap3() = retrObjsNonStrict<typename accessor_t::pT3>(type_t<boost::shared_ptr<typename TL::TypeAtNonStrict<TSubList, 3, EmptyType>::Result> >());
         accessor.getMap4() = retrObjsNonStrict<typename accessor_t::pT4>(type_t<boost::shared_ptr<typename TL::TypeAtNonStrict<TSubList, 4, EmptyType>::Result> >());
         accessor.getMap5() = retrObjsNonStrict<typename accessor_t::pT5>(type_t<boost::shared_ptr<typename TL::TypeAtNonStrict<TSubList, 5, EmptyType>::Result> >());
         accessor.getMap6() = retrObjsNonStrict<typename accessor_t::pT6>(type_t<boost::shared_ptr<typename TL::TypeAtNonStrict<TSubList, 6, EmptyType>::Result> >());
         accessor.getMap7() = retrObjsNonStrict<typename accessor_t::pT7>(type_t<boost::shared_ptr<typename TL::TypeAtNonStrict<TSubList, 7, EmptyType>::Result> >());
         accessor.getMap8() = retrObjsNonStrict<typename accessor_t::pT8>(type_t<boost::shared_ptr<typename TL::TypeAtNonStrict<TSubList, 8, EmptyType>::Result> >());
         accessor.getMap9() = retrObjsNonStrict<typename accessor_t::pT9>(type_t<boost::shared_ptr<typename TL::TypeAtNonStrict<TSubList, 9, EmptyType>::Result> >());
         accessor.getMap10() = retrObjsNonStrict<typename accessor_t::pT10>(type_t<boost::shared_ptr<typename TL::TypeAtNonStrict<TSubList, 10, EmptyType>::Result> >());
         accessor.getMap11() = retrObjsNonStrict<typename accessor_t::pT11>(type_t<boost::shared_ptr<typename TL::TypeAtNonStrict<TSubList, 11, EmptyType>::Result> >());
         accessor.getMap12() = retrObjsNonStrict<typename accessor_t::pT12>(type_t<boost::shared_ptr<typename TL::TypeAtNonStrict<TSubList, 12, EmptyType>::Result> >());
         accessor.getMap13() = retrObjsNonStrict<typename accessor_t::pT13>(type_t<boost::shared_ptr<typename TL::TypeAtNonStrict<TSubList, 13, EmptyType>::Result> >());
         accessor.getMap14() = retrObjsNonStrict<typename accessor_t::pT14>(type_t<boost::shared_ptr<typename TL::TypeAtNonStrict<TSubList, 14, EmptyType>::Result> >());

         return sub;
      }

   private:
      typesMap_t m_types;

      typedef std::map<TNameIdentifier, pT0> map0_t;
      typedef std::map<TNameIdentifier, pT1> map1_t;
      typedef std::map<TNameIdentifier, pT2> map2_t;
      typedef std::map<TNameIdentifier, pT3> map3_t;
      typedef std::map<TNameIdentifier, pT4> map4_t;
      typedef std::map<TNameIdentifier, pT5> map5_t;
      typedef std::map<TNameIdentifier, pT6> map6_t;
      typedef std::map<TNameIdentifier, pT7> map7_t;
      typedef std::map<TNameIdentifier, pT8> map8_t;
      typedef std::map<TNameIdentifier, pT9> map9_t;
      typedef std::map<TNameIdentifier, pT10> map10_t;
      typedef std::map<TNameIdentifier, pT11> map11_t;
      typedef std::map<TNameIdentifier, pT12> map12_t;
      typedef std::map<TNameIdentifier, pT13> map13_t;
      typedef std::map<TNameIdentifier, pT14> map14_t;

      boost::shared_ptr<map0_t> m_pMap0;   boost::shared_ptr<map1_t> m_pMap1;   boost::shared_ptr<map2_t> m_pMap2;
      boost::shared_ptr<map3_t> m_pMap3;   boost::shared_ptr<map4_t> m_pMap4;   boost::shared_ptr<map5_t> m_pMap5;
      boost::shared_ptr<map6_t> m_pMap6;   boost::shared_ptr<map7_t> m_pMap7;   boost::shared_ptr<map8_t> m_pMap8;
      boost::shared_ptr<map9_t> m_pMap9;   boost::shared_ptr<map10_t> m_pMap10; boost::shared_ptr<map11_t> m_pMap11;
      boost::shared_ptr<map12_t> m_pMap12; boost::shared_ptr<map13_t> m_pMap13; boost::shared_ptr<map14_t> m_pMap14;

      boost::shared_ptr<map0_t> retrieveObjects_(type_t<pT0>) const { return m_pMap0; }
      boost::shared_ptr<map1_t> retrieveObjects_(type_t<pT1>) const { return m_pMap1; }
      boost::shared_ptr<map2_t> retrieveObjects_(type_t<pT2>) const { return m_pMap2; }
      boost::shared_ptr<map3_t> retrieveObjects_(type_t<pT3>) const { return m_pMap3; }
      boost::shared_ptr<map4_t> retrieveObjects_(type_t<pT4>) const { return m_pMap4; }
      boost::shared_ptr<map5_t> retrieveObjects_(type_t<pT5>) const { return m_pMap5; }
      boost::shared_ptr<map6_t> retrieveObjects_(type_t<pT6>) const { return m_pMap6; }
      boost::shared_ptr<map7_t> retrieveObjects_(type_t<pT7>) const { return m_pMap7; }
      boost::shared_ptr<map8_t> retrieveObjects_(type_t<pT8>) const { return m_pMap8; }
      boost::shared_ptr<map9_t> retrieveObjects_(type_t<pT9>) const { return m_pMap9; }
      boost::shared_ptr<map10_t> retrieveObjects_(type_t<pT10>) const { return m_pMap10; }
      boost::shared_ptr<map11_t> retrieveObjects_(type_t<pT11>) const { return m_pMap11; }
      boost::shared_ptr<map12_t> retrieveObjects_(type_t<pT12>) const { return m_pMap12; }
      boost::shared_ptr<map13_t> retrieveObjects_(type_t<pT13>) const { return m_pMap13; }
      boost::shared_ptr<map14_t> retrieveObjects_(type_t<pT14>) const { return m_pMap14; }


      // --------------------------------------------------------------
      // Helper functions for spawnSubManager. Second overload catches calls for arg types T not recognised in this
      // instantiation of Manager_sharedPtr, hence is "NonStrict" wrapper around retrieveObjects_ functions.
      // --------------------------------------------------------------
      template <class T>
      boost::shared_ptr<std::map<TNameIdentifier, T> > retrObjsNonStrict(type_t<T>) const {
         return retrieveObjects_(type_t<T>());
      }

      template <class T>
      boost::shared_ptr<std::map<TNameIdentifier, T> > retrObjsNonStrict(type_t<boost::shared_ptr<EmptyType> >) const {
         return boost::shared_ptr<std::map<TNameIdentifier, T> >(new std::map<TNameIdentifier, T>());
      }
      // --------------------------------------------------------------


      void err0() { throw std::runtime_error("Error adding object; unknown type"); }
      void err1() { throw std::runtime_error("Error adding object; object not compatible with map container"); }

      // Dummy parameter (int) to prevent ambiguity with next function overload.
      void addT0Object(const TNameIdentifier& name, pT0 obj, int) { m_pMap0->insert(std::pair<TNameIdentifier, pT0>(name, obj)); }
      void addT0Object(const TNameIdentifier&, boost::shared_ptr<void>, ...) {}  // Function is never called. Exists so that calls to
                                                                                 // addT0Object(const TNameIdentifier&, boost::shared_ptr<T>, int)
                                                                                 // are legal (i.e. compilable) for any T. Ellipsis ensures this
                                                                                 // overload is lowest priority.

      void addT1Object(const TNameIdentifier& name, pT1 obj, int) { m_pMap1->insert(std::pair<TNameIdentifier, pT1>(name, obj)); }
      void addT1Object(const TNameIdentifier&, boost::shared_ptr<void>, ...) {}
      void addT2Object(const TNameIdentifier& name, pT2 obj, int) { m_pMap2->insert(std::pair<TNameIdentifier, pT2>(name, obj)); }
      void addT2Object(const TNameIdentifier&, boost::shared_ptr<void>, ...) {}
      void addT3Object(const TNameIdentifier& name, pT3 obj, int) { m_pMap3->insert(std::pair<TNameIdentifier, pT3>(name, obj)); }
      void addT3Object(const TNameIdentifier&, boost::shared_ptr<void>, ...) {}
      void addT4Object(const TNameIdentifier& name, pT4 obj, int) { m_pMap4->insert(std::pair<TNameIdentifier, pT4>(name, obj)); }
      void addT4Object(const TNameIdentifier&, boost::shared_ptr<void>, ...) {}
      void addT5Object(const TNameIdentifier& name, pT5 obj, int) { m_pMap5->insert(std::pair<TNameIdentifier, pT5>(name, obj)); }
      void addT5Object(const TNameIdentifier&, boost::shared_ptr<void>, ...) {}
      void addT6Object(const TNameIdentifier& name, pT6 obj, int) { m_pMap6->insert(std::pair<TNameIdentifier, pT6>(name, obj)); }
      void addT6Object(const TNameIdentifier&, boost::shared_ptr<void>, ...) {}
      void addT7Object(const TNameIdentifier& name, pT7 obj, int) { m_pMap7->insert(std::pair<TNameIdentifier, pT7>(name, obj)); }
      void addT7Object(const TNameIdentifier&, boost::shared_ptr<void>, ...) {}
      void addT8Object(const TNameIdentifier& name, pT8 obj, int) { m_pMap8->insert(std::pair<TNameIdentifier, pT8>(name, obj)); }
      void addT8Object(const TNameIdentifier&, boost::shared_ptr<void>, ...) {}
      void addT9Object(const TNameIdentifier& name, pT9 obj, int) { m_pMap9->insert(std::pair<TNameIdentifier, pT9>(name, obj)); }
      void addT9Object(const TNameIdentifier&, boost::shared_ptr<void>, ...) {}
      void addT10Object(const TNameIdentifier& name, pT10 obj, int) { m_pMap10->insert(std::pair<TNameIdentifier, pT10>(name, obj)); }
      void addT10Object(const TNameIdentifier&, boost::shared_ptr<void>, ...) {}
      void addT11Object(const TNameIdentifier& name, pT11 obj, int) { m_pMap11->insert(std::pair<TNameIdentifier, pT11>(name, obj)); }
      void addT11Object(const TNameIdentifier&, boost::shared_ptr<void>, ...) {}
      void addT12Object(const TNameIdentifier& name, pT12 obj, int) { m_pMap12->insert(std::pair<TNameIdentifier, pT12>(name, obj)); }
      void addT12Object(const TNameIdentifier&, boost::shared_ptr<void>, ...) {}
      void addT13Object(const TNameIdentifier& name, pT13 obj, int) { m_pMap13->insert(std::pair<TNameIdentifier, pT13>(name, obj)); }
      void addT13Object(const TNameIdentifier&, boost::shared_ptr<void>, ...) {}
      void addT14Object(const TNameIdentifier& name, pT14 obj, int) { m_pMap14->insert(std::pair<TNameIdentifier, pT14>(name, obj)); }
      void addT14Object(const TNameIdentifier&, boost::shared_ptr<void>, ...) {}
};


namespace {

// Is friend class of Manager_sharedPtr<TTList, TTypeIdentifier, TNameIdentifier>. Provides access to private members.
template <class TTList, typename TTypeIdentifier, typename TNameIdentifier>
class Accessor {
   private:
      typedef Manager_sharedPtr<TTList, TTypeIdentifier, TNameIdentifier> manager_t;

   public:
      Accessor(manager_t& inst)
         : m_inst(inst) {}

      boost::shared_ptr<typename manager_t::map0_t>& getMap0() { return m_inst.m_pMap0; }
      boost::shared_ptr<typename manager_t::map1_t>& getMap1() { return m_inst.m_pMap1; }
      boost::shared_ptr<typename manager_t::map2_t>& getMap2() { return m_inst.m_pMap2; }
      boost::shared_ptr<typename manager_t::map3_t>& getMap3() { return m_inst.m_pMap3; }
      boost::shared_ptr<typename manager_t::map4_t>& getMap4() { return m_inst.m_pMap4; }
      boost::shared_ptr<typename manager_t::map5_t>& getMap5() { return m_inst.m_pMap5; }
      boost::shared_ptr<typename manager_t::map6_t>& getMap6() { return m_inst.m_pMap6; }
      boost::shared_ptr<typename manager_t::map7_t>& getMap7() { return m_inst.m_pMap7; }
      boost::shared_ptr<typename manager_t::map8_t>& getMap8() { return m_inst.m_pMap8; }
      boost::shared_ptr<typename manager_t::map9_t>& getMap9() { return m_inst.m_pMap9; }
      boost::shared_ptr<typename manager_t::map10_t>& getMap10() { return m_inst.m_pMap10; }
      boost::shared_ptr<typename manager_t::map11_t>& getMap11() { return m_inst.m_pMap11; }
      boost::shared_ptr<typename manager_t::map12_t>& getMap12() { return m_inst.m_pMap12; }
      boost::shared_ptr<typename manager_t::map13_t>& getMap13() { return m_inst.m_pMap13; }
      boost::shared_ptr<typename manager_t::map14_t>& getMap14() { return m_inst.m_pMap14; }

      typedef typename manager_t::pT0 pT0;
      typedef typename manager_t::pT1 pT1;
      typedef typename manager_t::pT2 pT2;
      typedef typename manager_t::pT3 pT3;
      typedef typename manager_t::pT4 pT4;
      typedef typename manager_t::pT5 pT5;
      typedef typename manager_t::pT6 pT6;
      typedef typename manager_t::pT7 pT7;
      typedef typename manager_t::pT8 pT8;
      typedef typename manager_t::pT9 pT9;
      typedef typename manager_t::pT10 pT10;
      typedef typename manager_t::pT11 pT11;
      typedef typename manager_t::pT12 pT12;
      typedef typename manager_t::pT13 pT13;
      typedef typename manager_t::pT14 pT14;

   private:
      manager_t& m_inst;
};

}


#endif
