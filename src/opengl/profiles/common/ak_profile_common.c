/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "ak_profile_common.h"
#include "ak_color_or_tex.h"
#include "ak_phong.h"
#include "ak_blinn.h"
#include "ak_lambert.h"
#include "ak_constant.h"

AkResult
agk_profileCommon(AgkContext  * __restrict ctx,
                  AkContext   * __restrict actx,
                  AkEffect    * __restrict effect,
                  GkMaterial ** __restrict dest) {
  AkProfile       *profilei;
  AkProfileCommon *profile;
  AkTechniqueFx   *tfx;
  GkMaterial      *material;

  /* find profile common */
  profilei = effect->profile;
  while (profilei) {
    if (profilei->type == AK_PROFILE_TYPE_COMMON)
      break;
    profilei = profilei->next;
  }

  if (!profilei) {
    *dest = NULL;
    return AK_EFOUND;
  }

  material = NULL;
  profile  = (AkProfileCommon *)profilei;
  tfx      = profile->technique;

  if (tfx->phong) {
    material = agk_phong(actx, tfx->phong, "phong");
  } else if (tfx->blinn) {
    material = agk_blinn(actx, tfx->blinn, "blinn");
  } else if (tfx->lambert) {
    material = agk_lambert(actx, tfx->lambert, "lambert");
  } else if (tfx->constant) {
    material = agk_constant(actx, tfx->constant, "constant");
  }

  *dest = material;

  return AK_OK;
}
