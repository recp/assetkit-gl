/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "enum.h"

GLenum
agk_type(AkTypeId typeId) {
  GLenum type;

  switch (typeId) {
    case AKT_INT:    type = GL_INT;                break;
    case AKT_UINT:   type = GL_UNSIGNED_INT;       break;
    case AKT_SHORT:  type = GL_SHORT;              break;
    case AKT_USHORT: type = GL_UNSIGNED_SHORT;     break;
    case AKT_FLOAT:  type = GL_FLOAT;              break;
    case AKT_BYTE:   type = GL_BYTE;               break;
    case AKT_UBYTE:  type = GL_UNSIGNED_BYTE;      break;
    default:         type = GL_INT;
  }

  return type;
}

GLenum
agk_isinteger(AkTypeId typeId) {
  switch (typeId) {
    case AKT_INT:
    case AKT_UINT:
    case AKT_SHORT:
    case AKT_USHORT:
    case AKT_BYTE:
    case AKT_UBYTE: return true;
    default:
      return false;
  }
}

GLenum
agk_wrapMode(AkWrapMode wrapMode) {
  switch (wrapMode) {
    case AK_WRAP_MODE_WRAP:        return GL_REPEAT;
    case AK_WRAP_MODE_MIRROR:      return GL_MIRRORED_REPEAT;
    case AK_WRAP_MODE_CLAMP:       return GL_CLAMP_TO_EDGE;
    case AK_WRAP_MODE_BORDER:      return GL_CLAMP_TO_BORDER;
    case AK_WRAP_MODE_MIRROR_ONCE: return GL_CLAMP_TO_BORDER; /* TODO: */
    default: return GL_REPEAT;
  }
}

GLenum
agk_minFilter(AkMinFilter minfilter) {
  switch (minfilter) {
    case AK_MINFILTER_LINEAR:  return GL_LINEAR;
    case AK_MINFILTER_NEAREST: return GL_NEAREST;
    case AK_MINFILTER_ANISOTROPIC: /* TODO: */
    default: return GL_LINEAR;
  }
}

GLenum
agk_magFilter(AkMagFilter magfilter){
  switch (magfilter) {
    case AK_MAGFILTER_LINEAR:      return GL_LINEAR;
    case AK_MAGFILTER_NEAREST:     return GL_NEAREST;
    default: return GL_LINEAR;
  }
}

GLenum
agk_mipFilter(AkMipFilter mipfilter) {
  switch (mipfilter) {
    case AK_MIPFILTER_LINEAR:      return GL_LINEAR;
    case AK_MIPFILTER_NEAREST:     return GL_NEAREST;
    default: return GL_NONE;
  }
}

GLenum
agk_drawMode(AkMeshPrimitive *primitive) {
  GLenum mode = GL_TRIANGLES;
  
  switch (primitive->type) {
    case AK_PRIMITIVE_POLYGONS:
      mode = GL_TRIANGLES;
      break;
    case AK_PRIMITIVE_TRIANGLES: {
      AkTriangles *triangles;
      triangles = (AkTriangles *)primitive;
      switch (triangles->mode) {
        case AK_TRIANGLES:
          mode = GL_TRIANGLES;
          break;
        case AK_TRIANGLE_STRIP:
          mode = GL_TRIANGLE_STRIP;
          break;
        case AK_TRIANGLE_FAN:
          mode = GL_TRIANGLE_FAN;
          break;
      }
      break;
    }
    case AK_PRIMITIVE_LINES: {
      AkLines *lines;
      
      lines = (AkLines *)primitive;
      switch (lines->mode) {
        case AK_LINES:
          mode = GL_LINES;
          break;
        case AK_LINE_STRIP:
          mode = GL_LINE_STRIP;
          break;
        case AK_LINE_LOOP:
          mode = GL_LINE_LOOP;
          break;
      }
      
      break;
    }
    default:
      mode = GL_TRIANGLES;
      break;
  }
  
  return mode;
}

GLenum
agk_textureTarget(AkTypeId typeId) {
  GLenum target;

  switch (typeId) {
    case AKT_SAMPLER1D:
      target = GL_TEXTURE_1D;
      break;
    case AKT_SAMPLER2D:
      target = GL_TEXTURE_2D;
      break;
    case AKT_SAMPLER3D:
      target = GL_TEXTURE_3D;
      break;
    case AKT_SAMPLER_CUBE:
      target = GL_TEXTURE_CUBE_MAP;
      break;
    case AKT_SAMPLER_RECT:
      target = GL_TEXTURE_RECTANGLE;
      break;
    case AKT_SAMPLER_DEPTH:
      target = GL_TEXTURE_DEPTH;
      break;
    default:
      return -1;
  }
  
  return target;
}

GkOpaque
agk_opaque(AkOpaque opaque) {
  GkOpaque glOpaque;

  switch (opaque) {
    case AK_OPAQUE_OPAQUE:   glOpaque = GK_OPAQUE_OPAQUE;       break;
    case AK_OPAQUE_A_ONE:    glOpaque = GK_OPAQUE_A_ONE;        break;
    case AK_OPAQUE_A_ZERO:   glOpaque = GK_OPAQUE_A_ZERO;       break;
    case AK_OPAQUE_RGB_ONE:  glOpaque = GK_OPAQUE_RGB_ONE;      break;
    case AK_OPAQUE_RGB_ZERO: glOpaque = GK_OPAQUE_RGB_ZERO;     break;
    case AK_OPAQUE_BLEND:    glOpaque = GK_OPAQUE_BLEND;        break;
    case AK_OPAQUE_MASK:     glOpaque = GK_OPAQUE_MASK;         break;
  }

  return glOpaque;
}
