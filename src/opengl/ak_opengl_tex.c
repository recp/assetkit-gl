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

  samplerCommon = newparam->val;
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

  glActiveTexture(GL_TEXTURE0); /* TODO: Texture Unit */
  gkTexLoad(tex, false);

  /* TODO: check existing mips */
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(tex->target, GL_TEXTURE_WRAP_S, sampler->wrapS);
  glTexParameteri(tex->target, GL_TEXTURE_WRAP_T, sampler->wrapT);
  glTexParameteri(tex->target, GL_TEXTURE_WRAP_R, sampler->wrapP);

  glTexParameteri(tex->target, GL_TEXTURE_MIN_FILTER, sampler->minfilter);
  glTexParameteri(tex->target, GL_TEXTURE_MAG_FILTER, sampler->magfilter);

  /* TODO: mip filter */

  /* cleanup */
  ak_free(imgdata);
  image->data = NULL;

ret:
  return tex;

err:
  return NULL;
}
