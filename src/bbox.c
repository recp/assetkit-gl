/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/agk.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <assetkit.h>
#include <gk/gk.h>
#include <cglm/cglm.h>

GkBBox*
agk_bbox(AkBoundingBox * __restrict bbox) {
  GkBBox *glbbox;

  if (!bbox)
    return NULL;

  glbbox = malloc(sizeof(*glbbox));
  glm_vec_copy(bbox->min, glbbox->min);
  glm_vec_copy(bbox->max, glbbox->max);

  ak_bbox_center(bbox, glbbox->center);
  glbbox->radius = ak_bbox_radius(bbox);

  return glbbox;
}
