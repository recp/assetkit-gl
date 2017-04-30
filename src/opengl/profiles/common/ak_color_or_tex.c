/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "ak_color_or_tex.h"
#include <gk-texture.h>
#include <cglm.h>

/* TODO: only color for now */
void
agk_copyColorOrTex(AkFxColorOrTex * __restrict src,
                   GkColorOrTex   * __restrict dest) {
  if (src->color)
    glm_vec4_copy(src->color->vec, dest->color.vec);
  else {
    AkHeap            *heap;
    AkNewParam        *newparam;
    AkContext          ctx = {0};

    heap      = ak_heap_getheap(src);
    ctx.doc   = ak_heap_data(heap);
    newparam  = ak_sid_resolve(&ctx, src->texture->texture);

    if (!newparam)
      return;

    dest->tex = agk_loadTexture(newparam);
  }
}
