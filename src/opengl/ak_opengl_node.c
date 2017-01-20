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
ak_glLoadNode(AkGLContext * __restrict ctx,
              AkNode      * __restrict node,
              GkNode     ** __restrict dest) {
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
    AkInstanceBase *geomInst;
    AkGeometry     *geom;
    GkModel        *model;
    GkModelInst    *modelInst;
    AkResult        ret;

    geomInst = &node->geometry->base;
    while (geomInst) {
      geom = ak_instanceObject(geomInst);
      ret  = ak_glLoadGeometry(ctx, geom, &model);
      if (ret == AK_OK) {
        modelInst       = gkMakeInstance(model);
        modelInst->next = glnode->model;
        glnode->model   = modelInst;
      }

      geomInst = geomInst->next;
    }
  }

  /* make extra light ref to global scene for multiple light shader */
  if (node->light) {
    AkInstanceBase *lightInst;
    AkLight        *light;
    GkLight        *gllight;
    AkResult        ret;

    lightInst = node->light;
    while (lightInst) {
      light = ak_instanceObject(lightInst);
      ret   = ak_glLoadLight(ctx->doc,
                             glnode,
                             light,
                             &gllight);
      if (ret == AK_OK) {
        gllight->next = glnode->light;
        glnode->light = gllight;

        gllight->ref.next  = ctx->scene->lights;
        ctx->scene->lights = &gllight->ref;

        if (ctx->scene->lights->prev)
          ctx->scene->lights->prev = &gllight->ref;
      }

      ctx->scene->lightCount++;
      lightInst = lightInst->next;
    }
  }

  if (node->node) {
    AkNode *nodei;
    if ((nodei = ak_instanceObjectNode(node)))
      ak_glLoadNode(ctx, nodei, &glnode->nodeInst);
  }

  if (node->chld) {
    AkNode  *nodei;
    GkNode **glnodei;

    nodei   = node->chld;
    glnodei = &glnode->chld;
    do {
      ak_glLoadNode(ctx, nodei, glnodei);
      glnodei = &(*glnodei)->next;
      nodei   = nodei->next;
    } while (nodei);
  }

  *dest = glnode;
  return AK_OK;
}
