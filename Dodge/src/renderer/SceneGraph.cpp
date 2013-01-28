/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <renderer/SceneGraph.hpp>


namespace Dodge {


//===========================================
// SceneGraph::SceneGraph
//===========================================
SceneGraph::SceneGraph() {}

//===========================================
// SceneGraph::insert
//===========================================
void SceneGraph::insert(const IModel* model) {
   IModel* ptr;
   uint_t offset = m_scratchSpace.cursor;

   size_t totalSize = model->getSizeOf() + model->vertexDataSize();

   while (m_scratchSpace.buffer.size() < offset + totalSize) {
      m_scratchSpace.buffer.resize(m_scratchSpace.buffer.size() * 2 + 1);
//      m_scratchSpace.buffer.resize(offset + totalSize);
   }

   ptr = reinterpret_cast<IModel*>(m_scratchSpace.buffer.data() + offset);
   m_scratchSpace.cursor += totalSize;

   model->copyTo(ptr);

   m_container.insert(entry_t(key_t(subKey_t(ptr->getDepth(), ptr->getRenderMode()), ptr->getTextureHandle()), offset));
}

//===========================================
// SceneGraph::clear
//===========================================
void SceneGraph::clear() {
   m_container.clear();
   m_scratchSpace.buffer.clear();
   m_scratchSpace.cursor = 0;
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
