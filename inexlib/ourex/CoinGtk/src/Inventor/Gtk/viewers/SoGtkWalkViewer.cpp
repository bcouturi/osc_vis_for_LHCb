/**************************************************************************
 *
 *  This file is part of the Coin GUI binding libraries.
 *  Copyright (C) 2000 by Systems in Motion.  All rights reserved.
 *
 *  The libraries this file is part of is free software; you can
 *  redistribute them and/or modify them under the terms of the GNU
 *  Lesser General Public License version 2.1 as published by the
 *  Free Software Foundation.  See the file LICENSE.LGPL at the root
 *  directory of the distribution for all the details.
 *
 *  If you want to use the Coin GUI binding libraries for applications
 *  not compatible with the LGPL, contact SIM about acquiring a
 *  Professional Edition License.
 *
 *  Systems in Motion, Prof Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
 **************************************************************************/

// @configure_input@

#if SOGTK_DEBUG
static const char rcsid[] =
  "";
#endif // SOGTK_DEBUG

// This file contains the generic, "templatize-able" parts of the
// So*WalkViewer sourcecode.


// *************************************************************************

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/Gtk/viewers/SoGtkWalkViewer.h>
#include <Inventor/SbLinear.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoCamera.h>
#include <assert.h>
#include <sogtkdefs.h>


// This method dollies the camera back and forth in the scene.
void
SoGtkWalkViewer::dollyCamera(const float delta) const
{
  SoCamera * const camera = this->getCamera();
  if (camera == NULL) { return; } // if there's no scenegraph, for instance

  SbPlane walkplane(this->getUpDirection(), camera->position.getValue());
  SbVec3f campos = camera->position.getValue();
  SbVec3f camvec;
  camera->orientation.getValue().multVec(SbVec3f(0, 0, -1), camvec);
  SbLine cross(campos + camvec,
                campos + camvec + this->getUpDirection());
  SbVec3f intersect;
  walkplane.intersect(cross, intersect);
  SbVec3f dir = intersect - campos;
  dir.normalize();

  camera->position = campos - dir * delta;
}

// This method elevates the camera up and down.
void
SoGtkWalkViewer::elevateCamera(const float delta) const
{
  SoCamera * const camera = this->getCamera();
  if (camera == NULL) { return; } // if there's no scenegraph, for instance

  SbVec3f up = this->getUpDirection();
  up.normalize();
  camera->position = camera->position.getValue() - up * delta;
}
