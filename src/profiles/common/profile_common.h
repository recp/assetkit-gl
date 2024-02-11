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
agkPhong(AgkContext           * __restrict ctx,
         AkContext            * __restrict actx,
          AkTechniqueFxCommon * __restrict phong);

GkMaterial*
agkBlinn(AgkContext           * __restrict ctx,
         AkContext            * __restrict actx,
          AkTechniqueFxCommon * __restrict blinn);

GkMaterial*
agkLambert(AgkContext          * __restrict ctx,
           AkContext           * __restrict actx,
           AkTechniqueFxCommon * __restrict lambert);

GkMaterial*
agkConstant(AgkContext          * __restrict ctx,
            AkContext           * __restrict actx,
            AkTechniqueFxCommon * __restrict constant);

GkMaterial*
agkMetalRough(AgkContext          * __restrict ctx,
              AkContext           * __restrict actx,
              AkTechniqueFxCommon * __restrict akmat);

GkMaterial*
agkSpecGloss(AgkContext          * __restrict ctx,
             AkContext           * __restrict actx,
             AkTechniqueFxCommon * __restrict akmat);

#endif /* agk_profile_common_h */
