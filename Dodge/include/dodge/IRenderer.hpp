/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __I_RENDERER_HPP__
#define __I_RENDERER_HPP__


namespace Dodge {


// Interface class for cross-platform renderer
class IRenderer {
   public:
      // Must contain the following typedefs:
      //    NAME                 SUGGESTED VALUE
      //    vertexElement_t      float
      //    matrixElement_t      float
      //    colourElement_t      float
      //    texCoordElement_t    float
      //    textureData_t        unsigned char
      //    textureId_t          unsigned int
      #include "rendererTypes.hpp"

      enum mode_t {
         TEXTURED_ALPHA,
         NONTEXTURED_ALPHA,
         TEXTURED_NONALPHA,
         NONTEXTURED_NONALPHA
      };

      // A file may be specified containing implementation-specific options.
      virtual void init(const char* optsFile = 0) = 0;

      virtual void setMode(mode_t mode) = 0;

      // Constructs a texture object from raw image data, and returns its id.
      virtual textureId_t newTexture(const textureData_t* texture, int width, int height) = 0;

      // Set current matrix (given in column-major order)
      virtual void setMatrix(const matrixElement_t* mat) = 0;

      virtual void setActiveTexture(textureId_t texId) = 0;

      // Specify geometry in triangles
      virtual void setGeometry(const vertexElement_t* verts, int count) = 0;

      virtual void setColours(const colourElement_t* colours, int count) = 0;

      virtual void setTextureCoords(const texCoordElement_t* texCoords, int count) = 0;

      virtual void render() = 0;

      // Clear surface (with optional colour)
      virtual void clear(const colourElement_t* colour = 0) = 0;
};


}


#endif /*!__I_RENDERER_HPP__*/
