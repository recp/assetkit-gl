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

  if (lambert->indexOfRefraction)
    material->indexOfRefraction = *lambert->indexOfRefraction->val;

  if (lambert->transparent) {
    GkTransparent *transp;

    transp = calloc(sizeof(*material->transparent), 1);

    if (lambert->transparency)
      transp->amount = *lambert->transparency->val;

    transp->color = calloc(sizeof(transp->color), 1);
    agk_copyColorOrTex(actx,
                       lambert->transparent,
                       transp->color);

    transp->mode = GK_ALPHA_BLEND;
    material->transparent = transp;
  }

  if (lambert->reflective) {
    GkReflective *refl;

    refl = calloc(sizeof(*refl), 1);
    if (lambert->reflectivity)
      refl->amount = *lambert->reflectivity->val;

    refl->color = calloc(sizeof(*refl->color), 1);
    agk_copyColorOrTex(actx,
                       lambert->reflective,
                       refl->color);
  }

  /* TODO: read param later */


  gllambert->base.subroutine = strdup(routine);
  material->technique = &gllambert->base;
  return material;
}
