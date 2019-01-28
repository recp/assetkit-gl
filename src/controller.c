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

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

GkModelInst*
load_basegeom(AgkContext           * __restrict ctx,
              GkNode               * __restrict glnode,
              AkInstanceController * __restrict ctlrInst,
              AkSkin               * __restrict skin,
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
    geom         = ak_skinBaseGeometry(skin);
    bindMaterial = ctlrInst->bindMaterial;
  }

  ret = agk_loadGeometry(ctx, geom, &model);
  if (ret == AK_OK) {
    modelInst       = gkMakeInstance(model);
    modelInst->next = glnode->model;
    glnode->model   = modelInst;

    /* bind material */
    if (bindMaterial)
      agk_loadMaterial(ctx, geom, bindMaterial, modelInst);

    *dest = modelInst;
    return modelInst;
  }

  return NULL;
}

void
set_joints(AgkContext * __restrict ctx,
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

GkSkin*
load_skin(AgkContext * __restrict ctx, AkSkin * __restrict skin) {
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
    set_joints(ctx, skin->joints, glskin->joints, count);

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
ctlr_walk(RBTree *tree, RBNode *rbnode) {
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

          load_basegeom(ctx, glnode, ctlrInst, skin, &baseGeom);

          if (!(glskin = rb_find(ctx->ctlr, skin)))
            glskin = load_skin(ctx, skin);

          glCtlrInst->ctlr = &glskin->base;

          /* per instance skin joints */
          if (ctlrInst->joints) {
            glCtlrInst->joints = calloc(skin->nJoints,
                                        sizeof(*glCtlrInst->joints));
            set_joints(ctx,
                       ctlrInst->joints,
                       glCtlrInst->joints,
                       skin->nJoints);
          }

          glskin->base.source = baseGeom;
          gkMakeInstanceSkin(ctx->scene, glnode, glCtlrInst);
          gkAttachSkin(glskin);

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
load_controllers(AgkContext * __restrict ctx) {
  ctx->instCtlr->userData = ctx;
  rb_walk(ctx->instCtlr, ctlr_walk);
  return AK_OK;
}