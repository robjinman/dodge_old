#ifndef __FUNCTOR_HANDLER_HPP__
#define __FUNCTOR_HANDLER_HPP__


#include "FunctorImpl.hpp"


template <class ParentFunctor, typename Fun>
class FunctorHandler
   : public FunctorImpl<typename ParentFunctor::ResultType, typename ParentFunctor::ParmList> {

   public:
      typedef typename ParentFunctor::ResultType ResultType;

      FunctorHandler(Fun fun) : m_fun(fun) {}
      virtual FunctorHandler* clone() const { return new FunctorHandler(*this); }

      bool operator==(const FunctorImpl<typename ParentFunctor::ResultType, typename ParentFunctor::ParmList>& func) const {
         try {
            const FunctorHandler* ptr = dynamic_cast<const FunctorHandler*>(&func);
            return ptr && (m_fun == ptr->m_fun);
         }
         catch (std::bad_cast&) { return false; }
      }

      ResultType operator()() const {
         return m_fun();
      }
      ResultType operator()(
         typename ParentFunctor::Parm1 p1
      ) const {
         return m_fun(p1);
      }
      ResultType operator()(
         typename ParentFunctor::Parm1 p1, typename ParentFunctor::Parm2 p2
      ) const {
         return m_fun(p1, p2);
      }
      ResultType operator()(
         typename ParentFunctor::Parm1 p1, typename ParentFunctor::Parm2 p2,
         typename ParentFunctor::Parm3 p3
      ) const {
         return m_fun(p1, p2, p3);
      }
      ResultType operator()(
         typename ParentFunctor::Parm1 p1, typename ParentFunctor::Parm2 p2,
         typename ParentFunctor::Parm1 p3, typename ParentFunctor::Parm4 p4
      ) const {
         return m_fun(p1, p2, p3, p4);
      }
      ResultType operator()(
         typename ParentFunctor::Parm1 p1, typename ParentFunctor::Parm2 p2,
         typename ParentFunctor::Parm3 p3, typename ParentFunctor::Parm4 p4,
         typename ParentFunctor::Parm5 p5
      ) const {
         return m_fun(p1, p2, p3, p4, p5);
      }
      ResultType operator()(
         typename ParentFunctor::Parm1 p1, typename ParentFunctor::Parm2 p2,
         typename ParentFunctor::Parm3 p3, typename ParentFunctor::Parm4 p4,
         typename ParentFunctor::Parm5 p5, typename ParentFunctor::Parm6 p6
      ) const {
         return m_fun(p1, p2, p3, p4, p5, p6);
      }
      ResultType operator()(
         typename ParentFunctor::Parm1 p1, typename ParentFunctor::Parm2 p2,
         typename ParentFunctor::Parm3 p3, typename ParentFunctor::Parm4 p4,
         typename ParentFunctor::Parm5 p5, typename ParentFunctor::Parm6 p6,
         typename ParentFunctor::Parm7 p7
      ) const {
         return m_fun(p1, p2, p3, p4, p5, p6, p7);
      }
      ResultType operator()(
         typename ParentFunctor::Parm1 p1, typename ParentFunctor::Parm2 p2,
         typename ParentFunctor::Parm3 p3, typename ParentFunctor::Parm4 p4,
         typename ParentFunctor::Parm5 p5, typename ParentFunctor::Parm6 p6,
         typename ParentFunctor::Parm7 p7, typename ParentFunctor::Parm8 p8
      ) const {
         return m_fun(p1, p2, p3, p4, p5, p6, p7, p8);
      }
      ResultType operator()(
         typename ParentFunctor::Parm1 p1, typename ParentFunctor::Parm2 p2,
         typename ParentFunctor::Parm3 p3, typename ParentFunctor::Parm4 p4,
         typename ParentFunctor::Parm5 p5, typename ParentFunctor::Parm6 p6,
         typename ParentFunctor::Parm7 p7, typename ParentFunctor::Parm8 p8,
         typename ParentFunctor::Parm9 p9
      ) const {
         return m_fun(p1, p2, p3, p4, p5, p6, p7, p8, p9);
      }
      ResultType operator()(
         typename ParentFunctor::Parm1 p1, typename ParentFunctor::Parm2 p2,
         typename ParentFunctor::Parm3 p3, typename ParentFunctor::Parm4 p4,
         typename ParentFunctor::Parm5 p5, typename ParentFunctor::Parm6 p6,
         typename ParentFunctor::Parm7 p7, typename ParentFunctor::Parm8 p8,
         typename ParentFunctor::Parm9 p9, typename ParentFunctor::Parm10 p10
      ) const {
         return m_fun(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
      }
      ResultType operator()(
         typename ParentFunctor::Parm1 p1, typename ParentFunctor::Parm2 p2,
         typename ParentFunctor::Parm3 p3, typename ParentFunctor::Parm4 p4,
         typename ParentFunctor::Parm5 p5, typename ParentFunctor::Parm6 p6,
         typename ParentFunctor::Parm7 p7, typename ParentFunctor::Parm8 p8,
         typename ParentFunctor::Parm9 p9, typename ParentFunctor::Parm10 p10,
         typename ParentFunctor::Parm11 p11
      ) const {
         return m_fun(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
      }
      ResultType operator()(
         typename ParentFunctor::Parm1 p1, typename ParentFunctor::Parm2 p2,
         typename ParentFunctor::Parm3 p3, typename ParentFunctor::Parm4 p4,
         typename ParentFunctor::Parm5 p5, typename ParentFunctor::Parm6 p6,
         typename ParentFunctor::Parm7 p7, typename ParentFunctor::Parm8 p8,
         typename ParentFunctor::Parm9 p9, typename ParentFunctor::Parm10 p10,
         typename ParentFunctor::Parm11 p11, typename ParentFunctor::Parm12 p12
      ) const {
         return m_fun(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
      }
      ResultType operator()(
         typename ParentFunctor::Parm1 p1, typename ParentFunctor::Parm2 p2,
         typename ParentFunctor::Parm3 p3, typename ParentFunctor::Parm4 p4,
         typename ParentFunctor::Parm5 p5, typename ParentFunctor::Parm6 p6,
         typename ParentFunctor::Parm7 p7, typename ParentFunctor::Parm8 p8,
         typename ParentFunctor::Parm9 p9, typename ParentFunctor::Parm10 p10,
         typename ParentFunctor::Parm11 p11, typename ParentFunctor::Parm12 p12,
         typename ParentFunctor::Parm13 p13
      ) const {
         return m_fun(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13);
      }
      ResultType operator()(
         typename ParentFunctor::Parm1 p1, typename ParentFunctor::Parm2 p2,
         typename ParentFunctor::Parm3 p3, typename ParentFunctor::Parm4 p4,
         typename ParentFunctor::Parm5 p5, typename ParentFunctor::Parm6 p6,
         typename ParentFunctor::Parm7 p7, typename ParentFunctor::Parm8 p8,
         typename ParentFunctor::Parm9 p9, typename ParentFunctor::Parm10 p10,
         typename ParentFunctor::Parm11 p11, typename ParentFunctor::Parm12 p12,
         typename ParentFunctor::Parm13 p13, typename ParentFunctor::Parm14 p14
      ) const {
         return m_fun(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14);
      }
      ResultType operator()(
         typename ParentFunctor::Parm1 p1, typename ParentFunctor::Parm2 p2,
         typename ParentFunctor::Parm3 p3, typename ParentFunctor::Parm4 p4,
         typename ParentFunctor::Parm5 p5, typename ParentFunctor::Parm6 p6,
         typename ParentFunctor::Parm7 p7, typename ParentFunctor::Parm8 p8,
         typename ParentFunctor::Parm9 p9, typename ParentFunctor::Parm10 p10,
         typename ParentFunctor::Parm11 p11, typename ParentFunctor::Parm12 p12,
         typename ParentFunctor::Parm13 p13, typename ParentFunctor::Parm14 p14,
         typename ParentFunctor::Parm15 p15
      ) const {
         return m_fun(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15);
      }

   private:
      Fun m_fun;
};


#endif
