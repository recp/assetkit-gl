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
ak_glGeometryLoad(AkGeometry * geometry,
                  GLenum usage,
                  GkComplexModel ** dest);

AkResult
ak_glMeshLoad(AkMesh * mesh,
              GLenum usage,
              GkComplexModel ** dest);

#ifdef __cplusplus
}
#endif
#endif /* ak_render_h */
