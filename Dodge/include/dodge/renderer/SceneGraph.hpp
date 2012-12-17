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
            const IModel* operator*() { return m_i->second; }
            const IModel* operator->() { return m_i->second; }
            bool operator!=(const iterator& rhs) const { return !(*this == rhs); }
            bool operator==(const iterator& rhs) const { return m_i == rhs.m_i; }

            iterator& operator++() {
               ++m_i;
               return *this;
            }

         private:
            iterator(SceneGraph* sg, SceneGraph::container_t::iterator i)
               : m_sg(sg), m_i(i) {}

            SceneGraph* m_sg;
            SceneGraph::container_t::iterator m_i;
      };

      void insert(const IModel* model) {
         m_container.insert(entry_t(key_t(subKey_t(model->getDepth(), model->getRenderMode()), model->getTextureHandle()), model));
      }

      void remove(const IModel* model) {
         if (m_container.erase(entry_t(key_t(subKey_t(model->getDepth(), model->getRenderMode()), model->getTextureHandle()), model)) == 0) {

            for (auto i = m_container.begin(); i != m_container.end(); ++i) {
               if (i->second == model) {
                  m_container.erase(i);
                  break;
               }
            }
         }
      }

      void clear() { m_container.clear(); }
      iterator begin() { return iterator(this, m_container.begin()); }
      iterator end() { return iterator(this, m_container.end()); }

   private:
      container_t m_container;
};


}
