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

      void setProjection(float32_t width, float32_t height);
      inline void setTranslation(const Vec2f& pos);
      void setTranslation(float32_t x, float32_t y);
      inline void translate(const Vec2f& ds);
      inline void translate(float32_t dx, float32_t dy);
      inline void translate_x(float32_t dx);
      inline void translate_y(float32_t dy);

      inline const cml::matrix44f_c& getMatrix() const;
      Vec2f getTranslation() const;
      inline const Vec2f& getViewSize() const;

   private:
      cml::matrix44f_c m_matrix;
      Vec2f m_viewSize;
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
   m_matrix.data()[12] -= dx;
   m_matrix.data()[13] -= dy;
}

//===========================================
// Camera::translate_x
//===========================================
inline void Camera::translate_x(float32_t dx) {
   m_matrix.data()[12] -= dx;
}

//===========================================
// Camera::translate_y
//===========================================
inline void Camera::translate_y(float32_t dy) {
   m_matrix.data()[13] -= dy;
}

//===========================================
// Camera::getMatrix
//===========================================
inline const cml::matrix44f_c& Camera::getMatrix() const {
   return m_matrix;
}

//===========================================
// Camera::getViewSize
//===========================================
inline const Vec2f& Camera::getViewSize() const {
   return m_viewSize;
}


}


#endif
