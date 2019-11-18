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
agkBlinn(AgkContext          * __restrict ctx,
         AkContext           * __restrict actx,
         AkTechniqueFxCommon * __restrict blinn) {
  GkMaterial *material;
  GkTechnique *glblinn;

  material = calloc(1, sizeof(*material));
  glblinn  = gkMaterialNewBlinn();

  if (blinn->ambient)
    glblinn->ambient = agkColorOrTex(ctx, actx, blinn->ambient);

  if (blinn->diffuse)
    glblinn->diffuse = agkColorOrTex(ctx, actx, blinn->diffuse);

  if (blinn->specular)
    glblinn->specular = agkColorOrTex(ctx, actx, blinn->specular);

  if (blinn->emission)
    glblinn->emission = agkColorOrTex(ctx, actx, blinn->emission);

  if (blinn->shininess)
    glblinn->shininess = *blinn->shininess->val;

  /* TODO: read param later */

  material->technique = glblinn;
  return material;
}
