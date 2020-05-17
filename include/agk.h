/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef agk_h
#define agk_h
#ifdef __cplusplus
extern "C" {
#endif

#if defined(HAVE_WINDOWS_H) && defined(_WIN32)
# include <windows.h>
#endif

#ifdef __APPLE__
#  include <OpenGL/gl3.h>
#else
#  ifdef _WIN32
#    include <windows.h>
#  endif
#  include <GL/gl.h>
#endif

#include <ak/assetkit.h>
#include <gk/gk.h>
#include <gk/gpu_state.h>

typedef struct AgkContext {
  AkDoc     *doc;
  RBTree    *bufftree;
  RBTree    *objMap;
  RBTree    *ctlr;
  RBTree    *materials;
  RBTree    *textures;
  RBTree    *texmap;
  GkScene   *scene;
  GkContext *ctx;
  GLenum     usage;
} AgkContext;

AkResult
agk_loadScene(GkContext *ctx,
              AkDoc     *doc,
              AkScene   *scene,
              GLenum     usage,
              GkScene  **dest);

AkResult
agk_loadNode(AgkContext * __restrict ctx,
             AkNode     * __restrict node,
             GkNode    ** __restrict dest);
  
AkResult
agk_loadGeometry(AgkContext  * __restrict ctx,
                 AkGeometry  * __restrict geom,
                 GkModel    ** __restrict dest);

AkResult
agkLoadMesh(AgkContext * __restrict ctx,
             AkMesh     * __restrict mesh,
             GkModel   ** __restrict dest);

AkResult
agk_loadLight(AkDoc    * __restrict doc,
              GkNode   * __restrict node,
              AkLight  * __restrict light,
              GkLight ** __restrict dest);

GkTexture*
agkLoadTexture(AgkContext   * __restrict ctx,
               AkContext    * __restrict actx,
               AkTextureRef * __restrict texture);

AkResult
agkLoadBindMaterial(AgkContext     * __restrict ctx,
                    AkGeometry     * __restrict geom,
                    AkBindMaterial * __restrict bindMaterial,
                    GkModelInst    * __restrict modelInst);


AkResult
agkLoadPrimMaterial(AgkContext      * __restrict ctx,
                    AkMeshPrimitive * __restrict prim,
                    GkPrimitive     * __restrict glprim);

void
agkLoadAnimations(AgkContext * __restrict ctx);

GkMorph*
akgLoadMorph(AgkContext * __restrict ctx, AkMorph * __restrict morph) ;

GkSkin*
akgLoadSkin(AgkContext * __restrict ctx, AkSkin * __restrict skin);

void
akgSetJoints(AgkContext * __restrict ctx,
             AkNode    ** __restrict joints,
             GkNode    ** __restrict gljoints,
             size_t                  count);

#ifdef __cplusplus
}
#endif
#endif /* agk_h */
