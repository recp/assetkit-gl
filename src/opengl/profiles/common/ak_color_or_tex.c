/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "ak_color_or_tex.h"
#include <gk/texture.h>
#include <cglm/cglm.h>

GkColorOrTex*
agk_colorOrTex(AkContext      * __restrict actx,
               AkFxColorOrTex * __restrict src) {
  GkColorOrTex *crtx;

  crtx = calloc(sizeof(*crtx), 1);
  if (src->color) {
    GkColor *color;
    color = malloc(sizeof(*color));
    glm_vec4_copy(src->color->vec, color->vec);

    crtx->val    = color;
    crtx->method = GK_COLOR_COLOR;
  } else if (src->texture) {
    crtx->val = agk_loadTexture(actx, src->texture);
    if (crtx->val) {
      crtx->method = GK_COLOR_TEX;
    } else {
      GkColor *color;
      color = malloc(sizeof(*color));

      glm_vec4_copy(GLM_VEC4_ONE, color->vec);

      crtx->val    = color;
      crtx->method = GK_COLOR_COLOR;
    }
  }
  
  return crtx;
}
