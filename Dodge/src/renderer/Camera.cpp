#include <renderer/Camera.hpp>


namespace Dodge {


//===========================================
// Camera::setProjection
//===========================================
void Camera::setProjection(float32_t width, float32_t height) {
   Vec2f s = getTranslation();

   m_mutex.lock();

   matrix_orthographic_RH(m_matrix, width, height, 0.01f, 100.f, cml::z_clip_neg_one);

   m_viewSize.x = width;
   m_viewSize.y = height;

   m_mutex.unlock();

   setTranslation(s.x, s.y);
}

//===========================================
// Camera::setTranslation
//===========================================
void Camera::setTranslation(float32_t x, float32_t y) {
   m_mutex.lock();

   m_matrix.data()[12] = -(x * 2.f) / (m_viewSize.x / m_viewSize.y) - 1.f;
   m_matrix.data()[13] = -(y * 2.f) - 1.f;

   // Move the camera back slightly (any small negative number will do as
   // we're using an orthographic projection).
   m_matrix.data()[14] = -0.001f;

   m_mutex.unlock();
}

//===========================================
// Camera::getTranslation
//===========================================
inline Vec2f Camera::getTranslation() const {
   m_mutex.lock();
   Vec2f tmp(-0.5 * (m_viewSize.x / m_viewSize.y) * (m_matrix.data()[12] + 1.f), -0.5 * (m_matrix.data()[13] + 1.f));
   m_mutex.unlock();

   return tmp;
}


}
