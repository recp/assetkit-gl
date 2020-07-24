/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../include/agk.h"
#include "common.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

// #define STBI_MALLOC(sz)           ak_malloc(NULL, sz)
// #define STBI_REALLOC(p,newsz)     ak_realloc(NULL, p, newsz)
// #define STBI_FREE(p)              ak_free(p)

#define STBIDEF static inline
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#pragma GCC diagnostic pop

void*
imageLoadFromFile(const char * __restrict path,
                  int        * __restrict width,
                  int        * __restrict height,
                  int        * __restrict components);

void*
imageLoadFromMemory(const char * __restrict data,
                    size_t                  len,
                    int        * __restrict width,
                    int        * __restrict height,
                    int        * __restrict components);

void
imageFlipVerticallyOnLoad(bool flip);
  
  
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

  ak_imageInitLoader(imageLoadFromFile,
                     imageLoadFromMemory,
                     imageFlipVerticallyOnLoad);
  
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

    free(agkCtx);
  }

  *dest = glscene;

  return AK_OK;
}

void*
imageLoadFromFile(const char * __restrict path,
                  int        * __restrict width,
                  int        * __restrict height,
                  int        * __restrict components) {
  return stbi_load(path, width, height, components, 0);
}

void*
imageLoadFromMemory(const char * __restrict data,
                    size_t                  len,
                    int        * __restrict width,
                    int        * __restrict height,
                    int        * __restrict components) {
  return stbi_load_from_memory((stbi_uc const*)data, (int)len, width, height, components, 0);
}

void
imageFlipVerticallyOnLoad(bool flip) {
  stbi_set_flip_vertically_on_load(flip);
}
