/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "ak_color_or_tex.h"
#include <gk/texture.h>
#include <cglm/cglm.h>

/* TODO: only color for now */
void
agk_copyColorOrTex(AkContext      * __restrict actx,
                   AkFxColorOrTex * __restrict src,
                   GkColorOrTex   * __restrict dest) {
  if (src->color) {
    GkColor *color;
    color = malloc(sizeof(*color));
    glm_vec4_copy(src->color->vec, color->vec);

    dest->val    = color;
    dest->method = GK_COLOR_COLOR;
  } else if (src->texture) {
    dest->val = agk_loadTexture(actx, src->texture);
    if (dest->val)
      dest->method = GK_COLOR_TEX;
    else {
      GkColor *color;
      color = malloc(sizeof(*color));

      glm_vec4_copy((vec4){1.0f, 1.0f, 1.0f, 1.0f},
                    color->vec);

      dest->val    = color;
      dest->method = GK_COLOR_COLOR;
    }
  }
}
