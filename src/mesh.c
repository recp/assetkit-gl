/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/agk.h"
#include "enum.h"
#include "common.h"

void
agkLoadSource(AgkContext  * __restrict ctx,
              AkAccessor  * __restrict acc,
              GkPrimitive * __restrict gprim,
              AkInput     * __restrict inp) {
  AkBuffer      *akbuff;
  GkGpuBuffer   *buff;
  GkVertexInput *vi;
  GkGPUAccessor *gacc;
  char           attribName[64];

  /* optimization: check if this input is bound or not */
  if (!acc || !inp->semanticRaw)
    return;

  ak_inputNameBySet(inp, attribName);

  if (!(akbuff = acc->buffer))
    return; /* TODO: assert or log */

  buff = rb_find(ctx->bufftree, akbuff);
  if (!buff) {
    buff = gkGpuBufferNew(ctx->ctx, GK_ARRAY, akbuff->length);
    gkGpuBufferFeed(buff, GK_STATIC_DRAW, akbuff->data);
    gkPrimAddBuffer(gprim, buff);

    rb_insert(ctx->bufftree, akbuff, buff);
  } else {
    glBindBuffer(buff->target, buff->vbo);
  }

  gacc         = agkAccessor(acc, buff);
  vi           = gkMakeVertexInput(attribName, gacc->itemType, 0);
  vi->accessor = gacc;

  gk_bindInputTo(gprim, vi);
}

AkResult
agkLoadMesh(AgkContext * __restrict ctx,
            AkMesh     * __restrict mesh,
            GkGeometry ** __restrict dest) {
  AkMeshPrimitive *prim;
  GkGeometry      *ggeom;
  GkPrimitive     *gprim;
  AkInput         *input;
  AkUIntArray     *indices;
  GkGpuBuffer     *ibuff;
  uint32_t         primc;

  primc  = mesh->primitiveCount;
  ggeom = calloc(1, sizeof(*ggeom) + sizeof(GkPrimitive) * primc);
  prim   = mesh->primitive;

  while (prim) {
    gprim = &ggeom->prims[ggeom->primc];

    glGenVertexArrays(1, &gprim->vao);
    gkBindPrimitive(gprim);

    /* per-primitive inputs */
    input = prim->input;
    while (input) {
      agkLoadSource(ctx, input->accessor, gprim, input);

      input = input->next;
    }

    /* indexed draw */
    if ((indices = prim->indices)) {
      ibuff = gkGpuBufferNew(ctx->ctx,
                             GK_INDEX,
                             indices->count * sizeof(AkUInt));
      gkGpuBufferFeed(ibuff, GK_STATIC_DRAW, indices->items);
      gkPrimAddBuffer(gprim, ibuff);

      gprim->count  = (GLsizei)indices->count;
      gprim->flags |= GK_DRAW_ELEMENTS;
    }

    /* els direct draw */
    else {
      gprim->flags |= GK_DRAW_ARRAYS;
      gprim->count  = prim->nPolygons;
    }

    gprim->mode = agk_drawMode(prim);

    prim->udata = (void *)(uintptr_t)ggeom->primc;
    ggeom->primc++;

    if (prim->bbox) {
      glm_vec3_copy(prim->bbox->min, gprim->bbox[0]);
      glm_vec3_copy(prim->bbox->max, gprim->bbox[1]);
    }

    if (prim->material)
      agkLoadPrimMaterial(ctx, prim, gprim);

    prim = prim->next;
  }

  /* nothing to render */
  if (ggeom->primc < 1)
    goto err;

  if (mesh->bbox) {
    glm_vec3_copy(mesh->bbox->min, ggeom->bbox[0]);
    glm_vec3_copy(mesh->bbox->max, ggeom->bbox[1]);
  }

  glm_vec3_copy(mesh->center, ggeom->center);

  *dest = ggeom;

  return AK_OK;
err:
  free(ggeom);
  return AK_ERR;
}
