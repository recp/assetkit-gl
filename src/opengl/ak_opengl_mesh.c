/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../include/ak-opengl.h"
#include <stdlib.h>
#include <assert.h>
#include <assetkit.h>
#include <gk.h>
#include <gk-rb.h>

#include <string.h>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define ak__align(size) ((size + 32 - 1) &~ (uint32_t)(32 - 1))

GLenum
agk_drawMode(AkMeshPrimitive *primitive) {
  GLenum mode;

  switch (primitive->type) {
    case AK_MESH_PRIMITIVE_TYPE_POLYGONS:
      mode = GL_TRIANGLES;
      break;
    case AK_MESH_PRIMITIVE_TYPE_TRIANGLES: {
      AkTriangles *triangles;
      triangles = (AkTriangles *)primitive;
      switch (triangles->mode) {
        case AK_TRIANGLE_MODE_TRIANGLES:
          mode = GL_TRIANGLES;
          break;
        case AK_TRIANGLE_MODE_TRIANGLE_STRIP:
          mode = GL_TRIANGLE_STRIP;
          break;
        case AK_TRIANGLE_MODE_TRIANGLE_FAN:
          mode = GL_TRIANGLE_FAN;
          break;
      }
      break;
    }
    case AK_MESH_PRIMITIVE_TYPE_LINES: {
      AkLines *lines;

      lines = (AkLines *)primitive;
      switch (lines->mode) {
        case AK_LINE_MODE_LINES:
          mode = GL_LINES;
          break;
        case AK_LINE_MODE_LINE_STRIP:
          mode = GL_LINE_STRIP;
          break;
        case AK_LINE_MODE_LINE_LOOP:
          mode = GL_LINE_LOOP;
          break;
      }
    }
  }

  return mode;
}

void
agk_loadSource(AgkContext   * __restrict ctx,
               AkAccessor   * __restrict acc,
               GkPrimitive  * __restrict glprim,
               AkInputBasic * __restrict inp,
               uint32_t set) {
  AkObject *sourceObj;
  void     *sourceData;
  void     *items;
  GkBuffer *buff;
  size_t    count;
  GLsizei   isize;
  GLenum    type;
  GLint     attribIndex;
  char      attribName[64];

  /* optimization: check if this input is bound or not */
  if (!inp->semanticRaw)
    return;

  strcpy(attribName, inp->semanticRaw);
  if (set > 0)
    sprintf(attribName + strlen(attribName), "%d", set);

  attribIndex = gk_progAttribIndex(ctx->ctx->pinfo,
                                   attribName);

  /* optimization: ignore unused input */
  if (attribIndex < 0)
    return;

  sourceObj = ak_getObjectByUrl(&acc->source);
  if (!sourceObj)
    return; /* TODO: assert or log */

  assert((sourceObj->type == AK_SOURCE_ARRAY_TYPE_FLOAT
          || sourceObj->type == AK_SOURCE_ARRAY_TYPE_INT)
         && "*Currently* only single floats and integers "
         "are supported!");

  sourceData = ak_objGet(sourceObj);
  
  if (sourceObj->type == AK_SOURCE_ARRAY_TYPE_INT) {
    AkSourceIntArray *intArray;
    intArray = (AkSourceIntArray *)sourceData;
    type  = GL_INT;
    count = intArray->base.count;
    items = intArray->items;
    isize = sizeof(GLint);
  } else {
    AkSourceFloatArray *floatArray;
    floatArray = (AkSourceFloatArray *)sourceData;
    type  = GL_FLOAT;
    count = floatArray->base.count;
    items = floatArray->items;
    isize = sizeof(GLfloat);
  }

  buff = rb_find(ctx->bufftree, sourceData);
  if (!buff) {
    glprim->bufc++;

    buff = calloc(sizeof(*buff), 1);
    buff->size   = (GLsizei)(count * isize);
    buff->usage  = ctx->usage;
    buff->type   = type;
    buff->target = GL_ARRAY_BUFFER;

    glGenBuffers(1, &buff->vbo);

    glBindBuffer(buff->target, buff->vbo);
    glBufferData(buff->target,
                 buff->size,
                 items,
                 ctx->usage);

    rb_insert(ctx->bufftree, sourceData, buff);

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
                        acc->stride * isize,
                        BUFFER_OFFSET(acc->offset * isize));
  glEnableVertexAttribArray(attribIndex);
}

AkResult
agk_loadMesh(AgkContext * __restrict ctx,
             AkMesh     * __restrict mesh,
             GkModel   ** __restrict dest) {
  AkMeshPrimitive *prim;
  GkModel         *glmodel;

  glmodel = calloc(sizeof(*glmodel), 1);

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
