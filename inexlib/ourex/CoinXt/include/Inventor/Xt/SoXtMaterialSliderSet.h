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

// 

#ifndef SOXT_MATERIALSLIDERSET_H
#define SOXT_MATERIALSLIDERSET_H

#include <Inventor/Xt/SoXtSliderSet.h>

// *************************************************************************

class SOXT_DLL_API SoXtMaterialSliderSet : public SoXtSliderSet {
  SOXT_OBJECT_HEADER(SoXtMaterialSliderSet, SoXtSliderSet);

public:
  SoXtMaterialSliderSet(
    Widget parent = NULL,
    const char * const name = NULL,
    SbBool embed = TRUE,
    SoNode * const node = NULL);
  ~SoXtMaterialSliderSet(void);

protected:
  SoXtMaterialSliderSet(
    Widget parent,
    const char * const name,
    SbBool embed,
    SoNode * const node,
    SbBool build);

  Widget buildWidget(Widget parent);

  virtual const char * getDefaultWidgetName(void) const;
  virtual const char * getDefaultTitle(void) const;
  virtual const char * getDefaultIconTitle(void) const;

private:
  void constructor(SbBool build);

}; // class SoXtMaterialSliderSet

// *************************************************************************

#endif // ! SOXT_MATERIALSLIDERSET_H
