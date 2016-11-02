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
  AkObject *primitive;

  primitive = geometry->gdata;
  switch ((AkGeometryType)primitive->type) {
    case AK_GEOMETRY_TYPE_MESH:
      return ak_glLoadMesh(doc,
                           ak_objGet(primitive),
                           usage,
                           dest);
    default:
      break;
  }
  return AK_OK;
}
