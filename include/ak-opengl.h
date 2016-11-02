/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef ak_render_h
#define ak_render_h
#ifdef __cplusplus
extern "C" {
#endif

#if defined(HAVE_WINDOWS_H) && defined(_WIN32)
# include <windows.h>
#endif

#ifdef __APPLE__
#  include <OpenGL/gl3.h>
#else
#  ifdef _WIN32
#    include <windows.h>
#  endif
#  include <GL/gl.h>
#endif

#include "assetkit.h"
#include <gk.h>

AkResult
ak_glLoadScene(AkDoc  * __restrict doc,
               AkScene * scene,
               GLenum usage,
               GkScene ** dest);

AkResult
ak_glLoadNode(AkDoc   * __restrict doc,
              AkNode  *node,
              GLenum   usage,
              mat4     parentTransform,
              GkNode **dest);
  
AkResult
ak_glLoadGeometry(AkDoc  * __restrict doc,
                  AkGeometry * geometry,
                  GLenum usage,
                  GkComplexModel ** dest);

AkResult
ak_glLoadMesh(AkDoc  * __restrict doc,
              AkMesh * mesh,
              GLenum usage,
              GkComplexModel ** dest);

#ifdef __cplusplus
}
#endif
#endif /* ak_render_h */
