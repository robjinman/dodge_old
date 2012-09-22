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
            Entry(const T& item_, const Range& rect_)
               : item(item_), rect(rect_) {}

            const T& item;
            const Range& rect;
      };

      //===========================================
      // Quadtree::Quadtree
      //===========================================
      Quadtree(uint_t maxCapacity, const Range& boundary)
         : m_maxCapacity(maxCapacity), m_boundary(boundary),
           m_SW(NULL), m_SE(NULL), m_NE(NULL), m_NW(NULL) {

         m_entries.reserve(maxCapacity);
      }

      //===========================================
      // Quadtree::insert
      //===========================================
      bool insert(const T& item, const Range& boundingBox) {
         if (!m_boundary.overlaps(boundingBox))
            return false;

         if (m_entries.size() < m_maxCapacity) {
            m_entries.push_back(Entry(item, boundingBox));
            return true;
         }

         if (hasChildren() == false) subdivide();

         return insertIntoChildren(Entry(item, boundingBox));
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

         if (hasChildren()) {
            m_SW->dbg_draw(z, col);
            m_SE->dbg_draw(z, col);
            m_NE->dbg_draw(z, col);
            m_NW->dbg_draw(z, col);
         }
      }
#endif

   private:
      int m_maxCapacity;
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
      // Quadtree::subdivide
      //===========================================
      void subdivide() {
         Vec2f halfSz = m_boundary.getSize() / 2.f;
         m_SW = new Quadtree(m_maxCapacity, Range(m_boundary.getPosition(), halfSz));
         m_SE = new Quadtree(m_maxCapacity, Range(m_boundary.getPosition() + Vec2f(halfSz.x, 0.f), halfSz));
         m_NE = new Quadtree(m_maxCapacity, Range(m_boundary.getPosition() + halfSz, halfSz));
         m_NW = new Quadtree(m_maxCapacity, Range(m_boundary.getPosition() + Vec2f(0.f, halfSz.y), halfSz));

         // Add all entries into the new leaves
         for (uint_t i = 0; i < m_entries.size(); ++i) {
            insertIntoChildren(m_entries[i]);
         }
         m_entries.clear();
      }

      //===========================================
      // Quadtree::insertIntoChildren
      //===========================================
      bool insertIntoChildren(const Entry& entry) {
         bool inserted = false;

         if (m_SW->getBoundary().overlaps(entry.rect)) {
            m_SW->insert(entry.item, entry.rect);
            inserted = true;
         }

         if (m_SE->getBoundary().overlaps(entry.rect)) {
            m_SE->insert(entry.item, entry.rect);
            inserted = true;
         }

         if (m_NE->getBoundary().overlaps(entry.rect)) {
            m_NE->insert(entry.item, entry.rect);
            inserted = true;
         }

         if (m_NW->getBoundary().overlaps(entry.rect)) {
            m_NW->insert(entry.item, entry.rect);
            inserted = true;
         }

         return inserted;
      }
};


}


#endif
