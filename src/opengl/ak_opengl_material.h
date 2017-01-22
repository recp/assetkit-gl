/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef ak_opengl_material_h
#define ak_opengl_material_h

#include "../../include/ak-opengl.h"

AkResult
ak_glLoadMaterial(AkGLContext        * __restrict ctx,
                  AkInstanceGeometry * __restrict geomInst,
                  GkMaterial        ** __restrict dest);

#endif /* ak_opengl_material_h */
