/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../include/ak-opengl.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <assetkit.h>
#include <gk.h>

AkResult
agk_loadScene(GkContext *ctx,
              AkDoc     *doc,
              AkScene   *scene,
              GLenum     usage,
              GkScene  **dest) {
  AkVisualScene *visualScene;
  AkNode        *node;
  GkScene       *glscene;
  GkNode       **glnodei;
  AgkContext   *agkCtx;
  AkResult       ret;

  if (!scene->visualScene)
    return AK_ERR;

  glscene     = calloc(sizeof(*glscene), 1);
  visualScene = ak_instanceObject(scene->visualScene);

  node    = visualScene->node;
  glnodei = &glscene->rootNode;

  if (visualScene->bbox) {
    GkBBox *bbox;

    bbox = malloc(sizeof(*glscene->bbox));
    glm_vec_copy(visualScene->bbox->min, bbox->min);
    glm_vec_copy(visualScene->bbox->max, bbox->max);

    bbox->isvalid = visualScene->bbox->isvalid;
    ak_bbox_center(visualScene->bbox, bbox->center);
    bbox->radius = ak_bbox_radius(visualScene->bbox);

    glscene->bbox = bbox;
  }

  glscene->usage = usage;
  glscene->pinfo = ctx->pinfo;

  if (node) {
    agkCtx           = calloc(sizeof(*agkCtx), 1);
    agkCtx->bufftree = rb_newtree_ptr();
    agkCtx->doc      = doc;
    agkCtx->usage    = usage;
    agkCtx->scene    = glscene;
    agkCtx->ctx      = ctx;

    do {
      ret = agk_loadNode(agkCtx, node, glnodei);
      if (ret != AK_OK) {
        free(glscene);
        return AK_ERR;
      }

      glnodei = &(*glnodei)->next;
      node    = node->next;
    } while (node);

    free(agkCtx);
  }

  *dest = glscene;

  return AK_OK;
}
