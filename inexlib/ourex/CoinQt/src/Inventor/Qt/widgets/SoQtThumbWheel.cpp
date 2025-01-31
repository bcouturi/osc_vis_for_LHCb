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

// FIXME: should probably collect thumbwheel doc to a common
// class. 20031008 mortene.

/*!
  \class SoQtThumbWheel SoQtThumbWheel.h Inventor/Qt/widgets/SoQtThumbWheel.h
  \brief The SoQtThumbWheel class is a UI component for fancy looking thumbwheel controls.
  \ingroup components
*/

// *************************************************************************

#include <math.h>
#include <assert.h>
#include <stdio.h>

// FIXME: get rid of this define. We should fix up the compile issues
// wrt Qt 4 properly. 20050629 mortene.
//G.Barrand_Qt4 #define QT3_SUPPORT

#include <QtCore/qglobal.h> //G.Barrand

#if QT_VERSION < 0x050000 // pre Qt 5
#include <QtGui/qdrawutil.h> //G.Barrand QtGui
#else
#include <QtWidgets/qdrawutil.h>
#endif

#include <QtGui/qpainter.h> //G.Barrand QtGui
#include <QtGui/qimage.h> //G.Barrand QtGui
#include <QtGui/qpixmap.h> //G.Barrand QtGui
#include <QtCore/qmetaobject.h> //G.Barrand QtCore
#include <QtGui/qevent.h> //G.Barrand QtGui

#include <Inventor/SbBasic.h>
#include <Inventor/errors/SoDebugError.h>

#include <Inventor/Qt/widgets/SoQtThumbWheel.h>
#include <Inventor/Qt/widgets/moc_SoQtThumbWheel.icc>
#include <Inventor/Qt/widgets/SoAnyThumbWheel.h>

#include <soqtdefs.h>

// *************************************************************************

static const int SHADEBORDERWIDTH = 0;

SoQtThumbWheel::SoQtThumbWheel(QWidget * parent,
                               const char * name)
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
  : QWidget(parent, name)
#else
  : QWidget(parent) //G.Barrand_Qt4
#endif
{
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
#else
#if QT_VERSION < 0x050000 // pre Qt 5
  setObjectName(QString::fromAscii(name));
#else
  setObjectName(QString::fromLatin1(name)); //G.Barrand_Qt4
#endif
#endif
  this->constructor(SoQtThumbWheel::Vertical);
}

SoQtThumbWheel::SoQtThumbWheel(Orientation orientation,
                               QWidget * parent,
                               const char * name)
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
  : QWidget(parent, name)
#else
  : QWidget(parent) //G.Barrand_Qt4
#endif
{
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
#else
#if QT_VERSION < 0x050000 // pre Qt 5
  setObjectName(QString::fromAscii(name));
#else
  setObjectName(QString::fromLatin1(name));
#endif
#endif
  this->constructor(orientation);
}

void
SoQtThumbWheel::constructor(Orientation orientation)
{
  this->orient = orientation;
  this->state = SoQtThumbWheel::Idle;
  this->wheelValue = this->tempWheelValue = 0.0f;
  this->wheel = new SoAnyThumbWheel;
  this->wheel->setMovement(SoAnyThumbWheel::UNIFORM);
  this->wheel->setGraphicsByteOrder(SoAnyThumbWheel::ARGB);
  this->pixmaps = NULL;
  this->numPixmaps = 0;
  this->currentPixmap = -1;
}

SoQtThumbWheel::~SoQtThumbWheel()
{
  delete this->wheel;
  if (this->pixmaps) {
    for (int i = 0; i < this->numPixmaps; i++)
      delete this->pixmaps[i];
    delete [] this->pixmaps;
  }
}

void
SoQtThumbWheel::setOrientation(Orientation orientation)
{
  this->orient = orientation;
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
  this->repaint(FALSE);
#else
  this->repaint(); //G.Barrand_Qt4
#endif
}

void
SoQtThumbWheel::paintEvent(QPaintEvent * event)
{
  //G.Barrand : the below with Qt-4.4.2, it induces warnings :
  //  QPainter::begin : A paint device can only be paint by one painter at a time.
  //QPainter p(this);  //G.Barrand : displaced below
  QRect paintRect = event->rect();
  //p.setClipRect(paintRect); //G.Barrand : displaced below
  //G.Barrand_Qt4 QColorGroup g = this->colorGroup();

  int w, dval;
  if (this->orient == SoQtThumbWheel::Vertical) {
    w = this->width() - 12;
    dval = this->height() - 6;
  } else {
    w = this->height() - 12;
    dval = this->width() - 6;
  }

  // Handle resizing to too small dimensions gracefully.
  if ((dval <= 0) || (w <= 0)) return;

  this->initWheel(dval, w);

  int pixmap = this->wheel->getBitmapForValue(this->tempWheelValue,
                                              (this->state == SoQtThumbWheel::Disabled) ?
                                              SoAnyThumbWheel::DISABLED : SoAnyThumbWheel::ENABLED);

  QRect widgetrect(0, 0, this->width(), this->height());
  QRect wheelrect(widgetrect);

  if (this->orient == Vertical) {
    wheelrect.setTop(   wheelrect.top() + 2);
    wheelrect.setBottom(wheelrect.bottom() - 2);
    wheelrect.setLeft(  wheelrect.left() + 5);
    wheelrect.setRight( wheelrect.right() - 5);
  } else {
    wheelrect.setTop(   wheelrect.top() + 5);
    wheelrect.setBottom(wheelrect.bottom() - 5);
    wheelrect.setLeft(  wheelrect.left() + 2);
    wheelrect.setRight( wheelrect.right() - 2);
  }
  
 {QPainter p(this);  //G.Barrand.
  p.setClipRect(paintRect); //G.Barrand
  qDrawPlainRect(&p, wheelrect.left(), wheelrect.top(), wheelrect.width(),
                 wheelrect.height(), QColor(0, 0, 0), 1);}
    
  wheelrect.setTop(   wheelrect.top() + 1);
  wheelrect.setBottom(wheelrect.bottom() - 1);
  wheelrect.setLeft(  wheelrect.left() + 1);
  wheelrect.setRight( wheelrect.right() - 1);
  // wheelrect is now wheel-only

  if (this->orient == Vertical) {
    //bitBlt(this, wheelrect.left(), wheelrect.top(), this->pixmaps[pixmap],
    //       0, 0, w, dval);
    QPainter pt(this);
    pt.drawPixmap(QPoint(wheelrect.left(), wheelrect.top()),
                  *(this->pixmaps[pixmap]),
                  QRect(0, 0, w, dval));
  } else {
    //bitBlt(this, wheelrect.left(), wheelrect.top(), this->pixmaps[pixmap],
    //       0, 0, dval, w);
    QPainter pt(this);
    pt.drawPixmap(QPoint(wheelrect.left(), wheelrect.top()),
                  *(this->pixmaps[pixmap]),
                  QRect(0, 0, dval, w));
  }
  this->currentPixmap = pixmap;
}

/*!
  \internal
*/

void
SoQtThumbWheel::mousePressEvent(QMouseEvent * event)
{
  if (this->state != SoQtThumbWheel::Idle)
    return;

  if (event->button() != Qt::LeftButton)
    return;

  QRect wheelrect;
  if (this->orient == Vertical) {
    wheelrect.setLeft(SHADEBORDERWIDTH + 3);
    wheelrect.setTop(SHADEBORDERWIDTH + 6);
    wheelrect.setRight(this->width() - SHADEBORDERWIDTH - 3);
    wheelrect.setBottom(this->height() - SHADEBORDERWIDTH - 6);
  } else {
    wheelrect.setLeft(SHADEBORDERWIDTH + 6);
    wheelrect.setTop(SHADEBORDERWIDTH + 3);
    wheelrect.setRight(this->width() - SHADEBORDERWIDTH - 6);
    wheelrect.setBottom(this->height() - SHADEBORDERWIDTH - 3);
  }

  if (!wheelrect.contains(event->pos()))
    return;

  this->state = SoQtThumbWheel::Dragging;

  if (this->orient == SoQtThumbWheel::Vertical)
    this->mouseDownPos = event->pos().y() - SHADEBORDERWIDTH - 6;
  else
    this->mouseDownPos = event->pos().x() - SHADEBORDERWIDTH - 6;

  this->mouseLastPos = this->mouseDownPos;

  emit wheelPressed();
}

/*!
  \internal
*/

void
SoQtThumbWheel::mouseMoveEvent(QMouseEvent * event)
{
  if (this->state != SoQtThumbWheel::Dragging)
    return;

  if (this->orient == SoQtThumbWheel::Vertical)
    this->mouseLastPos = event->pos().y() - SHADEBORDERWIDTH - 6;
  else
    this->mouseLastPos = event->pos().x() - SHADEBORDERWIDTH - 6;
 

  this->tempWheelValue = this->wheel->calculateValue(this->wheelValue,
      this->mouseDownPos, this->mouseLastPos - this->mouseDownPos);

  emit wheelMoved(this->tempWheelValue);

#ifdef QT3_SUPPORT //G.Barrand_Qt4 
  this->repaint(FALSE);
#else
  this->repaint(); //G.Barrand_Qt4
#endif
}

/*!
  \internal
*/

void
SoQtThumbWheel::mouseReleaseEvent(QMouseEvent * event)
{
  if (this->state != SoQtThumbWheel::Dragging)
    return;

  if (event->button() != Qt::LeftButton)
    return;

  this->wheelValue = this->tempWheelValue;
  this->mouseLastPos = this->mouseDownPos;
  this->state = SoQtThumbWheel::Idle;
  emit wheelReleased();
}

/*
float
SoQtThumbWheel::getNormalizedValue(int pos) const
{
  int relativepos = pos - this->mouseDownPos;
  return (float) relativepos / (float)this->getWheelLength() * 2.0f;
}
*/

/*
int
SoQtThumbWheel::getWheelLength(void) const
{
  return this->orient == SoQtThumbWheel::Vertical ?
    this->height() : this->width();
}
*/

/*
int
SoQtThumbWheel::orientedCoord(const QPoint &p) const
{
  return (this->orient == SoQtThumbWheel::Horizontal) ?  p.x() : p.y();
}
*/

QSize
SoQtThumbWheel::sizeHint(void) const
{
  const int length = 122;
  int thick = 24;

  if (this->orient == SoQtThumbWheel::Horizontal)
    return QSize(length, thick);
  else
    return QSize(thick, length);
}

SoQtThumbWheel::Orientation
SoQtThumbWheel::orientation(void) const
{
  return this->orient;
}

float
SoQtThumbWheel::value(void) const
{
  return this->wheelValue;
}

// *************************************************************************

void
SoQtThumbWheel::initWheel(int diameter, int width)
{
  int dval, w;
  this->wheel->getSize(dval, w);
  if (dval == diameter && w == width) return;

  this->wheel->setSize(diameter, width);

  int pwidth = width;
  int pheight = diameter;
  if (this->orient == Horizontal) {
    pwidth = diameter;
    pheight = width;
  }

  if (this->pixmaps != NULL) {
    for (int i = 0; i < this->numPixmaps; i++)
      delete this->pixmaps[i];
    delete [] this->pixmaps;
  }

  this->numPixmaps = this->wheel->getNumBitmaps();
  this->pixmaps = new QPixmap * [this->numPixmaps];
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
  QImage image(pwidth, pheight, 32);
#else
  QImage image(pwidth, pheight, QImage::Format_RGB32); //G.Barrand_Qt4
#endif
  for (int i = 0; i < this->numPixmaps; i++) {
    this->wheel->drawBitmap(i, image.bits(), (this->orient == Vertical) ?
                            SoAnyThumbWheel::VERTICAL : SoAnyThumbWheel::HORIZONTAL);
    this->pixmaps[i] = new QPixmap(QSize(pwidth, pheight));

#ifdef QT3_SUPPORT //G.Barrand_Qt4 
    bool s = this->pixmaps[i]->convertFromImage(image);
#else
    QPixmap& pixmap = *(this->pixmaps[i]); //G.Barrand_Qt4
    pixmap = pixmap.fromImage(image,Qt::AutoColor); //G.Barrand_Qt4
    bool s = !pixmap.isNull(); //G.Barrand_Qt4
#endif
    if (!s && SOQT_DEBUG) {
      SoDebugError::post("SoQtThumbWheel::initWheel",
                         "Could not convert QImage to QPixmap, "
                         "for unknown reason.");
    }
  }
}

// *************************************************************************

void
SoQtThumbWheel::setEnabled(bool enable)
{
  if (enable)
    this->state = SoQtThumbWheel::Idle;
  else
    this->state = SoQtThumbWheel::Disabled;
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
  this->repaint(FALSE);
#else
  this->repaint(); //G.Barrand_Qt4
#endif
}

bool
SoQtThumbWheel::isEnabled(void) const
{
  return (this->state != SoQtThumbWheel::Disabled);
}

void
SoQtThumbWheel::setValue(float value)
{
  this->wheelValue = this->tempWheelValue = value;
  this->mouseDownPos = this->mouseLastPos;
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
  this->repaint(FALSE);
#else
  this->repaint(); //G.Barrand_Qt4
#endif
}

// *************************************************************************

void
SoQtThumbWheel::setRangeBoundaryHandling(boundaryHandling handling)
{
  switch (handling) {
  case CLAMP:
    this->wheel->setBoundaryHandling(SoAnyThumbWheel::CLAMP);
    break;
  case MODULATE:
    this->wheel->setBoundaryHandling(SoAnyThumbWheel::MODULATE);
    break;
  case ACCUMULATE:
    this->wheel->setBoundaryHandling(SoAnyThumbWheel::ACCUMULATE);
    break;
  default:
    assert(0 && "impossible");
  }
}

// *************************************************************************

SoQtThumbWheel::boundaryHandling
SoQtThumbWheel::getRangeBoundaryHandling(void) const
{
  switch (this->wheel->getBoundaryHandling()) {
  case SoAnyThumbWheel::CLAMP:
    return CLAMP;
  case SoAnyThumbWheel::MODULATE:
    return MODULATE;
  case SoAnyThumbWheel::ACCUMULATE:
    return ACCUMULATE;
  default:
    assert(0 && "impossible");
  }
  return CLAMP; // never reached
}

// *************************************************************************
