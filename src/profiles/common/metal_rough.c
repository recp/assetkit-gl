/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../../include/agk.h"
#include "color_or_tex.h"

GkMaterial*
agk_metalRough(AkContext           * __restrict actx,
               AkMetallicRoughness * __restrict akmat) {
  GkMaterial   *material;
  GkMetalRough *metalRough;

  material   = calloc(1, sizeof(*material));
  metalRough = gkMaterialNewMetalRough();

  glm_vec4_copy(akmat->albedo.vec, metalRough->albedo.vec);

  metalRough->metallic      = akmat->metallic;
  metalRough->roughness     = akmat->roughness;
  metalRough->albedoMap     = agkLoadTexture(actx, akmat->albedoTex);
  metalRough->metalRoughMap = agkLoadTexture(actx, akmat->metalRoughTex);

  /* Other Properties */

  if (akmat->base.emission)
    metalRough->base.emission = agk_colorOrTex(actx, akmat->base.emission);

  if (akmat->base.occlusion) {
    GkOcclusion *occlusion;

    occlusion           = calloc(1, sizeof(*occlusion));
    occlusion->tex      = agkLoadTexture(actx, akmat->base.occlusion->tex);
    occlusion->strength = akmat->base.occlusion->strength;

    metalRough->base.occlusion = occlusion;
  }

  if (akmat->base.normal) {
    GkNormalMap *normal;

    normal        = calloc(1, sizeof(*normal));
    normal->tex   = agkLoadTexture(actx, akmat->base.normal->tex);
    normal->scale = akmat->base.normal->scale;

    metalRough->base.normal = normal;
  }

  material->technique = &metalRough->base;
  return material;
}
