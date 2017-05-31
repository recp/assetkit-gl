/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "ak_transform.h"
#include <ak-transform.h>

void
agk_loadTransforms(AkNode      * __restrict node,
                   GkTransform * __restrict gltrans) {
  AkObject        *transform;
  GkTransformItem *first, *last;

  first = last = NULL;
  transform = node->transform;
  while (transform) {
    switch (transform->type) {
      case AK_NODE_TRANSFORM_TYPE_MATRIX: {
        AkMatrix *matrix;
        GkMatrix *glmatrix;
        matrix = ak_objGet(transform);

        glmatrix = malloc(sizeof(*glmatrix));
        glmatrix->base.next = NULL;
        glmatrix->base.prev = last;
        glmatrix->base.type = GK_TRANS_MATRIX;

        last = &glmatrix->base;
        if (last->prev)
          last->prev->next = last;
        if (!first)
          first = last;

        glm_mat4_copy(matrix->val, glmatrix->value);
        break;
      }
      case AK_NODE_TRANSFORM_TYPE_LOOK_AT: {
        AkLookAt *lookAt;
        GkLookAt *gllookat;
        lookAt = ak_objGet(transform);

        gllookat = malloc(sizeof(*gllookat));
        gllookat->base.next = NULL;
        gllookat->base.prev = last;
        gllookat->base.type = GK_TRANS_LOOK_AT;

        last = &gllookat->base;
        if (last->prev)
          last->prev->next = last;
        if (!first)
          first = last;

        memcpy(gllookat->value, lookAt->val, sizeof(vec3) * 3);
        break;
      }
      case AK_NODE_TRANSFORM_TYPE_ROTATE: {
        AkRotate *rotate;
        GkRotate *glrotate;
        rotate = ak_objGet(transform);

        glrotate = malloc(sizeof(*glrotate));
        glrotate->base.next = NULL;
        glrotate->base.prev = last;
        glrotate->base.type = GK_TRANS_ROTATE;

        last = &glrotate->base;
        if (last->prev)
          last->prev->next = last;
        if (!first)
          first = last;

        glm_vec4_copy(rotate->val, glrotate->value);
        break;
      }
      case AK_NODE_TRANSFORM_TYPE_SCALE: {
        AkScale *scale;
        GkScale *glscale;
        scale = ak_objGet(transform);

        glscale = malloc(sizeof(*glscale));
        glscale->base.next = NULL;
        glscale->base.prev = last;
        glscale->base.type = GK_TRANS_SCALE;

        last = &glscale->base;
        if (last->prev)
          last->prev->next = last;
        if (!first)
          first = last;

        glm_vec_copy(scale->val, glscale->value);
        break;
      }
      case AK_NODE_TRANSFORM_TYPE_TRANSLATE: {
        AkTranslate *translate;
        GkTranslate *gltranslate;
        translate = ak_objGet(transform);

        gltranslate = malloc(sizeof(*gltranslate));
        gltranslate->base.next = NULL;
        gltranslate->base.prev = last;
        gltranslate->base.type = GK_TRANS_TRANSLATE;

        last = &gltranslate->base;
        if (last->prev)
          last->prev->next = last;
        if (!first)
          first = last;

        glm_vec_copy(translate->val, gltranslate->value);
        break;
      }
      case AK_NODE_TRANSFORM_TYPE_SKEW: {
        AkSkew *skew;
        GkSkew *glskew;
        skew = ak_objGet(transform);

        glskew = malloc(sizeof(*glskew));
        glskew->base.next = NULL;
        glskew->base.prev = last;
        glskew->base.type = GK_TRANS_SKEW;

        last = &glskew->base;
        if (last->prev)
          last->prev->next = last;
        if (!first)
          first = last;

        glskew->angle =  skew->angle;
        glm_vec_copy(skew->rotateAxis, glskew->rotateAxis);
        glm_vec_copy(skew->aroundAxis, glskew->aroundAxis);
        break;
      }
    }

    transform = transform->next;
  }
  
  gltrans->item = first;
}

