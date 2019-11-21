/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "transform.h"
#include <ak/transform.h>

void
agkLoadTransforms(AgkContext  * __restrict ctx,
                  AkNode      * __restrict node,
                  GkTransform * __restrict gtrans) {
  AkObject        *trans, *transGroup;
  GkTransformItem *first, *last;
  void            *transdata;

  if (!node->transform)
    return;

  first      = last = transdata = NULL;
  transGroup = node->transform->base;

again:
  trans = transGroup;
  while (trans) {
    switch (trans->type) {
      case AKT_MATRIX: {
        AkMatrix *matrix;
        GkMatrix *gmatrix;
        matrix = ak_objGet(trans);

        gmatrix = malloc(sizeof(*gmatrix));
        gmatrix->base.next = NULL;
        gmatrix->base.prev = last;
        gmatrix->base.type = GK_TRANS_MATRIX;

        last = &gmatrix->base;
        if (last->prev)
          last->prev->next = last;
        if (!first)
          first = last;

        transdata = &gmatrix->value;
        glm_mat4_copy(matrix->val, gmatrix->value);
        break;
      }
      case AKT_LOOKAT: {
        AkLookAt *lookAt;
        GkLookAt *glookat;
        lookAt = ak_objGet(trans);

        glookat = malloc(sizeof(*glookat));
        glookat->base.next = NULL;
        glookat->base.prev = last;
        glookat->base.type = GK_TRANS_LOOK_AT;

        last = &glookat->base;
        if (last->prev)
          last->prev->next = last;
        if (!first)
          first = last;

        transdata = &glookat->value;
        memcpy(glookat->value, lookAt->val, sizeof(vec3) * 3);
        break;
      }
      case AKT_ROTATE: {
        AkRotate *rotate;
        GkRotate *grotate;
        rotate = ak_objGet(trans);

        grotate = malloc(sizeof(*grotate));
        grotate->base.next = NULL;
        grotate->base.prev = last;
        grotate->base.type = GK_TRANS_ROTATE;

        last = &grotate->base;
        if (last->prev)
          last->prev->next = last;
        if (!first)
          first = last;

        transdata = &grotate->value;
        glm_vec4_copy(rotate->val, grotate->value);
        break;
      }
      case AKT_SCALE: {
        AkScale *scale;
        GkScale *gscale;
        scale = ak_objGet(trans);

        gscale = malloc(sizeof(*gscale));
        gscale->base.next = NULL;
        gscale->base.prev = last;
        gscale->base.type = GK_TRANS_SCALE;

        last = &gscale->base;
        if (last->prev)
          last->prev->next = last;
        if (!first)
          first = last;

        transdata = &gscale->value;
        glm_vec3_copy(scale->val, gscale->value);
        break;
      }
      case AKT_TRANSLATE: {
        AkTranslate *translate;
        GkTranslate *gtranslate;
        translate = ak_objGet(trans);

        gtranslate = malloc(sizeof(*gtranslate));
        gtranslate->base.next = NULL;
        gtranslate->base.prev = last;
        gtranslate->base.type = GK_TRANS_TRANSLATE;

        last = &gtranslate->base;
        if (last->prev)
          last->prev->next = last;
        if (!first)
          first = last;

        transdata = &gtranslate->value;
        glm_vec3_copy(translate->val, gtranslate->value);
        break;
      }
      case AKT_SKEW: {
        AkSkew *skew;
        GkSkew *gskew;
        skew = ak_objGet(trans);

        gskew = malloc(sizeof(*gskew));
        gskew->base.next = NULL;
        gskew->base.prev = last;
        gskew->base.type = GK_TRANS_SKEW;

        last = &gskew->base;
        if (last->prev)
          last->prev->next = last;
        if (!first)
          first = last;

        gskew->angle = skew->angle;

        glm_vec3_copy(skew->rotateAxis, gskew->rotateAxis);
        glm_vec3_copy(skew->aroundAxis, gskew->aroundAxis);
        break;
      }
      case AKT_QUATERNION: {
        AkQuaternion *quat;
        GkQuaternion *gquat;
        quat = ak_objGet(trans);

        gquat = malloc(sizeof(*gquat));
        gquat->base.next = NULL;
        gquat->base.prev = last;
        gquat->base.type = GK_TRANS_QUAT;

        last = &gquat->base;
        if (last->prev)
          last->prev->next = last;
        if (!first)
          first = last;

        transdata = &gquat->value;
        glm_vec4_copy(quat->val, gquat->value);
        break;
      }
    }

    rb_insert(ctx->objMap, trans, transdata);

    trans = trans->next;
  }

  if (transGroup != node->transform->item) {
    transGroup = node->transform->item;
    goto again;
  }

  gtrans->item = first;
}

