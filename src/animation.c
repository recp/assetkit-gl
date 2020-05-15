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

GK_INLINE
GkType
agkTargetType(size_t stride) {
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
agkLoadAnimations(AgkContext * __restrict ctx) {
  AkDoc       *doc;
  AkAnimation *anim;
  AkLibrary   *animItem;
  const char  *sidAttrib;
  AkContext    actx = {0};

  doc      = ctx->doc;
  actx.doc = doc;
  animItem = doc->lib.animations;

  while (animItem) {
    anim = (void *)animItem->chld;

    while (anim) {
      AkAnimSampler       *sampler;
      AkChannel           *ch;
      AkBuffer            *buff;
      GkKeyFrameAnimation *ganim;
      RBTree              *samplerMap;

      samplerMap = rb_newtree(NULL, ds_cmp_ptr, NULL);
      ganim      = gkKeyFrameAnimation();
      ganim->base.nRepeat = UINT_MAX;

      if ((sampler = anim->sampler)) {
        do {
          GkAnimSampler *gsampler;
          GkBuffer      *gbuff;
          AkAccessor    *acc;
          AkInput       *inp;
          size_t         its, ots, byteLength;

          gsampler               = calloc(1, sizeof(*gsampler));
          gsampler->preBehavior  = (GkSamplerBehavior)sampler->pre;
          gsampler->postBehavior = (GkSamplerBehavior)sampler->post;

          its = ots = 0;
          inp = sampler->input;

          while (inp) {
            acc         = inp->accessor;
            buff        = acc->buffer;
            gbuff       = malloc(sizeof(*gbuff));
            
            byteLength  = acc->byteLength;
            gbuff->data = malloc(byteLength);
            gbuff->len  = byteLength;

            /* TODO: check stride */
            memcpy(gbuff->data,
                   buff->data + acc->byteOffset,
                   byteLength);

            gbuff->count  = acc->count;// byteLength / acc->componentBytes; /* TODO: */
            gbuff->stride = acc->componentCount;

            switch (inp->semantic) {
              case AK_INPUT_SEMANTIC_INTERPOLATION:
                gsampler->interp       = gbuff;
                break;
              case AK_INPUT_SEMANTIC_INPUT:
                gsampler->input        = gbuff;
                gsampler->inputStride  = acc->componentCount;
                break;
              case AK_INPUT_SEMANTIC_OUTPUT:
                gsampler->output       = gbuff;
                gsampler->outputStride = acc->componentCount;
                break;
              case AK_INPUT_SEMANTIC_IN_TANGENT:
                gsampler->inTangent    = gbuff;
                its                    = acc->componentCount;
                break;
              case AK_INPUT_SEMANTIC_OUT_TANGENT:
                gsampler->outTangent   = gbuff;
                ots                    = acc->componentCount;
                break;
              default:
                free(gbuff);
                break;
            }
            inp = inp->next;
          }

          gsampler->inTangentStride  = (float)its;
          gsampler->outTangentStride = (float)ots;

          rb_insert(samplerMap, sampler, gsampler);
        } while ((sampler = (void *)sampler->base.next));
      }

      if ((ch = anim->channel)) {
        GkChannel *last_gch;

        last_gch = NULL;

        do {
          GkChannel *gch;
          char      *target, *gtarget;
          GkBuffer  *inp;
          uint32_t   attribOff;

          if (ch->target) {
            target = ak_sid_resolve(&actx, ch->target, &sidAttrib);
            if (target) {
              gtarget         = rb_find(ctx->objMap, target);
              attribOff       = ak_sid_attr_offset(sidAttrib);
              sampler         = ak_getObjectByUrl(&ch->source);
              gch             = calloc(1, sizeof(*gch));
              gch->sampler    = rb_find(samplerMap, sampler);

              /* TODO: currently only floats are supported */
              gch->target     = gtarget + attribOff * sizeof(float);
              gch->targetType = agkTargetType(gch->sampler->output->stride);
              gch->stride     = gch->sampler->outputStride;

              inp             = gch->sampler->input;
              gch->endAt      = *(float *)((char *)inp->data + inp->len - sizeof(float));
              gch->beginAt    = *(float *)inp->data;
              gch->duration   = gch->endAt - gch->beginAt;
 
              ganim->channelCount++;

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
                    gch->node             = rb_find(ctx->objMap, node);
                    gch->isTransform      = true;
                    gch->isLocalTransform = true;
                    break;
                  }
                  default:  break;
                }
              }

              if (last_gch)
                last_gch->next = gch;
              else
                ganim->channel = gch;
              last_gch = gch;
            }
          }

          /* pre-resolved e.g. glTF */
          else if (ch->resolvedTarget) {
            gtarget         = rb_find(ctx->objMap, ch->resolvedTarget);
            sampler         = ak_getObjectByUrl(&ch->source);
            gch             = calloc(1, sizeof(*gch));
            gch->sampler    = rb_find(samplerMap, sampler);
            gch->target     = gtarget;
            gch->property   = (GkTargetPropertyType)ch->targetType;
            gch->targetType = agkTargetType(gch->sampler->output->stride);
            gch->stride     = gch->sampler->outputStride;

            inp             = gch->sampler->input;
            gch->endAt      = *(float *)((char *)inp->data + inp->len - sizeof(float));
            gch->beginAt    = *(float *)inp->data;
            gch->duration   = gch->endAt - gch->beginAt;

            ganim->channelCount++;

            if (ch->targetType != AK_TARGET_WEIGHTS) {
              AkNode *node;
              node                  = ak_mem_parent(ch->resolvedTarget);
              gch->node             = rb_find(ctx->objMap, node);
              gch->isTransform      = true;
              gch->isLocalTransform = true;
            } else {
              gch->targetType       = GKT_WEIGHTS;
              gch->stride           = (uint32_t)(gch->sampler->output->count / gch->sampler->input->count);
            }

            if (gch->sampler
                && sampler->uniInterpolation != AK_INTERPOLATION_UNKNOWN)
              gch->sampler->uniInterp = (GkInterpType)sampler->uniInterpolation;

            if (last_gch)
              last_gch->next = gch;
            else
              ganim->channel = gch;
            last_gch = gch;
          }
        } while ((ch = ch->next));
      }

      rb_destroy(samplerMap);

      /* TODO: */
      gkAddAnimation(ctx->scene, NULL, &ganim->base);

      anim = (AkAnimation *)anim->base.next;
    }

    animItem = animItem->next;
  }
}
