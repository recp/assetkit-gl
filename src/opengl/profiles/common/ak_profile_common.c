/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "ak_profile_common.h"
#include "ak_color_or_tex.h"
#include "ak_phong.h"

AkResult
ak_glProfileCommon(AkGLContext    * __restrict ctx,
                   AkBindMaterial * __restrict bindMaterial,
                   AkProfile      * __restrict profileCommon,
                   GkMaterial    ** __restrict dest) {
  AkProfileCommon *profile;
  AkTechniqueFx   *tfx;
  GkMaterial      *material;

  material = calloc(sizeof(*material), 1);
  profile  = (AkProfileCommon *)profileCommon;
  tfx      = profile->technique;

  if (tfx->phong) {
    GkPhong *glphong;
    glphong = ak_glPhong(tfx->phong, "phong");

    if (glphong)
      material->technique = &glphong->base;
  }

  if (tfx->blinn) {
    GkBlinn *glblinn;
    glblinn = ak_glPhong(tfx->blinn, "blinn");

    if (glblinn)
      material->technique = &glblinn->base;
  }

  *dest = material;

  return AK_OK;
}
