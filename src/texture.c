/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/agk.h"
#include "enum.h"
#include "common.h"

GkTexture*
agkLoadTexture(AgkContext   * __restrict ctx,
               AkContext    * __restrict actx,
               AkTextureRef * __restrict texref) {
  AkSampler   *akSampler;
  GkSampler   *sampler;
  AkTexture   *akTexture;
  GkTexture   *tex;
  AkImage     *image;
  GkImage     *glimage;
  AkImageData *imgdata;
  GLenum       target;

  if (!texref)
    return NULL;
  
  if ((tex = rb_find(ctx->textures, texref->texture)))
    return tex;

  if (!texref
      || !(akTexture = texref->texture)
      || !(image     = akTexture->image)
      || !(akSampler = akTexture->sampler)
      || (target     = agk_textureTarget(akTexture->type)) == -1)
    goto err;

  ak_imageLoad(image);
  if (!(imgdata = image->data))
    goto err;

  sampler = calloc(1, sizeof(*sampler));
  tex     = calloc(1, sizeof(*tex));
  glimage = calloc(1, sizeof(*glimage));

  tex->target = target;

  if (imgdata->comp == 4) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    glimage->format     = GL_RGBA;
    tex->internalFormat = GL_RGBA8;
  } else {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glimage->format     = GL_RGB;
    tex->internalFormat = GL_RGB8;
  }

  glimage->mipLevels   = 1;
  glimage->mips.data   = imgdata->data;
  glimage->mips.width  = imgdata->width;
  glimage->mips.height = imgdata->height;
  glimage->type        = GL_UNSIGNED_BYTE;

  if (akSampler->borderColor) {
    GkColor *borderColor;
    borderColor = malloc(sizeof(*borderColor));
    glm_vec4_copy(akSampler->borderColor->vec,  borderColor->vec);

    sampler->borderColor = borderColor;
  }

  sampler->wrapS         = agk_wrapMode(akSampler->wrapS);
  sampler->wrapT         = agk_wrapMode(akSampler->wrapT);
  sampler->wrapP         = agk_wrapMode(akSampler->wrapP);

  sampler->minfilter     = agk_minFilter(akSampler->minfilter);
  sampler->magfilter     = agk_magFilter(akSampler->magfilter);
  sampler->mipfilter     = agk_mipFilter(akSampler->mipfilter);

  sampler->maxAnisotropy = akSampler->maxAnisotropy;
  sampler->mipMaxLevel   = akSampler->mipMaxLevel;
  sampler->mipMinLevel   = akSampler->mipMinLevel;
  sampler->mipBias       = akSampler->mipBias;

  tex->sampler = sampler;
  tex->image   = glimage;

  gkTexLoad(tex, false);

  /* TODO: check existing mips */
  glGenerateMipmap(target);
  
  glTexParameteri(tex->target, GL_TEXTURE_WRAP_S,     sampler->wrapS);
  glTexParameteri(tex->target, GL_TEXTURE_WRAP_T,     sampler->wrapT);
  glTexParameteri(tex->target, GL_TEXTURE_WRAP_R,     sampler->wrapP);
  glTexParameteri(tex->target, GL_TEXTURE_MIN_FILTER, sampler->minfilter);
  glTexParameteri(tex->target, GL_TEXTURE_MAG_FILTER, sampler->magfilter);
  
  /* TODO: mip filter */

  /* cleanup */
  /* ak_free(imgdata);
     image->data = NULL;
   */

ret:
  
  rb_insert(ctx->textures, texref->texture, tex);
  
  if (texref->texcoord)
    rb_insert(ctx->texmap, (void *)texref->texcoord, tex);

  return tex;

err:
  return NULL;
}

