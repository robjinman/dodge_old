#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__


#include <mutex>
#include <boost/shared_ptr.hpp>
#include <cml/cml.h>
#include "../definitions.hpp"
#include "../math/Vec2f.hpp"


namespace Dodge {


class Camera {
   public:
      Camera(float32_t w, float32_t h) {
         m_matrix = cml::identity_4x4();
         setProjection(w, h);
         setTranslation(0.f, 0.f);
      }

      void setProjection(float32_t width, float32_t height);
      inline void setTranslation(const Vec2f& pos);
      void setTranslation(float32_t x, float32_t y);
      inline void translate(const Vec2f& ds);
      inline void translate(float32_t dx, float32_t dy);
      inline void translate_x(float32_t dx);
      inline void translate_y(float32_t dy);

      inline void getMatrix(cml::matrix44f_c& matrix) const;
      Vec2f getTranslation() const;
      inline Vec2f getViewSize() const;

   private:
      cml::matrix44f_c m_matrix;
      Vec2f m_viewSize;

      mutable std::mutex m_mutex;
};

typedef boost::shared_ptr<Camera> pCamera_t;

//===========================================
// Camera::setTranslation
//===========================================
inline void Camera::setTranslation(const Vec2f& pos) {
   setTranslation(pos.x, pos.y);
}

//===========================================
// Camera::translate
//===========================================
inline void Camera::translate(const Vec2f& ds) {
   translate(ds.x, ds.y);
}

//===========================================
// Camera::translate
//===========================================
inline void Camera::translate(float32_t dx, float32_t dy) {
   m_mutex.lock();
   m_matrix.data()[12] -= dx;
   m_matrix.data()[13] -= dy;
   m_mutex.unlock();
}

//===========================================
// Camera::translate_x
//===========================================
inline void Camera::translate_x(float32_t dx) {
   m_mutex.lock();
   m_matrix.data()[12] -= dx;
   m_mutex.unlock();
}

//===========================================
// Camera::translate_y
//===========================================
inline void Camera::translate_y(float32_t dy) {
   m_mutex.lock();
   m_matrix.data()[13] -= dy;
   m_mutex.unlock();
}

//===========================================
// Camera::getMatrix
//===========================================
inline void Camera::getMatrix(cml::matrix44f_c& matrix) const {
   m_mutex.lock();
   matrix = m_matrix;
   m_mutex.unlock();
}

//===========================================
// Camera::getViewSize
//===========================================
inline Vec2f Camera::getViewSize() const {
   m_mutex.lock();
   Vec2f cpy = m_viewSize;
   m_mutex.unlock();

   return cpy;
}


}


#endif
