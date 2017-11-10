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
#include <gk/gk.h>
#include <cglm/cglm.h>
#include <ds/rb.h>

AkResult
agk_loadScene(GkContext *ctx,
              AkDoc     *doc,
              AkScene   *scene,
              GLenum     usage,
              GkScene  **dest) {
  AkVisualScene *visualScene;
  AkNode        *node;
  GkScene       *glscene;
  GkNode       **glnodei, *glnode;
  AgkContext   *agkCtx;
  AkResult       ret;

  if (!scene->visualScene)
    return AK_ERR;

  glscene     = calloc(sizeof(*glscene), 1);
  visualScene = ak_instanceObject(scene->visualScene);
  glscene->_priv.ctx = ctx;
  
  glnode = calloc(sizeof(*glnode), 1);
  gkMakeNodeTransform(glnode);
  glm_mat4_copy(GLM_MAT4_IDENTITY,
                glnode->trans->local);
  glnode->trans->flags |= GK_TRANSF_LOCAL_ISVALID;

  glscene->rootNode = glnode;

  node    = visualScene->node;
  glnodei = &glscene->rootNode->chld;

  if (visualScene->bbox)
    glscene->bbox = agk_bbox(visualScene->bbox);

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
