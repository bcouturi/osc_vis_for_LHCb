#ifndef COIN_SOSFUSHORT_H
#define COIN_SOSFUSHORT_H

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

#ifdef __PIVY__
%include Inventor/fields/SoSFUShort.i
#endif

#include <Inventor/fields/SoSField.h>
#include <Inventor/fields/SoSubField.h>

class COIN_DLL_API SoSFUShort : public SoSField {
  typedef SoSField inherited;

  SO_SFIELD_HEADER(SoSFUShort, unsigned short, unsigned short);

public:
  static void initClass(void);
};

#endif // !COIN_SOSFUSHORT_H
