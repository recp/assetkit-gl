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
agk_lambert(AkContext  * __restrict actx,
            AkLambert  * __restrict lambert,
            const char * routine) {
  GkLambert *gllambert;

  gllambert = gkMaterialNewLambert();

  if (lambert->ambient) {
    gllambert->ambient = calloc(sizeof(*gllambert->ambient), 1);
    agk_copyColorOrTex(actx,
                       lambert->ambient,
                       gllambert->ambient);
  }

  if (lambert->diffuse) {
    gllambert->diffuse = calloc(sizeof(*gllambert->diffuse), 1);
    agk_copyColorOrTex(actx,
                       lambert->diffuse,
                       gllambert->diffuse);
  }

  if (lambert->emission) {
    gllambert->emission = calloc(sizeof(*gllambert->emission), 1);
    agk_copyColorOrTex(actx,
                       lambert->emission,
                       gllambert->emission);
  }

  if (lambert->reflective) {
    gllambert->reflective = calloc(sizeof(*gllambert->reflective), 1);
    agk_copyColorOrTex(actx,
                       lambert->reflective,
                       gllambert->reflective);
  }

  if (lambert->transparent) {
    gllambert->transparent = calloc(sizeof(*gllambert->transparent), 1);
    agk_copyColorOrTex(actx,
                       lambert->transparent,
                       gllambert->transparent);
  }

  /* TODO: read param later */

  if (lambert->reflectivity)
    gllambert->reflectivity = *lambert->reflectivity->val;
  if (lambert->transparency)
    gllambert->transparency = *lambert->transparency->val;
  if (lambert->indexOfRefraction)
    gllambert->indexOfRefraction = *lambert->indexOfRefraction->val;

  gllambert->base.subroutine = strdup(routine);
  
  return gllambert;
}
