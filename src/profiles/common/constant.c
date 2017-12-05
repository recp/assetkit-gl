/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../../include/agk.h"
#include "color_or_tex.h"

#include <string.h>

GkMaterial*
agk_constant(AkContext    * __restrict actx,
             AkConstantFx * __restrict constant) {
  GkMaterial *material;
  GkConstant *glconstant;

  material   = calloc(1, sizeof(*material));
  glconstant = gkMaterialNewConstant();

  if (constant->emission)
    glconstant->emission = agk_colorOrTex(actx, constant->emission);

  /* TODO: read param later */

  material->technique = &glconstant->base;
  return material;
}
