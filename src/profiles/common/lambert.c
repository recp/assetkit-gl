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
agk_lambert(AkContext  * __restrict actx,
            AkLambert  * __restrict lambert) {
  GkMaterial *material;
  GkLambert  *gllambert;

  material  = calloc(sizeof(*material), 1);
  gllambert = gkMaterialNewLambert();

  if (lambert->ambient)
    gllambert->ambient = agk_colorOrTex(actx, lambert->ambient);

  if (lambert->diffuse)
    gllambert->diffuse = agk_colorOrTex(actx, lambert->diffuse);

  if (lambert->emission)
    gllambert->emission = agk_colorOrTex(actx, lambert->emission);

  /* TODO: read param later */

  material->technique = &gllambert->base;
  return material;
}
