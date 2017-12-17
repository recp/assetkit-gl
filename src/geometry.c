/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/agk.h"
#include <assetkit.h>
#include <stdlib.h>
#include <assert.h>
#include <gk/gk.h>

AkResult
agk_loadGeometry(AgkContext * __restrict ctx,
                 AkGeometry  * __restrict geom,
                 GkModel    ** __restrict dest) {
  AkObject    *prim;
  GkModel     *model;
  AkResult     ret;

  model = gk_model_find(ctx->ctx, geom);
  if (model && (model->flags & GK_COMPLEX)) {
    *dest = model;
    return AK_OK;
  }

  prim = geom->gdata;
  switch ((AkGeometryType)prim->type) {
    case AK_GEOMETRY_TYPE_MESH:
      ret = agk_loadMesh(ctx, ak_objGet(prim), dest);
      break;
    default:
      ret = AK_ERR;
  }

  if (*dest)
    gk_model_add(ctx->ctx, *dest, geom);

  return ret;
}
