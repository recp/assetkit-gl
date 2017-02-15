/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef agk_profile_common_h
#define agk_profile_common_h

#include "../../../../include/ak-opengl.h"

AkResult
agk_profileCommon(AgkContext     * __restrict ctx,
                  AkBindMaterial * __restrict bindMaterial,
                  AkProfile      * __restrict profile,
                  GkMaterial    ** __restrict dest);

#endif /* agk_profile_common_h */
