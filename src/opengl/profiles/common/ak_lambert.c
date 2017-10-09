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

GkMaterial*
agk_lambert(AkContext  * __restrict actx,
            AkLambert  * __restrict lambert,
            const char * routine) {
  GkMaterial *material;
  GkLambert  *gllambert;

  material  = calloc(sizeof(*material), 1);
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

  if (lambert->base.indexOfRefraction)
    material->indexOfRefraction = *lambert->base.indexOfRefraction->val;

  if (lambert->base.transparent) {
    AkTransparent *aktransp;
    GkTransparent *transp;

    transp   = calloc(sizeof(*material->transparent), 1);
    aktransp = lambert->base.transparent;

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

  if (lambert->base.reflective) {
    AkReflective *akrefl;
    GkReflective *refl;

    refl   = calloc(sizeof(*refl), 1);
    akrefl = lambert->base.reflective;

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


  gllambert->base.subroutine = strdup(routine);
  material->technique = &gllambert->base;
  return material;
}
