/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <renderer/SceneGraph.hpp>


namespace Dodge {


//===========================================
// SceneGraph::insert
//===========================================
void SceneGraph::insert(const IModel* model) {
   m_container.insert(entry_t(key_t(subKey_t(model->getDepth(), model->getRenderMode()), model->getTextureHandle()), model));
}

//===========================================
// SceneGraph::remove
//===========================================
void SceneGraph::remove(const IModel* model) {
   if (m_container.erase(entry_t(key_t(subKey_t(model->getDepth(), model->getRenderMode()), model->getTextureHandle()), model)) == 0) {

      for (auto i = m_container.begin(); i != m_container.end(); ++i) {
         if (i->second == model) {
            m_container.erase(i);
//            break;
         }
      }
   }
}

//===========================================
// SceneGraph::clear
//===========================================
void SceneGraph::clear() {
   m_container.clear();
}

//===========================================
// SceneGraph::begin
//===========================================
SceneGraph::iterator SceneGraph::begin() {
   return iterator(this, m_container.begin());
}

//===========================================
// SceneGraph::end
//===========================================
SceneGraph::iterator SceneGraph::end() {
   return iterator(this, m_container.end());
}


}
