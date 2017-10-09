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

  if (constant->base.indexOfRefraction)
    material->indexOfRefraction = *constant->base.indexOfRefraction->val;

  if (constant->base.transparent) {
    AkTransparent *aktransp;
    GkTransparent *transp;

    transp   = calloc(sizeof(*material->transparent), 1);
    aktransp = constant->base.transparent;

    if (aktransp->amount)
      transp->amount = *aktransp->amount->val;

    if (transp->color) {
      transp->color = calloc(sizeof(transp->color), 1);
      agk_copyColorOrTex(actx,
                         aktransp->color,
                         transp->color);
    }

    transp->cutoff = aktransp->cutoff;
    transp->mode   = GK_ALPHA_BLEND;

    material->transparent = transp;
  }

  if (constant->base.reflective) {
    AkReflective *akrefl;
    GkReflective *refl;

    refl   = calloc(sizeof(*refl), 1);
    akrefl = constant->base.reflective;

    if (akrefl->amount)
      refl->amount = *akrefl->amount->val;

    if (akrefl->color) {
      refl->color = calloc(sizeof(*refl->color), 1);
      agk_copyColorOrTex(actx,
                         akrefl->color,
                         refl->color);
    }
  }

  /* TODO: read param later */

  glconstant->base.subroutine = strdup(routine);
  material->technique = &glconstant->base;
  return material;
}
