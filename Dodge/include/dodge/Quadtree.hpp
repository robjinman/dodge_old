/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __QUAD_TREE_HPP__
#define __QUAD_TREE_HPP__


#include <vector>
#include "Range.hpp"
#include "definitions.hpp"
#ifdef DEBUG
   #include "Graphics2d.hpp"
#endif


namespace Dodge {


template <typename T> class Quadtree {
   public:
      class Entry {
         public:
            Entry(T item_, const Range& rect_)
               : item(item_), rect(rect_) {}

            T item;
            Range rect;
      };

      //===========================================
      // Quadtree::Quadtree
      //===========================================
      Quadtree(uint_t splittingThres, const Range& boundary)
         : m_splittingThres(splittingThres), m_boundary(boundary),
           m_SW(NULL), m_SE(NULL), m_NE(NULL), m_NW(NULL) {

         m_entries.reserve(splittingThres);
      }

      //===========================================
      // Quadtree::insert
      //===========================================
      bool insert(T item, const Range& boundingBox) {
         if (!m_boundary.overlaps(boundingBox))
            return false;

         if (hasChildren()) {
            m_SW->insert(item, boundingBox);
            m_SE->insert(item, boundingBox);
            m_NE->insert(item, boundingBox);
            m_NW->insert(item, boundingBox);
         }
         else {   // If this is a leaf
            m_entries.push_back(Entry(item, boundingBox));

            if (m_entries.size() > m_splittingThres)
               subdivide();
         }

         return true;
      }

      //===========================================
      // Quadtree::getEntries
      //===========================================
      void getEntries(const Range& region, std::vector<Entry>& entries) const {
         entries.clear();
         getEntries_r(region, entries);
      }

      //===========================================
      // Quadtree::getBoundary
      //===========================================
      const Range& getBoundary() const {
         return m_boundary;
      }

      //===========================================
      // Quadtree::operator new
      //===========================================
//      static void* operator new(size_t size) {
         // TODO
//      }

      //===========================================
      // Quadtree::operator delete
      //===========================================
//      static void operator delete(void* obj, size_t size) {
         // TODO
//      }

#ifdef DEBUG
      void dbg_draw(int z, const Colour& col) const {
         if (hasChildren()) {
            const Vec2f& pos = m_boundary.getPosition();
            const Vec2f& sz = m_boundary.getSize();
            Vec2f halfSz = sz / 2.f;

            // Horizontal
            Vec2f l1p1(pos.x, pos.y + halfSz.y);
            Vec2f l1p2(pos.x + sz.x, pos.y + halfSz.y);
            LineSegment line1(l1p1, l1p2);

            // Vertical
            Vec2f l2p1(pos.x + halfSz.x, pos.y);
            Vec2f l2p2(pos.x + halfSz.x, pos.y + sz.y);
            LineSegment line2(l2p1, l2p2);

            m_graphics2d.setLineWidth(1);
            m_graphics2d.setLineColour(col);
            m_graphics2d.drawPrimitive(line1, 0.f, 0.f, z);
            m_graphics2d.drawPrimitive(line2, 0.f, 0.f, z);

            m_SW->dbg_draw(z, col);
            m_SE->dbg_draw(z, col);
            m_NE->dbg_draw(z, col);
            m_NW->dbg_draw(z, col);
         }
      }
#endif

   private:
      unsigned int m_splittingThres;
      Range m_boundary;
      std::vector<Entry> m_entries;
      Quadtree<T>* m_SW;
      Quadtree<T>* m_SE;
      Quadtree<T>* m_NE;
      Quadtree<T>* m_NW;

//      PoolAllocator m_memPool;
#ifdef DEBUG
      Graphics2d m_graphics2d;
#endif

      //===========================================
      // Quadtree::hasChildren
      //
      // Assume no children if m_SW is not set
      //===========================================
      bool hasChildren() const {
         return m_SW != NULL;
      }

      //===========================================
      // Quadtree::getEntries_r
      //===========================================
      void getEntries_r(const Range& region, std::vector<Entry>& entries) const {

         // If this is a leaf
         if (hasChildren() == false) {
            entries.insert(entries.end(), m_entries.begin(), m_entries.end());
            return;
         }

         if (region.overlaps(m_SW->m_boundary)) m_SW->getEntries_r(region, entries);
         if (region.overlaps(m_SE->m_boundary)) m_SE->getEntries_r(region, entries);
         if (region.overlaps(m_NE->m_boundary)) m_NE->getEntries_r(region, entries);
         if (region.overlaps(m_NW->m_boundary)) m_NW->getEntries_r(region, entries);
      }

      //===========================================
      // Quadtree::insert_
      //
      // Separate version of insert() to avoid recursive subdivision
      //===========================================
      void insert_(T item, const Range& boundingBox) {
         if (!m_boundary.overlaps(boundingBox))
            return;

         m_entries.push_back(Entry(item, boundingBox));
      }

      //===========================================
      // Quadtree::subdivide
      //===========================================
      void subdivide() {
         Vec2f halfSz = m_boundary.getSize() / 2.f;
         m_SW = new Quadtree(m_splittingThres, Range(m_boundary.getPosition(), halfSz));
         m_SE = new Quadtree(m_splittingThres, Range(m_boundary.getPosition() + Vec2f(halfSz.x, 0.f), halfSz));
         m_NE = new Quadtree(m_splittingThres, Range(m_boundary.getPosition() + halfSz, halfSz));
         m_NW = new Quadtree(m_splittingThres, Range(m_boundary.getPosition() + Vec2f(0.f, halfSz.y), halfSz));

         // Add all entries into the new leaves
         for (uint_t i = 0; i < m_entries.size(); ++i) {
            m_SW->insert_(m_entries[i].item, m_entries[i].rect);
            m_SE->insert_(m_entries[i].item, m_entries[i].rect);
            m_NE->insert_(m_entries[i].item, m_entries[i].rect);
            m_NW->insert_(m_entries[i].item, m_entries[i].rect);
         }
         m_entries.clear();
      }
};


}


#endif
