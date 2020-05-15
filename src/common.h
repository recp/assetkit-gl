/*
* Copyright (c), Recep Aslantas.
*
* MIT License (MIT), http://opensource.org/licenses/MIT
* Full license can be found in the LICENSE file
*/

#ifndef agk_src_common_h
#define agk_src_common_h

#include "../include/agk.h"
#include <gk/gk.h>
#include <gk/vertex.h>

GkGPUAccessor*
agkAccessor(AkAccessor  * __restrict acc,
            GkGpuBuffer * __restrict gbuff);

#endif /* agk_src_common_h */
