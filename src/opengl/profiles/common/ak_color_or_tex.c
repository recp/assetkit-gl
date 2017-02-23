/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "ak_color_or_tex.h"
#include <cglm.h>

/* TODO: only color for now */
void
agk_copyColorOrTex(AkFxColorOrTex * __restrict src,
                    GkColorOrTex   * __restrict dest) {
  if (src->color)
    glm_vec4_copy(src->color->vec, dest->color.vec);
}
