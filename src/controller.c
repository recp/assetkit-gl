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
#include <math.h>

static
GkModelInst*
agkLoadBaseGeom(AgkContext           * __restrict ctx,
                GkNode               * __restrict glnode,
                AkInstanceController * __restrict ctlrInst,
                AkController         * __restrict ctlr,
                GkModelInst         ** __restrict dest) {
  AkGeometry         *geom;
  GkModel            *model;
  GkModelInst        *modelInst;
  AkInstanceGeometry *instGeom;
  AkBindMaterial     *bindMaterial;
  AkResult            ret;

  bindMaterial = NULL;
  geom         = NULL;

  if ((instGeom = ak_getObjectByUrl(&ctlrInst->geometry))) {
    geom         = ak_instanceObject(&instGeom->base);
    bindMaterial = instGeom->bindMaterial;
  }

  if (!geom) {
    geom         = ak_baseGeometry(ctlr);
    bindMaterial = ctlrInst->bindMaterial;
  }

  if (!geom)
    return NULL;

  ret = agk_loadGeometry(ctx, geom, &model);
  if (ret == AK_OK) {
    modelInst       = gkMakeInstance(model);
    modelInst->next = glnode->model;
    glnode->model   = modelInst;

    /* bind material */
    if (bindMaterial)
      agkLoadBindMaterial(ctx, geom, bindMaterial, modelInst);

    *dest = modelInst;
    return modelInst;
  }

  return NULL;
}

static
inline
void
akgSetJoints(AgkContext * __restrict ctx,
             AkNode    ** __restrict joints,
             GkNode    ** __restrict gljoints,
             size_t                  count) {
  size_t i;

  for (i = 0; i < count; i++) {
    GkNode *node;
    node = rb_find(ctx->objMap, joints[i]);

#ifdef DEBUG
    assert(node);
#endif

    gljoints[i] = node;
  }
}

static
GkSkin*
akgLoadSkin(AgkContext * __restrict ctx, AkSkin * __restrict skin) {
  GkSkin  *glskin;
  size_t   count, i;
  uint32_t primCount, maxJointCount;

  count                = skin->nJoints;
  primCount            = skin->nPrims;

  glskin               = calloc(1, sizeof(*glskin));
  glskin->base.type    = GK_CONTROLLER_SKIN;
  glskin->invBindPoses = malloc(sizeof(mat4) * count);
  glskin->joints       = malloc(sizeof(*glskin->joints)  * count);
  glskin->gbuffs       = calloc(primCount, sizeof(void*));

  glm_mat4_copy(skin->bindShapeMatrix, glskin->bindShapeMatrix);
  memcpy(glskin->invBindPoses, skin->invBindPoses, count * sizeof(mat4));
  maxJointCount = 4;

  /* default joints */
  if (skin->joints)
    akgSetJoints(ctx, skin->joints, glskin->joints, count);

  glskin->nJoints = skin->nJoints;

  for (i = 0; i < primCount; i++) {
    GkGpuBuffer   *gbuff;
    void          *tmp;
    GkBoneWeights *glweights;
    AkBoneWeights *w;
    size_t         size;

    tmp       = NULL;
    w         = skin->weights[i];
    glweights = malloc(sizeof(*glweights));
    size      = ak_skinFill(w, maxJointCount, maxJointCount, &tmp);

    glweights->nWeights = w->nWeights;
    glweights->nVertex  = w->nVertex;

    /* store weights in GPU as BONEx4 | WEIGHTx4 */
    glskin->gbuffs[i] = gbuff = gkGpuBufferNew(ctx->ctx, GK_ARRAY, size);
    gkGpuBufferFeed(gbuff, GK_STATIC_DRAW, tmp);

    ak_free(tmp);
  }

  glskin->nPrims  = primCount;

  return glskin;
}

void
akgWalkController(RBTree *tree, RBNode *rbnode) {
  AgkContext           *ctx;
  AkNode               *node;
  GkNode               *glnode;
  AkInstanceController *ctlrInst;
  AkController         *ctlr;

  node   = rbnode->key;
  glnode = rbnode->val;

  if (!(ctlrInst = node->controller))
    return;

  ctx = tree->userData;
  while (ctlrInst) {
    ctlr = ak_instanceObject(&ctlrInst->base);
    if (ctlr && ctlr->data) {
      switch (ctlr->data->type) {
        case AK_CONTROLLER_SKIN: {
          GkControllerInst *glCtlrInst;
          AkSkin           *skin;
          GkSkin           *glskin;
          GkModelInst      *baseGeom;

          skin       = ak_objGet(ctlr->data);
          glCtlrInst = calloc(1, sizeof(*glCtlrInst));
          baseGeom   = NULL;

          agkLoadBaseGeom(ctx, glnode, ctlrInst, ctlr, &baseGeom);
          if (baseGeom) {
            if (!(glskin = rb_find(ctx->ctlr, skin)))
              glskin = akgLoadSkin(ctx, skin);

            glCtlrInst->ctlr = &glskin->base;

            /* per instance skin joints */
            if (ctlrInst->joints) {
              glCtlrInst->joints = calloc(skin->nJoints,
                                          sizeof(*glCtlrInst->joints));
              akgSetJoints(ctx,
                           ctlrInst->joints,
                           glCtlrInst->joints,
                           skin->nJoints);
            }

            glskin->base.source = baseGeom;
            gkMakeInstanceSkin(ctx->scene, glnode, glCtlrInst);
            gkAttachSkin(glskin);
          }
          break;
        }
        case AK_CONTROLLER_MORPH:
          break;
        default:
          break;
      }
    }
    ctlrInst = (AkInstanceController *)ctlrInst->base.next;
  }
}

AkResult
akgLoadControllers(AgkContext * __restrict ctx) {
  ctx->instCtlr->userData = ctx;
  rb_walk(ctx->instCtlr, akgWalkController);
  return AK_OK;
}
