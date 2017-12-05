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
agk_phong(AkContext  * __restrict actx,
          AkPhong    * __restrict phong) {
  GkMaterial *material;
  GkPhong    *glphong;

  material = calloc(1, sizeof(*material));
  glphong  = gkMaterialNewPhong();

  if (phong->ambient)
    glphong->ambient = agk_colorOrTex(actx, phong->ambient);

  if (phong->diffuse)
    glphong->diffuse = agk_colorOrTex(actx, phong->diffuse);

  if (phong->specular)
    glphong->specular = agk_colorOrTex(actx, phong->specular);

  if (phong->emission)
    glphong->emission = agk_colorOrTex(actx, phong->emission);

  if (phong->shininess)
    glphong->shininess = *phong->shininess->val;

  /* TODO: read param later */

  material->technique = &glphong->base;
  return material;
}
