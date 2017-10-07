/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef agk_constant_h
#define agk_constant_h

#include "../../../../include/ak-opengl.h"

GkMaterial*
agk_constant(AkContext    * __restrict actx,
             AkConstantFx * __restrict constant,
             const char   * routine);

#endif /* agk_constant_h */
