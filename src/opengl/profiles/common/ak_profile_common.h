/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef ak_profile_common_h
#define ak_profile_common_h

#include "../../../../include/ak-opengl.h"

AkResult
ak_glProfileCommon(AkGLContext    * __restrict ctx,
                   AkBindMaterial * __restrict bindMaterial,
                   AkProfile      * __restrict profile,
                   GkMaterial    ** __restrict dest);

#endif /* ak_profile_common_h */
