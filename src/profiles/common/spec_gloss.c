/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../../include/agk.h"
#include "../../common.h"

GkMaterial*
agkSpecGloss(AgkContext          * __restrict ctx,
             AkContext           * __restrict actx,
             AkTechniqueFxCommon * __restrict akmat) {
  GkMaterial   *material;
  GkSpecGloss  *specGloss;
  AkMaterialSpecularProp *specularProp;

  material  = calloc(1, sizeof(*material));
  specGloss = gkMaterialNewSpecGloss();

  if (!(specularProp = akmat->specular)) { return material; }

  glm_vec4_copy(akmat->diffuse->color->vec, specGloss->diffuse.vec);

  glm_vec4_copy(specularProp->colorFactor.vec, specGloss->specular.vec);

  specGloss->gloss        = specularProp->strength;
  specGloss->diffuseMap   = agkLoadTexture(ctx, actx, specularProp->colorTex);
  specGloss->specGlossMap = agkLoadTexture(ctx, actx, specularProp->specularTex);

  material->technique = &specGloss->base;
  return material;
}
