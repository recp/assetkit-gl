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
ak_glLoadNode(AkDoc   * __restrict doc,
              AkNode  *node,
              GLenum   usage,
              mat4     parentTransform,
              GkNode **dest) {
  GkNode  *glnode, *gl_lastNode;
  mat4     transform;
  mat4     localTransform;

  glnode = gl_lastNode = NULL;

  while (node) {
    ak_transformCombine(node, localTransform[0]);
    glm_mat4_mul(parentTransform, localTransform, transform);

    if (node->nodeType != AK_NODE_TYPE_NODE)
      goto cont;

    if (node->geometry) {
      AkGeometry     *geom;
      GkComplexModel *model;
      GkNode         *n_glnode;
      AkResult        ret;

      geom = ak_instanceObjectGeom(node);
      ret = ak_glLoadGeometry(doc,
                              geom,
                              GL_STATIC_DRAW,
                              &model);

      if (ret == AK_OK) {
        n_glnode = calloc(sizeof(*n_glnode), 1);

        glm_mat4_dup(transform, model->base.matrix->matrix);
        n_glnode->model = &model->base;

        if (gl_lastNode)
          gl_lastNode->next = n_glnode;
        else
          glnode = n_glnode;

        gl_lastNode = n_glnode;
      }
    }

    if (node->chld)
      ak_glLoadNode(doc,
                    node->chld,
                    usage,
                    transform,
                    glnode ? &glnode : dest);

  cont:
    node = node->next;
  }

  if (glnode) {
    if (*dest) {
      (*dest)->chld = glnode;
    } else {
      *dest = glnode;
    }
  }

  return AK_OK;
}
