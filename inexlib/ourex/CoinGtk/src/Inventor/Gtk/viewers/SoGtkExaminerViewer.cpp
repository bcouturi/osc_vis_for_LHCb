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

// This file contains the generic, "templatize-able" parts of the
// So*ExaminerViewer sourcecode.


// *************************************************************************

#include <Inventor/Gtk/viewers/SoGtkExaminerViewer.h>

#include <assert.h>
#include <math.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <sogtkdefs.h>

#include <Inventor/Gtk/common/gl.h>

#include <Inventor/SbTime.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/projectors/SbSphereSheetProjector.h>
#include <Inventor/projectors/SbSpherePlaneProjector.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/events/SoMotion3Event.h>

#include <Inventor/Gtk/SoGtkBasic.h>


static const int MOUSEPOSLOGSIZE = 16;

// Bitmap representations of an "X", a "Y" and a "Z" for the axis cross.
static GLubyte xbmp[] = { 0x11,0x11,0x0a,0x04,0x0a,0x11,0x11 };
static GLubyte ybmp[] = { 0x04,0x04,0x04,0x04,0x0a,0x11,0x11 };
static GLubyte zbmp[] = { 0x1f,0x10,0x08,0x04,0x02,0x01,0x1f };

// ************************************************************************

/*!
*/

void
SoGtkExaminerViewer::genericConstructor(void)
{
  this->currentmode = SoGtkExaminerViewer::EXAMINE;

  this->prevRedrawTime = SbTime::getTimeOfDay();
  this->spinanimatingallowed = TRUE;
  this->spinanimating = FALSE;
  this->spinsamplecounter = 0;
  this->spinincrement = SbRotation::identity();

  // FIXME: use a smaller sphere than the default one to have a larger
  // area close to the borders that gives us "z-axis rotation"?
  // 19990425 mortene.
  this->spinprojector = new SbSphereSheetProjector;
  SbViewVolume volume;
  volume.ortho(-1, 1, -1, 1, -1, 1);
  this->spinprojector->setViewVolume(volume);

  this->axiscrossEnabled = FALSE;
  this->axiscrossSize = 25;

  this->spinRotation.setValue(SbVec3f(0, 0, -1), 0);

  this->log.size = MOUSEPOSLOGSIZE;
  this->log.position = new SbVec2s [ MOUSEPOSLOGSIZE ];
  this->log.time = new SbTime [ MOUSEPOSLOGSIZE ];
  this->log.historysize = 0;
  this->button1down = FALSE;
  this->button2down = FALSE;
  this->button3down = FALSE;
  this->controldown = FALSE;
  this->pointer.now = SbVec2s(0, 0);
  this->pointer.then = SbVec2s(0, 0);
  this->motion3OnCamera = TRUE;
} // SoGtkExaminerViewer()

/*!
*/

void
SoGtkExaminerViewer::genericDestructor(void)
{
  delete this->spinprojector;
  delete[] this->log.position;
  delete[] this->log.time;
} // ~SoGtkExaminerViewer()

// ************************************************************************

/*!
  The "rotX" wheel is the wheel on the left decoration on the examiner
  viewer.  This function translates interaction with the "rotX" wheel into
  camera movement.
*/

float
SoGtkExaminerViewer::rotXWheelMotion(
  float value,
  float oldvalue)
{
  SoCamera * camera = this->getCamera();
  if (camera == NULL) return 0.0f; // can happen for empty scenegraph

  SbVec3f dir;
  camera->orientation.getValue().multVec(SbVec3f(0, 0, -1), dir);

  SbVec3f focalpoint = camera->position.getValue() +
    camera->focalDistance.getValue() * dir;

  camera->orientation = SbRotation(SbVec3f(-1, 0, 0), value - oldvalue) *
    camera->orientation.getValue();

  camera->orientation.getValue().multVec(SbVec3f(0, 0, -1), dir);
  camera->position = focalpoint - camera->focalDistance.getValue() * dir;

  return value;
} // rotXWheelMotion()

/*!
  The "rotY" wheel is the wheel on the bottom decoration on the examiner
  viewer.  This function translates interaction with the "rotX" wheel into
  camera movement.
*/

float
SoGtkExaminerViewer::rotYWheelMotion(
  float value,
  float oldvalue)
{
  SoCamera * camera = this->getCamera();
  if (camera == NULL) return 0.0f; // can happen for empty scenegraph

  SbVec3f dir;
  camera->orientation.getValue().multVec(SbVec3f(0, 0, -1), dir);

  SbVec3f focalpoint = camera->position.getValue() +
    camera->focalDistance.getValue() * dir;

  camera->orientation = SbRotation(SbVec3f(0, 1, 0), oldvalue - value) *
    camera->orientation.getValue();

  camera->orientation.getValue().multVec(SbVec3f(0, 0, -1), dir);
  camera->position = focalpoint - camera->focalDistance.getValue() * dir;

  return value;
} // rotYWheelMotion()

// ************************************************************************

/*!
  Decide if it should be possible to start a spin animation of the model in
  the viewer by releasing the mouse button while dragging.

  If the \c on flag is \c FALSE and we're currently animating, the spin
  will be stopped.

  \sa isAnimationEnabled
*/

void
SoGtkExaminerViewer::setGenericAnimationEnabled(
  const SbBool enable)
{
  this->spinanimatingallowed = enable;
  if (! enable && this->isAnimating())
    this->stopAnimating();
} // setAnimationEnabled()

// *************************************************************************

/*!
  Query whether or not it is possible to start a spinning animation by
  releasing the left mouse button while dragging the mouse.

  \sa setAnimationEnabled
*/

SbBool
SoGtkExaminerViewer::isAnimationEnabled(
  void) const
{
  return this->spinanimatingallowed;
} // isAnimationEnabled()

// *************************************************************************

/*!
  Stop the model from spinning.
*/

void
SoGtkExaminerViewer::stopAnimating(
  void)
{
  if (this->spinanimating) {
    this->spinanimating = FALSE;
    this->interactiveCountDec();
  }
#if SOGTK_DEBUG
  else
    SoDebugError::postWarning("SoGtkExaminerViewer::stopAnimating",
      "not animating");
#endif // SOGTK_DEBUG
} // stopAnimating()

// *************************************************************************

/*!
  Query if the model in the viewer is currently in spinning mode after
  a user drag.
*/

SbBool
SoGtkExaminerViewer::isAnimating(
  void) const
{
  return this->spinanimating;
} // isAnimating()

// ************************************************************************

/*!
  \internal

  The viewer is a state machine, and all changes to the current state
  are made through this call.
*/

void
SoGtkExaminerViewer::setMode(const ViewerMode mode)
{
  this->setCursorRepresentation(mode);

  switch (mode) {
  case INTERACT:
    if (this->isAnimating()) this->stopAnimating();
    // FIXME: this looks like someone fixed the symptom instead of
    // fixing the cause? 20010709 mortene.
    while (this->getInteractiveCount())
      this->interactiveCountDec();
    break;

  case DRAGGING:
    // Set up initial projection point for the projector object when
    // first starting a drag operation.
    this->spinprojector->project(this->lastmouseposition);
    break;

  case PANNING:
    {
      // The plane we're projecting the mouse coordinates to get 3D
      // coordinates should stay the same during the whole pan
      // operation, so we should calculate this value here.
      SoCamera * cam = this->getCamera();
      if (cam == NULL) { // can happen for empty scenegraph
        this->panningplane = SbPlane(SbVec3f(0, 0, 1), 0);
      }
      else {
        SbViewVolume vv = cam->getViewVolume(this->getGLAspectRatio());
        this->panningplane = vv.getPlane(cam->focalDistance.getValue());
      }
    }
    break;

  default: // include default to avoid compiler warnings.
    break;
  }

  this->currentmode = mode;
} // setMode()

// ************************************************************************

/*!
  Set the flag deciding whether or not to show the axis cross.

  \sa isFeedbackVisible, getFeedbackSize, setFeedbackSize
*/

void
SoGtkExaminerViewer::setFeedbackVisibility(
  const SbBool enable)
{
  if (enable == this->axiscrossEnabled) {
#if SOGTK_DEBUG
    SoDebugError::postWarning("SoGtkExaminerViewer::setFeedbackVisibility",
      "feedback visibility already set to %s", enable ? "TRUE" : "FALSE");
#endif // SOGTK_DEBUG
    return;
  }
  this->axiscrossEnabled = enable;

  if (this->isViewing()) { this->scheduleRedraw(); }
} // setFeedbackVisibility()

/*!
  Check if the feedback axis cross is visible.

  \sa setFeedbackVisibility, getFeedbackSize, setFeedbackSize
*/

SbBool
SoGtkExaminerViewer::isFeedbackVisible(
  void) const
{
  return this->axiscrossEnabled;
} // isFeedbackVisibility()

// ************************************************************************

/*!
  Set the size of the feedback axiscross.  The value is interpreted as
  an approximate percentage chunk of the dimensions of the total canvas.

  \sa getFeedbackSize, isFeedbackVisible, setFeedbackVisibility
*/

void
SoGtkExaminerViewer::setGenericFeedbackSize(
  const int size)
{
#if SOGTK_DEBUG
  if (size < 1) {
    SoDebugError::postWarning("SoGtkExaminerViewer::setFeedbackSize",
      "the size setting should be larger than 0");
    return;
  }
#endif // SOGTK_DEBUG

  this->axiscrossSize = size;

  if (this->isFeedbackVisible() && this->isViewing())
    this->scheduleRedraw();
} // setFeedbackSize()

/*!
  Return the size of the feedback axis cross. Default is 25.

  \sa setFeedbackSize, isFeedbackVisible, setFeedbackVisibility
*/

int
SoGtkExaminerViewer::getFeedbackSize(
  void) const
{
  return this->axiscrossSize;
} // getFeedbackSize()

// ************************************************************************

/*!
*/

void
SoGtkExaminerViewer::drawAxisCross(
  void)
{
  // Store GL state.
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  GLfloat depthrange[2];
  glGetFloatv(GL_DEPTH_RANGE, depthrange);
  GLdouble projectionmatrix[16];
  glGetDoublev(GL_PROJECTION_MATRIX, projectionmatrix);

  glDepthFunc(GL_ALWAYS);
  glDepthMask(GL_TRUE);
  glDepthRange(0, 0);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
  glEnable(GL_COLOR_MATERIAL);
  glDisable(GL_BLEND); // Kills transparency.

  // Set the viewport in the OpenGL canvas. Dimensions are calculated
  // as a percentage of the total canvas size.
  SbVec2s view = this->getGLSize();
  const int pixelarea =
    int(float(this->axiscrossSize)/100.0f * SoGtkMin(view[0], view[1]));
#if 0 // middle of canvas
  SbVec2s origin(view[0]/2 - pixelarea/2, view[1]/2 - pixelarea/2);
#endif // middle of canvas
#if 1 // lower right of canvas
  SbVec2s origin(view[0] - pixelarea, 0);
#endif // lower right of canvas
  glViewport(origin[0], origin[1], pixelarea, pixelarea);



  // Set up the projection matrix.
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  const float NEARVAL = 0.1f;
  const float FARVAL = 10.0f;
  const float dim = NEARVAL * tan(M_PI/8.0f); // FOV is 45� (45/360 = 1/8)
  glFrustum(-dim, dim, -dim, dim, NEARVAL, FARVAL);


  // Set up the model matrix.
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  SbMatrix mx;
  SoCamera * cam = this->getCamera();

  // If there is no camera (like for an empty scene, for instance),
  // just use an identity rotation.
  if (cam) { mx = cam->orientation.getValue(); }
  else { mx = SbMatrix::identity(); }

  mx = mx.inverse();
  mx[3][2] = -3.5; // Translate away from the projection point (along z axis).
  glLoadMatrixf((float *)mx);


  // Find unit vector end points.
  SbMatrix px;
  glGetFloatv(GL_PROJECTION_MATRIX, (float *)px);
  SbMatrix comb = mx.multRight(px);

  SbVec3f xpos;
  comb.multVecMatrix(SbVec3f(1,0,0), xpos);
  xpos[0] = (1 + xpos[0]) * view[0]/2;
  xpos[1] = (1 + xpos[1]) * view[1]/2;
  SbVec3f ypos;
  comb.multVecMatrix(SbVec3f(0,1,0), ypos);
  ypos[0] = (1 + ypos[0]) * view[0]/2;
  ypos[1] = (1 + ypos[1]) * view[1]/2;
  SbVec3f zpos;
  comb.multVecMatrix(SbVec3f(0,0,1), zpos);
  zpos[0] = (1 + zpos[0]) * view[0]/2;
  zpos[1] = (1 + zpos[1]) * view[1]/2;


  // Render the cross.
  {
    glLineWidth(2.0);

    enum { XAXIS, YAXIS, ZAXIS };
    int idx[3] = { XAXIS, YAXIS, ZAXIS };
    float val[3] = { xpos[2], ypos[2], zpos[2] };

    // Bubble sort.. :-}
    if (val[0] < val[1]) { SoGtkSwap(val[0], val[1]); SoGtkSwap(idx[0], idx[1]); }
    if (val[1] < val[2]) { SoGtkSwap(val[1], val[2]); SoGtkSwap(idx[1], idx[2]); }
    if (val[0] < val[1]) { SoGtkSwap(val[0], val[1]); SoGtkSwap(idx[0], idx[1]); }
    assert((val[0] >= val[1]) && (val[1] >= val[2])); // Just checking..

    for (int i=0; i < 3; i++) {
      glPushMatrix();
      if (idx[i] == XAXIS) {                       // X axis.
        glColor3f(0.500f, 0.125f, 0.125f);
      } else if (idx[i] == YAXIS) {                // Y axis.
        glRotatef(90, 0, 0, 1);
        glColor3f(0.125f, 0.500f, 0.125f);
      } else {                                     // Z axis.
        glRotatef(-90, 0, 1, 0);
        glColor3f(0.125f, 0.125f, 0.500f);
      }
      this->drawArrow();
      glPopMatrix();
    }
  }

  // Render axis notation letters ("X", "Y", "Z").
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, view[0], 0, view[1], -1, 1);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  GLint unpack;
  glGetIntegerv(GL_UNPACK_ALIGNMENT, &unpack);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glColor3fv(SbVec3f(0.8f, 0.8f, 0.0f).getValue());

  glRasterPos2d(xpos[0], xpos[1]);
  glBitmap(8, 7, 0, 0, 0, 0, xbmp);
  glRasterPos2d(ypos[0], ypos[1]);
  glBitmap(8, 7, 0, 0, 0, 0, ybmp);
  glRasterPos2d(zpos[0], zpos[1]);
  glBitmap(8, 7, 0, 0, 0, 0, zbmp);

  glPixelStorei(GL_UNPACK_ALIGNMENT, unpack);
  glPopMatrix();

  // Reset original state.

  // FIXME: are these 3 lines really necessary, as we push
  // GL_ALL_ATTRIB_BITS at the start? 20000604 mortene.
  glDepthRange(depthrange[0], depthrange[1]);
  glMatrixMode(GL_PROJECTION);
  glLoadMatrixd(projectionmatrix);

  glPopAttrib();
} // drawAxisCross()

/*!
  \internal

  Draw an arrow for the axis representation directly through OpenGL.
*/

void
SoGtkExaminerViewer::drawArrow(// static
  void)
{
  glBegin(GL_LINES);
  glVertex3f(0, 0, 0);
  glVertex3f(1, 0, 0);
  glEnd();
  glDisable(GL_CULL_FACE);
  glBegin(GL_TRIANGLES);
  glVertex3f(1.0, 0, 0);
  glVertex3f(1.0-1.0/3, +0.5/4, 0);
  glVertex3f(1.0-1.0/3, -0.5/4, 0);
  glVertex3f(1.0, 0, 0);
  glVertex3f(1.0-1.0/3, 0, +0.5/4);
  glVertex3f(1.0-1.0/3, 0, -0.5/4);
  glEnd();
  glBegin(GL_QUADS);
  glVertex3f(1.0-1.0/3, +0.5/4, 0);
  glVertex3f(1.0-1.0/3, 0, +0.5/4);
  glVertex3f(1.0-1.0/3, -0.5/4, 0);
  glVertex3f(1.0-1.0/3, 0, -0.5/4);
  glEnd();
} // drawArrow()

// ************************************************************************

/*!
  \internal

  Rotate the camera by the given amount, then reposition it so we're still
  pointing at the same focal point.
*/

void
SoGtkExaminerViewer::reorientCamera(
  const SbRotation & rot)
{
  SoCamera * cam = this->getCamera();
  if (cam == NULL) return;

  // Find global coordinates of focal point.
  SbVec3f direction;
  cam->orientation.getValue().multVec(SbVec3f(0, 0, -1), direction);
  SbVec3f focalpoint = cam->position.getValue() +
    cam->focalDistance.getValue() * direction;

  // Set new orientation value by accumulating the new rotation.
  cam->orientation = rot * cam->orientation.getValue();

  // Reposition camera so we are still pointing at the same old focal point.
  cam->orientation.getValue().multVec(SbVec3f(0, 0, -1), direction);
  cam->position = focalpoint - cam->focalDistance.getValue() * direction;
} // reorientCamera()

// ************************************************************************

/*!
  \internal

  Uses the sphere sheet projector to map the mouseposition unto
  a 3D point and find a rotation from this and the last calculated point.
*/

void
SoGtkExaminerViewer::spin(
  const SbVec2f & pointerpos)
{
  if (this->log.historysize < 2) return;
  assert(this->spinprojector != NULL);

  SbVec2s glsize(this->getGLSize());
  SbVec2f lastpos;
  lastpos[0] = float(this->log.position[1][0]) / float(SoGtkMax((int)(glsize[0]-1), 1));
  lastpos[1] = float(this->log.position[1][1]) / float(SoGtkMax((int)(glsize[1]-1), 1));

  this->spinprojector->project(lastpos);
  SbRotation r;
  this->spinprojector->projectAndGetRotation(pointerpos, r);
  r.invert();
  this->reorientCamera(r);

  // Calculate an average angle magnitude value to make the transition
  // to a possible spin animation mode appear smooth.

  SbVec3f dummy_axis, newaxis;
  float acc_angle, newangle;
  this->spinincrement.getValue(dummy_axis, acc_angle);
  acc_angle *= this->spinsamplecounter; // weight
  r.getValue(newaxis, newangle);
  acc_angle += newangle;

  this->spinsamplecounter++;
  acc_angle /= this->spinsamplecounter;
  // FIXME: accumulate and average axis vectors aswell? 19990501 mortene.
  this->spinincrement.setValue(newaxis, acc_angle);

  // Don't carry too much baggage, as that'll give unwanted results
  // when the user quickly trigger (as in "click-drag-release") a spin
  // animation.
  if (this->spinsamplecounter > 3) this->spinsamplecounter = 3;
} // spin()

// ************************************************************************

/*!
  \internal

  Move scene parallel with the plane orthogonal to the camera
  direction vector.
*/

void
SoGtkExaminerViewer::pan(
  const SbVec2f & thispos,
  const SbVec2f & prevpos)
{
  SoCamera * cam = this->getCamera();
  if (cam == NULL) return; // can happen for empty scenegraph
  if (thispos == prevpos) {
#if SOGTK_DEBUG
    SoDebugError::postInfo("SoGtkExaminerViewer::pan", "useless invokation");
#endif // SOGTK_DEBUG
    return;
  }

#if SOGTK_DEBUG && 0
  SoDebugError::postInfo("SoGtkExaminerViewer::pan",
     "was(%.3g, %.3g) -> now(%.3g, %.3g)",
     prevpos[0], prevpos[1], thispos[0], thispos[1]);
#endif // SOGTK_DEBUG

  // Find projection points for the last and current mouse coordinates.
  SbViewVolume vv = cam->getViewVolume(this->getGLAspectRatio());
  SbLine line;
  vv.projectPointToLine(thispos, line);
  SbVec3f current_planept;
  this->panningplane.intersect(line, current_planept);
  vv.projectPointToLine(prevpos, line);
  SbVec3f old_planept;
  this->panningplane.intersect(line, old_planept);

  // Reposition camera according to the vector difference between the
  // projected points.
  cam->position = cam->position.getValue() - (current_planept - old_planept);
} // pan()

// ************************************************************************

/*!
  \internal

  Dependent on the camera type this will either shrink or expand
  the height of the viewport (orthogonal camera) or move the
  camera closer or further away from the focal point in the scene.
*/

void
SoGtkExaminerViewer::zoom(
  const float diffvalue)
{
  SoCamera * cam = this->getCamera();
  if (cam == NULL) return; // can happen for empty scenegraph
  SoType t = cam->getTypeId();

  // This will be in the range of <0, ->>.
  float multiplicator = exp(diffvalue);

  if (t.isDerivedFrom(SoOrthographicCamera::getClassTypeId())) {
    SoOrthographicCamera * oc = (SoOrthographicCamera *)cam;
    oc->height = oc->height.getValue() * multiplicator;
  } else if (t.isDerivedFrom(SoPerspectiveCamera::getClassTypeId())) {
    float oldfocaldist = cam->focalDistance.getValue();
    cam->focalDistance = oldfocaldist * multiplicator;

    SbVec3f direction;
    cam->orientation.getValue().multVec(SbVec3f(0, 0, -1), direction);
    cam->position = cam->position.getValue() +
      (cam->focalDistance.getValue() - oldfocaldist) * -direction;
  } else {
    assert(0 && "impossible");
  }
} // zoom()

// *************************************************************************

/*!
  \internal

  Calculate a zoom/dolly factor from the difference of the current
  cursor position and the last.
*/

void
SoGtkExaminerViewer::zoomByCursor(
  const SbVec2f & thispos,
  const SbVec2f & prevpos)
{
  // There is no "geometrically correct" value, 20 just seems to give
  // about the right "feel".
  this->zoom((thispos[1] - prevpos[1]) * 20.0f);
} // zoomByCursor()

// *************************************************************************

/*!
  This method is invoked so timing can be considered before the scene graph
  is redrawn.
*/

void
SoGtkExaminerViewer::actualGenericRedraw(
  void)
{
  SbTime thisRedrawTime = SbTime::getTimeOfDay();
  unsigned long msecs =
    thisRedrawTime.getMsecValue() - this->prevRedrawTime.getMsecValue();
  this->prevRedrawTime = thisRedrawTime;

  if (this->isAnimating()) {
    SbRotation deltaRotation = this->spinRotation;
    deltaRotation.scaleAngle((float) msecs / 200.0f);
    this->reorientCamera(deltaRotation);
  }
} // actualRedraw()

// *************************************************************************

#define MOUSEBUTTON_EVENT_TYPE  (SoMouseButtonEvent::getClassTypeId())
#define LOCATION2_EVENT_TYPE    (SoLocation2Event::getClassTypeId())
#define MOTION3_EVENT_TYPE      (SoMotion3Event::getClassTypeId())
#define KEYBOARD_EVENT_TYPE     (SoKeyboardEvent::getClassTypeId())

// Documented in superclass.
SbBool
SoGtkExaminerViewer::processSoEvent(const SoEvent * const ev)
{
#if SOGTK_DEBUG && 0 // debug
  SoDebugError::postInfo("SoGtkExaminerViewer::processSoEvent",
                          "[invoked], event '%s'",
                          ev->getTypeId().getName().getString());
#endif // debug

  // We're in "interact" mode (ie *not* the camera modification mode),
  // so don't handle the event here. It should either be forwarded to
  // the scenegraph, or caught by SoGtkViewer::processSoEvent() if
  // it's an ESC press (to switch modes).
  if (!this->isViewing()) { return inherited::processSoEvent(ev); }
    
  // Events during seeks are ignored, except those which influence the
  // seek mode itself -- these are handled further up the inheritance
  // hierarchy.
  if (this->isSeekMode()) { return inherited::processSoEvent(ev); }

  this->canvas = this->getGLSize();
  const SoType type(ev->getTypeId());
  const SbVec2f prevnormalized = this->lastmouseposition;
  const SbVec2s pos(ev->getPosition());
  const SbVec2f posn((float) pos[0] / (float) SoGtkMax((int)(canvas[0]-1), 1),
                     (float) pos[1] / (float) SoGtkMax((int)(canvas[1]-1), 1));

  this->lastmouseposition = posn;

  // Set to TRUE if any event processing happened. Note that it is not
  // necessary to restrict ourselves to only do one "action" for an
  // event, we only need this flag to see if any processing happened
  // at all.
  SbBool processed = FALSE;


  // Mismatches in state of the Ctrl key happens if the user presses
  // or releases it outside the viewer window.
  if (this->controldown != ev->wasCtrlDown()) {
    this->controldown = ev->wasCtrlDown();
    processed = TRUE;
  }

  // Mouse Button / Spaceball Button handling

  if (type.isDerivedFrom(MOUSEBUTTON_EVENT_TYPE)) {
    const SoMouseButtonEvent * const event = (const SoMouseButtonEvent *) ev;
    const int button = event->getButton();
    const SbBool press = event->getState() == SoButtonEvent::DOWN ? TRUE : FALSE;

    // SoDebugError::postInfo("processSoEvent", "button = %d", button);
    switch (button) {
    case SoMouseButtonEvent::BUTTON1:
      this->button1down = press;
      break;
    case SoMouseButtonEvent::BUTTON2:
      this->button2down = press;
      break;
    case SoMouseButtonEvent::BUTTON3:
      this->button3down = press;
      break;
    default:
      break;
    } // switch (button)

    if (press) {
      switch (button) {
      case SoMouseButtonEvent::BUTTON1:
        if (! this->isSeekMode()) {
          if (this->isAnimating()) this->stopAnimating();
          this->interactiveCountInc();
          this->clearLog();
          processed = TRUE;
        }
        break;

      case SoMouseButtonEvent::BUTTON3:
        {
          if (this->isAnimating()) this->stopAnimating();
          this->interactiveCountInc();
          SoCamera * const camera = this->getCamera();
          if (camera == NULL) { // can happen for empty scenegraph
            this->panningplane = SbPlane(SbVec3f(0, 0, 1), 0);
          }
          else {
            SbViewVolume volume = camera->getViewVolume(this->getGLAspectRatio());
            this->panningplane = volume.getPlane(camera->focalDistance.getValue());
          }
          processed = TRUE;
        }
        break;

#ifdef HAVE_SOMOUSEBUTTONEVENT_BUTTON5
      case SoMouseButtonEvent::BUTTON4:
        this->zoom(-0.1f);
        processed = TRUE;
        break;

      case SoMouseButtonEvent::BUTTON5:
        this->zoom(0.1f);
        processed = TRUE;
        break;
#endif // HAVE_SOMOUSEBUTTONEVENT_BUTTON5

      default:
        break;
      } // switch (button)

    } else { // ! press
      switch (button) {
      case SoMouseButtonEvent::BUTTON1:
        {
          this->spinanimating = FALSE;
          if (! this->isSeekMode()) {
            if (! this->isAnimationEnabled() || this->log.historysize < 3) {
              // FIXME: attention: here lies dragons! This will f*ck
              // up the interactioncounter if a mouse release "just
              // happens", as can be the case (dependent on the native
              // UI toolkit). At least Win32 can give us a release
              // event with no press event in advance. 20010709 mortene.
              this->interactiveCountDec();
            }
            else {
              const SbVec2s glsize(this->getGLSize());
              SbVec3f from = this->spinprojector->project(SbVec2f(float(this->log.position[2][0]) / float(SoGtkMax(glsize[0]-1, 1)),
                                                                   float(this->log.position[2][1]) / float(SoGtkMax(glsize[1]-1, 1))));
              SbVec3f to = this->spinprojector->project(posn);
              SbRotation rot = this->spinprojector->getRotation(from, to);

              SbTime stoptime = (event->getTime() - this->log.time[0]);
              if (stoptime.getMsecValue() > 100) {
                this->interactiveCountDec();
              }
              else {
                SbTime delta = (this->log.time[0] - this->log.time[2]);
                float deltatime = (float) delta.getMsecValue();
                if (deltatime == 1.0f) {
                  SoDebugError::postInfo("SoGtkExaminerViewer::processSoEvent", "time[0] = %ld, time[2] = %ld",
                                          this->log.time[0].getMsecValue(), this->log.time[2].getMsecValue());
                  this->interactiveCountDec();
                }
                else {
                  rot.invert();
                  rot.scaleAngle(200.0f / deltatime);

                  SbVec3f axis;
                  float radians;
                  rot.getValue(axis, radians);
                  if (radians < 0.01f || deltatime > 300.0f) {
                    this->interactiveCountDec();
                  }
                  else {
                    this->spinRotation = rot;
                    this->spinanimating = TRUE;
                    this->scheduleRedraw();
                  }
                }
              }
            }
          }
        }
        processed = TRUE;
        break;

      case SoMouseButtonEvent::BUTTON3:
        this->interactiveCountDec();
        processed = TRUE;
        break;

#ifdef HAVE_SOMOUSEBUTTONEVENT_BUTTON5
      case SoMouseButtonEvent::BUTTON4:
        processed = TRUE; // don't pass on
        break;

      case SoMouseButtonEvent::BUTTON5:
        processed = TRUE; // don't pass on
        break;
#endif // HAVE_SOMOUSEBUTTONEVENT_BUTTON5

      default:
        break;
      } // switch (button)
    }
  }

  // Mouse Movement handling
  if (type.isDerivedFrom(LOCATION2_EVENT_TYPE)) {
    const SoLocation2Event * const event = (const SoLocation2Event *) ev;

    if (this->button1down || this->button3down) processed = TRUE;

    if ((this->button1down && this->button3down) ||
         (this->button3down && this->controldown)) {
      this->zoomByCursor(posn, prevnormalized);
    }
    else if ((this->button1down && this->controldown) || this->button3down) {
      this->pan(posn, prevnormalized);
    }
    else if (this->button1down) {
      this->addToLog(event->getPosition(), event->getTime());
      this->spin(posn);
    }

  }

  // Keyboard handling
  if (type.isDerivedFrom(KEYBOARD_EVENT_TYPE)) {
    const SoKeyboardEvent * const event = (const SoKeyboardEvent *) ev;
    const SbBool press = event->getState() == SoButtonEvent::DOWN ? TRUE : FALSE;
    if (press) {
      if (event->getKey() == SoKeyboardEvent::LEFT_CONTROL
        || event->getKey() == SoKeyboardEvent::RIGHT_CONTROL) {
        this->controldown = TRUE;
        processed = TRUE;
      }
    } else {
      if (event->getKey() == SoKeyboardEvent::LEFT_CONTROL
        || event->getKey() == SoKeyboardEvent::RIGHT_CONTROL) {
        this->controldown = FALSE;
        processed = TRUE;
      }
    }
  }

  // Spaceball & Joystick handling
  if (type.isDerivedFrom(MOTION3_EVENT_TYPE)) {
    SoMotion3Event * const event = (SoMotion3Event *) ev;
    SoCamera * const camera = this->getCamera();
    if (camera) {
      if (this->motion3OnCamera) {
        camera->position =
          camera->position.getValue() + event->getTranslation();
        camera->orientation =
          camera->orientation.getValue() * event->getRotation();
        processed = TRUE;
      } else {
        // FIXME: move/rotate model
#if SOGTK_DEBUG
        SoDebugError::postInfo("SoGtkExaminerViewer::processSoEvent",
                               "SoMotion3Event for model movement is not implemented yet");
#endif // SOGTK_DEBUG
        processed = TRUE;
      }
    }
  }

  if (processed) {
    enum { STATE_LMB = 0x01, STATE_MMB = 0x02, STATE_CTRL = 0x04 };
    unsigned short stateflags =
      (this->button1down ? STATE_LMB : 0) +
      (this->button3down ? STATE_MMB : 0) +
      (this->controldown ? STATE_CTRL : 0);

    SoGtkExaminerViewer::ViewerMode mode;

    switch (stateflags) {
    case 0:
      mode = SoGtkExaminerViewer::EXAMINE;
      break;

    case STATE_LMB:
      mode = SoGtkExaminerViewer::DRAGGING;
      break;

    case STATE_MMB:
    case (STATE_LMB | STATE_CTRL):
      mode = SoGtkExaminerViewer::PANNING;
      break;

    case STATE_CTRL:
      mode = SoGtkExaminerViewer::WAITING_FOR_PAN;
      break;

    case (STATE_MMB | STATE_CTRL):
    case (STATE_LMB | STATE_MMB):
    case (STATE_LMB | STATE_MMB | STATE_CTRL):
      mode = SoGtkExaminerViewer::ZOOMING;
      break;

    default:
      assert(FALSE && "unhandled input state");
      break;
    }

    this->setMode(mode);
  }

  // If not handled in this class, pass on upwards in the inheritance
  // hierarchy.
  return processed || inherited::processSoEvent(ev);
}

// *************************************************************************
// Methods used for spin animation tracking.

/*!
  This method "clears" the mouse location log, used for spin animation
  calculations.
*/

void
SoGtkExaminerViewer::clearLog(
  void)
{
  this->log.historysize = 0;
} // clearLog()

/*!
  This method adds another point to the mouse location log, used for spin
  animation calculations.
*/

void
SoGtkExaminerViewer::addToLog(
  const SbVec2s pos,
  const SbTime time)
{
  // In case someone changes the const size setting at the top of this
  // file too small.
  assert (this->log.size > 2 && "mouse log too small!");

  if (this->log.historysize > 0 && pos == this->log.position[0]) {
#if SOGTK_DEBUG && 0 // debug
    // This can at least happen under SoQt.
    SoDebugError::postInfo("SoGtkExaminerViewer::addToLog", "got position already!");
#endif // debug
    return;
  }

  for (int i = this->log.size - 1; i > 0; i--) {
    this->log.position[i] = this->log.position[i-1];
    this->log.time[i] = this->log.time[i-1];
  }
  this->log.position[0] = pos;
  this->log.time[0] = time;
  if (this->log.historysize < this->log.size)
    this->log.historysize += 1;
} // addToLog()

// *************************************************************************

/*!
  This method sets whether Motion3 events should affect the camera or the
  model.
*/

void
SoGtkExaminerViewer::setMotion3OnCamera(
  SbBool enable)
{
  this->motion3OnCamera = enable;
} // setMotion3OnCamera()

/*!
  This method returns whether Motion3 events affects the camera or the
  model.
*/

SbBool
SoGtkExaminerViewer::getMotion3OnCamera(
  void) const
{
  return this->motion3OnCamera;
} // getMotion3OnCamera()

// *************************************************************************

/*!
  Decide whether or not the mouse pointer cursor should be visible in
  the rendering canvas.
*/
void
SoGtkExaminerViewer::setCursorEnabled(SbBool enable)
{
  inherited::setCursorEnabled(enable);
  this->setCursorRepresentation(this->currentmode);
}

// *************************************************************************
