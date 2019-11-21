/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/agk.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <gk/gk.h>
#include <cglm/cglm.h>
#include <ds/rb.h>
#include <ds/forward-list.h>

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

  glscene     = gkAllocScene(ctx);
  visualScene = ak_instanceObject(scene->visualScene);
  glnode      = gkAllocNode(glscene);

  gkMakeNodeTransform(glscene, glnode);

  glscene->rootNode = glnode;

  node    = visualScene->node;
  glnodei = &glscene->rootNode->chld;

  if (node) {
    agkCtx            = calloc(1, sizeof(*agkCtx));
    agkCtx->bufftree  = rb_newtree_ptr();
    agkCtx->objMap    = rb_newtree_ptr();
    agkCtx->instCtlr  = rb_newtree_ptr();
    agkCtx->ctlr      = rb_newtree_ptr();
    agkCtx->materials = rb_newtree_ptr();
    agkCtx->textures  = rb_newtree_ptr();
    agkCtx->doc       = doc;
    agkCtx->usage     = usage;
    agkCtx->scene     = glscene;
    agkCtx->ctx       = ctx;

    do {
      ret = agk_loadNode(agkCtx, node, glnodei);
      if (ret != AK_OK) {
        free(glscene);
        return AK_ERR;
      }

      (*glnodei)->parent = glnode;
      glnodei = &(*glnodei)->next;
      node    = node->next;
    } while (node);

    agkLoadAnimations(agkCtx);
    load_controllers(agkCtx);

    free(agkCtx);
  }

  *dest = glscene;

  return AK_OK;
}
