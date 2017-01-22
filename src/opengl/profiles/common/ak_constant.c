/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "ak_constant.h"
#include "../../../../include/ak-opengl.h"
#include "ak_color_or_tex.h"

#include <string.h>

GkConstant*
ak_glConstant(AkConstantFx * __restrict constant,
              const char *routine) {
  GkConstant *glconstant;

  glconstant = calloc(sizeof(*glconstant), 1);
  glconstant->base.type = GK_MATERIAL_LAMBERT;

  if (constant->emission)
    ak_glCopyColorOrTex(constant->emission,
                        &glconstant->emission);

  if (constant->reflective)
    ak_glCopyColorOrTex(constant->reflective,
                        &glconstant->reflective);

  if (constant->transparent)
    ak_glCopyColorOrTex(constant->transparent,
                        &glconstant->transparent);

  /* TODO: read param later */

  if (constant->reflectivity)
    glconstant->reflectivity = *constant->reflectivity->val;
  if (constant->transparency)
    glconstant->transparency = *constant->transparency->val;
  if (constant->indexOfRefraction)
    glconstant->indexOfRefraction = *constant->indexOfRefraction->val;

  glconstant->base.subroutine = malloc(sizeof(char) * 6);
  strcpy(glconstant->base.subroutine, routine);

  return glconstant;
}
