#ifndef COIN_SOGUITOGGLEBUTTON_H
#define COIN_SOGUITOGGLEBUTTON_H

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

#ifndef SOXT_INTERNAL
#error this is a private header file
#endif /* !SOXT_INTERNAL */

#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/nodekits/SoBaseKit.h>

class SoGuiToggleButton : public SoBaseKit {
  typedef SoBaseKit inherited;
  SO_KIT_HEADER(SoGuiToggleButton);
  SO_KIT_CATALOG_ENTRY_HEADER(root);
public:
  static void initClass(void);

  SoGuiToggleButton(void);

  SoSFVec3f size;
  SoSFBool on;

  virtual void handleEvent(SoHandleEventAction * action);

protected:
  virtual ~SoGuiToggleButton(void);

private:
  void * internals;

};

#endif // !COIN_SOGUITOGGLEBUTTON_H
