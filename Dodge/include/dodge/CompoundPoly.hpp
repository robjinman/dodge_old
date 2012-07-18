/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2012
 */

#ifndef __COMPOUND_POLY_HPP__
#define __COMPOUND_POLY_HPP__


#include <vector>
#include "rapidxml/rapidxml.hpp"
#include "Poly.hpp"
#include "Exception.hpp"


namespace Dodge {


class CompoundPoly {
   public:
      CompoundPoly();
      CompoundPoly(const CompoundPoly& poly);

      inline const Poly& getPoly(int idx) const;
      inline int getNumPolys() const;
      inline void setPoly(int idx, const Poly& poly);
      inline void addPoly(const Poly& poly);
      inline void removePoly(int idx);
      inline void clear();

      void rotate(double rads, const Vec2f& p = Vec2f(0, 0));
      void scale(const Vec2f& sv);
      Vec2f computeMinimum() const;
      Vec2f computeMaximum() const;

      virtual void assignData(const rapidxml::xml_node<>* node);

#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif

      inline bool operator!=(const CompoundPoly& rhs) const;
      bool operator==(const CompoundPoly& rhs) const;

   private:
      std::vector<Poly> m_polys;
};

//===========================================
// CompoundPoly::addPoly
//===========================================
inline void CompoundPoly::addPoly(const Poly& poly) {
   m_polys.push_back(poly);
}

//===========================================
// CompoundPoly::setPoly
//===========================================
inline void CompoundPoly::setPoly(int idx, const Poly& poly) {
   if (idx > static_cast<int>(m_polys.size()) - 1 || idx < 0)
      throw Exception("Could not retrieve polygon; index out of range", __FILE__, __LINE__);

   m_polys[idx] = poly;
}

//===========================================
// CompoundPoly::removePoly
//===========================================
inline void CompoundPoly::removePoly(int idx) {
   if (idx > static_cast<int>(m_polys.size()) - 1 || idx < 0)
      throw Exception("Could not retrieve polygon; index out of range", __FILE__, __LINE__);

   m_polys.erase(m_polys.begin() + idx);
}

//===========================================
// CompoundPoly::clear
//===========================================
inline void CompoundPoly::clear() {
   m_polys.clear();
}

//===========================================
// CompoundPoly::getNumPolys
//===========================================
inline int CompoundPoly::getNumPolys() const {
   return m_polys.size();
}

//===========================================
// CompoundPoly::getPoly
//===========================================
inline const Poly& CompoundPoly::getPoly(int idx) const {
   if (idx > static_cast<int>(m_polys.size()) - 1 || idx < 0)
      throw Exception("Could not retrieve polygon; index out of range", __FILE__, __LINE__);

   return m_polys[idx];
}

//===========================================
// CompoundPoly::operator!=
//===========================================
inline bool CompoundPoly::operator!=(const CompoundPoly& rhs) const {
   return !(*this == rhs);
}


}


#endif
