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
agk_constant(AkContext    * __restrict actx,
             AkConstantFx * __restrict constant,
             const char   * routine) {
  GkConstant *glconstant;

  glconstant = gkMaterialNewConstant();

  if (constant->emission)
    agk_copyColorOrTex(actx,
                       constant->emission,
                       &glconstant->emission);

  if (constant->reflective)
    agk_copyColorOrTex(actx,
                       constant->reflective,
                       &glconstant->reflective);

  if (constant->transparent)
    agk_copyColorOrTex(actx,
                       constant->transparent,
                       &glconstant->transparent);

  /* TODO: read param later */

  if (constant->reflectivity)
    glconstant->reflectivity = *constant->reflectivity->val;
  if (constant->transparency)
    glconstant->transparency = *constant->transparency->val;
  if (constant->indexOfRefraction)
    glconstant->indexOfRefraction = *constant->indexOfRefraction->val;

  glconstant->base.subroutine = strdup(routine);

  return glconstant;
}
