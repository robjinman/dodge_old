/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__


#include <GLES2/gl2.h>
#include <cml/cml.h>
#include <set>
#include <map>
#include <cstring>
#include <boost/shared_ptr.hpp>
#include "Colour.hpp"
#include "../Camera.hpp"
#include "../../GL_CHECK.hpp"
#include "../../definitions.hpp"


namespace Dodge {


// OpenGL ES 2.0 implementation
class Renderer {
   public:
      typedef GLint int_t;
      typedef GLfloat float_t;
      typedef GLfloat vertexElement_t;
      typedef GLfloat matrixElement_t;
      typedef GLfloat colourElement_t;
      typedef GLfloat texCoordElement_t;
      typedef byte_t textureData_t;
      typedef GLuint textureHandle_t;

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
               : m_fillColour(1.f, 1.f, 1.f, 1.f), m_lineColour(0.f, 0.f, 0.f, 1.f), m_lineWidth(1) {}

            Brush(const Colour& fCol, const Colour& lCol, int_t lineW) {
               setFillColour(fCol);
               setLineColour(lCol);
               m_lineWidth = lineW;
            }

            void setFillColour(const Colour& fCol) {
               m_fillColour = fCol;
            }

            void setLineColour(const Colour& lCol) {
               m_lineColour = lCol;
            }

            void setLineWidth(int_t w) {
               m_lineWidth = w;
            }

            const Colour& getFillColour() const {
               return m_fillColour;
            }

            const Colour& getLineColour() const {
               return m_lineColour;
            }

            int_t getLineWidth() const {
               return m_lineWidth;
            }

         private:
            Colour m_fillColour;
            Colour m_lineColour;
            int_t m_lineWidth;
      };

      enum primitive_t {
         TRIANGLES,
         LINES,
         QUADS,
         TRIANGLE_STRIP
      };

      void init(const char* optsFile = NULL);
      void setMode(mode_t mode);

      inline void attachCamera(boost::shared_ptr<Camera> camera);
      inline void attachBrush(boost::shared_ptr<Brush> brush);

      inline const Brush& getBrush() const;
      inline const Camera& getCamera() const;

      void onWindowResize(int_t w, int_t h);

      textureHandle_t newTexture(const textureData_t* texture, int_t width, int_t height);
      void setActiveTexture(textureHandle_t texId);
      void setGeometry(const vertexElement_t* verts, primitive_t primitiveType, int_t count);
      void setColours(const colourElement_t* colours, int_t count);
      void setTextureCoords(const texCoordElement_t* texCoords, int_t count);
      void render();
      void setMatrix(const matrixElement_t* mat);
      void clear();

   private:
      void constructShaderProgs();
      void constructTexturedShaderProg();
      void constructNonTexturedShaderProg();
      inline bool isSupportedPrimitive(primitive_t primitiveType) const;
      inline GLint primitiveToGLType(primitive_t primitiveType) const;

      void newShaderFromSource(const char** shaderSrc, GLint type, GLint prog);

      static std::map<mode_t, GLint> m_shaderProgIds;

      static GLint m_locPosition;
      static GLint m_locColour;
      static GLint m_locTexCoord;
      static GLint m_locMVP;

      static cml::matrix44f_c m_mv;

      static bool m_init;
      static mode_t m_mode;

      static bool m_geometrySet;
      static bool m_coloursSet;
      static bool m_texCoordsSet;

      static GLint m_vertCount;
      static GLint m_colCount;
      static GLint m_texCoordCount;

      static GLint m_primitiveType;

      static boost::shared_ptr<Brush> m_brush;
      static boost::shared_ptr<Camera> m_camera;
};

//===========================================
// Renderer::isSupportedPrimitive
//===========================================
inline bool Renderer::isSupportedPrimitive(primitive_t primitiveType) const {
   return primitiveType == TRIANGLES || primitiveType == LINES;
}

//===========================================
// Renderer::primitiveToGLType
//===========================================
inline GLint Renderer::primitiveToGLType(primitive_t primitiveType) const {
   switch (primitiveType) {
      case TRIANGLES: return GL_TRIANGLES;
      case LINES: return GL_LINES;
      default:
         throw Exception("Primitive not supported", __FILE__, __LINE__);
   }
}

//===========================================
// Renderer::attachBrush
//===========================================
inline void Renderer::attachBrush(boost::shared_ptr<Brush> brush) {
   m_brush = brush;
}

//===========================================
// Renderer::attachCamera
//===========================================
inline void Renderer::attachCamera(boost::shared_ptr<Camera> camera) {
   m_camera = camera;
}

//===========================================
// Renderer::getBrush
//===========================================
inline const Renderer::Brush& Renderer::getBrush() const {
   return *m_brush;
}

//===========================================
// Renderer::getCamera
//===========================================
inline const Camera& Renderer::getCamera() const {
   return *m_camera;
}


}


#endif /*!__RENDERER_HPP__*/
