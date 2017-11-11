/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "profile_common.h"
#include "color_or_tex.h"

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
  AkEffectCmnTechnique *technBase;
  const char           *subrtn;

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

  if (tfx->phong) {
    material  = agk_phong(actx, tfx->phong);
    technBase = &tfx->phong->base;
    subrtn    = "phong";
  } else if (tfx->blinn) {
    material  = agk_blinn(actx, tfx->blinn);
    technBase = &tfx->blinn->base;
    subrtn    = "blinn";
  } else if (tfx->lambert) {
    material  = agk_lambert(actx, tfx->lambert);
    technBase = &tfx->lambert->base;
    subrtn    = "lambert";
  } else if (tfx->constant) {
    material  = agk_constant(actx, tfx->constant);
    technBase = &tfx->constant->base;
    subrtn    = "constant";
  } else {
    goto ret;
  }
  
  material->technique->subroutine = strdup(subrtn);
  
  if (technBase->indexOfRefraction)
    material->indexOfRefraction = *technBase->indexOfRefraction->val;
  
  if (technBase->transparent) {
    AkTransparent *aktransp;
    GkTransparent *transp;
    
    transp   = calloc(sizeof(*material->transparent), 1);
    aktransp = technBase->transparent;
    
    if (aktransp->amount)
      transp->amount = *aktransp->amount->val;
    
    if (transp->color)
      transp->color = agk_colorOrTex(actx, aktransp->color);
    
    transp->cutoff = aktransp->cutoff;
    transp->mode   = GK_ALPHA_BLEND;
    
    material->transparent = transp;
  }
  
  if (technBase->reflective) {
    AkReflective *akrefl;
    GkReflective *refl;
    
    refl   = calloc(sizeof(*refl), 1);
    akrefl = technBase->reflective;
    
    if (akrefl->amount)
      refl->amount = *akrefl->amount->val;
    
    if (akrefl->color)
      refl->color = agk_colorOrTex(actx, akrefl->color);
    
    material->reflective = refl;
  }
  
ret:
  *dest = material;

  return AK_OK;
}
