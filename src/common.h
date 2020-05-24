/*
* Copyright (c), Recep Aslantas.
*
* MIT License (MIT), http://opensource.org/licenses/MIT
* Full license can be found in the LICENSE file
*/

#ifndef agk_src_common_h
#define agk_src_common_h

#include "../include/agk.h"

#include <cglm/cglm.h>

#include <ds/rb.h>
#include <ds/forward-list.h>

#include <gk/gk.h>
#include <gk/vertex.h>

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <assert.h>

GkGPUAccessor*
agkAccessor(AkAccessor  * __restrict acc,
            GkGpuBuffer * __restrict gbuff);

GkColorDesc*
agkColorOrTex(AgkContext  * __restrict ctx,
              AkContext   * __restrict actx,
              AkColorDesc * __restrict src);

#endif /* agk_src_common_h */
