/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <math/primitives/Primitive.hpp>
#include <cml/cml.h>


using namespace cml;


namespace Dodge {


Renderer Primitive::m_renderer;


//===========================================
// Primitive::setTransform
//===========================================
void Primitive::setTransform(float32_t x, float32_t y, int z, float32_t angle, const Vec2f& pivot) {
   matrix44f_c rot;
   matrix44f_c pToO;
   matrix44f_c oToP;
   matrix44f_c trans;
   matrix44f_c mv;

   matrix_translation(pToO, -pivot.x, -pivot.y, 0.f);
   matrix_translation(oToP, pivot.x, pivot.y, 0.f);
   matrix_translation(trans, x, y, static_cast<float32_t>(z));
   matrix_rotation_euler(rot, 0.f, 0.f, DEG_TO_RAD(angle), euler_order_xyz);

   mv = trans * oToP * rot * pToO;

   m_model.lock();
   m_model.setMatrix(mv.data());
   m_model.unlock();
}

//===========================================
// Primitive::operator=
//===========================================
Primitive& Primitive::operator=(const Primitive& copy) {
   m_model.lock();
   m_model = copy.m_model; // copy object
   m_model.unlock();

   return *this;
}

//===========================================
// Primitive::render
//===========================================
void Primitive::render() const {
   m_renderer.stageModel(&m_model);
}

//===========================================
// Primitive::unrender
//===========================================
void Primitive::unrender() const {
   m_renderer.unstageModel(&m_model);
}

//===========================================
// Primitive::setRenderBrush
//===========================================
void Primitive::setRenderBrush(pRenderBrush_t brush) {
   m_model.lock();
   m_model.brush = brush;
   m_model.unlock();
}

//===========================================
// Primitive::getRenderBrush
//===========================================
pRenderBrush_t Primitive::getRenderBrush() const {
   m_model.lock();
   pRenderBrush_t cpy = m_model.brush;
   m_model.unlock();

   return cpy;
}

//===========================================
// Primitive::~Primitive
//===========================================
Primitive::~Primitive() {
   m_renderer.unstageModel(&m_model);
}


}
