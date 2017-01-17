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
ak_glLoadGeometry(AkDoc  * __restrict doc,
                  AkGeometry * geometry,
                  GLenum usage,
                  GkComplexModel ** dest) {
  AkObject    *prim;
  GkModelBase *modelBase;
  AkResult     ret;

  modelBase = gk_model_find(geometry);
  if (modelBase && (modelBase->flags & GK_COMPLEX)) {
    *dest = (GkComplexModel *)modelBase;
    return AK_OK;
  }

  prim = geometry->gdata;
  switch ((AkGeometryType)prim->type) {
    case AK_GEOMETRY_TYPE_MESH:
      ret = ak_glLoadMesh(doc,
                          ak_objGet(prim),
                          usage,
                          dest);
      break;
    default:
      ret = AK_ERR;
  }

  if (*dest)
    gk_model_add(&(*dest)->base, geometry);

  return ret;
}
