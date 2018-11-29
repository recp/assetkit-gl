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

AkResult
agk_loadLight(AkDoc    * __restrict doc,
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

      ambientSrc  = (AkAmbientLight *)tcommon;
      ambientDest = calloc(1, sizeof(*ambientDest));
      gllight     = ambientDest;
      glm_vec4_copy(ambientSrc->color.vec,
                    ambientDest->color.vec);

      ambientDest->type = GK_LIGHT_TYPE_AMBIENT;
      ambientDest->name = strdup("ambient");
      break;
    }
    case AK_LIGHT_TYPE_DIRECTIONAL: {
      AkDirectionalLight *directionalSrc;
      GkDirectionalLight *directionalDest;

      directionalSrc  = (AkDirectionalLight *)tcommon;
      directionalDest = calloc(1, sizeof(*directionalDest));
      gllight         = &directionalDest->base;
      glm_vec4_copy(directionalSrc->color.vec,
                    directionalDest->base.color.vec);

      directionalDest->base.type = GK_LIGHT_TYPE_DIRECTIONAL;
      directionalDest->base.name = strdup("directional");
      break;
    }
    case AK_LIGHT_TYPE_POINT: {
      AkPointLight *pointSrc;
      GkPointLight *pointDest;

      pointSrc  = (AkPointLight *)tcommon;
      pointDest = calloc(1, sizeof(*pointDest));
      gllight   = &pointDest->base;
      glm_vec4_copy(pointSrc->base.color.vec,
                    pointDest->base.color.vec);

      pointDest->constAttn  = pointSrc->constAttn;
      pointDest->linearAttn = pointSrc->linearAttn;
      pointDest->quadAttn   = pointSrc->quadAttn;

      pointDest->base.type = GK_LIGHT_TYPE_POINT;
      pointDest->base.name = strdup("point");
      break;
    }
    case AK_LIGHT_TYPE_SPOT: {
      AkSpotLight *spotSrc;
      GkSpotLight *spotDest;

      spotSrc  = (AkSpotLight *)tcommon;
      spotDest = calloc(1, sizeof(*spotDest));
      gllight  = &spotDest->base;
      glm_vec4_copy(spotSrc->base.color.vec,
                    spotDest->base.color.vec);

      spotDest->constAttn    = spotSrc->constAttn;
      spotDest->linearAttn   = spotSrc->linearAttn;
      spotDest->quadAttn     = spotSrc->quadAttn;
      spotDest->cutoffCosine = cosf(spotSrc->falloffAngle * 0.5f);
      spotDest->cutoffExp    = spotSrc->falloffExp;
      spotDest->falloffAngle = spotSrc->falloffAngle;

      spotDest->base.type = GK_LIGHT_TYPE_SPOT;
      spotDest->base.name = strdup("spot");
      break;
    }
    default:
      return AK_ETCOMMON;
  }

  glm_vec3_copy(tcommon->direction, gllight->defdir);

  gllight->index   = -1;
  gllight->isvalid = 0;
  gllight->next    = NULL;
  gllight->node    = node;

  *dest = gllight;

  return AK_OK;
}
