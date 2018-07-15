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

GkType
agk_targetType(size_t stride) {
  switch (stride) {
    case 1:
      return GKT_FLOAT;
    case 3:
      return GKT_FLOAT3;
    case 4:
      return GKT_FLOAT4;
    default: break;
  }

  return GKT_FLOAT;
}

void
agk_loadAnimations(AgkContext * __restrict ctx) {
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
        do {
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

            glbuff->count  = glbuff->len / sizeof(float); /* TODO: */
            glbuff->stride = src->tcommon->bound;

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
        } while ((sampler = sampler->next));
      }

      if ((channel = anim->channel)) {
        GkChannel *last_glchannel;

        last_glchannel = NULL;

        do {
          GkChannel *glchannel;
          char      *target, *gltarget;
          GkBuffer  *inp;
          uint32_t   attribOff;

          if (channel->target) {
            target = ak_sid_resolve(&actx, channel->target, &sidAttrib);
            if (target) {
              gltarget              = rb_find(ctx->objMap, target);
              attribOff             = ak_sid_attr_offset(sidAttrib);
              sampler               = ak_getObjectByUrl(&channel->source);
              glchannel             = calloc(1, sizeof(*glchannel));
              glchannel->sampler    = rb_find(samplerMap, sampler);
              glchannel->target     = gltarget + attribOff;
              glchannel->targetType = agk_targetType(glchannel->sampler->output->stride);

              inp                  = glchannel->sampler->input;
              glchannel->endTime   = *(float *)((char *)inp->data + inp->len - sizeof(float));
              glchannel->beginTime = *(float *)inp->data;
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
                    break;
                  }
                  default:  break;
                }
              }

              if (last_glchannel)
                last_glchannel->next = glchannel;
              else
                glanim->channel = glchannel;
              last_glchannel = glchannel;
            }
          }

          /* pre-resolved e.g. glTF */
          else if (channel->resolvedTarget){
            AkNode *node;

            node = ak_mem_parent(channel->resolvedTarget);

            gltarget              = rb_find(ctx->objMap, channel->resolvedTarget);
            sampler               = ak_getObjectByUrl(&channel->source);
            glchannel             = calloc(1, sizeof(*glchannel));
            glchannel->sampler    = rb_find(samplerMap, sampler);
            glchannel->target     = gltarget;
            glchannel->property   = (GkTargetPropertyType)channel->targetType;
            glchannel->targetType = agk_targetType(glchannel->sampler->output->stride);

            inp                   = glchannel->sampler->input;
            glchannel->endTime    = *(float *)((char *)inp->data + inp->len - sizeof(float));
            glchannel->beginTime  = *(float *)inp->data;
            glchannel->duration   = glchannel->endTime - glchannel->beginTime;

            glanim->channelCount++;

            glchannel->node             = rb_find(ctx->objMap, node);
            glchannel->isTransform      = true;
            glchannel->isLocalTransform = true;

            if (glchannel->sampler
                && sampler->uniInterpolation != AK_INTERPOLATION_UNKNOWN)
              glchannel->sampler->uniInterp = (GkInterpType)sampler->uniInterpolation;

            if (last_glchannel)
              last_glchannel->next = glchannel;
            else
              glanim->channel = glchannel;
            last_glchannel = glchannel;
          }

        } while ((channel = channel->next));
      }

      rb_destroy(samplerMap);

      /* TODO: */
      gkAddAnimation(ctx->scene, NULL, &glanim->base);

      anim = anim->next;
    }

    animItem = animItem->next;
  }
}
