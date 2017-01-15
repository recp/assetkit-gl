/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../include/ak-opengl.h"
#include <stdlib.h>
#include <assert.h>
#include <assetkit.h>
#include <gk.h>

AkResult
ak_glLoadLight(AkDoc    * __restrict doc,
               AkLight  *light,
               GkLight **dest) {
  AkLightBase *tcommon;
  tcommon = light->tcommon;

  switch (tcommon->type) {
    case AK_LIGHT_TYPE_AMBIENT: {
      AkAmbientLight *ambientSrc;
      GkAmbientLight *ambientDest;

      ambientDest = malloc(sizeof(*ambientDest));
      glm_vec4_dup3(ambientSrc->color.vec,
                    ambientDest->color.vec);

      ambientDest->type = GK_LIGHT_TYPE_AMBIENT;
      *dest = ambientDest;
      break;
    }
    case AK_LIGHT_TYPE_DIRECTIONAL: {
      AkDirectionalLight *directionalSrc;
      GkDirectionalLight *directionalDest;

      directionalDest = malloc(sizeof(*directionalDest));
      glm_vec4_dup3(directionalSrc->color.vec,
                    directionalDest->color.vec);

      directionalDest->type = GK_LIGHT_TYPE_DIRECTIONAL;
      *dest = directionalDest;
      break;
    }
    case AK_LIGHT_TYPE_POINT: {
      AkPointLight *pointSrc;
      GkPointLight *pointDest;

      pointDest = malloc(sizeof(*pointDest));
      glm_vec4_dup3(pointSrc->base.color.vec,
                    pointDest->base.color.vec);

      pointDest->constAttn  = pointSrc->constAttn;
      pointDest->linearAttn = pointSrc->linearAttn;
      pointDest->quadAttn   = pointSrc->quadAttn;

      pointDest->base.type = GK_LIGHT_TYPE_POINT;
      *dest = &pointDest->base;
      break;
    }
    case AK_LIGHT_TYPE_SPOT: {
      AkSpotLight *spotSrc;
      GkSpotLight *spotDest;

      spotDest = malloc(sizeof(*spotDest));
      glm_vec4_dup3(spotSrc->base.color.vec,
                    spotDest->base.color.vec);

      spotDest->constAttn    = spotSrc->constAttn;
      spotDest->linearAttn   = spotSrc->linearAttn;
      spotDest->quadAttn     = spotSrc->quadAttn;
      spotDest->falloffAngle = spotSrc->falloffAngle;
      spotDest->falloffExp   = spotSrc->falloffExp;

      spotDest->base.type = GK_LIGHT_TYPE_POINT;
      *dest = &spotDest->base;
      break;
    }
    default:
      return AK_ETCOMMON;
  }

  return AK_OK;
}
