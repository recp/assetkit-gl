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
               AkInputBasic * __restrict inp,
               uint32_t     * __restrict inputIndex) {
  AkBuffer      *akbuff;
  GkBuffer      *buff;
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
  
  akbuff = ak_getObjectByUrl(&acc->source);
  if (!akbuff)
    return; /* TODO: assert or log */

  buff = rb_find(ctx->bufftree, akbuff);
  if (!buff) {
    glprim->bufc++;

    buff = calloc(1, sizeof(*buff));
    buff->size   = (GLsizei)akbuff->length;
    buff->usage  = ctx->usage;
    buff->type   = type;
    buff->target = GL_ARRAY_BUFFER;

    glGenBuffers(1, &buff->vbo);

    glBindBuffer(buff->target, buff->vbo);
    glBufferData(buff->target,
                 buff->size,
                 akbuff->data,
                 ctx->usage);

    rb_insert(ctx->bufftree, akbuff, buff);

    if (glprim->bufs)
      glprim->bufs->prev = buff;
    buff->next   = glprim->bufs;
    glprim->bufs = buff;
  } else {
    glBindBuffer(buff->target, buff->vbo);
  }

  glVertexAttribPointer(*inputIndex,
                        acc->bound,
                        type,
                        GL_FALSE,
                        (GLsizei)acc->byteStride,
                        BUFFER_OFFSET(acc->byteOffset));
  glEnableVertexAttribArray(*inputIndex);
  (*inputIndex)++;
}

AkResult
agk_loadMesh(AgkContext * __restrict ctx,
             AkMesh     * __restrict mesh,
             GkModel   ** __restrict dest) {
  AkMeshPrimitive *prim;
  GkModel         *glmodel;
  uint32_t         inputIndex, primc;

  primc   = mesh->primitiveCount;
  glmodel = calloc(1, sizeof(*glmodel) + sizeof(GkPrimitive) * primc);

  ak_meshReIndexInputs(mesh);

  prim = mesh->primitive;
  while (prim) {
    GkPrimitive  *glprim;
    AkInputBasic *vi;
    AkInput      *input;
    AkSource     *source;

    inputIndex = 0;

    glprim = &glmodel->prims[glmodel->primc];
    glGenVertexArrays(1, &glprim->vao);
    glBindVertexArray(glprim->vao);

    /* vertices */
    if (prim->vertices) {
      vi = prim->vertices->input;

      while (vi) {
        source = ak_getObjectByUrl(&vi->source);
        if (source && source->tcommon)
          agk_loadSource(ctx,
                         source->tcommon,
                         glprim,
                         vi,
                         &inputIndex);

        vi = vi->next;
      }
    }

    /* per-primitive inputs */
    input = prim->input;
    while (input) {
      /* vertices are already processed, skip */
      if (input->base.semantic == AK_INPUT_SEMANTIC_VERTEX) {
        input = (AkInput *)input->base.next;
        continue;
      }

      source = ak_getObjectByUrl(&input->base.source);
      if (source && source->tcommon)
        agk_loadSource(ctx,
                       source->tcommon,
                       glprim,
                       &input->base,
                       &inputIndex);

      input = (AkInput *)input->base.next;
    }

    /* indexed draw */
    if (prim->indices) {
      GkBuffer *ibuff;

      glprim->bufc++;

      ibuff = calloc(1, sizeof(*ibuff));
      ibuff->size   = (GLsizei)(prim->indices->count * sizeof(AkUInt));
      ibuff->usage  = ctx->usage;
      ibuff->type   = GL_UNSIGNED_INT;
      ibuff->target = GL_ELEMENT_ARRAY_BUFFER;

      glGenBuffers(1, &ibuff->vbo);

      /* only one GL_ELEMENT_ARRAY_BUFFER for VAO */
      glBindBuffer(ibuff->target, ibuff->vbo);
      glBufferData(ibuff->target,
                   ibuff->size,
                   prim->indices->items,
                   ibuff->usage);

      glprim->count = (GLsizei)prim->indices->count;
      glprim->flags |= GK_DRAW_ELEMENTS;

      if (glprim->bufs)
        glprim->bufs->prev = ibuff;
      ibuff->next  = glprim->bufs;
      glprim->bufs = ibuff;
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
      glm_vec_copy(prim->bbox->min, glprim->bbox[0]);
      glm_vec_copy(prim->bbox->max, glprim->bbox[1]);
    }

    prim = prim->next;
  }

  /* nothing to render */
  if (glmodel->primc < 1)
    goto err;

  if (mesh->bbox) {
    glm_vec_copy(mesh->bbox->min, glmodel->bbox[0]);
    glm_vec_copy(mesh->bbox->max, glmodel->bbox[1]);
  }

  *dest = glmodel;

  return AK_OK;
err:
  free(glmodel);
  return AK_ERR;
}
