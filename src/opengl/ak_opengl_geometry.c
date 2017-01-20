/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../include/ak-opengl.h"
#include <assetkit.h>
#include <stdlib.h>
#include <assert.h>
#include <gk.h>

AkResult
ak_glLoadGeometry(AkGLContext * __restrict ctx,
                  AkGeometry  * __restrict geom,
                  GkModel    ** __restrict dest) {
  AkObject    *prim;
  GkModel     *model;
  AkResult     ret;

  model = gk_model_find(geom);
  if (model && (model->flags & GK_COMPLEX)) {
    *dest = model;
    return AK_OK;
  }

  prim = geom->gdata;
  switch ((AkGeometryType)prim->type) {
    case AK_GEOMETRY_TYPE_MESH:
      ret = ak_glLoadMesh(ctx,
                          ak_objGet(prim),
                          dest);
      break;
    default:
      ret = AK_ERR;
  }

  if (*dest)
    gk_model_add(*dest, geom);

  return ret;
}
