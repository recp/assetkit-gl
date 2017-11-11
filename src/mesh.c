/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/agk.h"
#include "enum.h"

#include <assetkit.h>
#include <gk/gk.h>

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

void
agk_loadSource(AgkContext   * __restrict ctx,
               AkAccessor   * __restrict acc,
               GkPrimitive  * __restrict glprim,
               AkInputBasic * __restrict inp,
               uint32_t set) {
  AkBuffer *akbuff;
  GkBuffer *buff;
  GLenum    type;
  GLint     attribIndex;
  char      attribName[64];

  /* optimization: check if this input is bound or not */
  if (!inp->semanticRaw)
    return;

  ak_inputNameIndexed(inp, attribName);
  attribIndex = gk_progAttribIndex(ctx->ctx->pinfo,
                                   attribName);

  /* optimization: ignore unused input */
  if (attribIndex < 0)
    return;

  akbuff = ak_getObjectByUrl(&acc->source);
  if (!akbuff)
    return; /* TODO: assert or log */

  if (acc->itemTypeId == AKT_INT) {
    type  = GL_INT;
  } else {
    type  = GL_FLOAT;
  }

  buff = rb_find(ctx->bufftree, akbuff);
  if (!buff) {
    glprim->bufc++;

    buff = calloc(sizeof(*buff), 1);
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

  glVertexAttribPointer(attribIndex,
                        acc->bound,
                        type,
                        GL_FALSE,
                        (GLsizei)acc->byteStride,
                        BUFFER_OFFSET(acc->byteOffset));
  glEnableVertexAttribArray(attribIndex);
}

AkResult
agk_loadMesh(AgkContext * __restrict ctx,
             AkMesh     * __restrict mesh,
             GkModel   ** __restrict dest) {
  AkMeshPrimitive *prim;
  GkModel         *glmodel;

  glmodel = calloc(sizeof(*glmodel), 1);

  ak_meshReIndexInputs(mesh);

  prim = mesh->primitive;
  while (prim) {
    GkPrimitive  *glprim;
    AkInputBasic *vi;
    AkInput      *input;
    AkSource     *source;

    /* there is no position? */
    if (!prim->vertices) {
      prim = prim->next;
      continue;
    }

    glprim = calloc(sizeof(*glprim), 1);
    glGenVertexArrays(1, &glprim->vao);
    glBindVertexArray(glprim->vao);

    /* vertices */
    vi = prim->vertices->input;

    while (vi) {
      source = ak_getObjectByUrl(&vi->source);
      if (source && source->tcommon)
        agk_loadSource(ctx,
                       source->tcommon,
                       glprim,
                       vi,
                       0);

      vi = vi->next;
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
                       input->set);

      input = (AkInput *)input->base.next;
    }

    /* indexed draw */
    if (prim->indices) {
      GkBuffer *ibuff;

      glprim->bufc++;

      ibuff = calloc(sizeof(*ibuff), 1);
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

    if (glmodel->prim)
      glmodel->prim->prev = glprim;
    glprim->next  = glmodel->prim;
    glmodel->prim = glprim;

    glBindVertexArray(0);

    glmodel->primc++;
    prim->udata = glprim;

    prim = prim->next;
  }

  /* nothing to render */
  if (glmodel->primc < 1)
    goto err;

  *dest = glmodel;

  return AK_OK;
err:
  free(glmodel);
  return AK_ERR;
}
