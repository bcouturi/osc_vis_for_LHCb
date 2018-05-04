#ifndef COIN_SOGUINODES_H
#define COIN_SOGUINODES_H

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

#include <Inventor/Xt/nodes/SoGuiSceneTexture2.h>
#include <Inventor/Xt/nodes/SoGuiViewportFix.h>
#include <Inventor/Xt/nodes/SoGuiPane.h>
#include <Inventor/Xt/nodes/SoGuiPosition.h>
#include <Inventor/Xt/nodes/SoGuiTranslation.h>
#include <Inventor/Xt/nodes/SoGuiFrame.h>
#include <Inventor/Xt/nodes/SoGuiImage.h>
#include <Inventor/Xt/nodes/SoGuiLabel.h>
#include <Inventor/Xt/nodes/SoGuiToggleButton.h>
#include <Inventor/Xt/nodes/SoGuiRadioButton.h>
#include <Inventor/Xt/nodes/SoGuiClickCounter.h>
#include <Inventor/Xt/nodes/SoGuiSlider1.h>
#include <Inventor/Xt/nodes/SoGuiSlider2.h>
#include <Inventor/Xt/nodes/SoGuiColorEditor.h>
#include <Inventor/Xt/nodes/SoGuiMaterialEditor.h>
#include <Inventor/Xt/nodes/SoGuiViewpointWrapper.h>

class SoGuiNodes {
public:
  static void initClasses(void);

};

#endif // !COIN_SOGUINODES_H
