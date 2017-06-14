/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../include/ak-opengl.h"
#include "../ak_transform.h"
#include <stdlib.h>
#include <assert.h>
#include <assetkit.h>
#include <gk.h>

AkResult
agk_loadNode(AgkContext * __restrict ctx,
             AkNode     * __restrict node,
             GkNode    ** __restrict dest) {
  GkNode *glnode;

  glnode = calloc(sizeof(*glnode), 1);
  if (node->matrix) {
    gkMakeNodeTransform(glnode);
    glm_mat4_copy(node->matrix->val, glnode->trans->local);
    glnode->trans->flags |= GK_TRANSF_LOCAL_ISVALID;

    if (node->matrixWorld) {
      glm_mat4_copy(node->matrixWorld->val, glnode->trans->world);
      glnode->trans->flags |= GK_TRANSF_WORLD_ISVALID;
    }
  } else if (node->transform) {
    gkMakeNodeTransform(glnode);
    agk_loadTransforms(node, glnode->trans);
    /* ak_transformCombine(node, glnode->trans->local[0]);
       glnode->trans->flags |= GK_TRANSF_LOCAL_ISVALID; 
     */
  }

  if (node->geometry) {
    AkInstanceGeometry *geomInst;
    AkGeometry         *geom;
    GkModel            *model;
    GkModelInst        *modelInst;
    AkResult            ret;

    geomInst = node->geometry;
    while (geomInst) {
      geom = ak_instanceObject(&geomInst->base);
      if (geom) {
        ret  = agk_loadGeometry(ctx, geom, &model);
        if (ret == AK_OK) {
          modelInst       = gkMakeInstance(model);
          modelInst->next = glnode->model;
          glnode->model   = modelInst;

          /* bind material */
          if (geomInst->bindMaterial)
            agk_loadMaterial(ctx, geomInst, modelInst);
        }
      }
      geomInst = (AkInstanceGeometry *)geomInst->base.next;
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
      if (!light) {
        lightInst = lightInst->next;
        continue;
      }

      ret   = agk_loadLight(ctx->doc,
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
      agk_loadNode(ctx, nodei, &glnode->nodeInst);
  }

  if (node->chld) {
    AkNode  *nodei;
    GkNode **glnodei;

    nodei   = node->chld;
    glnodei = &glnode->chld;
    do {
      agk_loadNode(ctx, nodei, glnodei);
      glnodei = &(*glnodei)->next;
      nodei   = nodei->next;
    } while (nodei);
  }

  *dest = glnode;
  return AK_OK;
}
