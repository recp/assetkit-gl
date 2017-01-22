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
ak_glPhong(AkPhong * __restrict phong,
           const char *routine) {
  GkPhong *glphong;

  glphong = calloc(sizeof(*glphong), 1);
  glphong->base.type = GK_MATERIAL_PHONG;

  if (phong->ambient)
    ak_glCopyColorOrTex(phong->ambient,
                        &glphong->ambient);
  if (phong->diffuse)
    ak_glCopyColorOrTex(phong->diffuse,
                        &glphong->diffuse);

  if (phong->specular)
    ak_glCopyColorOrTex(phong->specular,
                        &glphong->specular);

  if (phong->emission)
    ak_glCopyColorOrTex(phong->emission,
                        &glphong->emission);

  if (phong->reflective)
    ak_glCopyColorOrTex(phong->reflective,
                        &glphong->reflective);

  if (phong->transparent)
    ak_glCopyColorOrTex(phong->transparent,
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

  glphong->base.subroutine = malloc(sizeof(char) * 6);
  strcpy(glphong->base.subroutine, routine);

  return glphong;
}
