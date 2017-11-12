/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef agk_enum_h
#define agk_enum_h

#include "../include/agk.h"

GLenum
agk_type(AkTypeId typeId);

GLenum
agk_wrapMode(AkWrapMode wrapMode);

GLenum
agk_minFilter(AkMinFilter minfilter);

GLenum
agk_magFilter(AkMagFilter magfilter);

GLenum
agk_mipFilter(AkMipFilter mipfilter);

GLenum
agk_drawMode(AkMeshPrimitive *primitive);

GLenum
agk_textureTarget(AkTypeId typeId);

#endif /* agk_enum_h */
