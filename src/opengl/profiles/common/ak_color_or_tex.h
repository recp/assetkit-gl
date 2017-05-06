/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef ak_color_or_tex_h
#define ak_color_or_tex_h

#include "../../../../include/ak-opengl.h"

void
agk_copyColorOrTex(AkContext      * __restrict actx,
                   AkFxColorOrTex * __restrict src,
                   GkColorOrTex   * __restrict dest);

#endif /* ak_color_or_tex_h */
