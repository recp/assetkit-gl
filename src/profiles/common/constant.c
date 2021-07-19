/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../../include/agk.h"
#include "../../common.h"

GkMaterial*
agkConstant(AgkContext          * __restrict ctx,
            AkContext           * __restrict actx,
            AkTechniqueFxCommon * __restrict constant) {
  GkMaterial  *material;
  GkTechnique *glconstant;

  material   = calloc(1, sizeof(*material));
  glconstant = gkMaterialNewConstant();

  /* TODO: read param later */

  material->technique = glconstant;
  return material;
}
