/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../include/ak-opengl.h"
#include "ak_enum.h"

GkSampler*
agk_loadSampler(AkFxSamplerCommon *samplerCommon){
  GkSampler *sampler;

  sampler = calloc(sizeof(*sampler), 1);

  return sampler;
}

GkTexture*
agk_loadTexture(AkNewParam *newparam) {
  AkFxSamplerCommon *samplerCommon;
  GkSampler         *sampler;
  GkTexture         *tex;
  AkImage           *image;
  GkImage           *glimage;
  AkImageData       *imgdata;
  GLenum             target;

  switch (newparam->valType) {
    case AK_VALUE_TYPE_SAMPLER1D:
      target = GL_TEXTURE_1D;
      break;
    case AK_VALUE_TYPE_SAMPLER2D:
      target = GL_TEXTURE_2D;
      break;
    case AK_VALUE_TYPE_SAMPLER3D:
      target = GL_TEXTURE_3D;
      break;
    case AK_VALUE_TYPE_SAMPLER_CUBE:
      target = GL_TEXTURE_CUBE_MAP;
      break;
    case AK_VALUE_TYPE_SAMPLER_RECT:
      target = GL_TEXTURE_RECTANGLE;
      break;
    case AK_VALUE_TYPE_SAMPLER_DEPTH:
      target = GL_TEXTURE_DEPTH;
      break;
    default:
      goto err;
  }

  samplerCommon = newparam->val;
  if (!samplerCommon->instanceImage)
    goto err;

  image = ak_instanceObject(samplerCommon->instanceImage);
  if (!image)
    goto err;

  ak_imageLoad(image);
  if (!(imgdata = image->data))
    goto err;

  sampler = calloc(sizeof(*sampler), 1);
  tex     = calloc(sizeof(*tex), 1);
  glimage = calloc(sizeof(*glimage), 1);

  glimage->target      = target;
  glimage->type        = GL_UNSIGNED_BYTE;
  glimage->mips.data   = NULL;

  glimage->mips.data   = imgdata->data;
  glimage->mips.width  = imgdata->width;
  glimage->mips.height = imgdata->height;

  if (imgdata->comp == 4) {
    glimage->format  = GL_RGBA;
    glimage->iformat = GL_RGBA;
  } else {
    glimage->format  = GL_RGB;
    glimage->iformat = GL_RGB;
  }

  if (samplerCommon->borderColor) {
    GkColor *borderColor;
    borderColor = malloc(sizeof(*borderColor));
    glm_vec4_copy(samplerCommon->borderColor->vec,
                  borderColor->vec);
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

  glGenTextures(1, &tex->texId);
  glBindTexture(glimage->target, tex->texId);

  glActiveTexture(GL_TEXTURE0); /* TODO: Texture Unit */

  gkImageLoad(glimage, image->data->data);

  /* TODO: check existing mips */
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(glimage->target, GL_TEXTURE_WRAP_S, sampler->wrapS);
  glTexParameteri(glimage->target, GL_TEXTURE_WRAP_T, sampler->wrapT);
  glTexParameteri(glimage->target, GL_TEXTURE_WRAP_T, sampler->wrapT);

  glTexParameteri(glimage->target, GL_TEXTURE_MIN_FILTER, sampler->minfilter);
  glTexParameteri(glimage->target, GL_TEXTURE_MAG_FILTER, sampler->magfilter);

  /* TODO: mip filter */

ret:
  return tex;

err:
  return NULL;
}
