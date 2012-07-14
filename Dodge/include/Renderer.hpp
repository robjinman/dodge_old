/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__


#include <GLES2/gl2.h>
#include <set>
#include <map>
#include "GL_CHECK.hpp"
#include "IRenderer.hpp"


namespace Dodge {


// OpenGL ES 2.0 implementation of IRenderer interface
class Renderer : public IRenderer {
   public:
      virtual void init(const char* optsFile = NULL);
      virtual void setMode(mode_t mode);
      virtual textureId_t newTexture(const textureData_t* texture, int width, int height);
      virtual void setActiveTexture(textureId_t texId);
      virtual void setGeometry(const vertexElement_t* verts, int count);
      virtual void setColours(const colourElement_t* colours, int count);
      virtual void setTextureCoords(const texCoordElement_t* texCoords, int count);
      virtual void render();
      virtual void setMatrix(const matrixElement_t* mat);
      virtual void clear(const colourElement_t* colour = NULL);

   private:
      void constructShaderProgs();
      void constructTexturedShaderProg();
      void constructNonTexturedShaderProg();

      void newShaderFromSource(const char** shaderSrc, GLint type, GLint prog);

      static std::map<mode_t, GLint> m_shaderProgIds;

      static GLint m_locPosition;
      static GLint m_locColour;
      static GLint m_locTexCoord;
      static GLint m_locMVP;

      static bool m_init;
      static mode_t m_mode;

      static bool m_geometrySet;
      static bool m_coloursSet;
      static bool m_texCoordsSet;

      static int m_vertCount;
      static int m_colCount;
      static int m_texCoordCount;
};


}


#endif /*!__RENDERER_HPP__*/
