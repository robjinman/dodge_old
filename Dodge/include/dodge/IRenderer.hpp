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
   typename T_INTEGER,        // integer (32 or 64)
   typename T_FLOAT,          // float (32 or 64)
   typename T_VERT_ELEM,      // float (32 or 64)
   typename T_MAT_ELEM,       // float (32 or 64)
   typename T_COL_ELEM,       // float (32 or 64)
   typename T_TEXCOORD_ELEM,  // float (32 or 64)
   typename T_TEXDATA,        // byte (char, unsigned char, etc.)
   typename T_TEXHANDLE       // usually an integer (int, unsigned int, long, etc.)
>
class IRenderer {
   public:
      typedef T_INTEGER int_t;
      typedef T_FLOAT float_t;
      typedef T_VERT_ELEM vertexElement_t;
      typedef T_MAT_ELEM matrixElement_t;
      typedef T_COL_ELEM colourElement_t;
      typedef T_TEXCOORD_ELEM texCoordElement_t;
      typedef T_TEXDATA textureData_t;
      typedef T_TEXHANDLE textureHandle_t;

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

            Brush(const colourElement_t fCol[4], const colourElement_t lCol[4], int_t lineW) {
               setFillColour(fCol);
               setLineColour(lCol);
               m_lineWidth = lineW;
            }

            void setFillColour(const colourElement_t fCol[4]) {
               memcpy(m_fillColour, fCol, 4 * sizeof(colourElement_t));
            }

            void setLineColour(const colourElement_t lCol[4]) {
               memcpy(m_lineColour, lCol, 4 * sizeof(colourElement_t));
            }

            void setLineWidth(int_t w) {
               m_lineWidth = w;
            }

            const colourElement_t* getFillColour() const {
               return m_fillColour;
            }

            const colourElement_t* getLineColour() const {
               return m_lineColour;
            }

            int_t getLineWidth() const {
               return m_lineWidth;
            }

         private:
            colourElement_t m_fillColour[4];
            colourElement_t m_lineColour[4];
            int_t m_lineWidth;
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
      virtual void setP(const matrixElement_t* matrix) {
         memcpy(m_projMat, matrix, 16 * sizeof(matrixElement_t));
      }

      //===========================================
      // IRenderer::P
      //===========================================
      virtual matrixElement_t* P(int idx = 0) const {
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
      virtual textureHandle_t newTexture(const textureData_t* texture, int_t width, int_t height) = 0;

      // Set current model view matrix
      virtual void setMatrix(const matrixElement_t* mat) = 0;

      virtual void setActiveTexture(textureHandle_t texId) = 0;

      virtual void setGeometry(const vertexElement_t* verts, primitive_t primitiveType, int_t count) = 0;

      virtual void setColours(const colourElement_t* colours, int_t count) = 0;

      virtual void setTextureCoords(const texCoordElement_t* texCoords, int_t count) = 0;

      virtual void render() = 0;

      virtual void clear() = 0;

   protected:
      static boost::shared_ptr<Brush> m_brush;
      static matrixElement_t m_projMat[16];
};


// Initialise static members
template <
   typename T_INTEGER, typename T_FLOAT, typename T_VERT_ELEM, typename T_MAT_ELEM, typename T_COL_ELEM,
   typename T_TEXCOORD_ELEM, typename T_TEXDATA, typename T_TEXHANDLE
>
typename boost::shared_ptr<typename IRenderer<T_INTEGER, T_FLOAT, T_VERT_ELEM, T_MAT_ELEM, T_COL_ELEM, T_TEXCOORD_ELEM, T_TEXDATA, T_TEXHANDLE>::Brush>
   IRenderer<T_INTEGER, T_FLOAT, T_VERT_ELEM, T_MAT_ELEM, T_COL_ELEM, T_TEXCOORD_ELEM, T_TEXDATA, T_TEXHANDLE>::m_brush
      = boost::shared_ptr<typename IRenderer<T_INTEGER, T_FLOAT, T_VERT_ELEM, T_MAT_ELEM, T_COL_ELEM, T_TEXCOORD_ELEM, T_TEXDATA, T_TEXHANDLE>::Brush>(
         new IRenderer<T_INTEGER, T_FLOAT, T_VERT_ELEM, T_MAT_ELEM, T_COL_ELEM, T_TEXCOORD_ELEM, T_TEXDATA, T_TEXHANDLE>::Brush
      );

template <
   typename T_INTEGER, typename T_FLOAT, typename T_VERT_ELEM, typename T_MAT_ELEM, typename T_COL_ELEM,
   typename T_TEXCOORD_ELEM, typename T_TEXDATA, typename T_TEXHANDLE
>
T_MAT_ELEM IRenderer<T_INTEGER, T_FLOAT, T_VERT_ELEM, T_MAT_ELEM, T_COL_ELEM, T_TEXCOORD_ELEM, T_TEXDATA, T_TEXHANDLE>::m_projMat[]
   = { 1.f, 0.f, 0.f, 0.f,
       0.f, 1.f, 0.f, 0.f,
       0.f, 0.f, 1.f, 0.f,
       0.f, 0.f, 0.f, 1.f };


}


#endif /*!__I_RENDERER_HPP__*/
