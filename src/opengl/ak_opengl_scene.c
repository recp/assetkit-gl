/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../include/ak-opengl.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <assetkit.h>
#include <gk.h>

AkResult
ak_glLoadScene(AkDoc    *doc,
               AkScene  *scene,
               GLenum    usage,
               GkScene **dest) {
  AkVisualScene *visualScene;
  AkNode        *node;
  GkScene       *glscene;
  GkNode       **glnodei;
  AkGLContext   *ctx;
  AkResult       ret;

  if (!scene->visualScene)
    return AK_ERR;

  glscene     = calloc(sizeof(*glscene), 1);
  visualScene = ak_instanceObject(scene->visualScene);

  node    = visualScene->node;
  glnodei = &glscene->rootNode;

  ctx = calloc(sizeof(*ctx), 1);
  ctx->bufftree = rb_newtree_ptr();
  ctx->doc      = doc;
  ctx->usage    = usage;
  ctx->scene    = glscene;

  if (visualScene->bbox) {
    GkBBox *bbox;

    bbox = malloc(sizeof(*glscene->bbox));
    glm_vec_dup(visualScene->bbox->min, bbox->min);
    glm_vec_dup(visualScene->bbox->min, bbox->min);

    bbox->isvalid = visualScene->bbox->isvalid;
    ak_bbox_center(visualScene->bbox, bbox->center);
    bbox->radius = ak_bbox_radius(visualScene->bbox);

    glscene->bbox = bbox;
  }

  glscene->usage = usage;
  while (node) {
    ret = ak_glLoadNode(ctx, node, glnodei);
    if (ret != AK_OK) {
      free(glscene);
      return AK_ERR;
    }

    glnodei = &(*glnodei)->next;
    node    = node->next;
  }

  *dest = glscene;

  return AK_OK;
}