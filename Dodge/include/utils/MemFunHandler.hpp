#ifndef __MEM_FUN_HANDLER_HPP__
#define __MEM_FUN_HANDLER_HPP__


#include "FunctorImpl.hpp"


template <class ParentFunctor, typename PointerToObj, typename PointerToMemFn>
class MemFunHandler
   : public FunctorImpl<typename ParentFunctor::ResultType, typename ParentFunctor::ParmList> {

   public:
      typedef typename ParentFunctor::ResultType ResultType;

      MemFunHandler(const PointerToObj& pObj, PointerToMemFn pMemFn)
         : m_pObj(pObj), m_pMemFn(pMemFn) {}

      MemFunHandler* clone() const { return new MemFunHandler(*this); }

      bool operator==(const FunctorImpl<typename ParentFunctor::ResultType, typename ParentFunctor::ParmList>& func) const {
         try {
            const MemFunHandler& ref = dynamic_cast<const MemFunHandler&>(func);
            return (m_pObj == ref.m_pObj) && (m_pMemFn == ref.m_pMemFn);
         }
         catch (std::bad_cast&) { return false; }
      }

      ResultType operator()() const {
         return ((*m_pObj).*m_pMemFn)();
      }
      ResultType operator()(
         typename ParentFunctor::Parm1 p1
      ) const {
         return ((*m_pObj).*m_pMemFn)(p1);
      }
      ResultType operator()(
         typename ParentFunctor::Parm1 p1, typename ParentFunctor::Parm2 p2
      ) const {
         return ((*m_pObj).*m_pMemFn)(p1, p2);
      }
      ResultType operator()(
         typename ParentFunctor::Parm1 p1, typename ParentFunctor::Parm2 p2,
         typename ParentFunctor::Parm3 p3
      ) const {
         return ((*m_pObj).*m_pMemFn)(p1, p2, p3);
      }
      ResultType operator()(
         typename ParentFunctor::Parm1 p1, typename ParentFunctor::Parm2 p2,
         typename ParentFunctor::Parm3 p3, typename ParentFunctor::Parm4 p4
      ) const {
         return ((*m_pObj).*m_pMemFn)(p1, p2, p3, p4);
      }
      ResultType operator()(
         typename ParentFunctor::Parm1 p1, typename ParentFunctor::Parm2 p2,
         typename ParentFunctor::Parm3 p3, typename ParentFunctor::Parm4 p4,
         typename ParentFunctor::Parm5 p5
      ) const {
         return ((*m_pObj).*m_pMemFn)(p1, p2, p3, p4, p5);
      }
      ResultType operator()(
         typename ParentFunctor::Parm1 p1, typename ParentFunctor::Parm2 p2,
         typename ParentFunctor::Parm3 p3, typename ParentFunctor::Parm4 p4,
         typename ParentFunctor::Parm5 p5, typename ParentFunctor::Parm6 p6
      ) const {
         return ((*m_pObj).*m_pMemFn)(p1, p2, p3, p4, p5, p6);
      }
      ResultType operator()(
         typename ParentFunctor::Parm1 p1, typename ParentFunctor::Parm2 p2,
         typename ParentFunctor::Parm3 p3, typename ParentFunctor::Parm4 p4,
         typename ParentFunctor::Parm5 p5, typename ParentFunctor::Parm6 p6,
         typename ParentFunctor::Parm7 p7
      ) const {
         return ((*m_pObj).*m_pMemFn)(p1, p2, p3, p4, p5, p6, p7);
      }
      ResultType operator()(
         typename ParentFunctor::Parm1 p1, typename ParentFunctor::Parm2 p2,
         typename ParentFunctor::Parm3 p3, typename ParentFunctor::Parm4 p4,
         typename ParentFunctor::Parm5 p5, typename ParentFunctor::Parm6 p6,
         typename ParentFunctor::Parm7 p7, typename ParentFunctor::Parm8 p8
      ) const {
         return ((*m_pObj).*m_pMemFn)(p1, p2, p3, p4, p5, p6, p7, p8);
      }
      ResultType operator()(
         typename ParentFunctor::Parm1 p1, typename ParentFunctor::Parm2 p2,
         typename ParentFunctor::Parm3 p3, typename ParentFunctor::Parm4 p4,
         typename ParentFunctor::Parm5 p5, typename ParentFunctor::Parm6 p6,
         typename ParentFunctor::Parm7 p7, typename ParentFunctor::Parm8 p8,
         typename ParentFunctor::Parm9 p9
      ) const {
         return ((*m_pObj).*m_pMemFn)(p1, p2, p3, p4, p5, p6, p7, p8, p9);
      }
      ResultType operator()(
         typename ParentFunctor::Parm1 p1, typename ParentFunctor::Parm2 p2,
         typename ParentFunctor::Parm3 p3, typename ParentFunctor::Parm4 p4,
         typename ParentFunctor::Parm5 p5, typename ParentFunctor::Parm6 p6,
         typename ParentFunctor::Parm7 p7, typename ParentFunctor::Parm8 p8,
         typename ParentFunctor::Parm9 p9, typename ParentFunctor::Parm10 p10
      ) const {
         return ((*m_pObj).*m_pMemFn)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
      }
      ResultType operator()(
         typename ParentFunctor::Parm1 p1, typename ParentFunctor::Parm2 p2,
         typename ParentFunctor::Parm3 p3, typename ParentFunctor::Parm4 p4,
         typename ParentFunctor::Parm5 p5, typename ParentFunctor::Parm6 p6,
         typename ParentFunctor::Parm7 p7, typename ParentFunctor::Parm8 p8,
         typename ParentFunctor::Parm9 p9, typename ParentFunctor::Parm10 p10,
         typename ParentFunctor::Parm11 p11
      ) const {
         return ((*m_pObj).*m_pMemFn)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
      }
      ResultType operator()(
         typename ParentFunctor::Parm1 p1, typename ParentFunctor::Parm2 p2,
         typename ParentFunctor::Parm3 p3, typename ParentFunctor::Parm4 p4,
         typename ParentFunctor::Parm5 p5, typename ParentFunctor::Parm6 p6,
         typename ParentFunctor::Parm7 p7, typename ParentFunctor::Parm8 p8,
         typename ParentFunctor::Parm9 p9, typename ParentFunctor::Parm10 p10,
         typename ParentFunctor::Parm11 p11, typename ParentFunctor::Parm12 p12
      ) const {
         return ((*m_pObj).*m_pMemFn)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
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
         return ((*m_pObj).*m_pMemFn)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13);
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
         return ((*m_pObj).*m_pMemFn)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14);
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
         return ((*m_pObj).*m_pMemFn)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15);
      }

   private:
      PointerToObj m_pObj;
      PointerToMemFn m_pMemFn;
};


#endif
