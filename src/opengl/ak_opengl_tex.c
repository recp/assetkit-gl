/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../include/ak-opengl.h"
#include "ak_enum.h"
#include <string.h>

GkTexture*
agk_loadTexture(AkContext   * __restrict actx,
                AkFxTexture * __restrict texref) {
  AkNewParam        *newparam;
  AkFxSamplerCommon *samplerCommon;
  GkSampler         *sampler;
  GkTexture         *tex;
  AkImage           *image;
  GkImage           *glimage;
  AkImageData       *imgdata;
  GLenum             target;

  newparam  = ak_sid_resolve(actx, texref->texture);

  if (!newparam || !newparam->val)
    return NULL;

  switch (newparam->val->type.type) {
    case AK_VALUE_SAMPLER1D:
      target = GL_TEXTURE_1D;
      break;
    case AK_VALUE_SAMPLER2D:
      target = GL_TEXTURE_2D;
      break;
    case AK_VALUE_SAMPLER3D:
      target = GL_TEXTURE_3D;
      break;
    case AK_VALUE_SAMPLER_CUBE:
      target = GL_TEXTURE_CUBE_MAP;
      break;
    case AK_VALUE_SAMPLER_RECT:
      target = GL_TEXTURE_RECTANGLE;
      break;
    case AK_VALUE_SAMPLER_DEPTH:
      target = GL_TEXTURE_DEPTH;
      break;
    default:
      goto err;
  }

  samplerCommon = newparam->val->value;
  if (!samplerCommon->instanceImage)
    goto err;

  image = ak_instanceObject(samplerCommon->instanceImage);
  if (!image)
    goto err;

  ak_imageLoad(image);
  if (!(imgdata = image->data)) {
    goto err;
  }

  sampler = calloc(sizeof(*sampler), 1);
  tex     = calloc(sizeof(*tex), 1);
  glimage = calloc(sizeof(*glimage), 1);

  tex->target    = target;

  if (imgdata->comp == 4) {
    glimage->format     = GL_RGBA;
    tex->internalFormat = GL_RGBA8;
  } else {
    glimage->format     = GL_RGB;
    tex->internalFormat = GL_RGB8;
  }

  glimage->mipLevels   = 1;
  glimage->mips.data   = imgdata->data;
  glimage->mips.width  = imgdata->width;
  glimage->mips.height = imgdata->height;
  glimage->type        = GL_UNSIGNED_BYTE;

  if (samplerCommon->borderColor) {
    GkColor *borderColor;
    borderColor = malloc(sizeof(*borderColor));
    glm_vec4_copy(samplerCommon->borderColor->vec,
                  borderColor->vec);

    sampler->borderColor = borderColor;
  }

  sampler->wrapS         = agk_wrapMode(samplerCommon->wrapS);
  sampler->wrapT         = agk_wrapMode(samplerCommon->wrapT);
  sampler->wrapP         = agk_wrapMode(samplerCommon->wrapP);

  sampler->minfilter     = agk_minFilter(samplerCommon->minfilter);
  sampler->magfilter     = agk_magFilter(samplerCommon->magfilter);
  sampler->mipfilter     = agk_mipFilter(samplerCommon->mipfilter);

  sampler->maxAnisotropy = samplerCommon->maxAnisotropy;
  sampler->mipMaxLevel   = samplerCommon->mipMaxLevel;
  sampler->mipMinLevel   = samplerCommon->mipMinLevel;
  sampler->mipBias       = samplerCommon->mipBias;

  tex->sampler = sampler;
  tex->image   = glimage;

  glEnable(target);
  gkTexLoad(tex, false);

  /* TODO: check existing mips */
  glGenerateMipmap(target);

  glTexParameteri(tex->target, GL_TEXTURE_WRAP_S, sampler->wrapS);
  glTexParameteri(tex->target, GL_TEXTURE_WRAP_T, sampler->wrapT);
  glTexParameteri(tex->target, GL_TEXTURE_WRAP_R, sampler->wrapP);

  glTexParameteri(tex->target, GL_TEXTURE_MIN_FILTER, sampler->minfilter);
  glTexParameteri(tex->target, GL_TEXTURE_MAG_FILTER, sampler->magfilter);

  /* TODO: mip filter */

  /* cleanup */
  /* ak_free(imgdata);
     image->data = NULL;
   */

  if (texref->texcoord)
    ak_multimap_add(actx->bindVertexInputIndex,
                    &tex->sampler->unit,
                    (void *)texref->texcoord);
ret:
  return tex;

err:
  return NULL;
}
