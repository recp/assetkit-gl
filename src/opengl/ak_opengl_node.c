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
              GkNode **dest) {
  GkNode *glnode;

  glnode = calloc(sizeof(*glnode), 1);
  if (node->transform) {
    GkMatrix *mat;

    mat = malloc(sizeof(*mat));
    mat->cmatIsValid = 0;
    mat->refc        = 1;
    mat->fmat        = NULL;
    glnode->matrix   = mat;
    glnode->flags   |= GK_HAVE_MATRIX;

    ak_transformCombine(node, glnode->matrix->matrix[0]);
  }

  if (node->geometry) {
    AkGeometry     *geom;
    GkComplexModel *model;
    AkResult        ret;

    geom = ak_instanceObjectGeom(node);
    ret = ak_glLoadGeometry(doc,
                            geom,
                            GL_STATIC_DRAW,
                            &model);

    if (ret == AK_OK)
      glnode->model = &model->base;
  }

  if (node->node) {
    AkNode *nodei;
    if ((nodei = ak_instanceObjectNode(node))) {
      ak_glLoadNode(doc,
                    nodei,
                    usage,
                    &glnode->nodeInst);
    }
  }

  if (node->chld) {
    AkNode  *nodei;
    GkNode **glnodei;

    nodei   = node->chld;
    glnodei = &glnode->chld;
    do {
      ak_glLoadNode(doc,
                    nodei,
                    usage,
                    glnodei);
      glnodei = &(*glnodei)->next;
      nodei   = nodei->next;
    } while (nodei);
  }

  *dest = glnode;

  return AK_OK;
}
