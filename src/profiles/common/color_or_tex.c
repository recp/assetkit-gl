/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "color_or_tex.h"
#include <gk/texture.h>
#include <cglm/cglm.h>

GkColorOrTex*
agk_colorOrTex(AkContext      * __restrict actx,
               AkFxColorOrTex * __restrict src) {
  GkColorOrTex *crtx;

  crtx = calloc(1, sizeof(*crtx));
  if (src->color) {
    GkColor *color;
    color = malloc(sizeof(*color));
    glm_vec4_copy(src->color->vec, color->vec);

    crtx->val    = color;
    crtx->method = GK_COLOR_COLOR;
  } else if (src->texture) {
    crtx->val    = agk_loadTexture(actx, src->texture);
    crtx->method = GK_COLOR_TEX;
  }
  
  return crtx;
}
