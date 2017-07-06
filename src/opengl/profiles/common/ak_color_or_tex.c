/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "ak_color_or_tex.h"
#include <gk/texture.h>
#include <cglm.h>

/* TODO: only color for now */
void
agk_copyColorOrTex(AkContext      * __restrict actx,
                   AkFxColorOrTex * __restrict src,
                   GkColorOrTex   * __restrict dest) {
  if (src->color) {
    glm_vec4_copy(src->color->vec, dest->color.vec);
    dest->method = GK_ONLY_COLOR;
  } else {
    dest->tex = agk_loadTexture(actx, src->texture);
    if (dest->tex)
      dest->method = GK_ONLY_TEX;
    else {
      glm_vec4_copy((vec4){1.0f, 1.0f, 1.0f, 1.0f},
                    dest->color.vec);
      dest->method = GK_ONLY_COLOR;
    }
  }
}
