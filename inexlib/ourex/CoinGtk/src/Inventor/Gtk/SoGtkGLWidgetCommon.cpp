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

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/Gtk/common/gl.h>
#include <Inventor/Gtk/SoGtkGLWidget.h>

/*!
  This function is provided as a convenience for the application
  programmer to help with acquiring the OpenGL implementation limits
  for rendering points.

  For robust application code, one needs to consider the range limits
  when setting the SoDrawStyle::pointSize field.
*/
void
SoGtkGLWidget::getPointSizeLimits(SbVec2f & range, float & granularity)
{
  this->glLockNormal();

  GLfloat vals[2];
  glGetFloatv(GL_POINT_SIZE_RANGE, vals);

  // Matthias Koenig reported on coin-discuss that the OpenGL
  // implementation on SGI Onyx 2 InfiniteReality returns 0 for the
  // lowest pointsize, but it will still set the return value of
  // glGetError() to GL_INVALID_VALUE if this size is attempted
  // used. So the boundary range fix in the next line of code is a
  // workaround for that OpenGL implementation bug.
  //
  // 0.0f and lower values are explicitly disallowed, according to
  // the OpenGL 1.3 specification, Chapter 3.3.

  if (vals[0] <= 0.0f) { vals[0] = SoGtkMin(1.0f, vals[1]); }

  range.setValue(vals[0], vals[1]);

  GLfloat gran[1];
  glGetFloatv(GL_POINT_SIZE_GRANULARITY, gran);
  granularity = gran[0];

  this->glUnlockNormal();
}

/*!
  This function is provided as a convenience for the application
  programmer to help with acquiring the OpenGL implementation limits
  for rendering lines.

  For robust application code, one needs to consider the range limits
  when setting the SoDrawStyle::lineWidth field.
*/
void
SoGtkGLWidget::getLineWidthLimits(SbVec2f & range, float & granularity)
{
  this->glLockNormal();

  GLfloat vals[2];
  glGetFloatv(GL_LINE_WIDTH_RANGE, vals);

  // Matthias Koenig reported on coin-discuss that the OpenGL
  // implementation on SGI Onyx 2 InfiniteReality returns 0 for the
  // lowest linewidth, but it will still set the return value of
  // glGetError() to GL_INVALID_VALUE if this size is attempted
  // used. This is a workaround for what looks like an OpenGL bug.

  if (vals[0] <= 0.0f) { vals[0] = SoGtkMin(1.0f, vals[1]); }

  range.setValue(vals[0], vals[1]);

  GLfloat gran[1];
  glGetFloatv(GL_LINE_WIDTH_GRANULARITY, gran);
  granularity = gran[0];

  this->glUnlockNormal();
}
