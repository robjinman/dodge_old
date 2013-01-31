/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <set>
#include <vector>
#include "Renderer.hpp"
#include "Model.hpp"
#include "../StackAllocator.hpp"


namespace Dodge {


// Stores model pointers in order; first by depth, then by render mode, and then by texture handle.
class SceneGraph {
   friend class iterator;

   private:
      typedef std::pair<float32_t, Renderer::mode_t> subKey_t;
      typedef std::pair<subKey_t, Renderer::textureHandle_t> key_t;
      typedef std::pair<key_t, IModel*> entry_t;
      typedef std::set<entry_t> container_t;

   public:
      static const size_t INIT_STACK_SIZE = 1024; // 1KB

      class iterator {
         friend class SceneGraph;

         public:
            const IModel* operator*();
            const IModel* operator->();
            bool operator!=(const iterator& rhs) const;
            bool operator==(const iterator& rhs) const;

            iterator& operator++();

         private:
            iterator(SceneGraph::container_t::iterator i)
               : m_i(i) {}

            SceneGraph::container_t::iterator m_i;
      };

      SceneGraph();

      void insert(const IModel* model);

      void clear();
      iterator begin();
      iterator end();

   private:
      container_t m_container;
      StackAllocator m_scratchSpace;
};

//===========================================
// SceneGraph::iterator::operator*
//===========================================
inline const IModel* SceneGraph::iterator::operator*() {
   return reinterpret_cast<const IModel*>(m_i->second);
}

//===========================================
// SceneGraph::iterator::operator->
//===========================================
inline const IModel* SceneGraph::iterator::operator->() {
   return reinterpret_cast<const IModel*>(m_i->second);
}

//===========================================
// SceneGraph::iterator::operator!=
//===========================================
inline bool SceneGraph::iterator::operator!=(const iterator& rhs) const {
   return !(*this == rhs);
}

//===========================================
// SceneGraph::iterator::operator==
//===========================================
inline bool SceneGraph::iterator::operator==(const iterator& rhs) const {
   return m_i == rhs.m_i;
}

//===========================================
// SceneGraph::iterator::operator++
//===========================================
inline SceneGraph::iterator& SceneGraph::iterator::operator++() {
   ++m_i;
   return *this;
}


}
