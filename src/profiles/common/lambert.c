/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../../include/agk.h"
#include "../../common.h"

GkMaterial*
agkLambert(AgkContext          * __restrict ctx,
           AkContext           * __restrict actx,
           AkTechniqueFxCommon * __restrict lambert) {
  GkMaterial  *material;
  GkTechnique *gllambert;

  material  = calloc(1, sizeof(*material));
  gllambert = gkMaterialNewLambert();

  if (lambert->ambient)
    gllambert->ambient = agkColorOrTex(ctx, actx, lambert->ambient);

  if (lambert->diffuse)
    gllambert->diffuse = agkColorOrTex(ctx, actx, lambert->diffuse);

  /* TODO: read param later */

  material->technique = gllambert;
  return material;
}
