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
               GkNode   * __restrict node,
               AkLight  * __restrict light,
               GkLight ** __restrict dest) {
  AkLightBase *tcommon;
  GkLight     *gllight;
  tcommon = light->tcommon;

  switch (tcommon->type) {
    case AK_LIGHT_TYPE_AMBIENT: {
      AkAmbientLight *ambientSrc;
      GkAmbientLight *ambientDest;

      ambientSrc  = (AkAmbientLight *)light->tcommon;
      ambientDest = malloc(sizeof(*ambientDest));
      gllight     = ambientDest;
      glm_vec4_dup3(ambientSrc->color.vec,
                    ambientDest->color.vec);

      ambientDest->type = GK_LIGHT_TYPE_AMBIENT;
      break;
    }
    case AK_LIGHT_TYPE_DIRECTIONAL: {
      AkDirectionalLight *directionalSrc;
      GkDirectionalLight *directionalDest;

      directionalSrc  = (AkDirectionalLight *)light->tcommon;
      directionalDest = malloc(sizeof(*directionalDest));
      gllight         = &directionalDest->base;
      glm_vec4_dup3(directionalSrc->color.vec,
                    directionalDest->base.color.vec);

      directionalDest->base.type = GK_LIGHT_TYPE_DIRECTIONAL;
      break;
    }
    case AK_LIGHT_TYPE_POINT: {
      AkPointLight *pointSrc;
      GkPointLight *pointDest;

      pointSrc  = (AkPointLight *)light->tcommon;
      pointDest = malloc(sizeof(*pointDest));
      gllight   = &pointDest->base;
      glm_vec4_dup3(pointSrc->base.color.vec,
                    pointDest->base.color.vec);

      pointDest->constAttn  = pointSrc->constAttn;
      pointDest->linearAttn = pointSrc->linearAttn;
      pointDest->quadAttn   = pointSrc->quadAttn;

      pointDest->base.type = GK_LIGHT_TYPE_POINT;
      break;
    }
    case AK_LIGHT_TYPE_SPOT: {
      AkSpotLight *spotSrc;
      GkSpotLight *spotDest;

      spotSrc  = (AkSpotLight *)light->tcommon;
      spotDest = malloc(sizeof(*spotDest));
      gllight  = &spotDest->base;
      glm_vec4_dup3(spotSrc->base.color.vec,
                    spotDest->base.color.vec);

      spotDest->constAttn    = spotSrc->constAttn;
      spotDest->linearAttn   = spotSrc->linearAttn;
      spotDest->quadAttn     = spotSrc->quadAttn;
      spotDest->falloffAngle = spotSrc->falloffAngle;
      spotDest->falloffExp   = spotSrc->falloffExp;

      spotDest->base.type = GK_LIGHT_TYPE_POINT;
      break;
    }
    default:
      return AK_ETCOMMON;
  }

  gllight->index   = -1;
  gllight->isvalid = 0;
  gllight->next    = NULL;
  gllight->node    = node;

  *dest = gllight;

  return AK_OK;
}
