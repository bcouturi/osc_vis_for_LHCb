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
// So*PlaneViewer sourcecode.


// *************************************************************************

#include <Inventor/Gtk/viewers/SoGtkPlaneViewer.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <assert.h>
#include <math.h>

#include <Inventor/Gtk/common/gl.h>

#include <Inventor/SbLinear.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/events/SoKeyboardEvent.h>

#include <sogtkdefs.h>

// ************************************************************************

/*!
*/
void
SoGtkPlaneViewer::commonConstructor(void)
{
  this->mode = SoGtkPlaneViewer::IDLE_MODE ;

  this->canvas = SbVec2s(0, 0);
  this->pointer.now = SbVec2s(0, 0);
  this->pointer.then = SbVec2s(0, 0);
  this->controldown = FALSE;
  this->button1down = FALSE;
  this->button2down = FALSE;
  this->button3down = FALSE;
}

// *************************************************************************

/*!
*/

void
SoGtkPlaneViewer::translateX(
  const float delta) const
{
  SoCamera * const camera = this->getCamera();
  if (! camera) return; // probably a scene-less viewer

  SbVec3f dir;
  camera->orientation.getValue().multVec(SbVec3f(1, 0, 0), dir);
  camera->position = camera->position.getValue() - dir * delta *
    camera->focalDistance.getValue() / 2.5f;
} // translateX()

/*!
*/

void
SoGtkPlaneViewer::translateY(
  const float delta) const
{
  SoCamera * const camera = this->getCamera();
  if (! camera) return; // probably a scene-less viewer

  SbVec3f dir;
  camera->orientation.getValue().multVec(SbVec3f(0, 1, 0), dir);
  camera->position = camera->position.getValue() + dir * delta *
    camera->focalDistance.getValue() / 2.5f;
} // translateY()

/*!
*/

void
SoGtkPlaneViewer::rotateZ(
  const float angle) const
{
  SoCamera * const camera = this->getCamera();
  if (! camera) return; // probably a scene-less viewer

  SbVec3f dir;
  camera->orientation.getValue().multVec(SbVec3f(0, 0, -1), dir);
  camera->orientation =
    camera->orientation.getValue() * SbRotation(dir, angle);
} // rotateZ()

// *************************************************************************

/*!
*/

void
SoGtkPlaneViewer::viewPlaneX(
  void) const
{
  SoCamera * const camera = this->getCamera();
  if (! camera) return; // probably a scene-less viewer

  SbVec3f dir;
  camera->orientation.getValue().multVec(SbVec3f(0, 0, -1), dir);
  SbVec3f focalpoint = camera->position.getValue() +
    camera->focalDistance.getValue() * dir;
  camera->position = focalpoint +
    camera->focalDistance.getValue() * SbVec3f(1, 0, 0);
  camera->orientation = SbRotation(SbVec3f(0, 1, 0), M_PI/2.0f);
} // viewPlaneX()

/*!
*/

void
SoGtkPlaneViewer::viewPlaneY(
  void) const
{
  SoCamera * const camera = this->getCamera();
  if (! camera) return; // probably a scene-less viewer

  SbVec3f dir;
  camera->orientation.getValue().multVec(SbVec3f(0, 0, -1), dir);
  SbVec3f focalpoint = camera->position.getValue() +
    camera->focalDistance.getValue() * dir;
  camera->position = focalpoint +
    camera->focalDistance.getValue() * SbVec3f(0, 1, 0);
  camera->orientation = SbRotation(SbVec3f(1, 0, 0), -M_PI/2.0f);
} // viewPlaneY()

/*!
*/

void
SoGtkPlaneViewer::viewPlaneZ(
  void) const
{
  SoCamera * const camera = this->getCamera();
  if (! camera) return; // probably a scene-less viewer

  SbVec3f dir;
  camera->orientation.getValue().multVec(SbVec3f(0, 0, -1), dir);
  SbVec3f focalpoint = camera->position.getValue() +
    camera->focalDistance.getValue() * dir;
  camera->position = focalpoint +
    camera->focalDistance.getValue() * SbVec3f(0, 0, 1);
  camera->orientation = SbRotation(SbVec3f(0, 1, 0), 0);
} // viewPlaneZ()

// *************************************************************************

/*!
  This method moves the camera inwards and outwards.
*/

void
SoGtkPlaneViewer::zoom(
  const float difference) const
{
  SoCamera * camera = this->getCamera();
  if (! camera) return; // probably a scene-less viewer

  SoType type = camera->getTypeId();
  float multiplicator = exp(difference); // in the range of <0, ->>

  if (type.isDerivedFrom(SoOrthographicCamera::getClassTypeId())) {
    SoOrthographicCamera * orthocam = (SoOrthographicCamera *) camera;
    orthocam->height = orthocam->height.getValue() * multiplicator;
  } else if (type.isDerivedFrom(SoPerspectiveCamera::getClassTypeId())) {
    float oldfocaldist = camera->focalDistance.getValue();
    camera->focalDistance = oldfocaldist * multiplicator;
    SbVec3f direction;
    camera->orientation.getValue().multVec(SbVec3f(0, 0, -1), direction);
    camera->position = camera->position.getValue() +
      (camera->focalDistance.getValue() - oldfocaldist) * -direction;
  } else {
    assert(0 && "impossible");
  }
} // zoom()

// *************************************************************************

/*!
*/

void
SoGtkPlaneViewer::setCanvasSize(
  const SbVec2s size)
{
  this->canvas = size;
} // setCanvasSize()

/*!
*/

void
SoGtkPlaneViewer::setPointerLocation(
  const SbVec2s position)
{
  this->pointer.then = this->pointer.now;
  this->pointer.now = position;
} // setPointerLocation()

/*!
*/

int
SoGtkPlaneViewer::getPointerXMotion(
  void) const
{
  return this->pointer.now[0] - this->pointer.then[0];
} // getPointerXMotion()

/*!
*/

int
SoGtkPlaneViewer::getPointerYMotion(
  void) const
{
  return this->pointer.now[1] - this->pointer.then[1];
} // getPointerYMotion()

/*!
*/

float
SoGtkPlaneViewer::getPointerOrigoAngle(
  void) const
{
  SbVec2s now = this->pointer.now;
  now[0] -= this->canvas[0] / 2;
  now[1] -= this->canvas[1] / 2;

  float nowradval = atan(fabs((float) now[1] / (float) now[0]));
  if (now[0] < 0) nowradval = M_PI - nowradval;
  if (now[1] < 0) nowradval = 2 * M_PI - nowradval;

  return nowradval;
} // getPointerOrigoAngle()

/*!
*/

float
SoGtkPlaneViewer::getPointerOrigoMotionAngle(
  void) const
{
  if (this->pointer.then == this->pointer.now)
    return 0.0f;

  SbVec2s now = this->pointer.now;
  now[0] -= this->canvas[0] / 2;
  now[1] -= this->canvas[1] / 2;

  SbVec2s then = this->pointer.then;
  then[0] -= this->canvas[0] / 2;
  then[1] -= this->canvas[1] / 2;

  float nowradval = atan(fabs((float) now[1] / (float) now[0]));
  if (now[0] < 0) nowradval = M_PI - nowradval;
  if (now[1] < 0) nowradval = 2 * M_PI - nowradval;

  float thenradval = atan(fabs((float) then[1] / (float) then[0]));
  if (then[0] < 0) thenradval = M_PI - thenradval;
  if (then[1] < 0) thenradval = 2 * M_PI - thenradval;

  return nowradval - thenradval;
} // getPointerMotionAngle()

// ************************************************************************

/*!
  This method draws the rotate mode graphics...
  FIXME: use Inventor instead of OpenGL...
*/

void
SoGtkPlaneViewer::drawRotateGraphics(
  void) const
{
  glPushAttrib(GL_LIGHTING_BIT|GL_DEPTH_BUFFER_BIT|GL_TRANSFORM_BIT|
               GL_VIEWPORT_BIT|GL_LINE_BIT|GL_ENABLE_BIT);
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
  glPushMatrix();

  // draw gfx
  float angle = this->getPointerOrigoAngle();
  glRotatef(angle * (180.0f/M_PI) - 90.0f, 0, 0, 1);

  float length = 0;
  float x = 0;
  x = (float) this->pointer.now[0] / (float) this->canvas[0];
  x = x * 2.0f - 1.0f;
  float y = 0;
  y = (float) this->pointer.now[1] / (float) this->canvas[1];
  y = y * 2.0f - 1.0f;

  length = sqrt(x * x + y * y);

  glColor3f(1, 1, 1);
  glLineWidth(3.0f);
  glBegin(GL_LINES);
  glVertex2f(0.0f, 0.0f);
  glVertex2f(0.0f, length);
  glEnd();

  // Reset original state.
  glPopMatrix();
  glDepthRange(depthrange[0], depthrange[1]);
  glMatrixMode(GL_PROJECTION);
  glLoadMatrixd(projectionmatrix);
  glPopAttrib();
} // drawRotateGraphics()

// ************************************************************************

#define MOUSE_BUTTON_EVENT_TYPE (SoMouseButtonEvent::getClassTypeId())
#define LOCATION2_EVENT_TYPE    (SoLocation2Event::getClassTypeId())
#define KEYBOARD_EVENT_TYPE     (SoKeyboardEvent::getClassTypeId())

// Documented in superclass.
SbBool
SoGtkPlaneViewer::processSoEvent(const SoEvent * const ev)
{
  // We're in "interact" mode (ie *not* the camera modification mode),
  // so don't handle the event here. It should either be forwarded to
  // the scenegraph, or caught by SoGtkViewer::processSoEvent() if
  // it's an ESC press (to switch modes).
  if (!this->isViewing()) { return inherited::processSoEvent(ev); }

  const SoType type(ev->getTypeId());

  const SbVec2s size(this->getGLSize());
  const SbVec2s pos(ev->getPosition());
  const SbVec2f posn((float)pos[0] / (float) SoGtkMax(size[0], (short int)1),
                      (float)pos[1] / (float) SoGtkMax(size[1], (short int)1));

  if (type.isDerivedFrom(MOUSE_BUTTON_EVENT_TYPE)) {
    const SoMouseButtonEvent * event = (const SoMouseButtonEvent *) ev;
    const SbBool press = (event->getState() == SoButtonEvent::DOWN) ? TRUE : FALSE;

    if (press) {
      this->setPointerLocation(pos);
      this->setPointerLocation(pos);
    }

    switch (event->getButton()) {
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
    } // switch (event->getButton())

    if (press) {
      switch (event->getButton()) {
      case SoMouseButtonEvent::BUTTON1:
        if (this->mode == IDLE_MODE) {
          this->interactiveCountInc();
          this->mode = DOLLY_MODE;
        } else if (this->mode == SEEK_WAIT_MODE) {
          this->interactiveCountInc();
          this->seekToPoint(pos);
        }
        break;

      case SoMouseButtonEvent::BUTTON2:
        // FIXME: shouldn't these events be passed on to the
        // superclass? 20020113 mortene.
        return FALSE;

      case SoMouseButtonEvent::BUTTON3:
        if (this->mode == IDLE_MODE) {
          this->interactiveCountInc();
          this->mode = TRANSLATE_MODE;
        } else if (this->mode == ROTZ_WAIT_MODE) {
          this->mode = ROTZ_MODE;
        }
        break;

#ifdef HAVE_SOMOUSEBUTTONEVENT_BUTTON5
      case SoMouseButtonEvent::BUTTON4:
        this->zoom(0.1f);
        break;

      case SoMouseButtonEvent::BUTTON5:
        this->zoom(-0.1f);
        break;
#endif // HAVE_SOMOUSEBUTTONEVENT_BUTTON5

      default:
        break;
      } // switch (event->getButton())
    } else {
      switch (event->getButton()) {
      case SoMouseButtonEvent::BUTTON1:
      case SoMouseButtonEvent::BUTTON3:
        if (this->mode != IDLE_MODE && this->mode != ROTZ_MODE) {
          this->interactiveCountDec();
          this->mode = IDLE_MODE;
        }
        break;

      default:
        break;
      } // switch (((XButtonEvent *) event)->button)
    }

    return TRUE;
  }

  if (type.isDerivedFrom(LOCATION2_EVENT_TYPE)) {
    const SoLocation2Event * event = (const SoLocation2Event *) ev;

    this->setPointerLocation(pos);

    const SbVec2f prevn((float) this->pointer.then[0] / (float) SoGtkMax(size[0], (short int)1),
                         (float) this->pointer.then[1] / (float) SoGtkMax(size[1], (short int)1));

    do {

      if (this->controldown && this->button1down) {
        this->rotateZ(this->getPointerOrigoMotionAngle());
        break;
      }

      if (! this->controldown && this->button3down) {
        if ((posn[0] != prevn[0]) || (posn[1] != prevn[1])) {
          float dx = (posn[0] - prevn[0]) * 2.5f;
          float dy = (posn[1] - prevn[1]) * 2.5f;
          float yvalue = this->getLeftWheelValue() - dy;
          float xvalue = this->getBottomWheelValue() + dx;
          this->leftWheelMotion(yvalue);
          this->setLeftWheelValue(yvalue);
          this->bottomWheelMotion(xvalue);
          this->setBottomWheelValue(xvalue);
        }
        break;
      }

      if (! this->controldown && this->button1down) {
        if (posn[1] != prevn[1]) {
          float value = this->getRightWheelValue() + (prevn[1] - posn[1]) * 10.0f;
          this->rightWheelMotion(value);
          this->setRightWheelValue(value);
        }
        break;
      }

    } while (FALSE);
    return TRUE;
  } // LOCATION2_EVENT_TYPE

  if (type.isDerivedFrom(KEYBOARD_EVENT_TYPE)) {
    const SoKeyboardEvent * event = (const SoKeyboardEvent *) ev;
    const SbBool press = (event->getState() == SoButtonEvent::DOWN) ? TRUE : FALSE;

    switch (event->getKey()) {
    case SoKeyboardEvent::LEFT_CONTROL:
    case SoKeyboardEvent::RIGHT_CONTROL:
      this->controldown = press;

      if (press) {
        this->setCanvasSize(size);
        this->setPointerLocation(pos);
        this->setPointerLocation(pos);
        this->scheduleRedraw();
      } else {
        this->scheduleRedraw();
      }
      return TRUE;
      break;
    default:
      break;
    }

  } // KEYBOARD_EVENT_TYPE

  // FIXME: shouldn't unhandled events be passed on to the superclass?
  // 20020113 mortene.
  return FALSE;
}

// ************************************************************************
