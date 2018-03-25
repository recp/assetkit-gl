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
#include <gk/gk.h>
#include <cglm/cglm.h>

GkBBox*
agk_bbox(AkBoundingBox * __restrict bbox) {
  GkBBox *glbbox;

  if (!bbox)
    return NULL;

  glbbox = malloc(sizeof(*glbbox));
  glm_vec_copy(bbox->min, glbbox->local[0]);
  glm_vec_copy(bbox->max, glbbox->local[1]);

  return glbbox;
}
