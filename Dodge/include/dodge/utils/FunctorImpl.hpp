#ifndef __FUNCTOR_IMPL_HPP__
#define __FUNCTOR_IMPL_HPP__


#include "TypeList.hpp"


template <typename R, class TList> class FunctorImpl;

template <typename R>
class FunctorImpl<R, NullType> {
   public:
      virtual R operator()() const = 0;
      virtual FunctorImpl* clone() const = 0;
      virtual bool operator==(const FunctorImpl&) const = 0;
      virtual ~FunctorImpl() {}
};

template <typename R, typename P1>
class FunctorImpl<R, TYPELIST_1(P1)> {
   public:
      virtual R operator()(P1) const = 0;
      virtual FunctorImpl* clone() const = 0;
      virtual bool operator==(const FunctorImpl&) const = 0;
      virtual ~FunctorImpl() {}
};

template <typename R, typename P1, typename P2>
class FunctorImpl<R, TYPELIST_2(P1, P2)> {
   public:
      virtual R operator()(P1, P2) const = 0;
      virtual FunctorImpl* clone() const = 0;
      virtual bool operator==(const FunctorImpl&) const = 0;
      virtual ~FunctorImpl() {}
};

template <typename R, typename P1, typename P2, typename P3>
class FunctorImpl<R, TYPELIST_3(P1, P2, P3)> {
   public:
      virtual R operator()(P1, P2, P3) const = 0;
      virtual FunctorImpl* clone() const = 0;
      virtual bool operator==(const FunctorImpl&) const = 0;
      virtual ~FunctorImpl() {}
};

template <typename R, typename P1, typename P2, typename P3, typename P4>
class FunctorImpl<R, TYPELIST_4(P1, P2, P3, P4)> {
   public:
      virtual R operator()(P1, P2, P3, P4) const = 0;
      virtual FunctorImpl* clone() const = 0;
      virtual bool operator==(const FunctorImpl&) const = 0;
      virtual ~FunctorImpl() {}
};

template <
   typename R, typename P1, typename P2, typename P3, typename P4, typename P5
>
class FunctorImpl<R, TYPELIST_5(P1, P2, P3, P4, P5)> {
   public:
      virtual R operator()(P1, P2, P3, P4, P5) const = 0;
      virtual FunctorImpl* clone() const = 0;
      virtual bool operator==(const FunctorImpl&) const = 0;
      virtual ~FunctorImpl() {}
};

template <
   typename R, typename P1, typename P2, typename P3, typename P4, typename P5,
   typename P6
>
class FunctorImpl<R, TYPELIST_6(P1, P2, P3, P4, P5, P6)> {
   public:
      virtual R operator()(P1, P2, P3, P4, P5, P6) const = 0;
      virtual FunctorImpl* clone() const = 0;
      virtual bool operator==(const FunctorImpl&) const = 0;
      virtual ~FunctorImpl() {}
};

template <
   typename R, typename P1, typename P2, typename P3, typename P4, typename P5,
   typename P6, typename P7
>
class FunctorImpl<R, TYPELIST_7(P1, P2, P3, P4, P5, P6, P7)> {
   public:
      virtual R operator()(P1, P2, P3, P4, P5, P6, P7) const = 0;
      virtual FunctorImpl* clone() const = 0;
      virtual bool operator==(const FunctorImpl&) const = 0;
      virtual ~FunctorImpl() {}
};

template <
   typename R, typename P1, typename P2, typename P3, typename P4, typename P5,
   typename P6, typename P7, typename P8
>
class FunctorImpl<R, TYPELIST_8(P1, P2, P3, P4, P5, P6, P7, P8)> {
   public:
      virtual R operator()(P1, P2, P3, P4, P5, P6, P7, P8) const = 0;
      virtual FunctorImpl* clone() const = 0;
      virtual bool operator==(const FunctorImpl&) const = 0;
      virtual ~FunctorImpl() {}
};

template <
   typename R, typename P1, typename P2, typename P3, typename P4, typename P5,
   typename P6, typename P7, typename P8, typename P9
>
class FunctorImpl<R, TYPELIST_9(P1, P2, P3, P4, P5, P6, P7, P8, P9)> {
   public:
      virtual R operator()(P1, P2, P3, P4, P5, P6, P7, P8, P9) const = 0;
      virtual FunctorImpl* clone() const = 0;
      virtual bool operator==(const FunctorImpl&) const = 0;
      virtual ~FunctorImpl() {}
};

template <
   typename R, typename P1, typename P2, typename P3, typename P4, typename P5,
   typename P6, typename P7, typename P8, typename P9, typename P10
>
class FunctorImpl<R, TYPELIST_10(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10)> {
   public:
      virtual R operator()(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10) const = 0;
      virtual FunctorImpl* clone() const = 0;
      virtual bool operator==(const FunctorImpl&) const = 0;
      virtual ~FunctorImpl() {}
};

template <
   typename R, typename P1, typename P2, typename P3, typename P4, typename P5,
   typename P6, typename P7, typename P8, typename P9, typename P10, typename P11
>
class FunctorImpl<R, TYPELIST_11(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11)> {
   public:
      virtual R operator()(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11) const = 0;
      virtual FunctorImpl* clone() const = 0;
      virtual bool operator==(const FunctorImpl&) const = 0;
      virtual ~FunctorImpl() {}
};

template <
   typename R, typename P1, typename P2, typename P3, typename P4, typename P5,
   typename P6, typename P7, typename P8, typename P9, typename P10, typename P11,
   typename P12
>
class FunctorImpl<R, TYPELIST_12(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12)> {
   public:
      virtual R operator()(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12) const = 0;
      virtual FunctorImpl* clone() const = 0;
      virtual bool operator==(const FunctorImpl&) const = 0;
      virtual ~FunctorImpl() {}
};

template <
   typename R, typename P1, typename P2, typename P3, typename P4, typename P5,
   typename P6, typename P7, typename P8, typename P9, typename P10, typename P11,
   typename P12, typename P13
>
class FunctorImpl<R, TYPELIST_13(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13)> {
   public:
      virtual R operator()(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13) const = 0;
      virtual FunctorImpl* clone() const = 0;
      virtual bool operator==(const FunctorImpl&) const = 0;
      virtual ~FunctorImpl() {}
};

template <
   typename R, typename P1, typename P2, typename P3, typename P4, typename P5,
   typename P6, typename P7, typename P8, typename P9, typename P10, typename P11,
   typename P12, typename P13, typename P14
>
class FunctorImpl<R, TYPELIST_14(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13, P14)> {
   public:
      virtual R operator()(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13, P14) const = 0;
      virtual FunctorImpl* clone() const = 0;
      virtual bool operator==(const FunctorImpl&) const = 0;
      virtual ~FunctorImpl() {}
};

template <
   typename R, typename P1, typename P2, typename P3, typename P4, typename P5,
   typename P6, typename P7, typename P8, typename P9, typename P10, typename P11,
   typename P12, typename P13, typename P14, typename P15
>
class FunctorImpl<R, TYPELIST_15(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13, P14, P15)> {
   public:
      virtual R operator()(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13, P14, P15) const = 0;
      virtual FunctorImpl* clone() const = 0;
      virtual bool operator==(const FunctorImpl&) const = 0;
      virtual ~FunctorImpl() {}
};


#endif
