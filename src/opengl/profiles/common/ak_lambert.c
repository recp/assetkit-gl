/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "ak_lambert.h"
#include "../../../../include/ak-opengl.h"
#include "ak_color_or_tex.h"

#include <string.h>

GkLambert*
agk_lambert(AkLambert * __restrict lambert,
            const char *routine) {
  GkLambert *gllambert;

  gllambert = gkMaterialNewLambert();

  if (lambert->ambient)
    agk_copyColorOrTex(lambert->ambient,
                       &gllambert->ambient);
  if (lambert->diffuse)
    agk_copyColorOrTex(lambert->diffuse,
                       &gllambert->diffuse);

  if (lambert->emission)
    agk_copyColorOrTex(lambert->emission,
                       &gllambert->emission);

  if (lambert->reflective)
    agk_copyColorOrTex(lambert->reflective,
                       &gllambert->reflective);

  if (lambert->transparent)
    agk_copyColorOrTex(lambert->transparent,
                       &gllambert->transparent);

  /* TODO: read param later */

  if (lambert->reflectivity)
    gllambert->reflectivity = *lambert->reflectivity->val;
  if (lambert->transparency)
    gllambert->transparency = *lambert->transparency->val;
  if (lambert->indexOfRefraction)
    gllambert->indexOfRefraction = *lambert->indexOfRefraction->val;

  gllambert->base.subroutine = malloc(sizeof(char) * 6);
  strcpy(gllambert->base.subroutine, routine);
  
  return gllambert;
}
