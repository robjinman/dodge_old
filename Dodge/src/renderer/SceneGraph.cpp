/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <renderer/SceneGraph.hpp>


namespace Dodge {


//===========================================
// SceneGraph::SceneGraph
//===========================================
SceneGraph::SceneGraph()
   : m_scratchSpace(INIT_STACK_SIZE) {}

//===========================================
// SceneGraph::insert
//===========================================
void SceneGraph::insert(const IModel* model) {
   size_t totalSize = model->getTotalSize();

   IModel* ptr = reinterpret_cast<IModel*>(m_scratchSpace.alloc(totalSize));

   model->copyTo(ptr);

//   Renderer& renderer = Renderer::getInstance();
//   renderer.bufferModel(ptr);

   m_container.insert(entry_t(key_t(subKey_t(ptr->getDepth(), ptr->getRenderMode()), ptr->getTextureHandle()), ptr));
}

//===========================================
// SceneGraph::clear
//===========================================
void SceneGraph::clear() {
   m_container.clear();
   m_scratchSpace.clear();
}

//===========================================
// SceneGraph::begin
//===========================================
SceneGraph::iterator SceneGraph::begin() {
   return iterator(m_container.begin());
}

//===========================================
// SceneGraph::end
//===========================================
SceneGraph::iterator SceneGraph::end() {
   return iterator(m_container.end());
}


}
