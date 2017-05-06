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
                 GkModelInst        * __restrict modelInst) {
  AkMaterial         *material;
  AkInstanceMaterial *materialInst;

  materialInst = geomInst->bindMaterial->tcommon;

  while (materialInst) {
    AkGeometry *geom;
    GkMaterial *glmaterial;
    AkResult    ret;

    ret        = AK_ERR;
    glmaterial = NULL;
    geom       = ak_instanceObject(&geomInst->base);

    /* load material */
    material = ak_instanceObject(&materialInst->base);
    if (material && material->effect) {
      AkHeap   *heap;
      AkEffect *effect;
      AkContext actx = {0};

      heap               = ak_heap_getheap(material);
      actx.doc           = ak_heap_data(heap);
      actx.techniqueHint = material->effect->techniqueHint;

      effect = ak_instanceObject(&material->effect->base);

      /* TODO: other profiles */
      ret = agk_profileCommon(ctx,
                              &actx,
                              effect,
                              &glmaterial);
    }

    /* there is symbol, bind only to specified primitive */
    if (materialInst->symbol) {
      AkMapItem *mi;
      mi = ak_map_find(geom->materialMap,
                       (void *)materialInst->symbol);
      while (mi) {
        AkMeshPrimitive *prim;
        GkPrimitive     *glprim;
        GkPrimInst      *glprimInst;

        prim   = mi->data;
        glprim = prim->udata;
        if (glprim) {
          glprimInst = gkMakePrimInst(modelInst, glprim);
          glprimInst->material = glmaterial;
        }
        mi = mi->next;
      }
    }

    /* bind to whole geometry  */
    else {
      if (ret == AK_OK)
        modelInst->material = glmaterial;
    }

    materialInst = (AkInstanceMaterial *)materialInst->base.next;
  }
  
  return AK_ERR;
}
