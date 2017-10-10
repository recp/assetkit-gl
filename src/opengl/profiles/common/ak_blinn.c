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

GkMaterial*
agk_blinn(AkContext  * __restrict actx,
          AkBlinn    * __restrict blinn,
          const char * routine) {
  GkMaterial *material;
  GkBlinn    *glblinn;

  material = calloc(sizeof(*material), 1);
  glblinn  = gkMaterialNewBlinn();

  if (blinn->ambient) {
    glblinn->ambient = calloc(sizeof(*glblinn->ambient), 1);
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

  if (blinn->shininess)
    glblinn->shininess = *blinn->shininess->val;

  if (blinn->base.indexOfRefraction)
    material->indexOfRefraction = *blinn->base.indexOfRefraction->val;

  if (blinn->base.transparent) {
    AkTransparent *aktransp;
    GkTransparent *transp;

    transp   = calloc(sizeof(*material->transparent), 1);
    aktransp = blinn->base.transparent;

    if (aktransp->amount)
      transp->amount = *aktransp->amount->val;

    if (transp->color) {
      transp->color = calloc(sizeof(*transp->color), 1);
      agk_copyColorOrTex(actx,
                         aktransp->color,
                         transp->color);
    }

    transp->cutoff = aktransp->cutoff;
    transp->mode   = GK_ALPHA_BLEND;

    material->transparent = transp;
  }

  if (blinn->base.reflective) {
    AkReflective *akrefl;
    GkReflective *refl;

    refl   = calloc(sizeof(*refl), 1);
    akrefl = blinn->base.reflective;
    
    if (akrefl->amount)
      refl->amount = *akrefl->amount->val;

    if (akrefl->color) {
      refl->color = calloc(sizeof(*refl->color), 1);
      agk_copyColorOrTex(actx,
                         akrefl->color,
                         refl->color);
    }

    material->reflective = refl;
  }

  /* TODO: read param later */

  glblinn->base.subroutine = strdup(routine);
  material->technique = &glblinn->base;
  return material;
}
