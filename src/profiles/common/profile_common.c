/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "profile_common.h"
#include "color_or_tex.h"
#include "../../enum.h"

#include <string.h>
#include <stdlib.h>

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

  if (!profilei) {
    *dest = NULL;
    return AK_EFOUND;
  }

  material = NULL;
  profile  = (AkProfileCommon *)profilei;
  tfx      = profile->technique;
  techn    = tfx->common;

  switch (tfx->common->type) {
    case AK_MATERIAL_SPECULAR_GLOSSINES:
      material = agk_specGloss(actx, (AkSpecularGlossiness *)techn);
      break;
    case AK_MATERIAL_METALLIC_ROUGHNESS:
      material = agk_metalRough(actx, (AkMetallicRoughness *)techn);
      break;
    case AK_MATERIAL_PHONG:
      material = agk_phong(actx, techn);
      break;
    case AK_MATERIAL_BLINN:
      material = agk_blinn(actx, techn);
      break;
    case AK_MATERIAL_LAMBERT:
      material = agk_lambert(actx, techn);
      break;
    case AK_MATERIAL_CONSTANT:
      material = agk_constant(actx, techn);
      break;
    default:
      goto ret;
  }

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
      transp->color = agk_colorOrTex(actx, aktransp->color);

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
      refl->color = agk_colorOrTex(actx, akrefl->color);
    
    gltechn->reflective = refl;
  }
  
ret:
  *dest = material;

  return AK_OK;
}
