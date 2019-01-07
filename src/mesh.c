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

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

void
agk_loadSource(AgkContext   * __restrict ctx,
               AkAccessor   * __restrict acc,
               GkPrimitive  * __restrict glprim,
               AkInput      * __restrict inp) {
  AkBuffer      *akbuff;
  GkGpuBuffer   *buff;
  GkVertexInput *vi;
  GLenum         type;
  char           attribName[64];

  /* optimization: check if this input is bound or not */
  if (!inp->semanticRaw)
    return;
  
  ak_inputNameIndexed(inp, attribName);

  type = agk_type(acc->itemTypeId);
  vi   = gkMakeVertexInput(attribName, type, 0);

  flist_sp_append(&glprim->inputs, vi);

  if (!(akbuff = ak_getObjectByUrl(&acc->source)))
    return; /* TODO: assert or log */

  buff = rb_find(ctx->bufftree, akbuff);
  if (!buff) {
    buff = gkGpuBufferNew(ctx->ctx, GK_ARRAY, akbuff->length);
    gkGpuBufferFeed(buff, GK_STATIC_DRAW, akbuff->data);
    gkPrimAddBuffer(glprim, buff);

    rb_insert(ctx->bufftree, akbuff, buff);
  } else {
    glBindBuffer(buff->target, buff->vbo);
  }

  if (agk_isinteger(acc->itemTypeId)) {
    glVertexAttribIPointer(glprim->lastInputIndex,
                           acc->bound,
                           type,
                           (GLsizei)acc->byteStride,
                           BUFFER_OFFSET(acc->byteOffset));
  } else {
    glVertexAttribPointer(glprim->lastInputIndex,
                          acc->bound,
                          type,
                          GL_FALSE,
                          (GLsizei)acc->byteStride,
                          BUFFER_OFFSET(acc->byteOffset));
  }

  glEnableVertexAttribArray(glprim->lastInputIndex);
  glprim->lastInputIndex++;
}

AkResult
agk_loadMesh(AgkContext * __restrict ctx,
             AkMesh     * __restrict mesh,
             GkModel   ** __restrict dest) {
  AkMeshPrimitive *prim;
  GkModel         *glmodel;
  uint32_t         primc;

  primc   = mesh->primitiveCount;
  glmodel = calloc(1, sizeof(*glmodel) + sizeof(GkPrimitive) * primc);

  ak_meshReIndexInputs(mesh);

  prim = mesh->primitive;
  while (prim) {
    GkPrimitive  *glprim;
    AkInput      *input;
    AkSource     *source;
    AkUIntArray  *indices;

    glprim = &glmodel->prims[glmodel->primc];

    glGenVertexArrays(1, &glprim->vao);
    glBindVertexArray(glprim->vao);

    /* per-primitive inputs */
    input = prim->input;
    while (input) {
      source = ak_getObjectByUrl(&input->source);

      if (source && source->tcommon)
        agk_loadSource(ctx, source->tcommon, glprim, input);

      input = input->next;
    }

    /* indexed draw */
    if ((indices = prim->indices)) {
      GkGpuBuffer *ibuff;

      ibuff = gkGpuBufferNew(ctx->ctx,
                             GK_INDEX,
                             indices->count * sizeof(AkUInt));
      gkGpuBufferFeed(ibuff, GK_STATIC_DRAW, indices->items);
      gkPrimAddBuffer(glprim, ibuff);

      glprim->count  = (GLsizei)indices->count;
      glprim->flags |= GK_DRAW_ELEMENTS;
    }

    /* els direct draw */
    else {
      glprim->flags |= GK_DRAW_ARRAYS;
    }

    glprim->mode = agk_drawMode(prim);

    glBindVertexArray(0);

    prim->udata = (void *)(uintptr_t)glmodel->primc;
    glmodel->primc++;

    if (prim->bbox) {
      glm_vec3_copy(prim->bbox->min, glprim->bbox[0]);
      glm_vec3_copy(prim->bbox->max, glprim->bbox[1]);
    }

    prim = prim->next;
  }

  /* nothing to render */
  if (glmodel->primc < 1)
    goto err;

  if (mesh->bbox) {
    glm_vec3_copy(mesh->bbox->min, glmodel->bbox[0]);
    glm_vec3_copy(mesh->bbox->max, glmodel->bbox[1]);
  }

  glm_vec3_copy(mesh->center, glmodel->center);

  *dest = glmodel;

  return AK_OK;
err:
  free(glmodel);
  return AK_ERR;
}
