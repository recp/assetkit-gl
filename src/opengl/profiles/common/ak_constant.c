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

GkMaterial*
agk_constant(AkContext    * __restrict actx,
             AkConstantFx * __restrict constant,
             const char   * routine) {
  GkMaterial *material;
  GkConstant *glconstant;

  material   = calloc(sizeof(*material), 1);
  glconstant = gkMaterialNewConstant();

  if (constant->emission) {
    glconstant->emission = calloc(sizeof(*glconstant->emission), 1);
    agk_copyColorOrTex(actx,
                       constant->emission,
                       glconstant->emission);
  }

  if (constant->indexOfRefraction)
    material->indexOfRefraction = *constant->indexOfRefraction->val;

  if (constant->transparent) {
    GkTransparent *transp;

    transp = calloc(sizeof(*material->transparent), 1);

    if (constant->transparency)
      transp->amount = *constant->transparency->val;

    transp->color = calloc(sizeof(transp->color), 1);
    agk_copyColorOrTex(actx,
                       constant->transparent,
                       transp->color);

    transp->mode = GK_ALPHA_BLEND;
    material->transparent = transp;
  }

  if (constant->reflective) {
    GkReflective *refl;

    refl = calloc(sizeof(*refl), 1);
    if (constant->reflectivity)
      refl->amount = *constant->reflectivity->val;

    refl->color = calloc(sizeof(*refl->color), 1);
    agk_copyColorOrTex(actx,
                       constant->reflective,
                       refl->color);
  }

  /* TODO: read param later */

  glconstant->base.subroutine = strdup(routine);
  material->technique = &glconstant->base;
  return material;
}
