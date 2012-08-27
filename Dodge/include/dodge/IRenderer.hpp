/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __I_RENDERER_HPP__
#define __I_RENDERER_HPP__


namespace Dodge {


// Interface class for cross-platform renderer
template <
   typename T_FLOAT,
   typename T_VERT_ELEM,
   typename T_MAT_ELEM,
   typename T_COL_ELEM,
   typename T_TEXCOORD_ELEM,
   typename T_TEXDATA,
   typename T_TEXID
>
class IRenderer {
   public:
      enum mode_t {
         TEXTURED_ALPHA,
         NONTEXTURED_ALPHA,
         TEXTURED_NONALPHA,
         NONTEXTURED_NONALPHA
      };

      enum primitive_t {
         TRIANGLES,
         LINES,
         QUADS,
         TRIANGLE_STRIP
      };

      // A file may be specified containing implementation-specific options.
      virtual void init(const char* optsFile = 0) = 0;

      virtual void setMode(mode_t mode) = 0;

      // Constructs a texture object from raw image data, and returns its id.
      virtual T_TEXID newTexture(const T_TEXDATA* texture, int width, int height) = 0;

      // Set current matrix (given in column-major order)
      virtual void setMatrix(const T_MAT_ELEM* mat) = 0;

      virtual void setActiveTexture(T_TEXID texId) = 0;

      virtual void setGeometry(const T_VERT_ELEM* verts, primitive_t primitiveType, int count) = 0;

      virtual void setColours(const T_COL_ELEM* colours, int count) = 0;

      virtual void setTextureCoords(const T_TEXCOORD_ELEM* texCoords, int count) = 0;

      virtual void render() = 0;

      // Clear surface (with optional colour)
      virtual void clear(const T_COL_ELEM* colour = 0) = 0;
};


}


#endif /*!__I_RENDERER_HPP__*/
