#ifndef __FUNCTOR_HPP__
#define __FUNCTOR_HPP__


#include <memory>
#include "utils/TypeList.hpp"
#include "utils/FunctorHandler.hpp"
#include "utils/MemFunHandler.hpp"
#include "utils/FunctorImpl.hpp"


template <typename R, class TList>
class Functor {
   public:
      typedef TList ParmList;
      typedef R ResultType;
      typedef typename TL::TypeAtNonStrict<TList, 0, EmptyType>::Result Parm1;
      typedef typename TL::TypeAtNonStrict<TList, 1, EmptyType>::Result Parm2;
      typedef typename TL::TypeAtNonStrict<TList, 2, EmptyType>::Result Parm3;
      typedef typename TL::TypeAtNonStrict<TList, 3, EmptyType>::Result Parm4;
      typedef typename TL::TypeAtNonStrict<TList, 4, EmptyType>::Result Parm5;
      typedef typename TL::TypeAtNonStrict<TList, 5, EmptyType>::Result Parm6;
      typedef typename TL::TypeAtNonStrict<TList, 6, EmptyType>::Result Parm7;
      typedef typename TL::TypeAtNonStrict<TList, 7, EmptyType>::Result Parm8;
      typedef typename TL::TypeAtNonStrict<TList, 8, EmptyType>::Result Parm9;
      typedef typename TL::TypeAtNonStrict<TList, 9, EmptyType>::Result Parm10;
      typedef typename TL::TypeAtNonStrict<TList, 10, EmptyType>::Result Parm11;
      typedef typename TL::TypeAtNonStrict<TList, 11, EmptyType>::Result Parm12;
      typedef typename TL::TypeAtNonStrict<TList, 12, EmptyType>::Result Parm13;
      typedef typename TL::TypeAtNonStrict<TList, 13, EmptyType>::Result Parm14;
      typedef typename TL::TypeAtNonStrict<TList, 14, EmptyType>::Result Parm15;

   private:
      typedef FunctorImpl<R, TList> Impl;
      std::auto_ptr<Impl> m_impl;

   public:
      Functor(const Functor& f) : m_impl(f.m_impl->clone()) {}
      Functor& operator=(const Functor& f) { m_impl = std::auto_ptr<Impl>(f.m_impl->clone()); return *this; }
      explicit Functor(std::auto_ptr<Impl> impl) : m_impl(impl) {}

      template <class Fun>
      Functor(Fun fun);

      template <class PtrObj, typename MemFn>
      Functor(const PtrObj& p, MemFn memFn)
         : m_impl(new MemFunHandler<Functor, PtrObj, MemFn>(p, memFn)) {}

      bool operator==(const Functor& f) const { return *m_impl == *f.m_impl; }

      R operator()() const {
         return (*m_impl)();
      }
      R operator()(Parm1 p1) const {
         return (*m_impl)(p1);
      }
      R operator()(Parm1 p1, Parm2 p2) const {
         return (*m_impl)(p1, p2);
      }
      R operator()(Parm1 p1, Parm2 p2, Parm3 p3) const {
         return (*m_impl)(p1, p2, p3);
      }
      R operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4) const {
         return (*m_impl)(p1, p2, p3, p4);
      }
      R operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5) const {
         return (*m_impl)(p1, p2, p3, p4, p5);
      }
      R operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5, Parm6 p6) const {
         return (*m_impl)(p1, p2, p3, p4, p5, p6);
      }
      R operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5, Parm6 p6, Parm7 p7) const {
         return (*m_impl)(p1, p2, p3, p4, p5, p6, p7);
      }
      R operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5, Parm6 p6, Parm7 p7, Parm8 p8) const {
         return (*m_impl)(p1, p2, p3, p4, p5, p6, p7, p8);
      }
      R operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5, Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9) const {
         return (*m_impl)(p1, p2, p3, p4, p5, p6, p7, p8, p9);
      }
      R operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5, Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9,
         Parm10 p10) const {
            return (*m_impl)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
      }
      R operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5, Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9,
         Parm10 p10, Parm11 p11) const {
            return (*m_impl)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
      }
      R operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5, Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9,
         Parm10 p10, Parm11 p11, Parm12 p12) const {
            return (*m_impl)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
      }
      R operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5, Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9,
         Parm10 p10, Parm11 p11, Parm12 p12, Parm13 p13) const {
            return (*m_impl)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13);
      }
      R operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5, Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9,
         Parm10 p10, Parm11 p11, Parm12 p12, Parm13 p13, Parm14 p14) const {
            return (*m_impl)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14);
      }
      R operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5, Parm6 p6, Parm7 p7, Parm8 p8, Parm9 p9,
         Parm10 p10, Parm11 p11, Parm12 p12, Parm13 p13, Parm14 p14, Parm15 p15) const {
            return (*m_impl)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15);
      }
};

template <typename R, class TList>
template <typename Fun>
Functor<R, TList>::Functor(Fun fun)
   : m_impl(new FunctorHandler<Functor, Fun>(fun)) {}


#endif
