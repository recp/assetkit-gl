/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../../include/agk.h"
#include "color_or_tex.h"

GkMaterial*
agk_specGloss(AkContext            * __restrict actx,
              AkSpecularGlossiness * __restrict akmat) {
  GkMaterial   *material;
  GkSpecGloss  *specGloss;

  material  = calloc(1, sizeof(*material));
  specGloss = gkMaterialNewSpecGloss();

  glm_vec4_copy(akmat->diffuse.vec,  specGloss->diffuse.vec);
  glm_vec4_copy(akmat->specular.vec, specGloss->specular.vec);

  specGloss->gloss        = akmat->glossiness;
  specGloss->diffuseMap   = agk_loadTexture(actx, akmat->diffuseTex);
  specGloss->specGlossMap = agk_loadTexture(actx, akmat->specGlossTex);

  /* Other Properties */

  if (akmat->base.emission)
    specGloss->base.emission = agk_colorOrTex(actx, akmat->base.emission);

  if (akmat->base.occlusion) {
    GkOcclusion *occlusion;

    occlusion           = calloc(1, sizeof(*occlusion));
    occlusion->tex      = agk_loadTexture(actx, akmat->base.occlusion->tex);
    occlusion->strength = akmat->base.occlusion->strength;

    specGloss->base.occlusion = occlusion;
  }

  if (akmat->base.normal) {
    GkNormalMap *normal;

    normal        = calloc(1, sizeof(*normal));
    normal->tex   = agk_loadTexture(actx, akmat->base.normal->tex);
    normal->scale = akmat->base.normal->scale;

    specGloss->base.normal = normal;
  }

  material->technique = &specGloss->base;
  return material;
}