/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2004 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

// @configure_input@

// This file contains the generic, "templatize-able" parts of the
// So*WalkViewer sourcecode.

// *************************************************************************

/*!
  \class SoXtWalkViewer Inventor/Xt/viewers/SoXtWalkViewer.h
  \brief The SoXtWalkViewer class is a viewer for moving about in "walk"-like style.
  \ingroup components viewers

  This is a viewer suitable for architectural-type walk-throughs, for
  instance.

  FIXME: more class doc
*/

// *************************************************************************

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/Xt/viewers/SoXtWalkViewer.h>
#include <Inventor/Xt/viewers/SoGuiWalkViewerP.h>
#include <Inventor/SbLinear.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoCamera.h>
#include <assert.h>
#include <soxtdefs.h>

#define PRIVATE(ptr) ptr->pimpl
#define PUBLIC(ptr) ptr->pub

#ifndef DOXYGEN_SKIP_THIS

SoGuiWalkViewerP::SoGuiWalkViewerP(SoXtWalkViewer * publ)
{
  PUBLIC(this) = publ;
}

SoGuiWalkViewerP::~SoGuiWalkViewerP()
{
}

// This method dollies the camera back and forth in the scene.
void
SoGuiWalkViewerP::dollyCamera(const float delta) const
{
  SoCamera * const camera = PUBLIC(this)->getCamera();
  if (camera == NULL) { return; } // if there's no scenegraph, for instance

  SbPlane walkplane(PUBLIC(this)->getUpDirection(), camera->position.getValue());
  SbVec3f campos = camera->position.getValue();
  SbVec3f camvec;
  camera->orientation.getValue().multVec(SbVec3f(0, 0, -1), camvec);
  SbLine cross(campos + camvec,
                campos + camvec + PUBLIC(this)->getUpDirection());
  SbVec3f intersect;
  walkplane.intersect(cross, intersect);
  SbVec3f dir = intersect - campos;
  dir.normalize();

  camera->position = campos - dir * delta;
}

// This method elevates the camera up and down.
void
SoGuiWalkViewerP::elevateCamera(const float delta) const
{
  SoCamera * const camera = PUBLIC(this)->getCamera();
  if (camera == NULL) { return; } // if there's no scenegraph, for instance

  SbVec3f up = PUBLIC(this)->getUpDirection();
  up.normalize();
  camera->position = camera->position.getValue() - up * delta;
}

#endif // DOXYGEN_SKIP_THIS

// *************************************************************************

#undef PRIVATE
#undef PUBLIC

