/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "ak_enum.h"

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
  GLenum mode;
  
  switch (primitive->type) {
    case AK_MESH_PRIMITIVE_TYPE_POLYGONS:
      mode = GL_TRIANGLES;
      break;
    case AK_MESH_PRIMITIVE_TYPE_TRIANGLES: {
      AkTriangles *triangles;
      triangles = (AkTriangles *)primitive;
      switch (triangles->mode) {
        case AK_TRIANGLE_MODE_TRIANGLES:
          mode = GL_TRIANGLES;
          break;
        case AK_TRIANGLE_MODE_TRIANGLE_STRIP:
          mode = GL_TRIANGLE_STRIP;
          break;
        case AK_TRIANGLE_MODE_TRIANGLE_FAN:
          mode = GL_TRIANGLE_FAN;
          break;
      }
      break;
    }
    case AK_MESH_PRIMITIVE_TYPE_LINES: {
      AkLines *lines;
      
      lines = (AkLines *)primitive;
      switch (lines->mode) {
        case AK_LINE_MODE_LINES:
          mode = GL_LINES;
          break;
        case AK_LINE_MODE_LINE_STRIP:
          mode = GL_LINE_STRIP;
          break;
        case AK_LINE_MODE_LINE_LOOP:
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

