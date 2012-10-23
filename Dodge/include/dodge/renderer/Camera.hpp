#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__


#include <boost/shared_ptr.hpp>
#include <cml/cml.h>
#include "../definitions.hpp"
#include "../math/Vec2f.hpp"


namespace Dodge {


class Camera {
   public:
      Camera(float32_t w, float32_t h) {
         setProjection(w, h);
         setTranslation(0.f, 0.f);
      }

      inline void setProjection(float32_t width, float32_t height);
      inline void setTranslation(const Vec2f& pos);
      inline void setTranslation(float32_t x, float32_t y);
      inline void translate(const Vec2f& ds);
      inline void translate(float32_t dx, float32_t dy);
      inline void translate_x(float32_t dx);
      inline void translate_y(float32_t dy);

      inline const cml::matrix44f_c& getMatrix() const;
      inline Vec2f getTranslation() const;
      inline const Vec2f& getViewSize() const;

   private:
      cml::matrix44f_c m_matrix;
      Vec2f m_viewSize;
};

typedef boost::shared_ptr<Camera> pCamera_t;

//===========================================
// Camera::setProjection
//===========================================
inline void Camera::setProjection(float32_t width, float32_t height) {
   Vec2f s = getTranslation();

   matrix_orthographic_RH(m_matrix, width, height, 0.01f, 100.f, cml::z_clip_neg_one);

   m_viewSize.x = width;
   m_viewSize.y = height;

   setTranslation(s.x, s.y);
}

//===========================================
// Camera::setTranslation
//===========================================
inline void Camera::setTranslation(const Vec2f& pos) {
   setTranslation(pos.x, pos.y);
}

//===========================================
// Camera::setTranslation
//===========================================
inline void Camera::setTranslation(float32_t x, float32_t y) {
   m_matrix.data()[12] = x - 1.f;
   m_matrix.data()[13] = y - 1.f;

   // Move the camera back slightly (any small negative number will do as
   // we're using an orthographic projection).
   m_matrix.data()[14] = -0.01f;
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
   m_matrix.data()[12] += dx;
   m_matrix.data()[13] += dy;
}

//===========================================
// Camera::translate_x
//===========================================
inline void Camera::translate_x(float32_t dx) {
   m_matrix.data()[12] += dx;
}

//===========================================
// Camera::translate_y
//===========================================
inline void Camera::translate_y(float32_t dy) {
   m_matrix.data()[13] += dy;
}

//===========================================
// Camera::getMatrix
//===========================================
inline const cml::matrix44f_c& Camera::getMatrix() const {
   return m_matrix;
}

//===========================================
// Camera::getTranslation
//===========================================
inline Vec2f Camera::getTranslation() const {
   return Vec2f(m_matrix.data()[12] + 1.f, m_matrix.data()[13] + 1.f);
}

//===========================================
// Camera::getViewSize
//===========================================
inline const Vec2f& Camera::getViewSize() const {
   return m_viewSize;
}


}


#endif
