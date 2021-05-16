/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/agk.h"
#include "common.h"

/* currently only common profile until full-impl */
#include "profiles/common/profile_common.h"

AkResult
agkLoadBindMaterial(AgkContext     * __restrict ctx,
                    AkGeometry     * __restrict geom,
                    AkBindMaterial * __restrict bindMaterial,
                    GkGeometryInst * __restrict geomInst) {
  AkMaterial         *material;
  AkInstanceMaterial *materialInst;

  materialInst = bindMaterial->tcommon;

  while (materialInst) {
    GkMaterial *glmaterial;
    AkHeap     *heap;
    AkContext   actx = {0};
    AkResult    ret;

    ret        = AK_ERR;
    glmaterial = NULL;

    heap                  = ak_heap_getheap(materialInst);
    actx.doc              = ak_heap_data(heap);
    actx.instanceMaterial = materialInst;

    /* load material */
    material = ak_instanceObject(&materialInst->base);
    if (material && material->effect) {
      AkEffect *effect;

      actx.techniqueHint = material->effect->techniqueHint;

      if ((effect = ak_instanceObject(&material->effect->base)))
        /* TODO: other profiles */
        ret = agk_profileCommon(ctx, &actx, effect, &glmaterial);
    }

    /* there is symbol, bind only to specified primitive */
    if (materialInst->symbol) {
      AkMapItem *mi;
      mi = ak_map_find(geom->materialMap, (void *)materialInst->symbol);

      while (mi) {
        AkMeshPrimitive *prim;
        GkPrimInst      *glprimInst;

        if ((prim = mi->data)) {
          glprimInst = &geomInst->prims[(uintptr_t)prim->udata];
          glprimInst->material = glmaterial;

          /* bind inputs (textures...) */
          if (materialInst->bindVertexInput) {
            AkBindVertexInput *bvi;

            /* bind vertex input */
            bvi = materialInst->bindVertexInput;
            while (bvi) {
              AkInput   *input;
              GkTexture *textobind;

              if (!bvi->semantic)
                goto cont;

              input     = ak_meshInputGet(prim, bvi->inputSemantic, bvi->inputSet);
              textobind = rb_find(ctx->texmap, (void *)bvi->semantic);
              if (input && textobind) {
                char           attribName[64];
                GkBindTexture *bindtex;
                
                ak_inputNameBySet(input, attribName);
                
                bindtex                 = calloc(1, sizeof(*bindtex));
                bindtex->texture        = textobind;
                bindtex->coordInputName = strdup(attribName);
                
                bindtex->next           = glprimInst->bindTexture;
                glprimInst->bindTexture = bindtex;
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
        geomInst->material = glmaterial;

      /* TODO: bind vertex inputs? */
    }

    materialInst = (AkInstanceMaterial *)materialInst->base.next;
  }
  
  return AK_ERR;
}

AkResult
agkLoadPrimMaterial(AgkContext      * __restrict ctx,
                    AkMeshPrimitive * __restrict prim,
                    GkPrimitive     * __restrict glprim) {
  AkMaterial *material;
  AkEffect   *effect;
  GkMaterial *glmaterial;
  AkContext   actx = {0};
  
  if (!(material = prim->material) || !material->effect)
    return AK_ERR;
  
  glmaterial         = NULL;
  actx.doc           = ctx->doc;
  actx.techniqueHint = material->effect->techniqueHint;
  
  if ((effect = ak_instanceObject(&material->effect->base))) {
    if (!(glmaterial = rb_find(ctx->materials, effect))) {
      /* TODO: other profiles */
      agk_profileCommon(ctx, &actx, effect, &glmaterial);
      rb_insert(ctx->materials, effect, glmaterial);
    }
  }

  glprim->material = glmaterial;

  return AK_ERR;
}
