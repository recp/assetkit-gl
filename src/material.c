/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/agk.h"
#include <stdlib.h>
#include <assert.h>
#include <assetkit.h>
#include <gk/gk.h>
#include <math.h>
#include <string.h>

/* currently only common profile until full-impl */
#include "profiles/common/profile_common.h"

AkResult
agk_loadMaterial(AgkContext         * __restrict ctx,
                 AkInstanceGeometry * __restrict geomInst,
                 GkModelInst        * __restrict modelInst) {
  AkMaterial         *material;
  AkInstanceMaterial *materialInst;

  materialInst = geomInst->bindMaterial->tcommon;

  while (materialInst) {
    GkMaterial *glmaterial;
    AkGeometry *geom;
    AkHeap     *heap;
    AkContext   actx = {0};
    AkResult    ret;

    ret        = AK_ERR;
    glmaterial = NULL;
    geom       = ak_instanceObject(&geomInst->base);

    heap                  = ak_heap_getheap(materialInst);
    actx.doc              = ak_heap_data(heap);
    actx.instanceMaterial = materialInst;
    actx.bindVertexInput  = ak_map_new(ak_cmp_str);

    /* load material */
    material = ak_instanceObject(&materialInst->base);
    if (material && material->effect) {
      AkEffect *effect;

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
        GkPrimInst      *glprimInst;

        if ((prim = mi->data)) {
          glprimInst = &modelInst->prims[(uintptr_t)prim->udata];
          glprimInst->material = glmaterial;

          /* bind inputs (textures...) */
          if (materialInst->bindVertexInput) {
            AkBindVertexInput *bvi;

            /* bind vertex input */
            bvi = materialInst->bindVertexInput;
            while (bvi) {
              AkInputBasic *input;
              if (!bvi->semantic)
                goto cont;
              input = ak_meshInputGet(prim,
                                      bvi->inputSemantic,
                                      bvi->inputSet);
              if (input) {
                AkMapItem *boundVertexItem;
                boundVertexItem = ak_map_find(actx.bindVertexInput,
                                              (void *)bvi->semantic);
                while (boundVertexItem) {
                  char **boundInputName;
                  char   attribName[64];

                  ak_inputNameIndexed(input, attribName);

                  boundInputName  = boundVertexItem->data;
                  *boundInputName = strdup(attribName);

                  boundVertexItem = boundVertexItem->next;
                }
              }
            cont:
              bvi = bvi->next;
            }
          }
        }
        mi = mi->next;
      }
    }

    /* bind to whole geometry  */
    else {
      if (ret == AK_OK)
        modelInst->material = glmaterial;

      /* TODO: bind vertex inputs? */
    }

    ak_map_destroy(actx.bindVertexInput);
    materialInst = (AkInstanceMaterial *)materialInst->base.next;
  }
  
  return AK_ERR;
}
