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

/*!
  \class SoGtkConstrainedViewer Inventor/Gtk/viewers/SoGtkConstrainedViewer.h
  \brief The SoGtkConstrainedViewer class is the superclass for viewers with constrains on the viewpoint.
  \ingroup components viewers common
*/
// FIXME: document class properly. 20010905 mortene.

#ifdef SOQT_INTERNAL
#include <qmetaobject.h>
#include <moc_SoQtConstrainedViewer.cpp>
#endif // SOQT_INTERNAL

#include <Inventor/Gtk/viewers/SoGtkConstrainedViewer.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/nodes/SoCamera.h>
#include <sogtkdefs.h>

/*!
  \var SbVec3f SoGtkConstrainedViewer::upVector
  The camera's "up" vector is contrained to be in this direction.
*/
/*!
  \var float SoGtkConstrainedViewer::sceneHeight
  FIXME: doc
*/

// ************************************************************************

class SoGtkConstrainedViewerP {
public:
  SbVec3f upvechome;
};

#define PRIVATE(o) (o->pimpl)

// ************************************************************************

SOGTK_OBJECT_ABSTRACT_SOURCE(SoGtkConstrainedViewer);

// ************************************************************************

/*!
  Constructor.
*/
SoGtkConstrainedViewer::SoGtkConstrainedViewer(// protected
  GtkWidget * parent,
  const char * const name,
  SbBool embed,
  SoGtkFullViewer::BuildFlag flag,
  SoGtkViewer::Type type,
  SbBool build)
: inherited(parent, name, embed, flag, type, FALSE)
{
  PRIVATE(this) = new SoGtkConstrainedViewerP;

  this->setClassName("SoGtkConstrainedViewer");
  this->setLeftWheelString("Tilt");
  this->setBottomWheelString("Rotate");
  this->setRightWheelString("Dolly");

  this->sceneHeight = 0.0f;
  this->upVector = SbVec3f(0.0f, 1.0f, 0.0f);
  PRIVATE(this)->upvechome = this->upVector;

  if (build) {
    GtkWidget * viewer = this->buildWidget(this->getParentWidget());
    this->setBaseWidget(viewer);
  }
}

// ************************************************************************

/*!
  Virtual destructor.
*/
SoGtkConstrainedViewer::~SoGtkConstrainedViewer()
{
  delete PRIVATE(this);
}

// ************************************************************************

/*!
  Set up-direction vector for the camera viewpoint.
*/
void
SoGtkConstrainedViewer::setUpDirection(
  const SbVec3f & dir)
{
  SbRotation change(this->upVector, dir);
  this->upVector = dir;

  SoCamera * cam = this->getCamera();
  if (cam == NULL) { return; } // can happen for empty scene, for instance

  cam->orientation = change * cam->orientation.getValue();
  this->checkForCameraUpConstrain();
}

// ************************************************************************

/*!
  Return up-direction vector for the camera viewpoint.
*/
const SbVec3f &
SoGtkConstrainedViewer::getUpDirection(void) const
{
  return this->upVector;
}

// ************************************************************************

// doc in super
void
SoGtkConstrainedViewer::setCamera(// virtual
  SoCamera * newcam)
{
  inherited::setCamera(newcam);
  this->checkForCameraUpConstrain();
}

// ************************************************************************

// doc in super
void
SoGtkConstrainedViewer::saveHomePosition(// virtual
  void)
{
  inherited::saveHomePosition();
  PRIVATE(this)->upvechome = this->upVector;
}

// ************************************************************************

// doc in super
void
SoGtkConstrainedViewer::resetToHomePosition(// virtual
  void)
{
  inherited::resetToHomePosition();
  this->upVector = PRIVATE(this)->upvechome;
  // should be unnecessary to reset camera position here (camera
  // should have been in a valid state on the last saveHomePosition()
  // call)
}

// ************************************************************************

/*!
  FIXME: doc
*/
void
SoGtkConstrainedViewer::recomputeSceneSize(// virtual
  void)
{
  SOGTK_STUB();  // FIXME: re-calculate sceneHeight value. 20010907 mortene.
  inherited::recomputeSceneSize();
}

// ************************************************************************

/*!
  This method tilts the camera \a delta degrees closer to the up
  direction.
*/
void
SoGtkConstrainedViewer::tiltCamera(// virtual, protected
  float delta)
{
  SoCamera * const camera = this->getCamera();
  if (camera == NULL) { return; } // if there's no scenegraph, for instance

  SbVec3f camdir;
  camera->orientation.getValue().multVec(SbVec3f(0, 0, -1), camdir);
  camera->orientation = camera->orientation.getValue() *
    SbRotation(this->getUpDirection().cross(camdir), delta);
}

// ************************************************************************

// doc in super
void
SoGtkConstrainedViewer::bottomWheelMotion(// virtual, protected
  float value)
{
  this->rotateCamera(value - this->getBottomWheelValue());
  inherited::bottomWheelMotion(value);
}

// ************************************************************************

// doc in super
void
SoGtkConstrainedViewer::leftWheelMotion(// virtual, protected
  float value)
{
  this->tiltCamera(value - this->getLeftWheelValue());
  inherited::leftWheelMotion(value);
}

// ************************************************************************

// doc in super
void
SoGtkConstrainedViewer::changeCameraValues(// virtual, protected
  SoCamera * camera)
{
  inherited::changeCameraValues(camera);
  this->checkForCameraUpConstrain();
}

// ************************************************************************

/*!
  Executes a raypick at the given mousepointer location and sets the
  viewer's camera up-vector direction to the normal vector of the
  picked geometry.
*/
void
SoGtkConstrainedViewer::findUpDirection(// protected
  SbVec2s mouselocation)
{
  SoRayPickAction rp(this->getViewportRegion());
  rp.setPoint(mouselocation);

  // Use the full scenegraph, not just the user-set scenegraph (which
  // is what SoGtkConstrainedViewer::getSceneGraph() would return).
  SoNode * realroot = this->getSceneManager()->getSceneGraph();
  rp.apply(realroot);

  SoPickedPoint * point = rp.getPickedPoint();
  if (point == NULL) { return; } // missed all geometry

  SbVec3f normvec = point->getNormal();
  // FIXME: need to flip normal if we hit the backside of a
  // polygon. 20010925 mortene.
  this->setUpDirection(normvec);
}

// ************************************************************************

/*!
  If necessary, "tilts" the camera sideways so the up-direction is
  heeded.
*/
void
SoGtkConstrainedViewer::checkForCameraUpConstrain(// protected
  void)
{
  SoCamera * cam = this->getCamera();
  assert(cam);

  SbRotation camorient = cam->orientation.getValue();

  SbVec3f campointdir;
  camorient.multVec(SbVec3f(0, 0, -1), campointdir);
  SbVec3f camrightdir;
  camorient.multVec(SbVec3f(1, 0, 0), camrightdir);

  SbVec3f correctrightdir = campointdir.cross(this->upVector);
  if (correctrightdir.length() == 0.0f) { return; } // upvector == pointdir

  SbRotation r(camrightdir, correctrightdir);
  SbRotation newcamorient = camorient * r;
  cam->orientation = newcamorient;
}

// ************************************************************************

// private
void
SoGtkConstrainedViewer::rotateCamera(const float delta) const
{
  SoCamera * const camera = this->getCamera();
  if (camera == NULL) { return; } // if there's no scenegraph, for instance

  camera->orientation = camera->orientation.getValue() *
    SbRotation(this->getUpDirection(), -delta);
}

// ************************************************************************
