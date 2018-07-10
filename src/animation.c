/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/agk.h"
#include <stdlib.h>
#include <assert.h>
#include <gk/gk.h>
#include <gk/animation.h>
#include <gk/animation-keyframe.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

void
agk_loadAnimations(AgkContext  * __restrict ctx) {
  AkDoc       *doc;
  AkAnimation *anim;
  AkLibItem   *animItem;
  const char  *sidAttrib;
  AkContext    actx = {0};

  doc      = ctx->doc;
  actx.doc = doc;
  animItem = doc->lib.animations;

  while (animItem) {
    anim = animItem->chld;

    while (anim) {
      AkAnimSampler       *sampler;
      AkChannel           *channel;
      AkBuffer            *buff;
      GkKeyFrameAnimation *glanim;
      RBTree              *samplerMap;

      samplerMap = rb_newtree(NULL, ds_cmp_ptr, NULL);
      glanim     = gkKeyFrameAnimation();
      glanim->base.nRepeat = UINT_MAX;

      if ((sampler = anim->sampler)) {
        GkAnimSampler *glSampler;
        GkBuffer      *glbuff;
        AkInput       *inp;
        AkSource      *src;

        glSampler               = calloc(1, sizeof(*glSampler));
        glSampler->preBehavior  = (GkSamplerBehavior)sampler->pre;
        glSampler->postBehavior = (GkSamplerBehavior)sampler->post;

        inp = sampler->input;
        while (inp) {
          src          = ak_getObjectByUrl(&inp->source);
          buff         = ak_getObjectByUrl(&src->tcommon->source);
          glbuff       = malloc(sizeof(*glbuff));
          glbuff->data = malloc(buff->length);
          glbuff->len  = buff->length;

          memcpy(glbuff->data, buff->data, buff->length);

          glbuff->count = glbuff->len / sizeof(float); /* TODO: */

          switch (inp->semantic) {
            case AK_INPUT_SEMANTIC_INTERPOLATION: glSampler->interp     = glbuff; break;
            case AK_INPUT_SEMANTIC_INPUT:         glSampler->input      = glbuff; break;
            case AK_INPUT_SEMANTIC_OUTPUT:        glSampler->output     = glbuff; break;
            case AK_INPUT_SEMANTIC_IN_TANGENT:    glSampler->inTangent  = glbuff; break;
            case AK_INPUT_SEMANTIC_OUT_TANGENT:   glSampler->outTangent = glbuff; break;
            default: free(glbuff); break;
          }
          inp = inp->next;
        }

        rb_insert(samplerMap, sampler, glSampler);
      }

      if ((channel = anim->channel)) {
        GkChannel *glchannel;
        char      *target, *gltarget;
        GkBuffer  *inp;
        uint32_t   attribOff;

        target = ak_sid_resolve(&actx, channel->target, &sidAttrib);
        if (target) {
          gltarget              = rb_find(ctx->objMap, target);
          attribOff             = ak_sid_attr_offset(sidAttrib);
          sampler               = ak_getObjectByUrl(&channel->source);
          glchannel             = calloc(1, sizeof(*glchannel));
          glchannel->sampler    = rb_find(samplerMap, sampler);
          glchannel->target     = gltarget + attribOff;
          glchannel->targetType = GKT_FLOAT;
          glanim->channel       = glchannel;

          inp = glchannel->sampler->input;

          glchannel->endTime   = 0.1 + *(float *)((char *)inp->data + inp->len - sizeof(float));
          glchannel->beginTime = 0.1 + *(float *)inp->data;
          glchannel->duration  = glchannel->endTime - glchannel->beginTime;

          glanim->channelCount++;

          if (ak_typeid(target) == AKT_OBJECT) {
            AkObject *obj;

            obj = (void *)target;
            switch (obj->type) {
              case AKT_TRANSLATE:
              case AKT_ROTATE:
              case AKT_MATRIX:
              case AKT_SCALE:
              case AKT_SKEW: {
                AkNode *node;

                node = ak_mem_parent(target); /* TODO Validate ? */
                glchannel->node             = rb_find(ctx->objMap, node);
                glchannel->isTransform      = true;
                glchannel->isLocalTransform = true;
                glchannel->target          += sizeof(GkTransformItem);
                break;
              }
              default:
                break;
            }
          }
        }
      }

      rb_destroy(samplerMap);

      /* TODO: */
      gkAddAnimation(ctx->scene, NULL, &glanim->base);

      anim = anim->next;
    }

    animItem = animItem->next;
  }
}
