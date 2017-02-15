/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "ak_blinn.h"
#include "../../../../include/ak-opengl.h"
#include "ak_color_or_tex.h"

#include <string.h>

GkBlinn*
agk_blinn(AkBlinn * __restrict blinn,
          const char *routine) {
  GkBlinn *glblinn;

  glblinn = gkMaterialNewBlinn();

  if (blinn->ambient)
    agk_copyColorOrTex(blinn->ambient,
                       &glblinn->ambient);
  if (blinn->diffuse)
    agk_copyColorOrTex(blinn->diffuse,
                       &glblinn->diffuse);

  if (blinn->specular)
    agk_copyColorOrTex(blinn->specular,
                       &glblinn->specular);

  if (blinn->emission)
    agk_copyColorOrTex(blinn->emission,
                       &glblinn->emission);

  if (blinn->reflective)
    agk_copyColorOrTex(blinn->reflective,
                       &glblinn->reflective);

  if (blinn->transparent)
    agk_copyColorOrTex(blinn->transparent,
                       &glblinn->transparent);

  /* TODO: read param later */

  if (blinn->shininess)
    glblinn->shininess = *blinn->shininess->val;
  if (blinn->reflectivity)
    glblinn->reflectivity = *blinn->reflectivity->val;
  if (blinn->transparency)
    glblinn->transparency = *blinn->transparency->val;
  if (blinn->indexOfRefraction)
    glblinn->indexOfRefraction = *blinn->indexOfRefraction->val;

  glblinn->base.subroutine = malloc(sizeof(char) * 6);
  strcpy(glblinn->base.subroutine, routine);
  
  return glblinn;
}
