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
agkConstant(AgkContext          * __restrict ctx,
            AkContext           * __restrict actx,
            AkTechniqueFxCommon * __restrict constant) {
  GkMaterial  *material;
  GkTechnique *glconstant;

  material   = calloc(1, sizeof(*material));
  glconstant = gkMaterialNewConstant();

  if (constant->emission)
    glconstant->emission = agkColorOrTex(ctx, actx, constant->emission);

  /* TODO: read param later */

  material->technique = glconstant;
  return material;
}
