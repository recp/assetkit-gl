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

  profile  = (AkProfileCommon *)profilei;
  material = calloc(sizeof(*material), 1);
  tfx      = profile->technique;

  if (tfx->phong) {
    GkPhong *glphong;
    glphong = agk_phong(actx, tfx->phong, "phong");

    if (glphong)
      material->technique = &glphong->base;
  }

  if (tfx->blinn) {
    GkBlinn *glblinn;
    glblinn = agk_blinn(actx, tfx->blinn, "blinn");

    if (glblinn)
      material->technique = &glblinn->base;
  }

  if (tfx->lambert) {
    GkLambert *gllambert;
    gllambert = agk_lambert(actx, tfx->lambert, "lambert");

    if (gllambert)
      material->technique = &gllambert->base;
  }

  if (tfx->constant) {
    GkConstant *glconstant;
    glconstant = agk_constant(actx, tfx->constant, "constant");

    if (glconstant)
      material->technique = &glconstant->base;
  }

  *dest = material;

  return AK_OK;
}
