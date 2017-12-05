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
agk_blinn(AkContext  * __restrict actx,
          AkBlinn    * __restrict blinn) {
  GkMaterial *material;
  GkBlinn    *glblinn;

  material = calloc(1, sizeof(*material));
  glblinn  = gkMaterialNewBlinn();

  if (blinn->ambient)
    glblinn->ambient = agk_colorOrTex(actx, blinn->ambient);

  if (blinn->diffuse)
    glblinn->diffuse = agk_colorOrTex(actx, blinn->diffuse);

  if (blinn->specular)
    glblinn->specular = agk_colorOrTex(actx, blinn->specular);

  if (blinn->emission)
    glblinn->emission = agk_colorOrTex(actx, blinn->emission);

  if (blinn->shininess)
    glblinn->shininess = *blinn->shininess->val;

  /* TODO: read param later */

  material->technique = &glblinn->base;
  return material;
}
