/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/agk.h"
#include "common.h"

AkResult
agk_loadGeometry(AgkContext  * __restrict ctx,
                 AkGeometry  * __restrict geom,
                 GkGeometry    ** __restrict dest) {
  AkObject    *prim;
  GkGeometry  *ggeom;
  AkResult     ret;

  ggeom = gk_geom_find(ctx->ctx, geom);
  if (ggeom && (ggeom->flags & GK_COMPLEX)) {
    *dest = ggeom;
    return AK_OK;
  }

  prim = geom->gdata;
  switch ((AkGeometryType)prim->type) {
    case AK_GEOMETRY_MESH:
       ret = agkLoadMesh(ctx, ak_objGet(prim), dest);
      break;
    default:
      ret = AK_ERR;
  }

  if (*dest)
    gk_geom_add(ctx->ctx, *dest, geom);

  return ret;
}
