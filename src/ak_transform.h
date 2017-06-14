/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef agk_transform_h
#define agk_transform_h

#include "../include/ak-opengl.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <assetkit.h>
#include <gk.h>
#include <cglm.h>

void
agk_loadTransforms(AkNode      * __restrict node,
                   GkTransform * __restrict gltrans);

#endif /* agk_transform_h */