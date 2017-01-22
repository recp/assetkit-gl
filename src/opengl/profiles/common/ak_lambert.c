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
ak_glLambert(AkLambert * __restrict lambert,
             const char *routine) {
  GkLambert *gllambert;

  gllambert = calloc(sizeof(*gllambert), 1);
  gllambert->base.type = GK_MATERIAL_LAMBERT;

  if (lambert->ambient)
    ak_glCopyColorOrTex(lambert->ambient,
                        &gllambert->ambient);
  if (lambert->diffuse)
    ak_glCopyColorOrTex(lambert->diffuse,
                        &gllambert->diffuse);

  if (lambert->emission)
    ak_glCopyColorOrTex(lambert->emission,
                        &gllambert->emission);

  if (lambert->reflective)
    ak_glCopyColorOrTex(lambert->reflective,
                        &gllambert->reflective);

  if (lambert->transparent)
    ak_glCopyColorOrTex(lambert->transparent,
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
