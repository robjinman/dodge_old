/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <set>
#include <vector>
#include "Renderer.hpp"
#include "Model.hpp"


namespace Dodge {


// Stores model pointers in order; first by depth, then by render mode, and then by texture handle.
class SceneGraph {
   friend class iterator;

   private:
      typedef std::pair<float32_t, Renderer::mode_t> subKey_t;
      typedef std::pair<subKey_t, Renderer::textureHandle_t> key_t;
      typedef std::pair<key_t, uint_t> entry_t;
      typedef std::set<entry_t> container_t;

   public:
      class iterator {
         friend class SceneGraph;

         public:
            const IModel* operator*();
            const IModel* operator->();
            bool operator!=(const iterator& rhs) const;
            bool operator==(const iterator& rhs) const;

            iterator& operator++();

         private:
            iterator(SceneGraph* sg, SceneGraph::container_t::iterator i)
               : m_sg(sg), m_i(i) {}

            SceneGraph* m_sg;
            SceneGraph::container_t::iterator m_i;
      };

      SceneGraph();

      void insert(const IModel* model);

      void clear();
      iterator begin();
      iterator end();

   private:
      struct scratchSpace_t {
         scratchSpace_t() : cursor(0) {}

         std::vector<byte_t> buffer;
         uint_t cursor;
      };

      container_t m_container;
      scratchSpace_t m_scratchSpace;
};

//===========================================
// SceneGraph::iterator::operator*
//===========================================
inline const IModel* SceneGraph::iterator::operator*() {
   return reinterpret_cast<const IModel*>(m_sg->m_scratchSpace.buffer.data() + m_i->second);
}

//===========================================
// SceneGraph::iterator::operator->
//===========================================
inline const IModel* SceneGraph::iterator::operator->() {
   return reinterpret_cast<const IModel*>(m_sg->m_scratchSpace.buffer.data() + m_i->second);
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
