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
#include <math.h>

/* currently only common profile until full-impl */
#include "profiles/common/ak_profile_common.h"

AkResult
agk_loadMaterial(AgkContext         * __restrict ctx,
                 AkInstanceGeometry * __restrict geomInst,
                 GkMaterial        ** __restrict dest) {
  AkMaterial *material;
  AkInstanceMaterial *materialInst;
  materialInst = geomInst->bindMaterial->tcommon;
  material     = ak_instanceObject(&materialInst->base);
  if (material && material->effect) {
    AkEffect *effect;
    effect = ak_instanceObject(&material->effect->base);

    /* TODO: other profiles */
    if (effect->profile->type == AK_PROFILE_TYPE_COMMON) {
      GkMaterial *material;
      AkResult    ret;
      ret = agk_profileCommon(ctx,
                              geomInst->bindMaterial,
                              effect->profile,
                              &material);

      if (ret == AK_OK)
        *dest = material;
    }
  }
  
  return AK_OK;
}
