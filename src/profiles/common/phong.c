/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../../include/agk.h"
#include "../../common.h"

GkMaterial*
agkPhong(AgkContext          * __restrict ctx,
         AkContext           * __restrict actx,
         AkTechniqueFxCommon * __restrict phong) {
  GkMaterial  *material;
  GkTechnique *glphong;

  material = calloc(1, sizeof(*material));
  glphong  = gkMaterialNewPhong();

  if (phong->ambient)
    glphong->ambient = agkColorOrTex(ctx, actx, phong->ambient);

  if (phong->diffuse)
    glphong->diffuse = agkColorOrTex(ctx, actx, phong->diffuse);

  if (phong->specular)
    glphong->specular = agkColorOrTex(ctx, actx, phong->specular);

  if (phong->shininess)
    glphong->shininess = *phong->shininess->val;

  /* TODO: read param later */

  material->technique = glphong;
  return material;
}
