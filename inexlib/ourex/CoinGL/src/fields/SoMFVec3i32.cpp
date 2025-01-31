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
  \class SoMFVec3i32 SoMFVec3i32.h Inventor/fields/SoMFVec3i32.h
  \brief The SoMFVec3i32 class is a container for SbVec3i32 vectors.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store an array of vectors with three elements.

  This field supports application data sharing through a
  setValuesPointer() method. See SoMField documentation for
  information on how to use this function.

  \sa SbVec3i32, SoSFVec3i32
  \sa Coin 2.5
*/

// *************************************************************************

#include <Inventor/fields/SoMFVec3i32.h>

#include <assert.h>
#include <Inventor/fields/SoSubFieldP.h>
#include <Inventor/SoInput.h>
#include <Inventor/errors/SoDebugError.h>

#include "shared.h"

// *************************************************************************

SO_MFIELD_SOURCE(SoMFVec3i32, SbVec3i32, const SbVec3i32 &);

SO_MFIELD_SETVALUESPOINTER_SOURCE(SoMFVec3i32, SbVec3i32, SbVec3i32);
SO_MFIELD_SETVALUESPOINTER_SOURCE(SoMFVec3i32, SbVec3i32, int32_t);

// *************************************************************************

// Override from parent class.
void
SoMFVec3i32::initClass(void)
{
  SO_MFIELD_INTERNAL_INIT_CLASS(SoMFVec3i32);
}

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

SbBool
SoMFVec3i32::read1Value(SoInput * in, int idx)
{
#if 1 // 12-25% speed increase when this is activated. pederb, 2004-02-17
  assert(idx < this->maxNum);
  return 
    in->read(this->values[idx][0]) &&
    in->read(this->values[idx][1]) &&
    in->read(this->values[idx][2]);
#else // end of new, optimized version
  SbVec3i32 v;
  if (!sosfvec3i32_read_value(in, v)) return FALSE;
  this->set1Value(idx, v);
  return TRUE;
#endif // old, slower version
}

void
SoMFVec3i32::write1Value(SoOutput * out, int idx) const
{
  sosfvec3i32_write_value(out, (*this)[idx]);
}

#endif // DOXYGEN_SKIP_THIS

// *************************************************************************

/*!
  Set \a num vector array elements from the \a xyz array, reading from
  index 0 in the given float-triplet vector array and storing values
  from index \a start in this SoMFVec3f's array.

  If \a start + \a num is larger than the current number of elements
  in the SoMFVec3f field, this method will automatically expand the
  field to accommodate all given float-vector element values.
*/
void
SoMFVec3i32::setValues(int start, int numarg, const int32_t xyz[][3])
{
  if (start+numarg > this->maxNum) this->allocValues(start+numarg);
  else if (start+numarg > this->num) this->num = start+numarg;

  for(int i=0; i < numarg; i++) this->values[start+i] = SbVec3i32(xyz[i]);
  this->valueChanged();
}

/*!
  Set the vector at \a idx.
*/
void
SoMFVec3i32::set1Value(int idx, int32_t x, int32_t y, int32_t z)
{
  this->set1Value(idx, SbVec3i32(x, y, z));
}

/*!
  Set the vector at \a idx.
*/
void
SoMFVec3i32::set1Value(int idx, const int32_t xyz[3])
{
  this->set1Value(idx, SbVec3i32(xyz));
}

/*!
  Set this field to contain a single vector with the given
  element values.
*/
void
SoMFVec3i32::setValue(int32_t x, int32_t y, int32_t z)
{
  this->setValue(SbVec3i32(x, y, z));
}

/*!
  Set this field to contain a single vector with the given
  element values.
*/
void
SoMFVec3i32::setValue(const int32_t xyz[3])
{
  if (xyz == NULL) this->setNum(0);
  else this->setValue(SbVec3i32(xyz));
}

// *************************************************************************
