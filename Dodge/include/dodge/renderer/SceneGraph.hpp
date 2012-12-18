/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include "Renderer.hpp"
#include "Model.hpp"


namespace Dodge {


// Stores model pointers in order; first by depth, then by render mode, and then by texture handle.
class SceneGraph {
   friend class iterator;

   private:
      typedef std::pair<float32_t, Renderer::mode_t> subKey_t;
      typedef std::pair<subKey_t, Renderer::textureHandle_t> key_t;
      typedef std::pair<key_t, const IModel*> entry_t;
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

      void insert(const IModel* model);
      void remove(const IModel* model);

      void clear();
      iterator begin();
      iterator end();

   private:
      container_t m_container;
};

//===========================================
// SceneGraph::iterator::operator*
//===========================================
inline const IModel* SceneGraph::iterator::operator*() {
   return m_i->second;
}

//===========================================
// SceneGraph::iterator::operator->
//===========================================
inline const IModel* SceneGraph::iterator::operator->() {
   return m_i->second;
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
