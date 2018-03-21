/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "metal_rough.h"

GkMaterial*
agk_metalRough(AkContext           * __restrict actx,
               AkMetallicRoughness * __restrict akmat) {
  GkMaterial   *material;
  GkMetalRough *metalRough;

  material   = calloc(1, sizeof(*material));
  metalRough = gkMaterialNewMetalRough();

  glm_vec4_copy(akmat->baseColor.vec, metalRough->albedo.vec);

  metalRough->metallic      = akmat->metallic;
  metalRough->roughness     = akmat->roughness;
  metalRough->albedoMap     = agk_loadTexture(actx, akmat->baseColorTex);
  metalRough->metalRoughMap = agk_loadTexture(actx, akmat->metalRoughTex);

  material->technique = &metalRough->base;
  return material;
}
