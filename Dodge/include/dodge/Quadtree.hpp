/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __QUAD_TREE_HPP__
#define __QUAD_TREE_HPP__


#include <vector>
#include <memory>
#include "Range.hpp"
#include "definitions.hpp"
#include "SpatialContainer.hpp"
#ifdef DEBUG
#include "Graphics2d.hpp"
#endif


namespace Dodge {


template <typename T>
class Quadtree : public SpatialContainer<T> {
   public:
      class Entry {
         public:
            Entry(T item_, const Range& rect_)
               : item(item_), rect(rect_) {}
/*
            static void* operator new(size_t size) {
               // TODO
            }

            static void operator delete(void* obj, size_t size) {
               // TODO
            }
*/
            T item;
            Range rect;
      };

      //===========================================
      // Quadtree::Quadtree
      //===========================================
      Quadtree(uint_t splittingThres, const Range& boundary)
         : m_splittingThres(splittingThres), m_boundary(boundary),
           m_children({NULL, NULL, NULL, NULL}), m_n(0) {

         m_entries.reserve(splittingThres); // TODO: find optimum
      }

      //===========================================
      // Quadtree::insert
      //===========================================
      virtual bool insert(T item, const Range& boundingBox) {
         return insert_r(item, boundingBox);
      }

      //===========================================
      // Quadtree::remove
      //===========================================
      virtual bool remove(T item, const Range& boundingBox) {
         return remove_r(item, boundingBox);
      }

      //===========================================
      // Quadtree::removeAll
      //===========================================
      virtual void removeAll() {
         m_entries.clear();

         if (hasChildren()) {
            for (int i = 0; i < 4; ++i) {
               delete m_children[i];
               m_children[i] = NULL;
            }
         }
      }

      //===========================================
      // Quadtree::getNumEntries
      //===========================================
      virtual int getNumEntries() const {
         return getNumEntries_r();
      }

      //===========================================
      // Quadtree::getEntries
      //===========================================
      virtual void getEntries(const Range& region, std::vector<T>& entries) const {
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
      //===========================================
      // Quadtree::dbg_draw
      //===========================================
/*
      virtual void dbg_draw(int z) const {
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

            m_graphics2d.drawShape(line1, 0.f, 0.f, z);
            m_graphics2d.drawShape(line2, 0.f, 0.f, z);

            for (int i = 0; i < 4; ++i)
               m_children[i]->dbg_draw(z);
         }
      }*/
#endif

      //===========================================
      // Quadtree::~Quadtree
      //===========================================
      virtual ~Quadtree() {
         if (hasChildren()) {
            for (int i = 0; i < 4; ++i) {
               delete m_children[i];
               m_children[i] = NULL;
            }
         }
      }

   private:
      int m_splittingThres;
      Range m_boundary;
      std::vector<std::unique_ptr<Entry> > m_entries;
      Quadtree<T>* m_children[4];

      // Number of entries that are fully contained within a quadrant, but
      // not contained in a child tree (should therefore be zero for any tree with children)
      int m_n;

//      PoolAllocator m_memPool;
#ifdef DEBUG
      Graphics2d m_graphics2d;
#endif

      //===========================================
      // Quadtree::hasChildren
      //
      // Assume no children if first child tree is not set
      //===========================================
      bool hasChildren() const {
         return m_children[0] != NULL;
      }

      //===========================================
      // Quadtree::getIndex
      //===========================================
      int getIndex(const Range& range) const {
         Vec2f halfSz = m_boundary.getSize() / 2.f;
         if (Range(m_boundary.getPosition(), halfSz).contains(range)) return 0;
         if (Range(m_boundary.getPosition() + Vec2f(halfSz.x, 0.f), halfSz).contains(range)) return 1;
         if (Range(m_boundary.getPosition() + halfSz, halfSz).contains(range)) return 2;
         if (Range(m_boundary.getPosition() + Vec2f(0.f, halfSz.y), halfSz).contains(range)) return 3;

         return -1;
      }

      //===========================================
      // Quadtree::getNumEntries_r
      //===========================================
      int getNumEntries_r() const {
         int t = 0;

         if (hasChildren()) {
            for (int i = 0; i < 4; ++i)
               t += m_children[i]->getNumEntries_r();
         }

         return m_entries.size() + t;
      }

      //===========================================
      // Quadtree::getEntries_r
      //===========================================
      void getEntries_r(const Range& region, std::vector<T>& entries) const {
         for (uint_t i = 0; i < m_entries.size(); ++i)
            entries.push_back(m_entries[i]->item);

         if (hasChildren()) {
            for (int i = 0; i < 4; ++i) {
               if (region.overlaps(m_children[i]->m_boundary))
                  m_children[i]->getEntries_r(region, entries);
            }
         }
      }

      //===========================================
      // Quadtree::remove_
      //===========================================
      bool remove_(T item) {
         for (uint_t i = 0; i < m_entries.size(); ++i) {
            if (m_entries[i]->item == item) {
               if (getIndex(m_entries[i]->rect) != -1)
                  --m_n;

               m_entries.erase(m_entries.begin() + i);

               return true;
            }
         }
         return false;
      }

      //===========================================
      // Quadtree::remove_r
      //===========================================
      bool remove_r(T item, const Range& boundingBox) {
         if (!m_boundary.overlaps(boundingBox))
            return false;

         if (hasChildren()) {
            int t = 0;
            for (uint_t i = 0; i < 4; ++i) {
               m_children[i]->remove_r(item, boundingBox);
               t += m_children[i]->getNumEntries();
            }

            if (t <= m_splittingThres)
               remerge();
         }

         return remove_(item);
      }

      //===========================================
      // Quadtree::insert_
      //===========================================
      void insert_(T item, const Range& boundingBox) {
         m_entries.push_back(std::unique_ptr<Entry>(new Entry(item, boundingBox)));
      }

      //===========================================
      // Quadtree::insert_r
      //===========================================
      bool insert_r(T item, const Range& boundingBox) {
         if (!m_boundary.contains(boundingBox))
            return false;

         int i = getIndex(boundingBox);

         if (hasChildren()) {
            if (i != -1)
               m_children[i]->insert_r(item, boundingBox);
            else
               insert_(item, boundingBox);
         }
         else {
            insert_(item, boundingBox);
            if (i != -1) ++m_n;

            if (m_n > m_splittingThres)
               subdivide();
         }

         return true;
      }

      //===========================================
      // Quadtree::remerge
      //===========================================
      void remerge() {
         for (int i = 0; i < 4; ++i) {
            m_n += m_children[i]->m_entries.size();
            for (uint_t j = 0; j < m_children[i]->m_entries.size(); ++j) {
               m_entries.push_back(std::move(m_children[i]->m_entries[j]));
            }
            m_children[i]->m_entries.clear();

            delete m_children[i];
            m_children[i] = NULL;
         }
      }

      //===========================================
      // Quadtree::subdivide
      //===========================================
      void subdivide() {
         Vec2f halfSz = m_boundary.getSize() / 2.f;
         m_children[0] = new Quadtree(m_splittingThres, Range(m_boundary.getPosition(), halfSz));
         m_children[1] = new Quadtree(m_splittingThres, Range(m_boundary.getPosition() + Vec2f(halfSz.x, 0.f), halfSz));
         m_children[2] = new Quadtree(m_splittingThres, Range(m_boundary.getPosition() + halfSz, halfSz));
         m_children[3] = new Quadtree(m_splittingThres, Range(m_boundary.getPosition() + Vec2f(0.f, halfSz.y), halfSz));

         // Add entries into the new leaves
         for (uint_t i = 0; i < m_entries.size(); ++i) {
            int c = getIndex(m_entries[i]->rect);
            if (c != -1) {
               m_children[c]->insert(m_entries[i]->item, m_entries[i]->rect);
               m_entries.erase(m_entries.begin() + i);
               --m_n;
               --i;
            }
         }
      }
};


}


#endif
