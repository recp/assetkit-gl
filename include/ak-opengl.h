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
#include <gk-rb.h>

typedef struct AkGLContext {
  AkDoc   *doc;
  RBTree  *bufftree;
  GkScene *scene;
  GLenum   usage;
} AkGLContext;

AkResult
ak_glLoadScene(AkDoc    *doc,
               AkScene  *scene,
               GLenum    usage,
               GkScene **dest);

AkResult
ak_glLoadNode(AkGLContext * __restrict ctx,
              AkNode      * __restrict node,
              GkNode     ** __restrict dest);
  
AkResult
ak_glLoadGeometry(AkGLContext * __restrict ctx,
                  AkGeometry  * __restrict geom,
                  GkModel    ** __restrict dest);

AkResult
ak_glLoadMesh(AkGLContext * __restrict ctx,
              AkMesh      * __restrict mesh,
              GkModel    ** __restrict dest);

AkResult
ak_glLoadLight(AkDoc    * __restrict doc,
               GkNode   * __restrict node,
               AkLight  * __restrict light,
               GkLight ** __restrict dest);

#ifdef __cplusplus
}
#endif
#endif /* ak_render_h */
