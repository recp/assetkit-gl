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

GkMaterial*
agk_phong(AkContext  * __restrict actx,
          AkPhong    * __restrict phong,
          const char * routine) {
  GkMaterial *material;
  GkPhong    *glphong;

  material = calloc(sizeof(*material), 1);
  glphong  = gkMaterialNewPhong();

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

  if (phong->shininess)
    glphong->shininess = *phong->shininess->val;

  if (phong->base.indexOfRefraction)
    material->indexOfRefraction = *phong->base.indexOfRefraction->val;

  if (phong->base.transparent) {
    AkTransparent *aktransp;
    GkTransparent *transp;

    transp   = calloc(sizeof(*material->transparent), 1);
    aktransp = phong->base.transparent;

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

  if (phong->base.reflective) {
    AkReflective *akrefl;
    GkReflective *refl;

    refl   = calloc(sizeof(*refl), 1);
    akrefl = phong->base.reflective;

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

  glphong->base.subroutine = strdup(routine);
  material->technique = &glphong->base;
  return material;
}
