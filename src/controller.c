/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "enum.h"
#include "common.h"

#include "../include/agk.h"

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
    // GkBoneWeights *glweights;
    AkBoneWeights *w;
    size_t         size;

    tmp       = NULL;
    w         = skin->weights[i];
    // glweights = malloc(sizeof(*glweights));
    size      = ak_skinInterleave(w, maxJointCount, maxJointCount, &tmp);

    // glweights->nWeights = w->nWeights;
    // glweights->nVertex  = w->nVertex;

    /* store weights in GPU as BONEx4 | WEIGHTx4 */
    glskin->gbuffs[i] = gbuff = gkGpuBufferNew(ctx->ctx, GK_ARRAY, size);
    gkGpuBufferFeed(gbuff, GK_STATIC_DRAW, tmp);

    ak_free(tmp);
  }

  glskin->nPrims  = primCount;

  return glskin;
}

GkMorph*
akgLoadMorph(AgkContext * __restrict ctx, AkMorph * __restrict morph) {
//  GkMorph       *glmorph;
//  GkMorphTarget *gltarget, *last_gltarget;
//  AkMorphTarget *target;
//  GkVertexInput *vi, *last_vi;
//  GkGPUAccessor *gacc;
//  AkInput       *inp;
//  GkGpuBuffer   *gbuff;
//  void          *buff;
//  char           attribName[64], *pAttribName;
//  size_t         buffSize, byteOffset, byteStride;
//  uint32_t       i, targetIndex, foundInpCount;
//  bool           found;
//
//  if ((glmorph = rb_find(ctx->objMap, morph)))
//    return glmorph;
//
//  static AkInputSemantic desiredInp[] = {
//    AK_INPUT_POSITION,
//    AK_INPUT_NORMAL,
//    AK_INPUT_TANGENT
//  };
//
//  static uint32_t nDesiredInp  = AK_ARRAY_LEN(desiredInp);
//
//  if (!(target = morph->target))
//    return NULL;
//
//  glmorph           = calloc(1, sizeof(*glmorph));
//  glmorph->method   = (GkMorphMethod)morph->method;
//  /* glmorph->nTargets = morph->targetCount; */
//  targetIndex       = 0;
//  foundInpCount     = 0;
//  byteOffset        = 0;
//  last_gltarget     = NULL;
//
//  ak_morphInterleaveInspect(&buffSize, &byteStride, morph, desiredInp, nDesiredInp);
//  gbuff = gkGpuBufferNew(ctx->ctx, GK_ARRAY, buffSize);
//
//  do {
//    if (!(inp = target->input))
//      continue;
//
//    gltarget          = calloc(1, sizeof(*gltarget));
//    gltarget->weight  = target->weight;
//    gltarget->nInputs = target->inputCount;
//    last_vi           = NULL;
//
//    do {
//      found = false;
//
//      for (i = 0; i < nDesiredInp; i++) {
//        if (desiredInp[i] == inp->semantic) {
//          found = true;
//
//          if (++foundInpCount >= nDesiredInp)
//            goto nxt;
//
//          break;
//        }
//      }
//
//      if (!found)
//        continue;
//
//      pAttribName = attribName + sprintf(attribName, "TARGET%d_", targetIndex);
//      ak_inputNameBySet(inp, pAttribName);
//
//      gacc             = agkAccessor(inp->accessor, gbuff);
//      gacc->byteOffset = byteOffset;
//      gacc->byteStride = byteStride;
//
//      vi               = calloc(1, sizeof(*vi));
//      vi->name         = strdup(attribName);
//      vi->accessor     = gacc;
//
//      byteOffset      += gacc->filledSize;
//
//      AK_APPEND_FLINK(gltarget->inputs, last_vi, vi);
//    } while ((inp = inp->next));
//
//  nxt:
//    targetIndex++;
//
//    AK_APPEND_FLINK(glmorph->targets, last_gltarget, gltarget);
//  } while ((target = target->next));
//
//  glmorph->nTargets = targetIndex;
//
//  buff = malloc(buffSize);
//  ak_morphInterleave(buff, morph, desiredInp, nDesiredInp);
//  gkGpuBufferFeed(gbuff, GK_STATIC_DRAW, buff);
//  free(buff);
//
//  glmorph->buff = gbuff;
//
//  rb_insert(ctx->objMap, morph, glmorph);
//
//  return glmorph;
  return NULL;
}
