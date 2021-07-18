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
              AkMetallicRoughness * __restrict akmat) {
  GkMaterial   *material;
  GkMetalRough *metalRough;

  material   = calloc(1, sizeof(*material));
  metalRough = gkMaterialNewMetalRough();

  glm_vec4_copy(akmat->albedo.vec, metalRough->albedo.vec);

  metalRough->metallic      = akmat->metallic;
  metalRough->roughness     = akmat->roughness;
  metalRough->albedoMap     = agkLoadTexture(ctx, actx, akmat->albedoTex);
  metalRough->metalRoughMap = agkLoadTexture(ctx, actx, akmat->metalRoughTex);

  /* Other Properties */

  if (akmat->base.emission)
    metalRough->base.emission = agkColorOrTex(ctx, actx, akmat->base.emission);

  material->technique = &metalRough->base;
  return material;
}
