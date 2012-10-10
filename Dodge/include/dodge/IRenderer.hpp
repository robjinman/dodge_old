/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __I_RENDERER_HPP__
#define __I_RENDERER_HPP__


#include <cstring>
#include <exception>
#include <boost/shared_ptr.hpp>


namespace Dodge {


// Interface class for cross-platform renderer
template <
   typename T_FLOAT,          // float (32 or 64)
   typename T_VERT_ELEM,      // float (32 or 64)
   typename T_MAT_ELEM,       // float (32 or 64)
   typename T_COL_ELEM,       // float (32 or 64)
   typename T_TEXCOORD_ELEM,  // float (32 or 64)
   typename T_TEXDATA,        // byte (char, unsigned char, etc.)
   typename T_TEXID           // integer (int, unsigned int, long, etc.)
>
class IRenderer {
   public:
      enum mode_t {
         UNDEFINED,
         TEXTURED_ALPHA,
         NONTEXTURED_ALPHA,
         TEXTURED_NONALPHA,
         NONTEXTURED_NONALPHA
      };

      class Brush {
         public:
            Brush()
               : m_fillColour({1.f, 1.f, 1.f, 1.f}), m_lineColour({0.f, 0.f, 0.f, 1.f}), m_lineWidth(1) {}

            Brush(const T_COL_ELEM fCol[4], const T_COL_ELEM lCol[4], int lineW) {
               setFillColour(fCol);
               setLineColour(lCol);
               m_lineWidth = lineW;
            }

            void setFillColour(const T_COL_ELEM fCol[4]) {
               memcpy(m_fillColour, fCol, 4 * sizeof(T_COL_ELEM));
            }

            void setLineColour(const T_COL_ELEM lCol[4]) {
               memcpy(m_lineColour, lCol, 4 * sizeof(T_COL_ELEM));
            }

            void setLineWidth(int w) { // TODO: use T_INTEGER
               m_lineWidth = w;
            }

            const T_COL_ELEM* getFillColour() const {
               return m_fillColour;
            }

            const T_COL_ELEM* getLineColour() const {
               return m_lineColour;
            }

            int getLineWidth() const { // TODO: T_INTEGER
               return m_lineWidth;
            }

         private:
            T_COL_ELEM m_fillColour[4];
            T_COL_ELEM m_lineColour[4];
            int m_lineWidth;
      };

      enum primitive_t {
         TRIANGLES,
         LINES,
         QUADS,
         TRIANGLE_STRIP
      };

      //===========================================
      // IRenderer::setP
      //
      // 4x4 projection matrix (in column-major order)
      //===========================================
      virtual void setP(const T_MAT_ELEM* matrix) {
         memcpy(m_projMat, matrix, 16 * sizeof(T_MAT_ELEM));
      }

      //===========================================
      // IRenderer::P
      //===========================================
      virtual T_MAT_ELEM* P(int idx = 0) const {
         if (idx < 0 || idx > 15)
            throw std::runtime_error("Error returning element of P; index out of range");

         return m_projMat + idx;
      }

      //===========================================
      // IRenderer::attachBrush
      //===========================================
      virtual void attachBrush(boost::shared_ptr<Brush> brush) {
         m_brush = brush;
      }

      //===========================================
      // IRenderer::getBrush
      //===========================================
      virtual const Brush& getBrush() const {
         return *m_brush;
      }

      // A file may be specified containing implementation-specific options.
      virtual void init(const char* optsFile = 0) = 0;

      virtual void setMode(mode_t mode) = 0;

      // Constructs a texture object from raw image data, and returns its id.
      virtual T_TEXID newTexture(const T_TEXDATA* texture, int width, int height) = 0;

      // Set current model view matrix
      virtual void setMatrix(const T_MAT_ELEM* mat) = 0;

      virtual void setActiveTexture(T_TEXID texId) = 0;

      virtual void setGeometry(const T_VERT_ELEM* verts, primitive_t primitiveType, int count) = 0;

      virtual void setColours(const T_COL_ELEM* colours, int count) = 0;

      virtual void setTextureCoords(const T_TEXCOORD_ELEM* texCoords, int count) = 0;

      virtual void render() = 0;

      virtual void clear() = 0;

   protected:
      static boost::shared_ptr<Brush> m_brush;
      static T_MAT_ELEM m_projMat[16];
};


// Initialise static members
template <
   typename T_FLOAT, typename T_VERT_ELEM, typename T_MAT_ELEM, typename T_COL_ELEM,
   typename T_TEXCOORD_ELEM, typename T_TEXDATA, typename T_TEXID
>
typename boost::shared_ptr<typename IRenderer<T_FLOAT, T_VERT_ELEM, T_MAT_ELEM, T_COL_ELEM, T_TEXCOORD_ELEM, T_TEXDATA, T_TEXID>::Brush>
   IRenderer<T_FLOAT, T_VERT_ELEM, T_MAT_ELEM, T_COL_ELEM, T_TEXCOORD_ELEM, T_TEXDATA, T_TEXID>::m_brush
      = boost::shared_ptr<typename IRenderer<T_FLOAT, T_VERT_ELEM, T_MAT_ELEM, T_COL_ELEM, T_TEXCOORD_ELEM, T_TEXDATA, T_TEXID>::Brush>(
         new IRenderer<T_FLOAT, T_VERT_ELEM, T_MAT_ELEM, T_COL_ELEM, T_TEXCOORD_ELEM, T_TEXDATA, T_TEXID>::Brush
      );

template <
   typename T_FLOAT, typename T_VERT_ELEM, typename T_MAT_ELEM, typename T_COL_ELEM,
   typename T_TEXCOORD_ELEM, typename T_TEXDATA, typename T_TEXID
>
T_MAT_ELEM IRenderer<T_FLOAT, T_VERT_ELEM, T_MAT_ELEM, T_COL_ELEM, T_TEXCOORD_ELEM, T_TEXDATA, T_TEXID>::m_projMat[]
   = { 1.f, 0.f, 0.f, 0.f,
       0.f, 1.f, 0.f, 0.f,
       0.f, 0.f, 1.f, 0.f,
       0.f, 0.f, 0.f, 1.f };


}


#endif /*!__I_RENDERER_HPP__*/
