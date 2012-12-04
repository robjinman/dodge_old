#include <renderer/Model.hpp>


namespace Dodge {


//===========================================
// Model::Model
//===========================================
Model::Model(Renderer::mode_t kind, bool perVertexColours)
   : m_matrix({1.f, 0.f, 0.f, 0.f,
               0.f, 1.f, 0.f, 0.f,
               0.f, 0.f, 1.f, 0.f,
               0.f, 0.f, 0.f, 1.f}),
     m_texHandle(0),
     m_lineWidth(1),
     m_primitiveType(Renderer::TRIANGLES),
     m_verts(NULL),
     m_n(0),
     m_bytes(0),
     m_handle(0),
     m_renderMode(kind),
     m_colData(perVertexColours) {}

//===========================================
// Model::Model
//===========================================
Model::Model(const Model& cpy) {
   copyData(cpy);
}

//===========================================
// Model::operator=
//===========================================
Model& Model::operator=(const Model& rhs) {
   m_mutex.lock();
   copyData(rhs);
   m_mutex.unlock();

   return *this;
}

//===========================================
// Model::setVertices
//===========================================
void Model::setVertices(Renderer::primitive_t primitiveType, const void* verts, Renderer::int_t n, size_t vertSz) {
   m_mutex.lock();

   m_primitiveType = primitiveType;

   if (m_bytes != n * vertSz) {
      m_bytes = n * vertSz;

      if (m_verts) deleteVertices();
      m_verts = new byte_t[m_bytes];
   }

   m_n = n;
   memcpy(m_verts, verts, m_bytes);

   m_mutex.unlock();
}

//===========================================
// Model::getVertexData
//===========================================
void Model::getVertexData(void* dest, int startByte, int endByte) const {
   m_mutex.lock();
   memcpy(dest, reinterpret_cast<byte_t*>(m_verts) + startByte, endByte - startByte);
   m_mutex.unlock();
}

//===========================================
// Model::getNumVertices
//===========================================
Renderer::int_t Model::getNumVertices() const {
   m_mutex.lock();
   Renderer::int_t cpy = m_n;
   m_mutex.unlock();

   return cpy;
}

//===========================================
// Model::setMatrix
//===========================================
void Model::setMatrix(const Renderer::matrixElement_t* matrix) {
   m_mutex.lock();
   memcpy(m_matrix, matrix, 16 * sizeof(Renderer::matrixElement_t));
   m_mutex.unlock();
}

//===========================================
// Model::setMatrixElement
//===========================================
void Model::setMatrixElement(int idx, Renderer::matrixElement_t val) {
   m_mutex.lock();
   m_matrix[idx] = val;
   m_mutex.unlock();
}

//===========================================
// Model::getMatrixElement
//===========================================
Renderer::matrixElement_t Model::getMatrixElement(int idx) const {
   m_mutex.lock();
   Renderer::matrixElement_t m = m_matrix[idx];
   m_mutex.unlock();

   return m;
}

//===========================================
// Model::getMatrix
//===========================================
void Model::getMatrix(Renderer::matrixElement_t* matrix) const {
   m_mutex.lock();
   memcpy(matrix, m_matrix, 16 * sizeof(Renderer::matrixElement_t));
   m_mutex.unlock();
}

//===========================================
// Model::setColour
//===========================================
void Model::setColour(const Colour& colour) {
   m_mutex.lock();
   m_colour = colour;
   m_mutex.unlock();
}

//===========================================
// Model::getColour
//===========================================
Colour Model::getColour() const {
   m_mutex.lock();
   Colour cpy = m_colour;
   m_mutex.unlock();

   return cpy;
}

//===========================================
// Model::setLineWidth
//===========================================
void Model::setLineWidth(Renderer::int_t lineWidth) {
   m_mutex.lock();
   m_lineWidth = lineWidth;
   m_mutex.unlock();
}
//===========================================
// Model::getLineWidth
//===========================================
Renderer::int_t Model::getLineWidth() const {
   m_mutex.lock();
   Renderer::int_t cpy = m_lineWidth;
   m_mutex.unlock();

   return cpy;
}

//===========================================
// Model::setTextureHandle
//===========================================
void Model::setTextureHandle(Renderer::textureHandle_t texHandle) {
   m_mutex.lock();
   m_texHandle = texHandle;
   m_mutex.unlock();
}

//===========================================
// Model::getTextureHandle
//===========================================
Renderer::textureHandle_t Model::getTextureHandle() const {
   m_mutex.lock();
   Renderer::textureHandle_t cpy = m_texHandle;
   m_mutex.unlock();

   return cpy;
}

//===========================================
// Model::getRenderMode
//===========================================
Renderer::mode_t Model::getRenderMode() const {
   m_mutex.lock();
   Renderer::mode_t cpy = m_renderMode;
   m_mutex.unlock();

   return cpy;
}

//===========================================
// Model::copyData
//===========================================
void Model::copyData(const Model& cpy) {
   setMatrix(cpy.m_matrix);
   m_texHandle = cpy.m_texHandle;
   m_colour = cpy.m_colour;
   m_lineWidth = cpy.m_lineWidth;
   m_primitiveType = cpy.m_primitiveType;
   memcpy(m_verts, cpy.m_verts, cpy.m_bytes);
   m_n = cpy.m_n;
   m_bytes = cpy.m_bytes;
   m_handle = cpy.m_handle;
   m_renderMode = cpy.m_renderMode;
   m_colData = cpy.m_colData;
}

//===========================================
// Model::deleteVertices
//===========================================
void Model::deleteVertices() {
   delete[] reinterpret_cast<byte_t*>(m_verts);
   m_verts = NULL;
}

//===========================================
// Model::~Model
//===========================================
Model::~Model() {
   deleteVertices();
}


}
