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

AkResult
ak_glMeshLoad(AkMesh * mesh,
              GLenum usage,
              GkComplexModel ** dest) {
  GkComplexModel  *model;
  AkMeshPrimitive *primitive;
  AkDoc    *doc;
  AkHeap   *heap;
  GLuint   *vbo;
  GLuint   *vao;
  GLsizei  *count;
  GLenum   *modes;
  uint32_t  vaoCount;
  uint32_t  vboCount;
  uint32_t  vaoIndex;
  uint32_t  vboIndex;

  primitive = mesh->primitive;
  model     = calloc(sizeof(*model), 1);
  heap      = ak_heap_getheap(mesh->vertices);
  doc       = ak_heap_attachment(heap);

  vao       = calloc(sizeof(*vao) * vaoCount, 1);
  count     = calloc(sizeof(*count) * vaoCount, 1);
  modes     = calloc(sizeof(*modes) * vaoCount, 1);

  vaoIndex  = vboIndex = vboCount = 0;
  vaoCount  = mesh->primitiveCount;
  vbo       = NULL;

  gkModelEmptyMatrix(&model->base);
  glGenVertexArrays(vaoCount, vao);

  primitive = mesh->primitive;
  while (primitive) {
    AkUIntArray  *indices;
    AkInputBasic *verticesInput;
    AkInput      *input;

    if (!primitive->vertices)
      continue;

    verticesInput = NULL;
    input         = primitive->input;
    indices       = primitive->indices;

    /* VBO count */
    vboCount += ak_meshInputCount(mesh) + 1;

    glBindVertexArray(vao[vaoIndex]);

    vbo = calloc(sizeof(*vbo) * vboCount, 1);
    glGenBuffers(vboCount, vbo);

    /* vertices */
    verticesInput = primitive->vertices->input;

    while (verticesInput) {
      AkSource      *vertexSource;
      AkFloatArrayN *sourceData;

      vertexSource = ak_getObjectByUrl(doc, verticesInput->source);
      if (!vertexSource)
        continue;

      assert(vertexSource->data->type == AK_SOURCE_ARRAY_TYPE_FLOAT
             && "*Currently* only single floats are supported!");

      sourceData = ak_objGet(vertexSource->data);

      glBindBuffer(GL_ARRAY_BUFFER, vbo[vboIndex]);
      glBufferData(GL_ARRAY_BUFFER,
                   sourceData->count * sizeof(AkFloat),
                   sourceData->items,
                   usage);

      glVertexAttribPointer(vboIndex,
                            3, /* TODO: */
                            GL_FLOAT,
                            GL_FALSE,
                            0,
                            NULL);
      glEnableVertexAttribArray(vboIndex);

      vboIndex++;
      verticesInput = verticesInput->next;
    }

    /* other inputs e.g. normals */
    input = primitive->input;
    while (input) {
      AkSource      *source;
      AkFloatArrayN *sourceData;
      GLenum         type;

      /* vertices are already processed, skip */
      if (input->base.semantic == AK_INPUT_SEMANTIC_VERTEX) {
        input = (AkInput *)input->base.next;
        continue;
      }

      source     = ak_getObjectByUrl(doc, input->base.source);
      sourceData = ak_objGet(source->data);

      assert((source->data->type == AK_SOURCE_ARRAY_TYPE_FLOAT
              || source->data->type == AK_SOURCE_ARRAY_TYPE_INT)
             && "*Currently* only single floats and integers "
             "are supported!");

      if (source->data->type == AK_SOURCE_ARRAY_TYPE_INT)
        type = GL_INT;
      else
        type = GL_FLOAT;

      glBindBuffer(GL_ARRAY_BUFFER, vbo[vboIndex]);
      glBufferData(GL_ARRAY_BUFFER,
                   sourceData->count * sizeof(GLfloat),
                   sourceData->items,
                   usage);

      glVertexAttribPointer(vboIndex,
                            3, /* TODO: !!! */
                            type,
                            GL_FALSE,
                            0,
                            NULL);

      glEnableVertexAttribArray(vboIndex);

      vboIndex++;
      input = (AkInput *)input->base.next;
    }

    /* only one GL_ELEMENT_ARRAY_BUFFER for VAO */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[vboIndex]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 primitive->indices->count * sizeof(AkUInt),
                 primitive->indices->items,
                 usage);

    count[vaoIndex] = (GLsizei)primitive->indices->count;

    if (primitive->type != AK_MESH_PRIMITIVE_TYPE_LINES)
      modes[vaoIndex] = GL_TRIANGLES;
    else
      modes[vaoIndex] = GL_LINES;

    vboIndex++;
    vaoIndex++;

    glBindVertexArray(0);

    primitive = primitive->next;
  }

  /* nothing to render */
  if (vaoCount < 1 || vboCount < 1)
    goto err;

  model->vao        = vao;
  model->vbo        = vbo;
  model->vaoCount   = vaoCount;
  model->vboCount   = vboCount;
  model->count      = count;
  model->modes      = modes;
  model->base.flags = GK_DRAW_ELEMENTS | GK_COMPLEX;

  *dest = model;
  return AK_OK;
  
err:
  if (vboCount > 1)
    glDeleteBuffers(vboCount, vbo);

  glDeleteVertexArrays(vaoCount, vao);
  glBindVertexArray(0);
  
  free(vao);
  free(count);
  free(model);
  
  return AK_ERR;
}
