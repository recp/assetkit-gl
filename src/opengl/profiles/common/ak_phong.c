/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "ak_phong.h"
#include "../../../../include/ak-opengl.h"
#include "ak_color_or_tex.h"

#include <string.h>

GkPhong*
agk_phong(AkContext  * __restrict actx,
          AkPhong    * __restrict phong,
          const char * routine) {
  GkPhong *glphong;

  glphong = gkMaterialNewPhong();

  if (phong->ambient) {
    glphong->ambient = calloc(sizeof(*glphong->ambient), 1);
    agk_copyColorOrTex(actx,
                       phong->ambient,
                       glphong->ambient);
  }

  if (phong->diffuse) {
    glphong->diffuse = calloc(sizeof(*glphong->diffuse), 1);
    agk_copyColorOrTex(actx,
                       phong->diffuse,
                       glphong->diffuse);
  }

  if (phong->specular) {
    glphong->specular = calloc(sizeof(*glphong->specular), 1);
    agk_copyColorOrTex(actx,
                       phong->specular,
                       glphong->specular);
  }

  if (phong->emission) {
    glphong->emission = calloc(sizeof(*glphong->emission), 1);
    agk_copyColorOrTex(actx,
                       phong->emission,
                       glphong->emission);
  }

  if (phong->reflective) {
    glphong->reflective = calloc(sizeof(*glphong->reflective), 1);
    agk_copyColorOrTex(actx,
                       phong->reflective,
                       glphong->reflective);
  }

  if (phong->transparent) {
    glphong->transparent = calloc(sizeof(*glphong->transparent), 1);
    agk_copyColorOrTex(actx,
                       phong->transparent,
                       glphong->transparent);
  }

  /* TODO: read param later */

  if (phong->shininess)
    glphong->shininess = *phong->shininess->val;
  if (phong->reflectivity)
    glphong->reflectivity = *phong->reflectivity->val;
  if (phong->transparency)
    glphong->transparency = *phong->transparency->val;
  if (phong->indexOfRefraction)
    glphong->indexOfRefraction = *phong->indexOfRefraction->val;

  glphong->base.subroutine = strdup(routine);

  return glphong;
}
