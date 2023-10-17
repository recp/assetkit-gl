/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/agk.h"
#include "common.h"

#include <im/im.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

// #define STBI_MALLOC(sz)           ak_malloc(NULL, sz)
// #define STBI_REALLOC(p,newsz)     ak_realloc(NULL, p, newsz)
// #define STBI_FREE(p)              ak_free(p)

#define STBIDEF static inline
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#pragma GCC diagnostic pop

void
agk_loadskinners(AgkContext * __restrict ctx);

AkImageData*
imageLoadFromFile(AkHeap     * __restrict heap,
                  AkImage    * __restrict image,
                  const char * __restrict path,
                  bool                    flipVertically);

AkImageData*
imageLoadFromMemory(AkHeap   * __restrict heap,
                    AkImage  * __restrict image,
                    AkBuffer * __restrict buff,
                    bool                  flipVertically);

AkResult
agk_loadScene(GkContext *ctx,
              AkDoc     *doc,
              AkScene   *scene,
              GLenum     usage,
              GkScene  **dest) {
  AkVisualScene *visualScene;
  AkNode        *node;
  GkScene       *glscene;
  GkNode       **glnodei, *glnode;
  AgkContext   *agkCtx;
  AkResult       ret;

  if (!scene->visualScene)
    return AK_ERR;

  ak_imageInitLoader(imageLoadFromFile, imageLoadFromMemory);

  glscene     = gkAllocScene(ctx);
  visualScene = ak_instanceObject(scene->visualScene);
  glnode      = gkAllocNode(glscene);

  gkMakeNodeTransform(glscene, glnode);

  glscene->rootNode = glnode;

  node    = visualScene->node;
  glnodei = &glscene->rootNode->chld;

  if (node) {
    agkCtx            = calloc(1, sizeof(*agkCtx));
    agkCtx->bufftree  = rb_newtree_ptr();
    agkCtx->objMap    = rb_newtree_ptr();
    agkCtx->ctlr      = rb_newtree_ptr();
    agkCtx->materials = rb_newtree_ptr();
    agkCtx->textures  = rb_newtree_ptr();
    agkCtx->texmap    = rb_newtree_str();
    agkCtx->doc       = doc;
    agkCtx->usage     = usage;
    agkCtx->scene     = glscene;
    agkCtx->ctx       = ctx;

    do {
      ret = agk_loadNode(agkCtx, node, glnodei);
      if (ret != AK_OK) {
        free(glscene);
        return AK_ERR;
      }

      (*glnodei)->parent = glnode;
      glnodei = &(*glnodei)->next;
      node    = node->next;
    } while (node);

    agkLoadAnimations(agkCtx);
    agk_loadskinners(agkCtx);

    free(agkCtx);
  }

  *dest = glscene;

  return AK_OK;
}

AkImageData*
imageLoadFromFile(AkHeap     * __restrict heap,
                  AkImage    * __restrict image,
                  const char * __restrict path,
                  bool                    flipVertically) {
  AkImageData *imdata;
  int          width, height, comp;

  if (flipVertically) {
    stbi_set_flip_vertically_on_load(flipVertically);
  }

  imdata         = ak_heap_calloc(heap, image, sizeof(*imdata));
  imdata->data   = stbi_load(path, &width, &height, &comp, 0);
  imdata->width  = width;
  imdata->height = height;
  imdata->comp   = comp;

  return imdata;
//
//  ImImage *image;
//
//  im_load(&image, path, NULL, IM_OPEN_INTENT_READONLY);
//
//  *width      = image->width;
//  *height     = image->height;
//  *components = image->componentsPerPixel;
//
//  return image->data.data;
}

AkImageData*
imageLoadFromMemory(AkHeap   * __restrict heap,
                    AkImage  * __restrict image,
                    AkBuffer * __restrict buff,
                    bool                  flipVertically) {
  AkImageData *imdata;
  int          width, height, comp;

  if (flipVertically) {
    stbi_set_flip_vertically_on_load(flipVertically);
  }

  imdata         = ak_heap_calloc(heap, image, sizeof(*imdata));
  imdata->data   = stbi_load_from_memory(buff->data, (int)buff->length, &width, &height, &comp, 0);
  imdata->width  = width;
  imdata->height = height;
  imdata->comp   = comp;

  return imdata;
}

void
imageFlipVerticallyOnLoad(bool flip) {
  stbi_set_flip_vertically_on_load(flip);
}

void
agk_loadskinners(AgkContext * __restrict ctx) {
  AkInstanceSkin   *skinner;
  AkSkin           *skin;
  GkSkin           *glskin;
  GkNode           *glnode;
  GkControllerInst *glCtlrInst;
  GkGeometryInst   *geomInst;
  AgkSkin2Load     *skin2load, *tofree;

  skin2load = ctx->skin2load;
  while (skin2load) {
    skinner    = skin2load->skinner;
    glnode     = skin2load->glnode;
    geomInst   = skin2load->geomInst;

    skin       = skinner->skin;
    glCtlrInst = calloc(1, sizeof(*glCtlrInst));

    if (!(glskin = rb_find(ctx->ctlr, skin)))
      glskin = akgLoadSkin(ctx, skin);

    glCtlrInst->ctlr = &glskin->base;

    /* per instance skin joints */
    if (skinner->overrideJoints) {
      glCtlrInst->joints = calloc(skin->nJoints, sizeof(*glCtlrInst->joints));
      akgSetJoints(ctx, skinner->overrideJoints, glCtlrInst->joints, skin->nJoints);
    }

    glskin->base.source = geomInst;
    gkMakeInstanceSkin(ctx->scene, glnode, glCtlrInst);
    gkAttachSkin(glskin);

    tofree = skin2load;
    skin2load = skin2load->next;
    free(tofree);
  }
  
  ctx->skin2load = NULL;
}
