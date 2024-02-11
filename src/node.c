/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/agk.h"
#include "common.h"
#include "transform.h"

AkResult
agk_loadNode(AgkContext * __restrict ctx,
             AkNode     * __restrict node,
             GkNode    ** __restrict dest) {
  AkInstanceBase *instNode;
  GkNode         *glnode;

  glnode = gkAllocNode(ctx->scene);
  rb_insert(ctx->objMap, node, glnode);

  if (node->matrix) {
    gkMakeNodeTransform(ctx->scene, glnode);

    glm_mat4_copy(node->matrix->val, glnode->trans->local);
    if (node->matrixWorld) {
      glm_mat4_copy(node->matrixWorld->val, glnode->trans->world);
      glnode->trans->flags |= GK_TRANSF_WORLD_ISVALID;
    }
  } else if (node->transform) {
    gkMakeNodeTransform(ctx->scene, glnode);
    glnode->trans->flags &= ~GK_TRANSF_LOCAL_ISVALID;
    agkLoadTransforms(ctx, node, glnode->trans);
    /* ak_transformCombine(node, glnode->trans->local[0]);
       glnode->trans->flags |= GK_TRANSF_LOCAL_ISVALID;
     */
  }

  if (node->geometry) {
    AkInstanceGeometry *geomInst;
    AkGeometry         *geom;
    GkGeometry         *ggeom;
    GkGeometryInst     *ggeomInst;
    AkResult            ret;

    geomInst = node->geometry;
    while (geomInst) {
      geom = ak_instanceObject(&geomInst->base);
      if (geom) {
        AkInstanceMorph *morphInst;
        AkInstanceSkin  *skinner;

        ret = agk_loadGeometry(ctx, geom, &ggeom);
        if (ret == AK_OK) {
          ggeomInst       = gkMakeInstance(ggeom);
          ggeomInst->next = glnode->geom;
          glnode->geom    = ggeomInst;
          
          /* bind material */
          if (geomInst->bindMaterial)
            agkLoadBindMaterial(ctx, geom, geomInst->bindMaterial, ggeomInst);

          if ((morphInst = geomInst->morpher)) {
            GkInstanceMorph *gmorphInst;
            GkMorph         *glmorph;
            
            glmorph    = akgLoadMorph(ctx, morphInst->morph);
            gmorphInst = calloc(1, sizeof(*gmorphInst));
            
            gmorphInst->overrideWeights  = calloc(1, sizeof(*gmorphInst->overrideWeights) * morphInst->overrideWeights->count);
            gmorphInst->nOverrideWeights = (uint32_t)morphInst->overrideWeights->count;

            gmorphInst->baseGeometry     = ggeom;
            gmorphInst->morph            = glmorph;
            glnode->morpher              = gmorphInst;

            rb_insert(ctx->objMap, morphInst->overrideWeights, gmorphInst->overrideWeights);
          }
          
          if ((skinner = geomInst->skinner) && skinner->skin) {
            /* postpone load skin until all nodes are loaded */
            AgkSkin2Load *skin2load;
            
            skin2load = calloc(1, sizeof(*skin2load));
            skin2load->glnode    = glnode;
            skin2load->skinner   = skinner;
            skin2load->geomInst = ggeomInst;
            
            skin2load->next      = ctx->skin2load;
            ctx->skin2load       = skin2load;
          }
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

      ret = agk_loadLight(ctx->doc, glnode, light, &gllight);
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

  if (node->chld) {
    AkNode  *nodei;
    GkNode **glnodei;

    nodei   = node->chld;
    glnodei = &glnode->chld;
    do {
      agk_loadNode(ctx, nodei, glnodei);
      (*glnodei)->parent = glnode;

      glnodei = &(*glnodei)->next;
      nodei   = nodei->next;
    } while (nodei);
  }

  /* Append instance node as child node,

     TODO: this behavior may be changed in the future,
     because gk may support node instances */
  if ((instNode = (AkInstanceBase *)node->node)) {
    AkNode *nodei;

    do {
      if ((nodei = ak_instanceObject(instNode))) {
        GkNode *inst;
        agk_loadNode(ctx, nodei, &inst);

        inst->next   = glnode->chld;
        glnode->chld = inst;
        inst->parent = glnode;
      }

      instNode = instNode->next;
    } while (instNode);
  }

  *dest = glnode;
  return AK_OK;
}
