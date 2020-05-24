/*
* Copyright (c), Recep Aslantas.
*
* MIT License (MIT), http://opensource.org/licenses/MIT
* Full license can be found in the LICENSE file
*/

#include "common.h"
#include "enum.h"

GkGPUAccessor*
agkAccessor(AkAccessor  * __restrict acc,
            GkGpuBuffer * __restrict gbuff) {
  GkGPUAccessor *gacc;
  
  gacc             = calloc(1, sizeof(*gacc));
  gacc->buffer     = gbuff;
  gacc->itemType   = agk_type(acc->componentType);
  gacc->byteOffset = acc->byteOffset;
  gacc->byteStride = acc->byteStride;
  gacc->itemCount  = acc->componentCount;
  gacc->count      = acc->count;
  gacc->itemSize   = acc->componentBytes;
  gacc->normalized = false;
  gacc->filledSize = acc->fillByteSize;
  gacc->gpuTarget  = gbuff->target;
  
  return gacc;
}

GkColorDesc*
agkColorOrTex(AgkContext  * __restrict ctx,
              AkContext   * __restrict actx,
              AkColorDesc * __restrict src) {
  GkColorDesc *crtx;

  crtx = calloc(1, sizeof(*crtx));
  if (src->color) {
    GkColor *color;
    color = malloc(sizeof(*color));
    glm_vec4_copy(src->color->vec, color->vec);

    crtx->val    = color;
    crtx->method = GK_COLOR_COLOR;
  } else if (src->texture) {
    crtx->val    = agkLoadTexture(ctx, actx, src->texture);
    crtx->method = GK_COLOR_TEX;
  }
  
  return crtx;
}
