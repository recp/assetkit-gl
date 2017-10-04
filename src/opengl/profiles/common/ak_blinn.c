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
agk_blinn(AkContext  * __restrict actx,
          AkBlinn    * __restrict blinn,
          const char * routine) {
  GkBlinn *glblinn;

  glblinn = gkMaterialNewBlinn();

  if (blinn->ambient) {
    glblinn->ambient = calloc(sizeof(*blinn->ambient), 1);
    agk_copyColorOrTex(actx,
                       blinn->ambient,
                       glblinn->ambient);
  }

  if (blinn->diffuse) {
    glblinn->diffuse = calloc(sizeof(*glblinn->diffuse), 1);
    agk_copyColorOrTex(actx,
                       blinn->diffuse,
                       glblinn->diffuse);
  }

  if (blinn->specular) {
    glblinn->specular = calloc(sizeof(*glblinn->specular), 1);
    agk_copyColorOrTex(actx,
                       blinn->specular,
                       glblinn->specular);
  }

  if (blinn->emission) {
    glblinn->emission = calloc(sizeof(*glblinn->emission), 1);
    agk_copyColorOrTex(actx,
                       blinn->emission,
                       glblinn->emission);
  }

  if (blinn->reflective) {
    glblinn->reflective = calloc(sizeof(*glblinn->reflective), 1);
    agk_copyColorOrTex(actx,
                       blinn->reflective,
                       glblinn->reflective);
  }

  if (blinn->transparent) {
    glblinn->transparent = calloc(sizeof(*glblinn->transparent), 1);
    agk_copyColorOrTex(actx,
                       blinn->transparent,
                       glblinn->transparent);
  }

  /* TODO: read param later */

  if (blinn->shininess)
    glblinn->shininess = *blinn->shininess->val;
  if (blinn->reflectivity)
    glblinn->reflectivity = *blinn->reflectivity->val;
  if (blinn->transparency)
    glblinn->transparency = *blinn->transparency->val;
  if (blinn->indexOfRefraction)
    glblinn->indexOfRefraction = *blinn->indexOfRefraction->val;

  glblinn->base.subroutine = strdup(routine);
  
  return glblinn;
}
