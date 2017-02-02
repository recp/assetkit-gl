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
    glblinn = ak_glBlinn(tfx->blinn, "blinn");

    if (glblinn)
      material->technique = &glblinn->base;
  }

  if (tfx->lambert) {
    GkLambert *gllambert;
    gllambert = ak_glLambert(tfx->lambert, "lambert");

    if (gllambert)
      material->technique = &gllambert->base;
  }

  if (tfx->constant) {
    GkConstant *glconstant;
    glconstant = ak_glConstant(tfx->constant, "constant");

    if (glconstant)
      material->technique = &glconstant->base;
  }

  *dest = material;

  return AK_OK;
}
