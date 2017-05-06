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

  if (phong->ambient)
    agk_copyColorOrTex(actx,
                       phong->ambient,
                       &glphong->ambient);
  if (phong->diffuse)
    agk_copyColorOrTex(actx,
                       phong->diffuse,
                       &glphong->diffuse);

  if (phong->specular)
    agk_copyColorOrTex(actx,
                       phong->specular,
                       &glphong->specular);

  if (phong->emission)
    agk_copyColorOrTex(actx,
                       phong->emission,
                       &glphong->emission);

  if (phong->reflective)
    agk_copyColorOrTex(actx,
                       phong->reflective,
                       &glphong->reflective);

  if (phong->transparent)
    agk_copyColorOrTex(actx,
                       phong->transparent,
                       &glphong->transparent);

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
