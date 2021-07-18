/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../../include/agk.h"
#include "../../common.h"

GkMaterial*
agkSpecGloss(AgkContext           * __restrict ctx,
             AkContext            * __restrict actx,
             AkSpecularGlossiness * __restrict akmat) {
  GkMaterial   *material;
  GkSpecGloss  *specGloss;

  material  = calloc(1, sizeof(*material));
  specGloss = gkMaterialNewSpecGloss();

  glm_vec4_copy(akmat->diffuse.vec,  specGloss->diffuse.vec);
  glm_vec4_copy(akmat->specular.vec, specGloss->specular.vec);

  specGloss->gloss        = akmat->glossiness;
  specGloss->diffuseMap   = agkLoadTexture(ctx, actx, akmat->diffuseTex);
  specGloss->specGlossMap = agkLoadTexture(ctx, actx, akmat->specGlossTex);

  /* Other Properties */

  if (akmat->base.emission)
    specGloss->base.emission = agkColorOrTex(ctx, actx, akmat->base.emission);

  material->technique = &specGloss->base;
  return material;
}
