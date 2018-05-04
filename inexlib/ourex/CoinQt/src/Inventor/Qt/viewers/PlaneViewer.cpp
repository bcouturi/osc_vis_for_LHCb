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

// ************************************************************************

// Class is documented in common/viewers/SoGuiPlaneViewer.cpp.in.

// ************************************************************************

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

// FIXME: get rid of this define. We should fix up the compile issues
// wrt Qt 4 properly. 20050629 mortene.
//G.Barrand_Qt4 #define QT3_SUPPORT

#include <QtCore/qglobal.h> //G.Barrand

#if QT_VERSION < 0x050000 // pre Qt 5
#include <QtGui/qpushbutton.h> //G.Barrand QtGui
#else
#include <QtWidgets/qpushbutton.h>
#endif

#include <QtGui/qevent.h> //G.Barrand QtGui
#include <QtGui/qpixmap.h> //G.Barrand QtGui
#include <QtCore/qnamespace.h> //G.Barrand QtCore
#include <QtCore/qmetaobject.h> //G.Barrand QtCore

#ifdef HAVE_QSTYLEFACTORY_H
#include <qstylefactory.h>
#endif // HAVE_QSTYLEFACTORY_H

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoOrthographicCamera.h>

#include <Inventor/Qt/viewers/SoQtPlaneViewerP.h>
#include <Inventor/Qt/viewers/moc_SoQtPlaneViewerP.icc>

#include <soqtdefs.h>
#include <Inventor/Qt/widgets/SoQtThumbWheel.h>
#include <Inventor/Qt/viewers/SoQtPlaneViewer.h>
#include <Inventor/Qt/common/pixmaps/ortho.xpm>
#include <Inventor/Qt/common/pixmaps/perspective.xpm>
#include <Inventor/Qt/common/pixmaps/x.xpm>
#include <Inventor/Qt/common/pixmaps/y.xpm>
#include <Inventor/Qt/common/pixmaps/z.xpm>

// ************************************************************************

// Take care of namespace incompatibilities between Qt 3 and Qt 4.

#if QT_VERSION < 0x040000 // pre Qt 4
#define QTWIDGET_NOFOCUS QWidget::NoFocus
#else // Qt 4.0.0+
#define QTWIDGET_NOFOCUS Qt::NoFocus
#endif // Qt 4.0.0+

// ************************************************************************

#define PUBLIC(ptr) (ptr->pub)
#define PRIVATE(ptr) (ptr->pimpl)

// ************************************************************************

#ifdef QT3_SUPPORT //G.Barrand_Qt4 
#else
static void setPixmap(QAbstractButton& ab,const QPixmap &p) {
        ab.setIcon(QIcon(p));
        ab.setIconSize(p.size());
}
#endif

// Documented in superclass.
void
SoQtPlaneViewer::setCamera(SoCamera * camera)
{
  if (camera) {
    SoType type = camera->getTypeId();
    SbBool orthogonal =
      type.isDerivedFrom(SoOrthographicCamera::getClassTypeId());
    this->setRightWheelString(orthogonal ? "Zoom" : "Dolly");
    if (PRIVATE(this)->buttons.camera) {
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
      PRIVATE(this)->buttons.camera->setPixmap(orthogonal ?
        *(PRIVATE(this)->pixmaps.orthogonal) : *(PRIVATE(this)->pixmaps.perspective));
#else
      setPixmap(*(PRIVATE(this)->buttons.camera),orthogonal ?
        *(PRIVATE(this)->pixmaps.orthogonal) : *(PRIVATE(this)->pixmaps.perspective));
#endif
    }
  }
  inherited::setCamera(camera);
}

// ************************************************************************

// Documented in superclass.
QWidget *
SoQtPlaneViewer::buildWidget(QWidget * parent)
{
  return inherited::buildWidget(parent);
}

// ************************************************************************

// Documented in superclass. Overridden so we can append the X/Y/Z
// buttons and the camera type switch button in the rightside button
// column.
void
SoQtPlaneViewer::createViewerButtons(QWidget * parent,
                                     SbPList * buttons)
{
  inherited::createViewerButtons(parent, buttons);

  // add X, Y, Z viewpoint buttons
  PRIVATE(this)->buttons.x = new QPushButton(parent);
  PRIVATE(this)->buttons.x->setFocusPolicy(QTWIDGET_NOFOCUS);
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
  PRIVATE(this)->buttons.x->setToggleButton(FALSE);
  PRIVATE(this)->buttons.x->setPixmap(QPixmap((const char **) x_xpm));
#else
  PRIVATE(this)->buttons.x->setCheckable(FALSE);
  setPixmap(*(PRIVATE(this)->buttons.x),QPixmap((const char **) x_xpm));
#endif
  QObject::connect(PRIVATE(this)->buttons.x, SIGNAL(clicked()),
                   PRIVATE(this), SLOT(xClicked()));
  buttons->append(PRIVATE(this)->buttons.x);
  PRIVATE(this)->buttons.y = new QPushButton(parent);
  PRIVATE(this)->buttons.y->setFocusPolicy(QTWIDGET_NOFOCUS);
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
  PRIVATE(this)->buttons.y->setToggleButton(FALSE);
  PRIVATE(this)->buttons.y->setPixmap(QPixmap((const char **) y_xpm));
#else
  PRIVATE(this)->buttons.y->setCheckable(FALSE);
  setPixmap(*(PRIVATE(this)->buttons.y),QPixmap((const char **) y_xpm));
#endif
  QObject::connect(PRIVATE(this)->buttons.y, SIGNAL(clicked()),
                   PRIVATE(this), SLOT(yClicked()));
  buttons->append(PRIVATE(this)->buttons.y);
  PRIVATE(this)->buttons.z = new QPushButton(parent);
  PRIVATE(this)->buttons.z->setFocusPolicy(QTWIDGET_NOFOCUS);
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
  PRIVATE(this)->buttons.z->setToggleButton(FALSE);
  PRIVATE(this)->buttons.z->setPixmap(QPixmap((const char **) z_xpm));
#else
  PRIVATE(this)->buttons.z->setCheckable(FALSE);
  setPixmap(*(PRIVATE(this)->buttons.z),QPixmap((const char **) z_xpm));
#endif
  QObject::connect(PRIVATE(this)->buttons.z, SIGNAL(clicked()),
                   PRIVATE(this), SLOT(zClicked()));
  buttons->append(PRIVATE(this)->buttons.z);

  // add camera toggle button
  assert(PRIVATE(this)->pixmaps.perspective != NULL);
  assert(PRIVATE(this)->pixmaps.orthogonal != NULL);
  PRIVATE(this)->buttons.camera = new QPushButton(parent);
  PRIVATE(this)->buttons.camera->setFocusPolicy(QTWIDGET_NOFOCUS);

#if (defined Q_WS_MAC && QT_VERSION >= 0x030100) && defined(HAVE_QSTYLEFACTORY_H)
  // Since Qt/Mac 3.1.x, all pushbuttons (even those < 32x32) are
  // drawn using the Aqua style, i.e. with rounded edges and
  // shading. This looks really ugly in the viewer decoration. Drawing
  // the buttons in the Windows style gives us the flat, square
  // buttons we want.
  QStyle * style = QStyleFactory::create("windows");
  if (style) {
    PRIVATE(this)->buttons.x->setStyle(style);
    PRIVATE(this)->buttons.y->setStyle(style);
    PRIVATE(this)->buttons.z->setStyle(style);
    PRIVATE(this)->buttons.camera->setStyle(style);
  }
#endif

  QPixmap * pixmap = NULL;
  SoType t = this->getCameraType();
  if (t.isDerivedFrom(SoOrthographicCamera::getClassTypeId()))
    pixmap = PRIVATE(this)->pixmaps.orthogonal;
  else if (t.isDerivedFrom(SoPerspectiveCamera::getClassTypeId()))
    pixmap = PRIVATE(this)->pixmaps.perspective;
  else assert(0 && "unsupported cameratype");

#ifdef QT3_SUPPORT //G.Barrand_Qt4 
  PRIVATE(this)->buttons.camera->setPixmap(*pixmap);
#else
  setPixmap(*(PRIVATE(this)->buttons.camera),*pixmap);
#endif
  buttons->append(PRIVATE(this)->buttons.camera);

  QObject::connect(PRIVATE(this)->buttons.camera, SIGNAL(clicked()),
                   PRIVATE(this), SLOT(cameraToggleClicked()));

}

// ************************************************************************

// SoQtPlaneViewerP "private implementation" class.

#ifndef DOXYGEN_SKIP_THIS

SoQtPlaneViewerP::SoQtPlaneViewerP(SoQtPlaneViewer * publ)
  : SoGuiPlaneViewerP(publ)
{
  this->pixmaps.orthogonal = new QPixmap((const char **) ortho_xpm);
  this->pixmaps.perspective = new QPixmap((const char **) perspective_xpm);

  this->buttons.x = NULL;
  this->buttons.y = NULL;
  this->buttons.z = NULL;
  this->buttons.camera = NULL;
}

SoQtPlaneViewerP::~SoQtPlaneViewerP()
{
  delete this->pixmaps.orthogonal;
  delete this->pixmaps.perspective;
}

// Private constructor-code common for all public / protected
// constructors.
void
SoQtPlaneViewerP::constructor(SbBool buildnow)
{
  this->commonConstructor(); // init generic stuff

  PUBLIC(this)->setClassName("SoQtPlaneViewer");
  PUBLIC(this)->setLeftWheelString("transY");
  PUBLIC(this)->setBottomWheelString("transX");

  if (!buildnow) return;

  PUBLIC(this)->setSize(SbVec2s(550, 490)); // extra buttons -> more height
  QWidget * viewer = PUBLIC(this)->buildWidget(PUBLIC(this)->getParentWidget());
  PUBLIC(this)->setBaseWidget(viewer);
}

void
SoQtPlaneViewerP::xClicked(void)
{
  this->viewPlaneX();
}

void
SoQtPlaneViewerP::yClicked(void)
{
  this->viewPlaneY();
}

void
SoQtPlaneViewerP::zClicked(void)
{
  this->viewPlaneZ();
}

void
SoQtPlaneViewerP::cameraToggleClicked(void)
{
  PUBLIC(this)->toggleCameraType();
}

// ************************************************************************

#endif // DOXYGEN_SKIP_THIS

#undef PUBLIC
#undef PRIVATE

