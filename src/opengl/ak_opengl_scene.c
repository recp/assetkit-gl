/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../../include/ak-opengl.h"
#include <stdlib.h>
#include <assert.h>
#include <assetkit.h>
#include <gk.h>

AkResult
ak_glLoadScene(AkDoc  * __restrict doc,
               AkScene * scene,
               GLenum usage,
               GkScene ** dest) {
  AkVisualScene *visualScene;
  AkNode        *node;
  GkScene       *glscene;
  AkResult       ret;

  if (!scene->visualScene)
    return AK_ERR;

  glscene = calloc(sizeof(*glscene), 1);

  visualScene = ak_instanceObject(doc, &scene->visualScene->base);
  node = visualScene->node;

  ret = ak_glLoadNode(doc,
                      node,
                      usage,
                      GLM_MAT4_IDENTITY,
                      &glscene->rootNode);

  if (ret != AK_OK) {
    free(glscene);
    return AK_ERR;
  }

  *dest = glscene;

  return AK_OK;
}