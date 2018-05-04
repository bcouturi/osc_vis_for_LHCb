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

#ifndef SOXTPLANEVIEWERP_H
#define SOXTPLANEVIEWERP_H

#include <Inventor/Xt/viewers/SoGuiPlaneViewerP.h>

// ************************************************************************

// This class contains private data and methods used within the
// SoXtPlaneViewer class.

class SoXtPlaneViewerP : public SoGuiPlaneViewerP
{
public:
  SoXtPlaneViewerP(SoXtPlaneViewer * publ);
  ~SoXtPlaneViewerP();

  void constructor(SbBool build);

  static struct SoXtViewerButton SoXtPlaneViewerButtons[];
  struct SoXtViewerButton * buttons;
  int findButton(Widget button) const;
  
  static void buttonCB(Widget, XtPointer, XtPointer);

  struct {
    Pixmap ortho, ortho_ins;
    Pixmap perspective, perspective_ins;
  } pixmaps;
};

// ************************************************************************

#endif // ! SOXTPLANEVIEWERP_H
