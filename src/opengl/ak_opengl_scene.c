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
ak_glLoadScene(AkDoc    * __restrict doc,
               AkScene  * scene,
               GLenum     usage,
               GkScene ** dest) {
  AkVisualScene *visualScene;
  AkNode        *node;
  GkScene       *glscene;
  GkNode       **glnodei;
  AkResult       ret;

  if (!scene->visualScene)
    return AK_ERR;

  glscene     = calloc(sizeof(*glscene), 1);
  visualScene = ak_instanceObject(scene->visualScene);

  node    = visualScene->node;
  glnodei = &glscene->rootNode;

  glscene->usage = usage;
  while (node) {
    ret = ak_glLoadNode(doc,
                        node,
                        usage,
                        glscene,
                        glnodei);

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