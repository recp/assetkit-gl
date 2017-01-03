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

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define ak__align(size) ((size + 32 - 1) &~ (uint32_t)(32 - 1))

typedef struct AkGLVBODesc {
  void     *source;
  GLuint    vbo;
  struct AkGLVBODesc *next;
} AkGLVBODesc;

AkResult
ak_glLoadMesh(AkDoc  * __restrict doc,
              AkMesh * mesh,
              GLenum usage,
              GkComplexModel ** dest) {
  GkComplexModel  *model;
  AkMeshPrimitive *primitive;
  AkAccessor      *acc;
  AkGLVBODesc     *vboDesc;
  GkMatrix        *matrix;
  AkGLVBODesc     *vboDescList;
  GLuint   *vbo;
  GLuint   *vao;
  GLsizei  *count;
  GLenum   *modes;
  uint32_t  vaoCount;
  uint32_t  vboCount;
  uint32_t  vaoIndex;
  uint32_t  vboIndex;
  uint32_t  inputIndex;
  GLsizei   vboSize;

  vaoIndex    = vboIndex = vboCount = 0;
  vaoCount    = mesh->primitiveCount;
  vbo         = NULL;
  vboDescList = NULL;

  vboSize   = 32;
  model     = calloc(sizeof(*model), 1);
  vao       = calloc(sizeof(*vao)   * vaoCount, 1);
  vbo       = calloc(vboSize, 1);
  count     = calloc(sizeof(*count) * vaoCount, 1);
  modes     = calloc(sizeof(*modes) * vaoCount, 1);
  matrix    = malloc(sizeof(*matrix));

  glm_mat4_dup(GLM_MAT4_IDENTITY, matrix->matrix);
  model->base.matrix = matrix;
  matrix->index = 0;

  glGenVertexArrays(vaoCount, vao);

  primitive = mesh->primitive;
  while (primitive) {
    AkInputBasic *verticesInput;
    AkInput      *input;

    if (!primitive->vertices) {
      primitive = primitive->next;
      continue;
    }

    verticesInput = NULL;

    glBindVertexArray(vao[vaoIndex]);

    /* vertices */
    verticesInput = primitive->vertices->input;

    inputIndex = 0;
    while (verticesInput) {
      AkSourceFloatArray *sourceData;
      AkSource *vertexSource;
      AkObject *sourceObj;

      vertexSource = ak_getObjectByUrl(&verticesInput->source);
      if (!vertexSource) {
        verticesInput = verticesInput->next;
        continue; /* TODO: assert or log */
      }

      acc       = vertexSource->techniqueCommon;
      sourceObj = ak_getObjectByUrl(&acc->source);

      if (!sourceObj) {
        verticesInput = verticesInput->next;
        continue; /* TODO: assert or log */
      }

      sourceData = ak_objGet(sourceObj);


      assert(vertexSource->data->type == AK_SOURCE_ARRAY_TYPE_FLOAT
             && "*Currently* only single floats are supported!");

      /* check if is there vbo for this source */
      vboDesc = vboDescList;
      while (vboDesc) {
        if (vboDesc->source == sourceData)
          break;

        vboDesc = vboDesc->next;
      }

      if (!vboDesc) {
        AkGLVBODesc *vboDescNew;
        vboCount++;

        if (ak__align(vboCount * sizeof(*vbo)) > vboSize) {
          vboSize = ak__align(vboCount * sizeof(*vbo));
          vbo = realloc(vbo, vboSize);
        }

        glGenBuffers(1, &vbo[vboIndex]);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[vboIndex]);
        glBufferData(GL_ARRAY_BUFFER,
                     sourceData->count * sizeof(GLfloat),
                     sourceData->items,
                     usage);

        vboDescNew = calloc(sizeof(*vboDescNew), 1);
        vboDescNew->source = sourceData;
        vboDescNew->vbo    = vbo[vboIndex];

        vboDescNew->next = vboDescList;
        vboDescList      = vboDescNew;

        vboIndex++;
      } else {
        glBindBuffer(GL_ARRAY_BUFFER, vboDesc->vbo);
      }

      glVertexAttribPointer(inputIndex,
                            acc->bound,
                            GL_FLOAT,
                            GL_FALSE,
                            acc->stride * sizeof(GLfloat),
                            BUFFER_OFFSET(acc->offset * sizeof(GLfloat)));
      glEnableVertexAttribArray(inputIndex);

      verticesInput = verticesInput->next;
      inputIndex++;
    }

    /* other inputs e.g. normals */
    input = primitive->input;
    while (input) {
      AkSourceFloatArray *sourceData;
      AkObject *sourceObj;
      AkSource *source;
      GLenum    type;

      /* vertices are already processed, skip */
      if (input->base.semantic == AK_INPUT_SEMANTIC_VERTEX) {
        input = (AkInput *)input->base.next;
        continue;
      }

      source = ak_getObjectByUrl(&input->base.source);
      if (!source) {
        input = (AkInput *)input->base.next;
        continue; /* TODO: assert or log */
      }

      acc        = source->techniqueCommon;
      sourceObj  = ak_getObjectByUrl(&acc->source);

      if (!sourceObj) {
        input = (AkInput *)input->base.next;
        continue; /* TODO: assert or log */
      }

      assert((sourceObj->type == AK_SOURCE_ARRAY_TYPE_FLOAT
              || sourceObj->type == AK_SOURCE_ARRAY_TYPE_INT)
             && "*Currently* only single floats and integers "
                "are supported!");

      sourceData = ak_objGet(sourceObj);
      
      if (sourceObj->type == AK_SOURCE_ARRAY_TYPE_INT)
        type = GL_INT;
      else
        type = GL_FLOAT;

      /* check if is there vbo for this source */
      vboDesc = vboDescList;
      while (vboDesc) {
        if (vboDesc->source == sourceData)
          break;

        vboDesc = vboDesc->next;
      }

      if (!vboDesc) {
        AkGLVBODesc *vboDescNew;

        vboCount++;

        if (ak__align(vboCount * sizeof(*vbo)) > vboSize) {
          vboSize = ak__align(vboCount * sizeof(*vbo));
          vbo = realloc(vbo, vboSize);
        }

        glGenBuffers(1, &vbo[vboIndex]);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[vboIndex]);
        glBufferData(GL_ARRAY_BUFFER,
                     sourceData->count * sizeof(GLfloat),
                     sourceData->items,
                     usage);

        vboDescNew = calloc(sizeof(*vboDescNew), 1);
        vboDescNew->source = sourceData;
        vboDescNew->vbo    = vbo[vboIndex];

        vboDescNew->next = vboDescList;
        vboDescList      = vboDescNew;

        vboIndex++;
      } else {
        glBindBuffer(GL_ARRAY_BUFFER, vboDesc->vbo);
      }

      glVertexAttribPointer(inputIndex,
                            acc->bound,
                            type,
                            GL_FALSE,
                            acc->stride * sizeof(GLfloat),
                            BUFFER_OFFSET(acc->offset * sizeof(GLfloat)));
      glEnableVertexAttribArray(inputIndex);

      inputIndex++;
      input = (AkInput *)input->base.next;
    }

    /* indexed draw */
    if (primitive->indices) {
      vboCount++;

      if (ak__align(vboCount * sizeof(*vbo)) > vboSize) {
        vboSize = ak__align(vboCount * sizeof(*vbo));
        vbo = realloc(vbo, vboSize);
      }

      glGenBuffers(1, &vbo[vboIndex]);

      /* only one GL_ELEMENT_ARRAY_BUFFER for VAO */
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[vboIndex]);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                   primitive->indices->count * sizeof(AkUInt),
                   primitive->indices->items,
                   usage);

      count[vaoIndex] = (GLsizei)primitive->indices->count;
      model->base.flags |= GK_DRAW_ELEMENTS;
    }

    /* els direct draw */
    else {
      model->base.flags |= GK_DRAW_ARRAYS;
    }

    if (primitive->type != AK_MESH_PRIMITIVE_TYPE_LINES)
      modes[vaoIndex] = GL_TRIANGLES;
    else
      modes[vaoIndex] = GL_LINES;

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
  model->base.flags |= GK_COMPLEX;
  
  *dest = model;

  vboDesc = vboDescList;
  while (vboDesc) {
    void *tofree;

    tofree = vboDesc;
    vboDesc = vboDesc->next;
    free(tofree);
  }

  return AK_OK;
  
err:
  if (vboCount > 1)
    glDeleteBuffers(vboCount, vbo);
  
  glDeleteVertexArrays(vaoCount, vao);
  glBindVertexArray(0);
  
  free(vao);
  free(vbo);
  free(count);
  free(model);
  free(modes);
  free(matrix);

  vboDesc = vboDescList;
  while (vboDesc) {
    void *tofree;

    tofree = vboDesc;
    vboDesc = vboDesc->next;
    free(tofree);
  }

  return AK_ERR;
}
