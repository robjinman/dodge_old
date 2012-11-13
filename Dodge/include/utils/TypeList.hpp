#ifndef __TYPE_LIST_HPP__
#define __TYPE_LIST_HPP__


#define TYPELIST_0() NullType
#define TYPELIST_1(T1) TypeList<T1, NullType>
#define TYPELIST_2(T1, T2) TypeList<T1, TYPELIST_1(T2) >
#define TYPELIST_3(T1, T2, T3) TypeList<T1, TYPELIST_2(T2, T3) >
#define TYPELIST_4(T1, T2, T3, T4) TypeList<T1, TYPELIST_3(T2, T3, T4) >
#define TYPELIST_5(T1, T2, T3, T4, T5) TypeList<T1, TYPELIST_4(T2, T3, T4, T5) >
#define TYPELIST_6(T1, T2, T3, T4, T5, T6) TypeList<T1, TYPELIST_5(T2, T3, T4, T5, T6) >
#define TYPELIST_7(T1, T2, T3, T4, T5, T6, T7) TypeList<T1, TYPELIST_6(T2, T3, T4, T5, T6, T7) >
#define TYPELIST_8(T1, T2, T3, T4, T5, T6, T7, T8) TypeList<T1, TYPELIST_7(T2, T3, T4, T5, T6, T7, T8) >
#define TYPELIST_9(T1, T2, T3, T4, T5, T6, T7, T8, T9) \
   TypeList<T1, TYPELIST_8(T2, T3, T4, T5, T6, T7, T8, T9) >
#define TYPELIST_10(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10) \
   TypeList<T1, TYPELIST_9(T2, T3, T4, T5, T6, T7, T8, T9, T10) >
#define TYPELIST_11(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11) \
   TypeList<T1, TYPELIST_10(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11) >
#define TYPELIST_12(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12) \
   TypeList<T1, TYPELIST_11(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12) >
#define TYPELIST_13(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13) \
   TypeList<T1, TYPELIST_12(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13) >
#define TYPELIST_14(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14) \
   TypeList<T1, TYPELIST_13(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14) >
#define TYPELIST_15(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15) \
   TypeList<T1, TYPELIST_14(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15) >


class NullType {};
struct EmptyType {};

template <class T, class U>
struct TypeList {
   typedef T head;
   typedef U tail;
};


namespace TL {


//========|Length|============
template <class TList> struct Length;

template <>
struct Length<NullType> {
   enum { value = 0 };
};

template <class T, class U>
struct Length<TypeList<T, U> > {
   enum { value = 1 + Length<U>::value };
};
//============================


//========|TypeAt|============
template<class TList, unsigned int index> struct TypeAt;

template<class Head, class Tail>
struct TypeAt<TypeList<Head, Tail>, 0> {
   typedef Head Result;
};

template <class Head, class Tail, unsigned int i>
struct TypeAt<TypeList<Head, Tail>, i> {
   typedef typename TypeAt<Tail, i - 1>::Result Result;
};
//============================


//========|TypeAtNonStrict|===
template<class TList, unsigned int index, class T> struct TypeAtNonStrict;

template<class T, unsigned int i>
struct TypeAtNonStrict<NullType, i, T> {
   typedef T Result;
};

template<class Head, class Tail, class T>
struct TypeAtNonStrict<TypeList<Head, Tail>, 0, T> {
   typedef Head Result;
};

template <class Head, class Tail, unsigned int i, class T>
struct TypeAtNonStrict<TypeList<Head, Tail>, i, T> {
   typedef typename TypeAtNonStrict<Tail, i - 1, T>::Result Result;
};
//============================


//========|IndexOf|===========
template<class TList, class T> struct IndexOf;

template <class T>
struct IndexOf<NullType, T> {
   enum { value = -1 };
};

template<class T, class Tail>
struct IndexOf<TypeList<T, Tail>, T> {
   enum { value = 0 };
};

template <class Head, class Tail, class T>
struct IndexOf<TypeList<Head, Tail>, T> {
   private:
      enum { tmp = IndexOf<Tail, T>::value };
   public:
      enum { value = tmp == -1 ? -1 : tmp + 1 };
};
//============================


//========|Append|============
template<class TList, class T> struct Append;

template<>
struct Append<NullType, NullType> {
   typedef NullType Result;
};

template<class T>
struct Append<NullType, T> {
   typedef TYPELIST_1(T) Result;
};

template<class Head, class Tail>
struct Append<NullType, TypeList<Head, Tail> > {
   typedef TypeList<Head, Tail> Result;
};

template<class Head, class Tail, class T>
struct Append<TypeList<Head, Tail>, T> {
   private:
      typedef typename Append<Tail, T>::Result tmp;
   public:
      typedef TypeList<Head, tmp> Result;
};
//============================


//========|Erase|=============
template<class TList, class T> struct Erase;

template<class T>
struct Erase<NullType, T> {
   typedef NullType Result;
};

template<class Tail, class T>
struct Erase<TypeList<T, Tail>, T> {
   typedef Tail Result;
};

template<class Head, class Tail, class T>
struct Erase<TypeList<Head, Tail>, T> {
   private:
      typedef typename Erase<Tail, T>::Result tmp;
   public:
      typedef TypeList<Head, tmp> Result;
};
//============================


//========|EraseAll|==========
template<class TList, class T> struct EraseAll;

template<class T>
struct EraseAll<NullType, T> {
   typedef NullType Result;
};

template<class Tail, class T>
struct EraseAll<TypeList<T, Tail>, T> {
   typedef typename EraseAll<Tail, T>::Result Result;
};

template<class Head, class Tail, class T>
struct EraseAll<TypeList<Head, Tail>, T> {
   private:
      typedef typename EraseAll<Tail, T>::Result tmp;
   public:
      typedef TypeList<Head, tmp> Result;
};
//============================


//========|NoDuplicates|======
template<class TList> struct NoDuplicates;

template<>
struct NoDuplicates<NullType> {
   typedef NullType Result;
};

template<class Head, class Tail>
struct NoDuplicates<TypeList<Head, Tail> > {
   private:
      typedef typename NoDuplicates<Tail>::Result tmp1;
      typedef typename Erase<tmp1, Head>::Result tmp2;
   public:
      typedef TypeList<Head, tmp2> Result;
};
//============================


//========|Replace|===========
template<class TList, class T, class U> struct Replace;

template<class T, class U>
struct Replace<NullType, T, U> {
   typedef NullType Result;
};

template<class Tail, class T, class U>
struct Replace<TypeList<T, Tail>, T, U> {
   typedef TypeList<U, Tail> Result;
};

template<class Head, class Tail, class T, class U>
struct Replace<TypeList<Head, Tail>, T, U> {
   private:
      typedef typename Replace<Tail, T, U>::Result tmp;
   public:
      typedef TypeList<Head, tmp> Result;
};
//============================


//========|ReplaceAll|========
template<class TList, class T, class U> struct ReplaceAll;

template<class T, class U>
struct ReplaceAll<NullType, T, U> {
   typedef NullType Result;
};

template<class Tail, class T, class U>
struct ReplaceAll<TypeList<T, Tail>, T, U> {
   private:
      typedef typename ReplaceAll<Tail, T, U>::Result tmp;
   public:
      typedef TypeList<U, tmp> Result;
};

template<class Head, class Tail, class T, class U>
struct ReplaceAll<TypeList<Head, Tail>, T, U> {
   private:
      typedef typename ReplaceAll<Tail, T, U>::Result tmp;
   public:
      typedef TypeList<Head, tmp> Result;
};
//============================


//========|Conversion|========
template <class T, class U>
class Conversion {
   private:
      typedef char small_t;
      class big_t { char dummy[2]; };
      static small_t test(const U&);
      static big_t test(...);
      static T makeT();
   public:
      enum {
         exists = sizeof(test(makeT())) == sizeof(small_t),
         sameType = false
      };
};

template <class T>
class Conversion<T, T> {
   public:
      enum { exists = 1, sameType = 1 };
};
//============================


//========|ChooseTOrNull|=====
template <class T, unsigned int i>
class ChooseTOrNull;

template <class T>
class ChooseTOrNull<T, 1> {
   public:
      typedef T Result;
};

template <class T>
class ChooseTOrNull<T, 0> {
   public:
      typedef NullType Result;
};
//============================


//========|NotConvertibleTo|==
template <class TTList, class T>
class NotConvertibleTo;

template <class T>
class NotConvertibleTo<NullType, T> {
   public:
      typedef NullType Result;
};

template <class Head, class Tail, class T>
class NotConvertibleTo<TypeList<Head, Tail>, T> {
   private:
      enum { x = (Conversion<Head, T>::exists == 1) ? 0 : 1 };
      typedef typename ChooseTOrNull<Head, x>::Result tmp;
   public:
      typedef typename TL::Append<typename NotConvertibleTo<Tail, T>::Result, tmp>::Result Result;
};
//============================


//========|IsConvertibleTo|===
template <class TTList, class T>
class IsConvertibleTo;

template <class T>
class IsConvertibleTo<NullType, T> {
   public:
      typedef NullType Result;
};

template <class Head, class Tail, class T>
class IsConvertibleTo<TypeList<Head, Tail>, T> {
   private:
      enum { x = Conversion<Head, T>::exists };
      typedef typename ChooseTOrNull<Head, x>::Result tmp;
   public:
      typedef typename TL::Append<typename IsConvertibleTo<Tail, T>::Result, tmp>::Result Result;
};
//============================


//========|ApplyToTemplate|===
template <class TTList, template <class> class T>
struct ApplyToTemplate;

template <template <class> class T>
struct ApplyToTemplate<NullType, T> {
   typedef NullType Result;
};

template <class Head, class Tail, template <class> class T>
struct ApplyToTemplate<TypeList<Head, Tail>, T> {
   typedef TypeList<T<Head>, typename ApplyToTemplate<Tail, T>::Result> Result;
};
//============================


}


#endif
