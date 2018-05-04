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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/Xt/devices/SoXtMouse.h>
#include <Inventor/Xt/devices/SoGuiMouseP.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/events/SoLocation2Event.h>

/*!
  \class SoXtMouse SoXtMouse.h Inventor/Xt/devices/SoXtMouse.h
  \brief The SoXtMouse class is the mouse input device abstraction.
  \ingroup devices

  The SoXtMouse class is the glue between native mouse handling and
  mouse interaction in the Inventor scenegraph.

  All components derived from the SoXtRenderArea have got an
  SoXtMouse device attached by default.

  One important note for application programmers: our mappings to
  SoMouseButtonEvent::BUTTON2 and SoMouseButtonEvent::BUTTON3 do not
  match the mappings in SGI's InventorXt library or TGS's SoWin
  library for 3-button mice. They map mouse buttons like this:

  <ul>
  <li>left button: SoMouseButtonEvent::BUTTON1</li>
  <li>middle button: SoMouseButtonEvent::BUTTON2</li>
  <li>right button: SoMouseButtonEvent::BUTTON3</li>
  </ul>

  While in this SIM SoXt library the mappings are:

  <ul>
  <li>left button: SoMouseButtonEvent::BUTTON1</li>
  <li>middle button: SoMouseButtonEvent::BUTTON3</li>
  <li>right button: SoMouseButtonEvent::BUTTON2</li>
  </ul>

  This is a conscious design decision we've made.  The reason is that
  BUTTON2 should be the right mouse button whether you have a 2-button
  mouse or a 3-button mouse.
*/

// *************************************************************************

SOXT_OBJECT_SOURCE(SoXtMouse);

// *************************************************************************

/*!
  \enum SoXtMouse::Events
  Enumeration over supported mouse events.
*/
/*!
  \var SoXtMouse::Events SoXtMouse::BUTTON_PRESS
  Maskbit for mousebutton press events.
*/
/*!
  \var SoXtMouse::Events SoXtMouse::BUTTON_RELEASE
  Maskbit for mousebutton release events.
*/
/*!
  \var SoXtMouse::Events SoXtMouse::POINTER_MOTION
  Maskbit for mousepointer motion events.
*/
/*!
  \var SoXtMouse::Events SoXtMouse::BUTTON_MOTION
  Maskbit for mousepointer motion events with one or more mousebuttons
  pressed.
*/
/*!
  \var SoXtMouse::Events SoXtMouse::ALL_EVENTS

  Mask which includes all the maskbits in the enum (ie use this to
  signal interest in all kinds of events for the mouse device).
*/

// *************************************************************************

/*!
  \fn SoXtMouse::SoXtMouse(int mask)

  Constructor. The \a mask argument should contain the set of
  SoXtMouse::Events one is interested in tracking.
*/

/*!
  \fn SoXtMouse::~SoXtMouse()
  Destructor.
*/

/*!
  \fn const SoEvent * SoXtMouse::translateEvent(XAnyEvent * event)

  Translates a native event from the underlying toolkit into a generic
  event.

  This is then returned in the form of an instance of a subclass of
  the Inventor API's SoEvent class, either an SoMouseButtonEvent or an
  SoLocation2Event, depending on whether the native event is a
  mousebutton press / release, or a mousecursor movement event.

  The mapping of the mousebuttons upon generation of
  SoMouseButtonEvent events will be done as follows:

  <ul>
  <li>left mousebutton: SoMouseButtonEvent::BUTTON1</li>
  <li>right mousebutton: SoMouseButtonEvent::BUTTON2</li>
  <li>middle mousebutton, if available: SoMouseButtonEvent::BUTTON3</li>
  <li>forward motion on a wheel mouse: SoMouseButtonEvent::BUTTON4</li>
  <li>backward motion on a wheel mouse: SoMouseButtonEvent::BUTTON5</li>
  </ul>

  Note that the rightmost mousebutton will always map to
  SoMouseButtonEvent::BUTTON2, even on a 3-button mouse.
*/

// *************************************************************************

#ifndef DOXYGEN_SKIP_THIS

SoGuiMouseP::SoGuiMouseP(SoXtMouse * p)
{
  this->pub = p;

  // Allocate system-neutral event objects once and reuse.
  this->buttonevent = new SoMouseButtonEvent;
  this->locationevent = new SoLocation2Event;
}

SoGuiMouseP::~SoGuiMouseP()
{
  delete this->buttonevent;
  delete this->locationevent;
}

#endif // !DOXYGEN_SKIP_THIS

// *************************************************************************
