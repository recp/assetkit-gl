/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef agk_profile_common_h
#define agk_profile_common_h

#include "../../../include/agk.h"

AkResult
agk_profileCommon(AgkContext  * __restrict ctx,
                  AkContext   * __restrict actx,
                  AkEffect    * __restrict effect,
                  GkMaterial ** __restrict dest);

GkMaterial*
agk_phong(AkContext  * __restrict actx,
          AkPhong    * __restrict phong);

GkMaterial*
agk_blinn(AkContext  * __restrict actx,
          AkBlinn    * __restrict blinn);

GkMaterial*
agk_lambert(AkContext  * __restrict actx,
            AkLambert  * __restrict lambert);

GkMaterial*
agk_constant(AkContext    * __restrict actx,
             AkConstantFx * __restrict constant);

#endif /* agk_profile_common_h */
