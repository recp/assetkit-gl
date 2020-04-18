/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/agk.h"
#include "enum.h"

#include <gk/gk.h>
#include <gk/vertex.h>

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define BUFFER_OFFSET(i) ((void *)(i))

void
agkLoadSource(AgkContext  * __restrict ctx,
              AkAccessor  * __restrict acc,
              GkPrimitive * __restrict gprim,
              AkInput     * __restrict inp) {
  AkBuffer      *akbuff;
  GkGpuBuffer   *buff;
  GkVertexInput *vi;
  GLenum         type;
  char           attribName[64];

  /* optimization: check if this input is bound or not */
  if (!acc || !inp->semanticRaw)
    return;

  ak_inputNameBySet(inp, attribName);

  type = agk_type(acc->componentType);
  vi   = gkMakeVertexInput(attribName, type, 0);

  flist_sp_append(&gprim->inputs, vi);

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

  if (agk_isinteger(acc->componentType)) {
    glVertexAttribIPointer(gprim->lastInputIndex,
                           acc->componentCount,
                           type,
                           (GLsizei)acc->byteStride,
                           BUFFER_OFFSET(acc->byteOffset));
  } else {
    glVertexAttribPointer(gprim->lastInputIndex,
                          acc->componentCount,
                          type,
                          GL_FALSE,
                          (GLsizei)acc->byteStride,
                          BUFFER_OFFSET(acc->byteOffset));
  }

  glEnableVertexAttribArray(gprim->lastInputIndex);

  gprim->lastInputIndex++;
}

AkResult
agkLoadMesh(AgkContext * __restrict ctx,
            AkMesh     * __restrict mesh,
            GkModel   ** __restrict dest) {
  AkMeshPrimitive *prim;
  GkModel         *gmodel;
  uint32_t         primc;

  primc  = mesh->primitiveCount;
  gmodel = calloc(1, sizeof(*gmodel) + sizeof(GkPrimitive) * primc);
  prim   = mesh->primitive;

  while (prim) {
    GkPrimitive *gprim;
    AkInput     *input;
    AkUIntArray *indices;

    gprim = &gmodel->prims[gmodel->primc];

    glGenVertexArrays(1, &gprim->vao);
    glBindVertexArray(gprim->vao);

    /* per-primitive inputs */
    input = prim->input;
    while (input) {
      agkLoadSource(ctx, input->accessor, gprim, input);

      input = input->next;
    }

    /* indexed draw */
    if ((indices = prim->indices)) {
      GkGpuBuffer *ibuff;

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
    }

    gprim->mode = agk_drawMode(prim);

    glBindVertexArray(0);

    prim->udata = (void *)(uintptr_t)gmodel->primc;
    gmodel->primc++;

    if (prim->bbox) {
      glm_vec3_copy(prim->bbox->min, gprim->bbox[0]);
      glm_vec3_copy(prim->bbox->max, gprim->bbox[1]);
    }

    if (prim->material) {
      agkLoadPrimMaterial(ctx, prim, gprim);
    }
    prim = prim->next;
  }

  /* nothing to render */
  if (gmodel->primc < 1)
    goto err;

  if (mesh->bbox) {
    glm_vec3_copy(mesh->bbox->min, gmodel->bbox[0]);
    glm_vec3_copy(mesh->bbox->max, gmodel->bbox[1]);
  }

  glm_vec3_copy(mesh->center, gmodel->center);

  *dest = gmodel;

  return AK_OK;
err:
  free(gmodel);
  return AK_ERR;
}
