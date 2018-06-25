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

void
agk_loadAnimations(AgkContext  * __restrict ctx) {
  AkDoc       *doc;
  AkAnimation *anim;
  AkLibItem   *animItem;

  doc      = ctx->doc;
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

      if ((sampler = anim->sampler)) {
        GkAnimSampler *glSampler;
        GkBuffer      *glbuff;
        AkInput       *inp;
        AkSource      *src;

        glSampler               = calloc(1, sizeof(*glSampler));
        glSampler->preBehavior  = (GkSamplerBehavior)sampler->preBehavior;
        glSampler->postBehavior = (GkSamplerBehavior)sampler->postBehavior;

        inp = sampler->input;
        while (inp) {
          src          = ak_getObjectByUrl(&inp->source);
          buff         = ak_getObjectByUrl(&src->tcommon->source);
          glbuff       = malloc(sizeof(*glbuff));
          glbuff->data = malloc(buff->length);
          glbuff->len  = buff->length;

          memcpy(glbuff->data, buff->data, buff->length);

          switch (inp->semantic) {
            case AK_INPUT_SEMANTIC_INTERPOLATION: glSampler->interpolation = glbuff; break;
            case AK_INPUT_SEMANTIC_INPUT:         glSampler->input         = glbuff; break;
            case AK_INPUT_SEMANTIC_OUTPUT:        glSampler->output        = glbuff; break;
            case AK_INPUT_SEMANTIC_IN_TANGENT:    glSampler->inTangent     = glbuff; break;
            case AK_INPUT_SEMANTIC_OUT_TANGENT:   glSampler->outTangent    = glbuff; break;
            default: free(glbuff); break;
          }
          inp = inp->next;
        }

        rb_insert(samplerMap, sampler, glSampler);
      }

      if ((channel = anim->channel)) {
        GkChannel *glchannel;

        sampler            = ak_getObjectByUrl(&channel->source);
        glchannel          = calloc(1, sizeof(*glchannel));
        glchannel->sampler = rb_find(samplerMap, sampler);
        glchannel->target  = channel->target; /* TODO: */
        glanim->channel    = glchannel;
      }

      rb_destroy(samplerMap);

      anim = anim->next;
    }

    animItem = animItem->next;
  }
}
