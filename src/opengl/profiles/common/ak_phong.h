/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef ak_phong_h
#define ak_phong_h

#include "../../../../include/ak-opengl.h"

AkResult
ak_glPhong(AkGLContext    * __restrict ctx,
           AkBindMaterial * __restrict bindMaterial,
           GkModelInst    * __restrict modelInst,
           GkMaterial    ** __restrict dest);

#endif /* ak_phong_h */
