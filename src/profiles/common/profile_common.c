/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "profile_common.h"
#include "../../common.h"
#include "../../enum.h"

AkResult
agk_profileCommon(AgkContext  * __restrict ctx,
                  AkContext   * __restrict actx,
                  AkEffect    * __restrict effect,
                  GkMaterial ** __restrict dest) {
  AkProfile            *profilei;
  AkProfileCommon      *profile;
  AkTechniqueFx        *tfx;
  GkMaterial           *material;
  GkTechnique          *gltechn;
  AkTechniqueFxCommon  *techn;

  /* find profile common */
  profilei = effect->profile;
  while (profilei) {
    if (profilei->type == AK_PROFILE_TYPE_COMMON)
      break;
    profilei = profilei->next;
  }

  if (!(profile = (AkProfileCommon *)profilei)
      || !(tfx = profile->technique)
      || !(techn = tfx->common)) {
    *dest = NULL;
    return AK_EFOUND;
  }

  material = NULL;

  switch (techn->type) {
    case AK_MATERIAL_SPECULAR_GLOSSINES:
      material = agkSpecGloss(ctx, actx, (AkSpecularGlossiness *)techn);
      break;
    case AK_MATERIAL_METALLIC_ROUGHNESS:
      material = agkMetalRough(ctx, actx, (AkMetallicRoughness *)techn);
      break;
    case AK_MATERIAL_PHONG:
      material = agkPhong(ctx, actx, techn);
      break;
    case AK_MATERIAL_BLINN:
      material = agkBlinn(ctx, actx, techn);
      break;
    case AK_MATERIAL_LAMBERT:
      material = agkLambert(ctx, actx, techn);
      break;
    case AK_MATERIAL_CONSTANT:
      material = agkConstant(ctx, actx, techn);
      break;
    default:
      goto ret;
  }
  
  if (material && material->technique) {
    if (material->technique->emission)
      material->technique->emission = agkColorOrTex(ctx, actx, techn->emission);

    if (techn->occlusion) {
      GkOcclusion *occlusion;

      occlusion           = calloc(1, sizeof(*occlusion));
      occlusion->tex      = agkLoadTexture(ctx, actx, techn->occlusion->tex);
      occlusion->strength = techn->occlusion->strength;

      material->technique->occlusion = occlusion;
    }

    if (techn->normal) {
      GkNormalMap *normal;

      normal        = calloc(1, sizeof(*normal));
      normal->tex   = agkLoadTexture(ctx, actx, techn->normal->tex);
      normal->scale = techn->normal->scale;
      
      material->technique->normal = normal;
    }
  }

  material->doubleSided = techn->doubleSided;
  gltechn = material->technique;

  if (techn->indexOfRefraction)
    material->indexOfRefraction = *techn->indexOfRefraction->val;
  
  if (techn->transparent) {
    AkTransparent *aktransp;
    GkTransparent *transp;
    
    transp   = calloc(1, sizeof(*gltechn->transparent));
    aktransp = techn->transparent;
    
    if (aktransp->amount)
      transp->amount = *aktransp->amount->val;
    else
      transp->amount = 1.0f;

    if (aktransp->color)
      transp->color = agkColorOrTex(ctx, actx, aktransp->color);

    transp->cutoff = aktransp->cutoff;
    transp->mode   = GK_ALPHA_BLEND;
    transp->opaque = agk_opaque(aktransp->opaque);

    gltechn->transparent = transp;
  }
  
  if (techn->reflective) {
    AkReflective *akrefl;
    GkReflective *refl;

    refl   = calloc(1, sizeof(*refl));
    akrefl = techn->reflective;
    
    if (akrefl->amount)
      refl->amount = *akrefl->amount->val;
    
    if (akrefl->color)
      refl->color = agkColorOrTex(ctx, actx, akrefl->color);
    
    gltechn->reflective = refl;
  }
  
ret:
  *dest = material;

  return AK_OK;
}
