/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../../include/agk.h"
#include "../../common.h"

GkMaterial*
agkMetalRough(AgkContext          * __restrict ctx,
              AkContext           * __restrict actx,
              AkTechniqueFxCommon * __restrict akmat) {
  GkMaterial             *material;
  GkMetalRough           *metalRough;
  AkMaterialMetallicProp *metalness;
  AkMaterialMetallicProp *roughness;

  material   = calloc(1, sizeof(*material));
  metalRough = gkMaterialNewMetalRough();

  glm_vec4_copy(akmat->albedo->color->vec, metalRough->albedo.vec);

  if (!(metalness = akmat->metalness)) { return material; }
  if (!(roughness = akmat->roughness)) { return material; }

  metalRough->metallic      = metalness->intensity;
  metalRough->roughness     = roughness->intensity;
  metalRough->albedoMap     = agkLoadTexture(ctx, actx, akmat->albedo->texture);
  metalRough->metalRoughMap = agkLoadTexture(ctx, actx, metalness->tex);

  material->technique = &metalRough->base;
  return material;
}
