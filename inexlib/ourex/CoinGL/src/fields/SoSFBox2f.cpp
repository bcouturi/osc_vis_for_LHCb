/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2007 by Systems in Motion.  All rights reserved.
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
 *  See http://www.coin3d.org/ for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*!
  \class SoSFBox2f SoSFBox2f.h Inventor/fields/SoSFBox2f.h
  \brief The SoSFBox2f class is a container for an SbBox2f vector.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store a box.

  \COIN_CLASS_EXTENSION
  \since Coin 2.5
*/

// *************************************************************************

#include <Inventor/fields/SoSFBox2f.h>

#include <Inventor/fields/SoSubFieldP.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/errors/SoReadError.h>

// *************************************************************************

SO_SFIELD_SOURCE(SoSFBox2f, SbBox2f, const SbBox2f &);

// *************************************************************************

// Override from parent class.
void
SoSFBox2f::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFBox2f);
}

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS


SbBool
SoSFBox2f::readValue(SoInput * in)
{
  float min[2];
  float max[2];
  if (!in->read(min[0]) || 
      !in->read(min[1]) || 
      !in->read(max[0]) || 
      !in->read(max[1])) {
    SoReadError::post(in, "Couldn't read SoSFBox2f");
    return FALSE;
  }
  this->setValue(min[0], min[1], max[0], max[1]);
  return TRUE;
}

void
SoSFBox2f::writeValue(SoOutput * out) const
{
  float min[2];
  float max[2];
  SbBox2f b = this->getValue();
  b.getBounds(min[0], min[1], max[0], max[1]);

  out->write(min[0]);
  if (!out->isBinary()) out->write(' ');
  out->write(min[1]);
  if (!out->isBinary()) out->write(' ');
  out->write(max[0]);
  if (!out->isBinary()) out->write(' ');
  out->write(max[1]);
}

#endif // DOXYGEN_SKIP_THIS

// *************************************************************************

/*!
  Set value of vector.
*/
void
SoSFBox2f::setValue(float xmin, float ymin, float xmax, float ymax)
{
  this->setValue(SbBox2f(xmin, ymin, xmax, ymax));
}


/*!
  Set value of vector.
*/
void
SoSFBox2f::setValue(const SbVec2f & minvec, const SbVec2f & maxvec)
{
  this->setValue(SbBox2f(minvec, maxvec));
}


/*!
  Set value of vector.
*/
void
SoSFBox2f::getValue(SbBox2f & box) const
{
  box = value;
}

// *************************************************************************
