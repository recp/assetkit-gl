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
