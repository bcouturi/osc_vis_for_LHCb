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

// Class documentation in common/SoGuiGLWidgetCommon.cpp.in.

// *************************************************************************

// FIXME: as far as I can see, this implementation doesn't support
// destructing and re-making the gl-widget if for instance the
// doublebuffer flag is flipped. That's an ugly limitation, which is
// not present in at least SoQtGLWidget. 20020612 mortene.

// *************************************************************************

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <assert.h>
#include <stdlib.h>

#include <X11/IntrinsicP.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#ifdef HAVE_LIBXMU
#include <X11/Xmu/StdCmap.h>
#endif // HAVE_LIBXMU
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/DrawingA.h>

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/misc/SoBasic.h>
#include <Inventor/SbViewportRegion.h>

#include <soxtdefs.h>
#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/SoXtResource.h>
#include <Inventor/Xt/widgets/SoXtGLArea.h>
#include <Inventor/Xt/SoXtGLWidget.h>
#include <Inventor/Xt/SoXtGLWidgetP.h>
#include <Inventor/Xt/SoAny.h>

#define PRIVATE(obj) ((obj)->pimpl)
#define PUBLIC(obj) ((obj)->pub)

// *************************************************************************

// The private data and code for the SoXtGLWidget.

#ifndef DOXYGEN_SKIP_THIS
SoXtGLWidgetP::SoXtGLWidgetP(SoXtGLWidget * w)
  : SoGuiGLWidgetP(w)
{
  this->glsize = SbVec2s(-1, -1);
  this->normalcolormapsize = 0;
  this->overlaycolormapsize = 0;
  this->transparentpixel = 0;
  this->border = FALSE;
  this->borderwidth = 2;
  this->glxwidget = NULL;
  this->glxmanager = NULL;
  this->normalcontext = NULL;
  this->normalvisual = NULL;
  this->overlaycontext = NULL;
  this->overlayvisual = NULL;
  this->doublebuffer = TRUE;
  this->enablealphachannel = FALSE;
  this->firstexpose = TRUE;
}

SoXtGLWidgetP::~SoXtGLWidgetP()
{
  if ( this->normalvisual ) {
    XFree(this->normalvisual);
  }
  // FIXME: if this is correct, overlayvisual should also be freed I guess
  // 20040901 larsa
}
#endif // DOXYGEN_SKIP_THIS

// *************************************************************************

SOXT_OBJECT_ABSTRACT_SOURCE(SoXtGLWidget);

// *************************************************************************

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
SoXtGLWidget::SoXtGLWidget(Widget const parent,
                           const char * const name,
                           const SbBool embed,
                           const int glModes,
                           const SbBool build)
  : inherited(parent, name, embed),
    waitForExpose(TRUE), drawToFrontBuffer(FALSE)
{
  PRIVATE(this) = new SoXtGLWidgetP(this);

  if (build) {
    Widget glarea = this->buildWidget(this->getParentWidget());
    this->setBaseWidget(glarea);
    XtVaSetValues(glarea,
                  XmNleftAttachment, XmATTACH_FORM,
                  XmNtopAttachment, XmATTACH_FORM,
                  XmNrightAttachment, XmATTACH_FORM,
                  XmNbottomAttachment, XmATTACH_FORM,
                  NULL);
  }
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
SoXtGLWidget::~SoXtGLWidget()
{
  this->unregisterWidget(PRIVATE(this)->glxmanager);
  this->unregisterWidget(PRIVATE(this)->glxwidget);

  if (PRIVATE(this)->normalcontext) {
    SoAny::si()->unregisterGLContext((void *)this);
  }
  delete PRIVATE(this);
}

// *************************************************************************

/*!
  This method returns the transparent pixel for the overlay planes.
*/

unsigned long
SoXtGLWidget::getOverlayTransparentPixel(void)
{
  return PRIVATE(this)->transparentpixel;
}

/*!
  This method returns the size of the colormap for the overlay planes.
*/

int
SoXtGLWidget::getOverlayColorMapSize(void)
{
  return PRIVATE(this)->overlaycolormapsize;
}

/*!
  This methos returns the size of the colormap for the normal planes.
*/

int
SoXtGLWidget::getColorMapSize(void)
{
  return PRIVATE(this)->normalcolormapsize;
}

/*!
  Returns the GL context for normal rendering.

  \sa getOverlayContext
*/

GLXContext
SoXtGLWidget::getNormalContext(void)
{
  return PRIVATE(this)->normalcontext;
}

/*!
  Returns the GL context for overlay rendering.

  \sa getNormalContext
*/

GLXContext
SoXtGLWidget::getOverlayContext(void)
{
  return PRIVATE(this)->overlaycontext;
}

/*!
  This method returns the Window associated with the normal GL context.
*/
Window
SoXtGLWidget::getNormalWindow(void)
{
  assert(PRIVATE(this)->glxwidget != (Widget) NULL);
  return XtWindow(PRIVATE(this)->glxwidget);
}

/*!
  This method returns the Window associated with the overlay GL context.
*/
Window
SoXtGLWidget::getOverlayWindow(void)
{
  return (Window) NULL;
}

/*!
  This method returns the Widget associated with the normal GL
  context.
*/
Widget
SoXtGLWidget::getNormalWidget(void) const
{
  return PRIVATE(this)->glxwidget;
}

/*!
  This method returns the Widget associated with the overlay GL
  context.
*/
Widget
SoXtGLWidget::getOverlayWidget(void) const
{
  return (Widget) NULL;
}

/*!
  This method sets the visual for the normal GL context.
*/

void
SoXtGLWidget::setNormalVisual(XVisualInfo * visual)
{
  PRIVATE(this)->normalvisual = visual;
}

/*!
  This method returns the visual for the normal GL context.
*/

XVisualInfo *
SoXtGLWidget::getNormalVisual(void)
{
  return PRIVATE(this)->normalvisual;
}

/*!
  This method sets the visual for the overlay GL context.
*/

void
SoXtGLWidget::setOverlayVisual(XVisualInfo * visual)
{
  PRIVATE(this)->overlayvisual = visual;
}

/*!
  This method returns the visual for the overlay GL context.
*/

XVisualInfo *
SoXtGLWidget::getOverlayVisual(void)
{
  return PRIVATE(this)->overlayvisual;
}

// FIXME: make doc in SoQtGLWidget valid for this also. 20011116 mortene.
void
SoXtGLWidget::setOverlayRender(const SbBool onoff)
{
  SOXT_STUB();
}

// FIXME: make doc in SoQtGLWidget valid for this also. 20011116 mortene.
SbBool
SoXtGLWidget::isOverlayRender(void) const
{
  SOXT_STUB();
  return FALSE;
}

/*!
  This method sets whether double buffering is to be used or not.
*/
void        // virtual
SoXtGLWidget::setDoubleBuffer(const SbBool enable)
{
  PRIVATE(this)->doublebuffer = enable;
}

/*!
  This method returns whether double buffering is used or not.
*/
SbBool
SoXtGLWidget::isDoubleBuffer(void) const
{
  return PRIVATE(this)->doublebuffer;
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
void
SoXtGLWidget::setBorder(const SbBool enable)
{
  PRIVATE(this)->border = enable;
  if (PRIVATE(this)->glxwidget != (Widget) NULL) {
    if (PRIVATE(this)->border == FALSE)
      XtVaSetValues(PRIVATE(this)->glxwidget,
                    XmNleftOffset, 0,
                    XmNtopOffset, 0,
                    XmNrightOffset, 0,
                    XmNbottomOffset, 0,
                    NULL);
    else
      XtVaSetValues(PRIVATE(this)->glxwidget,
                    XmNleftOffset, PRIVATE(this)->borderwidth,
                    XmNtopOffset, PRIVATE(this)->borderwidth,
                    XmNrightOffset, PRIVATE(this)->borderwidth,
                    XmNbottomOffset, PRIVATE(this)->borderwidth,
                    NULL);
  }
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
SbBool
SoXtGLWidget::isBorder(void) const
{
  return PRIVATE(this)->border;
}

/*!
  This method sets whether rendering will be done to the front buffer
  or not.
*/

void
SoXtGLWidget::setDrawToFrontBufferEnable(const SbBool enable)
{
  this->drawToFrontBuffer = enable;
}

/*!
  This method returns whether rendering will be done to the front buffer
  or not.
*/

SbBool
SoXtGLWidget::isDrawToFrontBufferEnable(void) const
{
  return this->drawToFrontBuffer;
}

/*!
  Enables or disables quad buffer stereo.
*/

/*!  
  Enables/disables the OpenGL accumulation buffer. Not implemented
  yet for this toolkit.  
*/
void 
SoXtGLWidget::setAccumulationBuffer(const SbBool enable)
{
  // FIXME: not implemented yet. 20020503 mortene.
  SOXT_STUB();
}

/*!
  Returns whether the OpenGL accumulation buffer is enabled.
*/
SbBool 
SoXtGLWidget::getAccumulationBuffer(void) const
{
  return FALSE;
}

/*!
  Enables/disables the OpenGL stencil buffer. Not implemented yet
  for this toolkit.
*/
void 
SoXtGLWidget::setStencilBuffer(const SbBool enable)
{
  // FIXME: not implemented yet. 20020503 mortene.
  SOXT_STUB();
}

/*!
  Returns whether the OpenGL stencil buffer is enabled.
*/
SbBool 
SoXtGLWidget::getStencilBuffer(void) const
{
  return FALSE;
}

/*!
  Enables/disables an alpha channel for the rendering context.
*/
void 
SoXtGLWidget::setAlphaChannel(const SbBool enable)
{

  SOXT_STUB();
  PRIVATE(this)->enablealphachannel = FALSE;

  // FIXME: As the GLX context is initialized at startup, changing the
  // 'enablealphachannel' flag will have no effect on the current
  // context. A proper destruction and reconstruction of the context
  // must be done here to make this work. (20031201 handegar)

}

/*!
  Returns whether the an alpha channel is enabled.
*/
SbBool 
SoXtGLWidget::getAlphaChannel(void) const
{
  SOXT_STUB();
  return PRIVATE(this)->enablealphachannel; // See fixme in 'setAlphaChannel'
}

void 
SoXtGLWidget::setQuadBufferStereo(const SbBool enable)
{
  // FIXME: do proper implementation. 20001123 mortene.
}

/*!
  Returns \c TRUE if quad buffer stereo is enabled for this widget.
*/
SbBool 
SoXtGLWidget::isQuadBufferStereo(void) const
{
  // FIXME: do proper implementation. 20001123 mortene.
  return FALSE;
}

/*!
  This method is invoked when the overlay graphics should be redrawn.
*/

void
SoXtGLWidget::redrawOverlay(void)
{
  SOXT_STUB();
}

/*!
  Any events from the native window system that goes to the OpenGL
  canvas gets piped through this method.

  It is overloaded in the subclasses to catch user interaction with
  the render canvas in the viewers, aswell as forwarding relevant
  events to the scenegraph.
*/
void
SoXtGLWidget::processEvent(XAnyEvent * event)
{
  switch (event->type) {
  case MapNotify:
    if ( !PRIVATE(this)->normalcontext ) {
      PRIVATE(this)->initNormalContext();
      this->initGraphic();
    }
    break;

  case Expose:
    if ( !PRIVATE(this)->normalcontext ) {
      PRIVATE(this)->initNormalContext();
      this->initGraphic();
    }
    this->waitForExpose = FALSE; // Gets flipped from TRUE on first expose.
    if (!this->glScheduleRedraw()) {
      this->redraw();
    }
    break;

  case ConfigureNotify:
    if (PRIVATE(this)->glxwidget != (Widget) NULL) {
      Dimension width, height;
      // XtVaGetValues(PRIVATE(this)->glxwidget,
      //               XmNwidth, &width, XmNheight, &height, NULL);
      XtVaGetValues(this->getWidget(),
                    XmNwidth, &width, XmNheight, &height, NULL);
      this->sizeChanged(SbVec2s(width, height));
    }
    break;

  } // switch (event->type)

}

/*!
  This method initializes the graphics.
*/

void
SoXtGLWidget::initGraphic(void)
{
  assert(PRIVATE(this)->glxwidget != (Widget) NULL);

  glLockNormal();
  Dimension width, height;
  XtVaGetValues(PRIVATE(this)->glxwidget, XmNwidth, &width, XmNheight, &height, NULL);
  PRIVATE(this)->glsize = SbVec2s(width, height);
  glEnable(GL_DEPTH_TEST);
  glUnlockNormal();
}


#ifndef DOXYGEN_SKIP_THIS
// private method that initializes the normal GL context  
void 
SoXtGLWidgetP::initNormalContext(void)
{
  assert(this->glxwidget != (Widget) NULL);
  XVisualInfo * visual;
  Display * display = SoXt::getDisplay();
  XtVaGetValues(this->glxwidget, SoXtNvisualInfo, &visual, NULL);
  int screen = DefaultScreen(display);

  SoXtGLWidget * share = (SoXtGLWidget *)
    SoAny::si()->getSharedGLContext((void *)display, (void *)(long)screen); //GB
    //GB SoAny::si()->getSharedGLContext((void *)display, (void *)screen);
  
  this->normalcontext =
    glXCreateContext(display, visual, 
                     share ? PRIVATE(share)->normalcontext : None, 
                     GL_TRUE);
  if (! this->normalcontext) {
    SoDebugError::postInfo("SoXtGLWidget::glInit",
                           "glXCreateContext() returned NULL");
    XtAppError(SoXt::getAppContext(), "no context");
  }
  else {
    SoAny::si()->registerGLContext((void *)PUBLIC(this), (void *)display, (void *)(long)screen);
    //GB SoAny::si()->registerGLContext((void *)PUBLIC(this), (void *)display, (void *)screen);
  }
}
#endif // DOXYGEN_SKIP_THIS


/*!
  This method initializes the overlay graphics.
*/

void
SoXtGLWidget::initOverlayGraphic(void)
{
  // should be empty
}

/*!
  Will be called whenever scene graph needs to be redrawn().
  If this method return FALSE, redraw() will be called immediately.

  Default method simply returns FALSE. Overload this method to
  schedule a redraw and return TRUE if you're trying to do The Right
  Thing.  
*/
SbBool 
SoXtGLWidget::glScheduleRedraw(void)
{
  return FALSE;
}

/*!
  Should return TRUE if an overlay GL drawing area exists.
*/
SbBool 
SoXtGLWidget::hasOverlayGLArea(void) const 
{
  return ((SoXtGLWidget*)this)->getOverlayWidget() != 0;
}

/*!
  Should return TRUE if a normal GL drawing area exists.
*/
SbBool 
SoXtGLWidget::hasNormalGLArea(void) const 
{
  return ((SoXtGLWidget*)this)->getNormalWidget() != 0;
}
 
// doc in super
void    // virtual, protected
SoXtGLWidget::sizeChanged(const SbVec2s & size)
{
  // SoDebugError::postInfo("SoXtGLWidget::sizeChanged", "[invoked (%d, %d)]",
  //   size[0], size[1]);
  if (this->isBorder()) {
    PRIVATE(this)->glsize[0] = size[0] - 2 * PRIVATE(this)->borderwidth;
    PRIVATE(this)->glsize[1] = size[1] - 2 * PRIVATE(this)->borderwidth;
  } else {
    PRIVATE(this)->glsize = size;
  }
  XtResizeWidget(PRIVATE(this)->glxwidget, PRIVATE(this)->glsize[0], PRIVATE(this)->glsize[1], 0);
}

/*!
  This method is invoked when a widget in the component is changed.
*/
void        // virtual, protected
SoXtGLWidget::widgetChanged(Widget widget)
{
  SOXT_STUB();
}

// *************************************************************************

/*!
  \fn void SoXtGLWidget::setGlxSize(const SbVec2s size)
  This method sets the Glx size.
  \sa setGLSize
*/

/*!
  This methid sets the size of the GL widget.
*/

void
SoXtGLWidget::setGLSize(const SbVec2s size)
{
  // SoDebugError::postInfo("SoXtGLWidget::setGLSize", "[invoked (%d, %d)]",
  //   size[0], size[1]);
  PRIVATE(this)->glsize = size;
  Dimension oldwidth, oldheight;
  XtVaGetValues(PRIVATE(this)->glxwidget,
                XmNwidth, &oldwidth, XmNheight, &oldheight, NULL);
  if ( PRIVATE(this)->glxwidget != (Widget) NULL &&
       (oldwidth != size[0] || oldheight != size[1]) ) {
    Dimension width = size[0];
    Dimension height = size[1];
    XtVaSetValues(PRIVATE(this)->glxwidget,
                  XmNwidth, width,
                  XmNheight, height,
                  NULL);
  }
}

/*!
  \fn const SbVec2s SoXtGLWidget::getGlxSize(void) const

  This method returns the size of the Glx area.

  \sa getGLSize
*/

/*!
  This method returns the size of the GL area.
*/
SbVec2s // protected
SoXtGLWidget::getGLSize(void) const
{
  return PRIVATE(this)->glsize;
}

/*!
  \fn float SoXtGLWidget::getGlxAspectRatio(void) const

  This is the old name for getGLAspectRatio.
  
  \sa getGLAspectRatio
*/

/*!
  This method returns the aspect ratio of the GL area.
*/

float
SoXtGLWidget::getGLAspectRatio(void) const
{
  assert(PRIVATE(this)->glxwidget != (Widget) NULL);
  Dimension width, height;
  XtVaGetValues(PRIVATE(this)->glxwidget, XmNwidth, &width, XmNheight, &height, NULL);
  return float(width) / float(height);
}

// *************************************************************************

/*!
  This method returns whether the GL context is in RGB mode or not.
*/

SbBool
SoXtGLWidget::isRGBMode(void)
{
  return TRUE;
}

/*!
  This method returns the share group for the display lists for the
  GL context.
*/

int
SoXtGLWidget::getDisplayListShareGroup(GLXContext context)
{
  SOXT_STUB();
  return 0;
}

// *************************************************************************

/*!
  This method is a callback that dispatches events to processEvent().

  \sa processEvent
*/

void
SoXtGLWidget::eventHandler(Widget widget,
                           SoXtGLWidget * closure,
                           XAnyEvent * event,
                           Boolean * dispatch)
{
  assert(closure != NULL);
  SoXtGLWidget * component = closure;
  component->processEvent((XAnyEvent *) event);
  *dispatch = False;
}

// NOTE: the strategy applied here for iterating through OpenGL canvas
// settings is exactly the same as the one applied in Coin's
// src/misc/SoOffscreenRenderer.cpp. So if you make any fixes or other
// improvements here, migrate your changes.
static int
buildGLAttrs(int * attrs, int trynum)
{
  int pos = 0;
  attrs[pos++] = GLX_RGBA;
  attrs[pos++] = GLX_DEPTH_SIZE;
  attrs[pos++] = 1;
  if (! (trynum & 0x04)) {
    attrs[pos++] = GLX_STENCIL_SIZE;
    attrs[pos++] = 1;
  }
  if (! (trynum & 0x02)) {
    attrs[pos++] = GLX_DOUBLEBUFFER;
  }
  if (! (trynum & 0x01)) {
    attrs[pos++] = GLX_RED_SIZE;
    attrs[pos++] = 4;
    attrs[pos++] = GLX_GREEN_SIZE;
    attrs[pos++] = 4;
    attrs[pos++] = GLX_BLUE_SIZE;
    attrs[pos++] = 4;
  }
  attrs[pos++] = None;
  return pos;
}


// Documented in common/SoGuiGLWidgetCommon.cpp.in.
Widget
SoXtGLWidget::buildWidget(Widget parent)
{
  // This method builds the GL widget inside \a parent.  The returned
  // widget is the widget managing the GL widget and providing it with
  // a border.

#if SOXT_DEBUG && 0
  SoDebugError::postInfo("SoXtGLWidget::buildWidget", "[enter]");
#endif // SOXT_DEBUG

  PRIVATE(this)->glxmanager =
    XtVaCreateManagedWidget("SoXtRenderArea",
                            xmFormWidgetClass, parent,
                            XmNleftAttachment, XmATTACH_FORM,
                            XmNtopAttachment, XmATTACH_FORM,
                            XmNrightAttachment, XmATTACH_FORM,
                            XmNbottomAttachment, XmATTACH_FORM,
                            NULL);
  this->registerWidget(PRIVATE(this)->glxmanager);

  SoXtResource rsc(PRIVATE(this)->glxmanager);

  short width = 0;
  if (rsc.getResource("borderThickness", XmRShort, width))
    PRIVATE(this)->borderwidth = width;

  SbBool haveborder = FALSE;
  if (rsc.getResource("border", XmRBoolean, haveborder))
    PRIVATE(this)->border = haveborder;

  Display * const display = SoXt::getDisplay();
  int trynum = 0;
  const int ARRAYSIZE = 32;
  int attrs[ARRAYSIZE];
  int screen = DefaultScreen(display);
  while (PRIVATE(this)->normalvisual == NULL && trynum < 8) {
    int arraysize = buildGLAttrs(attrs, trynum);
    assert(arraysize < ARRAYSIZE);
    PRIVATE(this)->normalvisual = glXChooseVisual(display, screen, attrs);
    trynum++;
  }

  if (PRIVATE(this)->normalvisual == NULL) {
    SoDebugError::post("SoXtGLWidget::buildWidget",
                       "could not get satisfactory visual for GLX");
    XtAppError(SoXt::getAppContext(), "SoXtGLWidget::buildWidget()");
  }

  PRIVATE(this)->doublebuffer = ((trynum-1) & 0x02) ? FALSE : TRUE;

  if ((PRIVATE(this)->normalvisual->c_class != TrueColor) &&
      (PRIVATE(this)->normalvisual->c_class != PseudoColor)) {
    SoDebugError::post("SoXtGLWidget::buildWidget",
                       "Visual hasn't the necessary color capabilities");
    XtAppError(SoXt::getAppContext(), "SoXtGLWidget::buildWidget()");
  }

#ifndef HAVE_LIBXMU
  SoDebugError::post("SoXtGLWidget::buildWidget",
                     "SoXt does not support detecting best visual/colormap without the Xmu library (yet)");
  exit(1);
#else // HAVE_LIBXMU

  Colormap colors = (Colormap) NULL;
  XStandardColormap * cmaps = NULL;
  int nmaps = 0;

  if (XmuLookupStandardColormap(
                                display, PRIVATE(this)->normalvisual->screen, PRIVATE(this)->normalvisual->visualid,
                                PRIVATE(this)->normalvisual->depth, XA_RGB_DEFAULT_MAP, False, True) &&
      XGetRGBColormaps(display,
                       RootWindow(display, PRIVATE(this)->normalvisual->screen), &cmaps, &nmaps,
                       XA_RGB_DEFAULT_MAP))
    {
      SbBool found = FALSE;
      for (int i = 0; i < nmaps && ! found; i++) {
        if (cmaps[i].visualid == PRIVATE(this)->normalvisual->visualid) {
#if SOXT_DEBUG && 0
          SoDebugError::postInfo("SoXtGLWidget::buildWidget",
                                 "got shared color map");
#endif // SOXT_DEBUG
          colors = cmaps[i].colormap;
          XFree(cmaps);
          found = TRUE;
        }
      }
      if (! found)
        colors = XCreateColormap(display,
                                 RootWindow(display, PRIVATE(this)->normalvisual->screen),
                                 PRIVATE(this)->normalvisual->visual, AllocNone);
    } else {
      colors = XCreateColormap(display,
                               RootWindow(display, PRIVATE(this)->normalvisual->screen),
                               PRIVATE(this)->normalvisual->visual, AllocNone);
    }
  
  PRIVATE(this)->glxwidget =
    XtVaCreateManagedWidget("SoXtGLWidget",
                            soxtGLAreaWidgetClass, PRIVATE(this)->glxmanager,
                            SoXtNvisualInfo, PRIVATE(this)->normalvisual,
                            XmNcolormap, colors,
                            SoXtNstencilSize, 1,
                            XmNleftAttachment, XmATTACH_FORM,
                            XmNtopAttachment, XmATTACH_FORM,
                            XmNrightAttachment, XmATTACH_FORM,
                            XmNbottomAttachment, XmATTACH_FORM,
                            NULL);
  this->registerWidget(PRIVATE(this)->glxwidget);  
  XtAddCallback(PRIVATE(this)->glxwidget, SoXtNexposeCallback,
                SoXtGLWidgetP::exposeCB, PRIVATE(this));
  
  this->setBorder(this->isBorder()); // "refresh" the widget offsets

  // Our callback has this signature:
  // (void (*)(_WidgetRec *, SoXtGLWidget *, XAnyEvent *, char *))
  // ..so we need to cast to avoid a compiler warning or error.
  XtAddEventHandler(PRIVATE(this)->glxwidget,
                    ExposureMask | StructureNotifyMask | ButtonPressMask |
                    ButtonReleaseMask | PointerMotionMask | KeyPressMask | KeyReleaseMask,
                    False,
                    (void (*)(_WidgetRec *, void *, _XEvent *, char *))
                    SoXtGLWidget::eventHandler, this);

#if SOXT_DEBUG && 0
  SoDebugError::postInfo("SoXtGLWidget::buildWidget", "[exit]");
#endif // SOXT_DEBUG
#endif // HAVE_LIBXMU
  return PRIVATE(this)->glxmanager;
}

/*!
  This method returns the widget that is managing the GL widget.
*/

Widget
SoXtGLWidget::getGlxMgrWidget(void)
{
  return PRIVATE(this)->glxmanager;
}

/*!
  This method returns the actual GL widget.
*/
Widget      // protected
SoXtGLWidget::getGLWidget(void) const
{
  return PRIVATE(this)->glxwidget;
}

// *************************************************************************

/*!
  This method locks the GL context.

  On systems that use GL context locking and unlocking, this method will
  lock the GL context.  On other systems, only makeCurrent will be run.

  This method is an SoXt extension.
*/

void
SoXtGLWidget::glLockNormal(void)
{
  assert(PRIVATE(this)->glxwidget != (Widget) NULL);
  if(glXMakeCurrent(SoXt::getDisplay(),
                    XtWindow(PRIVATE(this)->glxwidget),
		    PRIVATE(this)->normalcontext)==False) {
    //G.Barrand
    SoDebugError::postWarning("SoXtGLWidget::glLockNormal",
                              "glXMakeCurrent failed.");
  }
  //printf("debug : lock %lu\n",glGetString(GL_VERSION));
  //printf("debug : lock %s\n",glGetString(GL_VERSION));
}

/*!
  This method unlocks the GL context.
*/

void
SoXtGLWidget::glUnlockNormal(void)
{
#ifdef __APPLE__ //G.Barrand
  //Must be done on a Mac, else <Error> at exit of osc-plot :
  //  <Error>: kCGErrorIllegalArgument: CGSGetSurfaceBounds
  //  ...
  if(glXMakeCurrent(SoXt::getDisplay(), None,NULL)==False) {//G.Barrand
    //G.Barrand
    SoDebugError::postWarning("SoXtGLWidget::glUnlockNormal",
                              "glXMakeCurrent failed.");
  }
#else
  //G.Barrand : should do the upper. But problems on ubuntu.
  //            it induces that a glGetString(GL_VERSION)
  //            in cc_glglue_instance (or done afater a next
  //            glXMakeCurrent fails !
#endif
}

/*!
  This method locks the GL context.

  On systems that use GL context locking and unlocking, this method will
  lock the GL context.  On other systems, only makeCurrent will be run.

  This method is an SoXt extension.
*/

void
SoXtGLWidget::glLockOverlay(void)
{
}

/*!
  This method unlocks the GL context.
*/

void
SoXtGLWidget::glUnlockOverlay(void)
{
}

/*!
  This method swaps the GL buffers.
*/

void
SoXtGLWidget::glSwapBuffers(void)
{
  assert(PRIVATE(this)->doublebuffer != FALSE);
#if SOXT_DEBUG && 0
  SoDebugError::postInfo("SoXtGLWidget::glSwapBuffers", "called");
#endif // SOXT_DEBUG
  glXSwapBuffers(SoXt::getDisplay(), XtWindow(PRIVATE(this)->glxwidget));
}

/*!
  This method flushes the GL context.
*/

void
SoXtGLWidget::glFlushBuffer(void)
{
  assert(PRIVATE(this)->glxwidget != (Widget) NULL);
  // nothing to do...
  glFlush();
}

#ifndef DOXYGEN_SKIP_THIS
// Return a flag indicating whether or not OpenGL rendering is
// happening directly from the CPU(s) to the GPU(s), ie on a local
// display.
SbBool
SoXtGLWidgetP::isDirectRendering(void)
{
  PUBLIC(this)->glLockNormal();
  GLXContext ctx = glXGetCurrentContext();
  if (!ctx) {
    SoDebugError::postWarning("SoXtGLWidgetP::isDirectRendering",
                              "Could not get hold of current context.");
    return TRUE;
  }
  Bool isdirect = glXIsDirect(SoXt::getDisplay(), ctx);
  PUBLIC(this)->glUnlockNormal();
  return isdirect ? TRUE : FALSE;
}

// *************************************************************************

void
SoXtGLWidgetP::exposeCB(Widget widget, XtPointer closure, XtPointer call_data)
{
  SoXtGLWidgetP * thisp = (SoXtGLWidgetP *) closure;
  assert(thisp);
  Dimension width = 0, height = 0;
  XtVaGetValues(widget, XtNwidth, &width, XtNheight, &height, NULL);
  thisp->glsize = SbVec2s(width, height);
  if ( thisp->firstexpose ) {
    PUBLIC(thisp)->sizeChanged(SbVec2s(width, height));
    thisp->firstexpose = FALSE;
  }
}
#endif // DOXYGEN_SKIP_THIS

// *************************************************************************

#undef PRIVATE
#undef PUBLIC

