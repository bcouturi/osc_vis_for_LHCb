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
// So*ExaminerViewer sourcecode.

// *************************************************************************

/*!
  \class SoXtExaminerViewer SoXtExaminerViewer.h Inventor/Xt/viewers/SoXtExaminerViewer.h
  \brief The SoXtExaminerViewer class is a 3D-model examination viewer.
  \ingroup components viewers

  This class is the viewer considered to be the most "general purpose"
  viewer, and it is often used in rapid prototyping to examine simple
  models aswell as complete scenes (although for the latter, you might
  be better off with one of the other viewer classes).

  <center>
  <img src="http://doc.coin3d.org/images/SoLibs/viewers/examinerviewer.png">
  </center>

  Here is a complete, stand-alone example that shows how to set up an
  SoXtExaminerViewer as a model viewer that loads Inventor and VRML
  files from disk and places them inside the viewer for the end-user
  to examine:

  \code
  #include <Inventor/Xt/SoXt.h>
  #include <Inventor/Xt/viewers/SoXtExaminerViewer.h>
  #include <Inventor/nodes/SoBaseColor.h>
  #include <Inventor/nodes/SoCone.h>
  #include <Inventor/nodes/SoSeparator.h>
  
  int
  main(int argc, char ** argv)
  {
    if (argc < 2) {
      (void)fprintf(stderr, "\n\n\tUsage: %s <modelfilename>\n\n",
                    argc > 0 ? argv[0] : "viewerapp");
      exit(1);
    }
  
    // Initialize SoXt and Inventor API libraries. This returns a main
    // window to use.
    Widget mainwin = SoXt::init(argc, argv, argv[0]);
  
    // Open the argument file..
    SoInput in;
    SbBool ok = in.openFile(argv[1]);
    if (!ok) { exit(1); }
  
    // ..and import it.
    SoSeparator * root = SoDB::readAll(&in);
    if (root == NULL) { exit(1); }
    root->ref();
  
    // Use the ExaminerViewer, for a nice interface for 3D model
    // inspection.
    SoXtExaminerViewer * viewer = new SoXtExaminerViewer(mainwin);
    viewer->setSceneGraph(root);
    viewer->show();
  
    // Pop up the main window.
    SoXt::show(mainwin);
    // Loop until exit.
    SoXt::mainLoop();
  
    // Clean up resources.
    delete viewer;
    root->unref();
  
    return 0;
  }
  \endcode

  SoXtExaminerViewer has a convenient interface for repositioning
  and reorientation of the camera, by panning, rotating and zooming
  it's position. The following controls can be used:

  <ul>

  <li>hold down left mousebutton and move mouse pointer to rotate the
  camera around it's current focal point (the focal point can be
  changed by doing a seek operation)</li>

  <li>hold middle mousebutton to pan (or a CTRL-key plus left
  mousebutton, or a SHIFT-key plus left mousebutton)</li>

  <li>hold down left + middle mousebutton to zoom / dolly, or CTRL +
  middle mousebutton, or CTRL + SHIFT + the left mousebutton</li>

  <li>click 's', then pick with the left mousebutton to seek</li>

  <li>right mousebutton opens the popup menu</li>

  <li>click 'ESC' key to switch to and from 'camera interaction' mode
  and 'scenegraph interaction' mode (see setViewing()
  documentation)</li>

  <li>hold down the 'ALT' key to temporary toggle from
  camera-interaction mode to scenegraph-interaction mode</li>

  <li>'q' quits the application</li>

  </ul>

  The SoXtExaminerViewer provides a user decoration's button for
  toggling between orthographic or perspective camera view volumes and
  projection methods. This is the bottom-most click button on the
  right decoration border.

  It also inherits the decoration buttons from the SoXtFullViewer:
  the arrow for switching to "scenegraph interaction" mode, the hand
  for setting back to "camera interaction" mode, the house for "reset
  camera to home position", the blueprint house for "set new camera
  home position", the eye for "zoom camera out to view full scene" and
  the flashlight for setting "click to seek" mode.

  Note that a common faulty assumption about all the viewer-classes is
  that user interaction (in the "examine"-mode, not the
  scenegraph-interaction mode) influences the model or 3D-scene in the
  view. This is not correct, as it is always the viewer's \e camera
  that is translated and rotated.

  The initial position of the camera is placed such that all of the
  scenegraph's geometry fits within it's view.

  \sa SoXtFlyViewer, SoXtPlaneViewer
*/

// *************************************************************************

// Documentation shared between So* toolkits follows below.

/*!
  \fn SoXtExaminerViewer::SoXtExaminerViewer(Widget parent, const char * name, SbBool embed, SoXtFullViewer::BuildFlag flag, SoXtViewer::Type type)

  Constructor.  See parent class for explanation of arguments.
  Calling this constructor will make sure the examiner viewer widget
  will be built immediately.
*/

/*!
  \fn SoXtExaminerViewer::SoXtExaminerViewer(Widget parent, const char * name, SbBool embed, SoXtFullViewer::BuildFlag flag, SoXtViewer::Type type, SbBool build)

  Constructor. See parent class for explanation of arguments.
*/

// *************************************************************************

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <assert.h>
#include <math.h>

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

#include <soxtdefs.h>

#include <Inventor/Xt/common/gl.h>
#include <Inventor/Xt/viewers/SoXtExaminerViewer.h>
#include <Inventor/Xt/viewers/SoXtExaminerViewerP.h>

#include <Inventor/Xt/SoXtBasic.h>
#include <Inventor/Xt/SoXtCursor.h>
#include <Inventor/Xt/viewers/SoGuiFullViewerP.h> // for pan() and zoom()

#define PRIVATE(obj) ((obj)->pimpl)
#define PUBLIC(obj) ((obj)->pub)

static const int MOUSEPOSLOGSIZE = 16;

// Bitmap representations of an "X", a "Y" and a "Z" for the axis cross.
static GLubyte xbmp[] = { 0x11,0x11,0x0a,0x04,0x0a,0x11,0x11 };
static GLubyte ybmp[] = { 0x04,0x04,0x04,0x04,0x0a,0x11,0x11 };
static GLubyte zbmp[] = { 0x1f,0x10,0x08,0x04,0x02,0x01,0x1f };

// ************************************************************************

/*!
  Decide if it should be possible to start a spin animation of the
  model in the viewer by releasing the mouse button while dragging.

  If the \a enable flag is \c FALSE and we're currently animating, the
  spin will be stopped.

  \sa isAnimationEnabled
*/
void
SoXtExaminerViewer::setAnimationEnabled(const SbBool enable)
{
  PRIVATE(this)->spinanimatingallowed = enable;
  if (!enable && this->isAnimating()) { this->stopAnimating(); }
}

// *************************************************************************

/*!
  Query whether or not it is possible to start a spinning animation by
  releasing the left mouse button while dragging the mouse.

  \sa setAnimationEnabled
*/

SbBool
SoXtExaminerViewer::isAnimationEnabled(void) const
{
  return PRIVATE(this)->spinanimatingallowed;
}

// *************************************************************************

/*!
  Stop the model from spinning.
*/

void
SoXtExaminerViewer::stopAnimating(void)
{
  if (PRIVATE(this)->currentmode != SoGuiExaminerViewerP::SPINNING) {
#if SOXT_DEBUG
    SoDebugError::postWarning("SoXtExaminerViewer::stopAnimating",
                              "not animating");
#endif // SOXT_DEBUG
    return;
  }
  PRIVATE(this)->setMode(this->isViewing() ?
                         SoGuiExaminerViewerP::IDLE :
                         SoGuiExaminerViewerP::INTERACT);
}

// *************************************************************************

/*!
  Query if the model in the viewer is currently in spinning mode after
  a user drag.
*/

SbBool
SoXtExaminerViewer::isAnimating(void) const
{
  return PRIVATE(this)->currentmode == SoGuiExaminerViewerP::SPINNING;
}

// ************************************************************************

/*!
  Set the flag deciding whether or not to show the axis cross.

  \sa isFeedbackVisible, getFeedbackSize, setFeedbackSize
*/

void
SoXtExaminerViewer::setFeedbackVisibility(const SbBool enable)
{
  if (enable == PRIVATE(this)->axiscrossEnabled) {
#ifdef SOXT_EXTRA_DEBUG
    SoDebugError::postWarning("SoXtExaminerViewer::setFeedbackVisibility",
                              "feedback visibility already set to %s", enable ? "TRUE" : "FALSE");
#endif // SOXT_EXTRA_DEBUG
    return;
  }
  PRIVATE(this)->axiscrossEnabled = enable;

  if (this->isViewing()) { this->scheduleRedraw(); }
}

/*!
  Check if the feedback axis cross is visible.

  \sa setFeedbackVisibility, getFeedbackSize, setFeedbackSize
*/

SbBool
SoXtExaminerViewer::isFeedbackVisible(void) const
{
  return PRIVATE(this)->axiscrossEnabled;
}

// ************************************************************************

/*!
  Set the size of the feedback axiscross.  The value is interpreted as
  an approximate percentage chunk of the dimensions of the total
  canvas.

  \sa getFeedbackSize, isFeedbackVisible, setFeedbackVisibility
*/
void
SoXtExaminerViewer::setFeedbackSize(const int size)
{
#if SOXT_DEBUG
  if (size < 1) {
    SoDebugError::postWarning("SoXtExaminerViewer::setFeedbackSize",
                              "the size setting should be larger than 0");
    return;
  }
#endif // SOXT_DEBUG

  PRIVATE(this)->axiscrossSize = size;

  if (this->isFeedbackVisible() && this->isViewing()) {
    this->scheduleRedraw();
  }
}

/*!
  Return the size of the feedback axis cross. Default is 25.

  \sa setFeedbackSize, isFeedbackVisible, setFeedbackVisibility
*/

int
SoXtExaminerViewer::getFeedbackSize(void) const
{
  return PRIVATE(this)->axiscrossSize;
}

// *************************************************************************

// Documented in superclass.
SbBool
SoXtExaminerViewer::processSoEvent(const SoEvent * const ev)
{
#if SOXT_DEBUG && 0 // debug
  SoDebugError::postInfo("SoXtExaminerViewer::processSoEvent",
                          "[invoked], event '%s'",
                          ev->getTypeId().getName().getString());
#endif // debug

  // Let the end-user toggle between camera-interaction mode
  // ("viewing") and scenegraph-interaction mode with ALT key(s).
  if (ev->getTypeId().isDerivedFrom(SoKeyboardEvent::getClassTypeId())) {
    SoKeyboardEvent * ke = (SoKeyboardEvent *)ev;
    switch (ke->getKey()) {
    case SoKeyboardEvent::LEFT_ALT:
    case SoKeyboardEvent::RIGHT_ALT:
      if (this->isViewing() && (ke->getState() == SoButtonEvent::DOWN)) {
        this->setViewing(FALSE);
        return TRUE;
      }
      else if (!this->isViewing() && (ke->getState() == SoButtonEvent::UP)) {
        this->setViewing(TRUE);
        return TRUE;
      }
    default:
      break;
    }
  }

  // We're in "interact" mode (ie *not* the camera modification mode),
  // so don't handle the event here. It should either be forwarded to
  // the scenegraph, or caught by SoXtViewer::processSoEvent() if
  // it's an ESC press (to switch modes).
  if (!this->isViewing()) { return inherited::processSoEvent(ev); }
    
  // Events when in "ready-to-seek" mode are ignored, except those
  // which influence the seek mode itself -- these are handled further
  // up the inheritance hierarchy.
  if (this->isSeekMode()) { return inherited::processSoEvent(ev); }

  const SoType type(ev->getTypeId());

  const SbVec2s size(this->getGLSize());
  const SbVec2f prevnormalized = PRIVATE(this)->lastmouseposition;
  const SbVec2s pos(ev->getPosition());
  const SbVec2f posn((float) pos[0] / (float) SoXtMax((int)(size[0] - 1), 1),
                     (float) pos[1] / (float) SoXtMax((int)(size[1] - 1), 1));

  PRIVATE(this)->lastmouseposition = posn;

  // Set to TRUE if any event processing happened. Note that it is not
  // necessary to restrict ourselves to only do one "action" for an
  // event, we only need this flag to see if any processing happened
  // at all.
  SbBool processed = FALSE;

  const SoGuiExaminerViewerP::ViewerMode currentmode = PRIVATE(this)->currentmode;
  SoGuiExaminerViewerP::ViewerMode newmode = currentmode;

  // Mismatches in state of the modifier keys happens if the user
  // presses or releases them outside the viewer window.
  if (PRIVATE(this)->ctrldown != ev->wasCtrlDown()) {
    PRIVATE(this)->ctrldown = ev->wasCtrlDown();
  }
  if (PRIVATE(this)->shiftdown != ev->wasShiftDown()) {
    PRIVATE(this)->shiftdown = ev->wasShiftDown();
  }

  // Mouse Button / Spaceball Button handling

  if (type.isDerivedFrom(SoMouseButtonEvent::getClassTypeId())) {
    processed = TRUE;

    const SoMouseButtonEvent * const event = (const SoMouseButtonEvent *) ev;
    const int button = event->getButton();
    const SbBool press = event->getState() == SoButtonEvent::DOWN ? TRUE : FALSE;

    // SoDebugError::postInfo("processSoEvent", "button = %d", button);
    switch (button) {
    case SoMouseButtonEvent::BUTTON1:
      PRIVATE(this)->button1down = press;
      if (press && (currentmode == SoGuiExaminerViewerP::SEEK_WAIT_MODE)) {
        newmode = SoGuiExaminerViewerP::SEEK_MODE;
        this->seekToPoint(pos); // implicitly calls interactiveCountInc()
      }
      break;
    case SoMouseButtonEvent::BUTTON2:
      processed = FALSE; // pass on to superclass, so popup menu is shown
      break;
    case SoMouseButtonEvent::BUTTON3:
      PRIVATE(this)->button3down = press;
      break;
#ifdef HAVE_SOMOUSEBUTTONEVENT_BUTTON5
    case SoMouseButtonEvent::BUTTON4:
      if (press) SoGuiFullViewerP::zoom(this->getCamera(), 0.1f);
      break;
    case SoMouseButtonEvent::BUTTON5:
      if (press) SoGuiFullViewerP::zoom(this->getCamera(), -0.1f);
      break;
#endif // HAVE_SOMOUSEBUTTONEVENT_BUTTON5
    default:
      break;
    }
  }

  // Keyboard handling
  if (type.isDerivedFrom(SoKeyboardEvent::getClassTypeId())) {
    const SoKeyboardEvent * const event = (const SoKeyboardEvent *) ev;
    const SbBool press = event->getState() == SoButtonEvent::DOWN ? TRUE : FALSE;
    switch (event->getKey()) {
    case SoKeyboardEvent::LEFT_CONTROL:
    case SoKeyboardEvent::RIGHT_CONTROL:
      processed = TRUE;
      PRIVATE(this)->ctrldown = press;
      break;
    case SoKeyboardEvent::LEFT_SHIFT:
    case SoKeyboardEvent::RIGHT_SHIFT:
      processed = TRUE;
      PRIVATE(this)->shiftdown = press;
      break;
    default:
      break;
    }
  }

  // Mouse Movement handling
  if (type.isDerivedFrom(SoLocation2Event::getClassTypeId())) {
    const SoLocation2Event * const event = (const SoLocation2Event *) ev;

    processed = TRUE;

    if (PRIVATE(this)->currentmode == SoGuiExaminerViewerP::ZOOMING) {
      PRIVATE(this)->zoomByCursor(posn, prevnormalized);
    }
    else if (PRIVATE(this)->currentmode == SoGuiExaminerViewerP::PANNING) {
      SoGuiFullViewerP::pan(this->getCamera(), this->getGLAspectRatio(),
                            PRIVATE(this)->panningplane, posn, prevnormalized);
    }
    else if (PRIVATE(this)->currentmode == SoGuiExaminerViewerP::DRAGGING) {
      PRIVATE(this)->addToLog(event->getPosition(), event->getTime());
      PRIVATE(this)->spin(posn);
    }
    else {
      processed = FALSE;
    }
  }

  // Spaceball & Joystick handling
  if (type.isDerivedFrom(SoMotion3Event::getClassTypeId())) {
    SoMotion3Event * const event = (SoMotion3Event *) ev;
    SoCamera * const camera = this->getCamera();
    if (camera) {
      if (PRIVATE(this)->motion3OnCamera) {
        SbVec3f dir = event->getTranslation();
        camera->orientation.getValue().multVec(dir,dir);
        camera->position = camera->position.getValue() + dir;
        camera->orientation = 
          event->getRotation() * camera->orientation.getValue();
        processed = TRUE;
      }
      else {
        // FIXME: move/rotate model
#if SOXT_DEBUG
        SoDebugError::postInfo("SoXtExaminerViewer::processSoEvent",
                               "SoMotion3Event for model movement is not implemented yet");
#endif // SOXT_DEBUG
        processed = TRUE;
      }
    }
  }

  enum {
    BUTTON1DOWN = 1 << 0,
    BUTTON3DOWN = 1 << 1,
    CTRLDOWN =    1 << 2,
    SHIFTDOWN =   1 << 3
  };
  unsigned int combo =
    (PRIVATE(this)->button1down ? BUTTON1DOWN : 0) |
    (PRIVATE(this)->button3down ? BUTTON3DOWN : 0) |
    (PRIVATE(this)->ctrldown ? CTRLDOWN : 0) |
    (PRIVATE(this)->shiftdown ? SHIFTDOWN : 0);

  switch (combo) {
  case 0:
    if (currentmode == SoGuiExaminerViewerP::SPINNING) { break; }
    newmode = SoGuiExaminerViewerP::IDLE;
    if ((currentmode == SoGuiExaminerViewerP::DRAGGING) &&
        this->isAnimationEnabled() && (PRIVATE(this)->log.historysize >= 3)) {
      SbTime stoptime = (ev->getTime() - PRIVATE(this)->log.time[0]);
      if (stoptime.getValue() < 0.100) {
        const SbVec2s glsize(this->getGLSize());
        SbVec3f from = PRIVATE(this)->spinprojector->project(SbVec2f(float(PRIVATE(this)->log.position[2][0]) / float(SoXtMax(glsize[0]-1, 1)),
                                                                     float(PRIVATE(this)->log.position[2][1]) / float(SoXtMax(glsize[1]-1, 1))));
        SbVec3f to = PRIVATE(this)->spinprojector->project(posn);
        SbRotation rot = PRIVATE(this)->spinprojector->getRotation(from, to);

        SbTime delta = (PRIVATE(this)->log.time[0] - PRIVATE(this)->log.time[2]);
        double deltatime = delta.getValue();
        rot.invert();
        rot.scaleAngle(float(0.200 / deltatime));

        SbVec3f axis;
        float radians;
        rot.getValue(axis, radians);
        if ((radians > 0.01f) && (deltatime < 0.300)) {
          newmode = SoGuiExaminerViewerP::SPINNING;
          PRIVATE(this)->spinRotation = rot;
        }
      }
    }
    break;
  case BUTTON1DOWN:
    newmode = SoGuiExaminerViewerP::DRAGGING;
    break;
  case BUTTON3DOWN:
  case CTRLDOWN|BUTTON1DOWN:
  case SHIFTDOWN|BUTTON1DOWN:
    newmode = SoGuiExaminerViewerP::PANNING;
    break;
  case BUTTON1DOWN|BUTTON3DOWN:
  case CTRLDOWN|BUTTON3DOWN:
  case CTRLDOWN|SHIFTDOWN|BUTTON1DOWN:
    newmode = SoGuiExaminerViewerP::ZOOMING;
    break;

    // There are many cases we don't handle that just falls through to
    // the default case, like SHIFTDOWN, CTRLDOWN, CTRLDOWN|SHIFTDOWN,
    // SHIFTDOWN|BUTTON3DOWN, SHIFTDOWN|CTRLDOWN|BUTTON3DOWN, etc.
    // This is a feature, not a bug. :-)
    //
    // mortene.

  default:
    // The default will make a spin stop and otherwise not do
    // anything.
    if ((currentmode != SoGuiExaminerViewerP::SEEK_WAIT_MODE) &&
        (currentmode != SoGuiExaminerViewerP::SEEK_MODE)) {
      newmode = SoGuiExaminerViewerP::IDLE;
    }
    break;
  }

  if (newmode != currentmode) {
    PRIVATE(this)->setMode(newmode);
  }

  // If not handled in this class, pass on upwards in the inheritance
  // hierarchy.
  return processed || inherited::processSoEvent(ev);
}

// *************************************************************************

// documented in superclass
void
SoXtExaminerViewer::setSeekMode(SbBool on)
{
  // Overrides this method to make sure any animations are stopped
  // before we go into seek mode.

  // Note: this method is almost identical to the setSeekMode() in the
  // SoXtFlyViewer and SoXtPlaneViewer, so migrate any changes.

#if SOXT_DEBUG
  if (on == this->isSeekMode()) {
    SoDebugError::postWarning("SoXtExaminerViewer::setSeekMode",
                              "seek mode already %sset", on ? "" : "un");
    return;
  }
#endif // SOXT_DEBUG

  if (this->isAnimating()) { this->stopAnimating(); }
  inherited::setSeekMode(on);
  PRIVATE(this)->setMode(on ?
                         SoGuiExaminerViewerP::SEEK_WAIT_MODE :
                         (this->isViewing() ?
                          SoGuiExaminerViewerP::IDLE : SoGuiExaminerViewerP::INTERACT));
}

// *************************************************************************

/*!
  Decide whether or not the mouse pointer cursor should be visible in
  the rendering canvas.
*/
void
SoXtExaminerViewer::setCursorEnabled(SbBool enable)
{
  inherited::setCursorEnabled(enable);
  PRIVATE(this)->setCursorRepresentation(PRIVATE(this)->currentmode);
}

// *************************************************************************

// Documented in superclass.
const char *
SoXtExaminerViewer::getDefaultWidgetName(void) const
{
  return "SoXtExaminerViewer";
}

// *************************************************************************

// Documented in superclass.
const char *
SoXtExaminerViewer::getDefaultTitle(void) const
{
  return "Examiner Viewer";
}

// *************************************************************************

// Documented in superclass.
const char *
SoXtExaminerViewer::getDefaultIconTitle(void) const
{
  return "Examiner Viewer";
}

// *************************************************************************

// Documented in superclass. Overrides this method to be able to draw
// the axis cross, if selected, and to keep a continuous animation
// upon spin.
void
SoXtExaminerViewer::actualRedraw(void)
{
  SbTime now = SbTime::getTimeOfDay();
  double secs = now.getValue() -  PRIVATE(this)->prevRedrawTime.getValue();

  PRIVATE(this)->prevRedrawTime = now;

  if (this->isAnimating()) {
    SbRotation deltaRotation = PRIVATE(this)->spinRotation;
    deltaRotation.scaleAngle(float(secs * 5.0));
    PRIVATE(this)->reorientCamera(deltaRotation);
  }

  inherited::actualRedraw();

  if (this->isFeedbackVisible()) { PRIVATE(this)->drawAxisCross(); }

  // Immediately reschedule to get continous spin animation.
  if (this->isAnimating()) { this->scheduleRedraw(); }
}

// *************************************************************************

// doc in super
void
SoXtExaminerViewer::afterRealizeHook(void)
{
  inherited::afterRealizeHook();
  PRIVATE(this)->setCursorRepresentation(PRIVATE(this)->currentmode);
}

// *************************************************************************

// Documented in superclass. Overridden to provide the examiner viewer
// functionality on the left thumbwheel (x axis rotation).
void
SoXtExaminerViewer::leftWheelMotion(float value)
{
  if (this->isAnimating()) this->stopAnimating();

  float newval = PRIVATE(this)->rotXWheelMotion(value, this->getLeftWheelValue());
  inherited::leftWheelMotion(newval);
}

// Documented in superclass. Overridden to provide the examiner viewer
// functionality on the bottom thumbwheel (y axis rotation).
void
SoXtExaminerViewer::bottomWheelMotion(float value)
{
  if (this->isAnimating()) this->stopAnimating();

  float newval = PRIVATE(this)->rotYWheelMotion(value, this->getBottomWheelValue());
  inherited::bottomWheelMotion(newval);
}

// Documented in superclass. Overridden to provide the examiner viewer
// functionality on the left thumbwheel (dolly/zoom).
void
SoXtExaminerViewer::rightWheelMotion(float value)
{
  SoGuiFullViewerP::zoom(this->getCamera(), this->getRightWheelValue() - value);
  inherited::rightWheelMotion(value);
}

// *************************************************************************

// Documented in superclass. This method overridden from parent class
// to make sure the mouse pointer cursor is updated.
void
SoXtExaminerViewer::setViewing(SbBool enable)
{
  if (!!this->isViewing() == !!enable) {
#if SOXT_DEBUG
    SoDebugError::postWarning("SoXtExaminerViewer::setViewing",
                              "current state already %s", enable ? "TRUE" : "FALSE");
#endif // SOXT_DEBUG
    return;
  }

  PRIVATE(this)->setMode(enable ?
                         SoGuiExaminerViewerP::IDLE :
                         SoGuiExaminerViewerP::INTERACT);
  inherited::setViewing(enable);
}

// *************************************************************************

#ifndef DOXYGEN_SKIP_THIS

// Remaining code is for the SoGuiExaminerViewerP "private
// implementation" class.

SoGuiExaminerViewerP::SoGuiExaminerViewerP(SoXtExaminerViewer * publ)
{
  PUBLIC(this) = publ;
}

SoGuiExaminerViewerP::~SoGuiExaminerViewerP()
{
}

void
SoGuiExaminerViewerP::genericConstructor(void)
{
  this->currentmode = SoGuiExaminerViewerP::IDLE;

  this->prevRedrawTime = SbTime::getTimeOfDay();
  this->spinanimatingallowed = TRUE;
  this->spinsamplecounter = 0;
  this->spinincrement = SbRotation::identity();

  // FIXME: use a smaller sphere than the default one to have a larger
  // area close to the borders that gives us "z-axis rotation"?
  // 19990425 mortene.
  this->spinprojector = new SbSphereSheetProjector(SbSphere(SbVec3f(0, 0, 0), 0.8f));
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
  this->button3down = FALSE;
  this->ctrldown = FALSE;
  this->shiftdown = FALSE;
  this->pointer.now = SbVec2s(0, 0);
  this->pointer.then = SbVec2s(0, 0);
  this->motion3OnCamera = TRUE;
}

void
SoGuiExaminerViewerP::genericDestructor(void)
{
  delete this->spinprojector;
  delete[] this->log.position;
  delete[] this->log.time;
}

// ************************************************************************

// The "rotX" wheel is the wheel on the left decoration on the
// examiner viewer.  This function translates interaction with the
// "rotX" wheel into camera movement.
float
SoGuiExaminerViewerP::rotXWheelMotion(float value, float oldvalue)
{
  SoCamera * camera = PUBLIC(this)->getCamera();
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
}

// The "rotY" wheel is the wheel on the bottom decoration on the
// examiner viewer.  This function translates interaction with the
// "rotX" wheel into camera movement.
float
SoGuiExaminerViewerP::rotYWheelMotion(float value, float oldvalue)
{
  SoCamera * camera = PUBLIC(this)->getCamera();
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
}

// ************************************************************************

// The viewer is a state machine, and all changes to the current state
// are made through this call.
void
SoGuiExaminerViewerP::setMode(const ViewerMode newmode)
{
  const ViewerMode oldmode = this->currentmode;
  if (newmode == oldmode) { return; }

  switch (newmode) {
  case DRAGGING:
    // Set up initial projection point for the projector object when
    // first starting a drag operation.
    this->spinprojector->project(this->lastmouseposition);
    PUBLIC(this)->interactiveCountInc();
    this->clearLog();
    break;

  case SPINNING:
    PUBLIC(this)->interactiveCountInc();
    PUBLIC(this)->scheduleRedraw();
    break;

  case PANNING:
    {
      // The plane we're projecting the mouse coordinates to get 3D
      // coordinates should stay the same during the whole pan
      // operation, so we should calculate this value here.
      SoCamera * cam = PUBLIC(this)->getCamera();
      if (cam == NULL) { // can happen for empty scenegraph
        this->panningplane = SbPlane(SbVec3f(0, 0, 1), 0);
      }
      else {
        SbViewVolume vv = cam->getViewVolume(PUBLIC(this)->getGLAspectRatio());
        this->panningplane = vv.getPlane(cam->focalDistance.getValue());
      }
    }
    PUBLIC(this)->interactiveCountInc();
    break;

  case ZOOMING:
    PUBLIC(this)->interactiveCountInc();
    break;

  default: // include default to avoid compiler warnings.
    break;
  }

  switch (oldmode) {
  case SPINNING:
  case DRAGGING:
  case PANNING:
  case ZOOMING:
    PUBLIC(this)->interactiveCountDec();
    break;

  default:
    break;
  }

#if SOXT_DEBUG && 0 // debug
  if (oldmode == ZOOMING) {
    SbVec3f v = PUBLIC(this)->getCamera()->position.getValue();
    SoDebugError::postInfo("SoXtExaminerViewerP::setMode",
                           "new camera position after zoom: <%e, %e, %e>",
                           v[0], v[1], v[2]);
  }
#endif // debug

  this->setCursorRepresentation(newmode);
  this->currentmode = newmode;
}

// ************************************************************************

void
SoGuiExaminerViewerP::drawAxisCross(void)
{
  // FIXME: convert this to a superimposition scenegraph instead of
  // OpenGL calls. 20020603 mortene.

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
  SbVec2s view = PUBLIC(this)->getGLSize();
  const int pixelarea =
    int(float(this->axiscrossSize)/100.0f * SoXtMin(view[0], view[1]));
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
  const float dim = NEARVAL * float(tan(M_PI / 8.0)); // FOV is 45� (45/360 = 1/8)
  glFrustum(-dim, dim, -dim, dim, NEARVAL, FARVAL);


  // Set up the model matrix.
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  SbMatrix mx;
  SoCamera * cam = PUBLIC(this)->getCamera();

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
    if (val[0] < val[1]) { SoXtSwap(val[0], val[1]); SoXtSwap(idx[0], idx[1]); }
    if (val[1] < val[2]) { SoXtSwap(val[1], val[2]); SoXtSwap(idx[1], idx[2]); }
    if (val[0] < val[1]) { SoXtSwap(val[0], val[1]); SoXtSwap(idx[0], idx[1]); }
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
}

// Draw an arrow for the axis representation directly through OpenGL.
void
SoGuiExaminerViewerP::drawArrow(void)
{
  glBegin(GL_LINES);
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(1.0f, 0.0f, 0.0f);
  glEnd();
  glDisable(GL_CULL_FACE);
  glBegin(GL_TRIANGLES);
  glVertex3f(1.0f, 0.0f, 0.0f);
  glVertex3f(1.0f - 1.0f / 3.0f, +0.5f / 4.0f, 0.0f);
  glVertex3f(1.0f - 1.0f / 3.0f, -0.5f / 4.0f, 0.0f);
  glVertex3f(1.0f, 0.0f, 0.0f);
  glVertex3f(1.0f - 1.0f / 3.0f, 0.0f, +0.5f / 4.0f);
  glVertex3f(1.0f - 1.0f / 3.0f, 0.0f, -0.5f / 4.0f);
  glEnd();
  glBegin(GL_QUADS);
  glVertex3f(1.0f - 1.0f / 3.0f, +0.5f / 4.0f, 0.0f);
  glVertex3f(1.0f - 1.0f / 3.0f, 0.0f, +0.5f / 4.0f);
  glVertex3f(1.0f - 1.0f / 3.0f, -0.5f / 4.0f, 0.0f);
  glVertex3f(1.0f - 1.0f / 3.0f, 0.0f, -0.5f / 4.0f);
  glEnd();
}

// ************************************************************************

// Rotate the camera by the given amount, then reposition it so we're
// still pointing at the same focal point.
void
SoGuiExaminerViewerP::reorientCamera(const SbRotation & rot)
{
  SoCamera * cam = PUBLIC(this)->getCamera();
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
}

// ************************************************************************

// Uses the sphere sheet projector to map the mouseposition unto
// a 3D point and find a rotation from this and the last calculated point.
void
SoGuiExaminerViewerP::spin(const SbVec2f & pointerpos)
{
  if (this->log.historysize < 2) return;
  assert(this->spinprojector != NULL);

  SbVec2s glsize(PUBLIC(this)->getGLSize());
  SbVec2f lastpos;
  lastpos[0] = float(this->log.position[1][0]) / float(SoXtMax((int)(glsize[0]-1), 1));
  lastpos[1] = float(this->log.position[1][1]) / float(SoXtMax((int)(glsize[1]-1), 1));

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
}

// ************************************************************************

// Calculate a zoom/dolly factor from the difference of the current
// cursor position and the last.
void
SoGuiExaminerViewerP::zoomByCursor(const SbVec2f & thispos,
                                   const SbVec2f & prevpos)
{
  // There is no "geometrically correct" value, 20 just seems to give
  // about the right "feel".
  SoGuiFullViewerP::zoom(PUBLIC(this)->getCamera(),
                         (thispos[1] - prevpos[1]) * 20.0f);
}

// *************************************************************************
// Methods used for spin animation tracking.

// This method "clears" the mouse location log, used for spin
// animation calculations.
void
SoGuiExaminerViewerP::clearLog(void)
{
  this->log.historysize = 0;
}

// This method adds another point to the mouse location log, used for spin
// animation calculations.
void
SoGuiExaminerViewerP::addToLog(const SbVec2s pos, const SbTime time)
{
  // In case someone changes the const size setting at the top of this
  // file too small.
  assert (this->log.size > 2 && "mouse log too small!");

  if (this->log.historysize > 0 && pos == this->log.position[0]) {
#if SOXT_DEBUG && 0 // debug
    // This can at least happen under SoQt.
    SoDebugError::postInfo("SoGuiExaminerViewerP::addToLog", "got position already!");
#endif // debug
    return;
  }

  int lastidx = this->log.historysize;
  // If we've filled up the log, we should throw away the last item:
  if (lastidx == this->log.size) { lastidx--; }

  assert(lastidx < this->log.size);
  for (int i = lastidx; i > 0; i--) {
    this->log.position[i] = this->log.position[i-1];
    this->log.time[i] = this->log.time[i-1];
  }

  this->log.position[0] = pos;
  this->log.time[0] = time;
  if (this->log.historysize < this->log.size)
    this->log.historysize += 1;
}

// *************************************************************************

// This method sets whether Motion3 events should affect the camera or
// the model.
void
SoGuiExaminerViewerP::setMotion3OnCamera(SbBool enable)
{
  this->motion3OnCamera = enable;
}

// This method returns whether Motion3 events affects the camera or
// the model.
SbBool
SoGuiExaminerViewerP::getMotion3OnCamera(void) const
{
  return this->motion3OnCamera;
}

// ************************************************************************

// Set cursor graphics according to mode.
void
SoGuiExaminerViewerP::setCursorRepresentation(int modearg)
{
  if (!PUBLIC(this)->isCursorEnabled()) {
    PUBLIC(this)->setComponentCursor(SoXtCursor::getBlankCursor());
    return;
  }

  switch (modearg) {
  case SoGuiExaminerViewerP::INTERACT:
    PUBLIC(this)->setComponentCursor(SoXtCursor(SoXtCursor::DEFAULT));
    break;

  case SoGuiExaminerViewerP::IDLE:
  case SoGuiExaminerViewerP::DRAGGING:
  case SoGuiExaminerViewerP::SPINNING:
    PUBLIC(this)->setComponentCursor(SoXtCursor::getRotateCursor());
    break;

  case SoGuiExaminerViewerP::ZOOMING:
    PUBLIC(this)->setComponentCursor(SoXtCursor::getZoomCursor());
    break;

  case SoGuiExaminerViewerP::SEEK_MODE:
  case SoGuiExaminerViewerP::SEEK_WAIT_MODE:
    PUBLIC(this)->setComponentCursor(SoXtCursor(SoXtCursor::CROSSHAIR));
    break;

  case SoGuiExaminerViewerP::PANNING:
    PUBLIC(this)->setComponentCursor(SoXtCursor::getPanCursor());
    break;

  default: assert(0); break;
  }
}

#endif // DOXYGEN_SKIP_THIS

// *************************************************************************

#undef PRIVATE
#undef PUBLIC

