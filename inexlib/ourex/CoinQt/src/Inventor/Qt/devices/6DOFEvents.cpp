/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "6DOFEvents.h"

#include <QtCore/qglobal.h> //G.Barrand QtCore
//G.Barrand #if (QT_VERSION >= 0x040000)
//G.Barrand #include <q3memarray.h>
//G.Barrand #endif

SoQt6dofDevicePressureEvent::SoQt6dofDevicePressureEvent(void)
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
  : inherited(QEvent::User, NULL)
#else
  : inherited(QEvent::User) //G.Barrand_Qt4
#endif
{
  this->t = (QEvent::Type) soqt6dofDevicePressureEvent;
  this->trans_x = 0.0f;
  this->trans_y = 0.0f;
  this->trans_z = 0.0f;
  this->rot_x = 0.0f;
  this->rot_y = 0.0f;
  this->rot_z = 0.0f;
}

void
SoQt6dofDevicePressureEvent::setTranslation(float x, float y, float z)
{
  this->trans_x = x;
  this->trans_y = y;
  this->trans_z = z;
}

void
SoQt6dofDevicePressureEvent::getTranslation(float & x,
                                            float & y,
                                            float & z) const
{
  x = this->trans_x;
  y = this->trans_y;
  z = this->trans_z;
}

void
SoQt6dofDevicePressureEvent::setRotation(float x, float y, float z)
{
  this->rot_x = x;
  this->rot_y = y;
  this->rot_z = z;
}

void
SoQt6dofDevicePressureEvent::getRotation(float & x, float & y, float & z) const
{
  x = this->rot_x;
  y = this->rot_y;
  z = this->rot_z;
}

// *************************************************************************

SoQt6dofDeviceButtonEvent::SoQt6dofDeviceButtonEvent(void)
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
  : inherited(QEvent::User, NULL)
#else
  : inherited(QEvent::User) //G.Barrand_Qt4
#endif
{
  this->t = (QEvent::Type) soqt6dofDeviceButtonReleasedEvent;
  this->state = 0;
  this->button = 0;
  this->buttons = 0;
}

void
SoQt6dofDeviceButtonEvent::setButton(unsigned int buttonarg)
{
  this->button = buttonarg;
  if ((1 << this->button) & this->state)
    this->t = (QEvent::Type) soqt6dofDeviceButtonPressedEvent;
  else
    this->t = (QEvent::Type) soqt6dofDeviceButtonReleasedEvent;
}

unsigned int
SoQt6dofDeviceButtonEvent::getButton(void) const
{
  return this->button;
}

void
SoQt6dofDeviceButtonEvent::setState(unsigned int statearg)
{
  this->state = statearg;
  if ((1 << this->button) & this->state)
    this->t = (QEvent::Type) soqt6dofDeviceButtonPressedEvent;
  else
    this->t = (QEvent::Type) soqt6dofDeviceButtonReleasedEvent;
}

unsigned int
SoQt6dofDeviceButtonEvent::getState(void) const
{
  return this->state;
}

void
SoQt6dofDeviceButtonEvent::setNumButtons(unsigned int buttonsarg)
{
  this->buttons = buttonsarg;
}

unsigned int
SoQt6dofDeviceButtonEvent::getNumButtons(void) const
{
  return this->buttons;
}

int
SoQt6dofDeviceButtonEvent::isButtonPress(void) const
{
  if (this->t == (QEvent::Type) soqt6dofDeviceButtonPressedEvent)
    return /*TRUE*/1; //G.Barrand
  return /*FALSE*/0;  //G.Barrand
}

// *************************************************************************
