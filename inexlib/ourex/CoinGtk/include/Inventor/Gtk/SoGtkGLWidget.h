/**************************************************************************
 *
 *  This file is part of the Coin GUI binding libraries.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
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

// 

#ifndef SOGTK_GLWIDGET_H
#define SOGTK_GLWIDGET_H

#include <Inventor/SbBasic.h>
#include <Inventor/Gtk/SoGtkBasic.h>
#include <Inventor/Gtk/SoGtkComponent.h>

#ifdef __COIN_SOQT__
#include <qgl.h>
#endif // __COIN_SOQT__
#ifdef __COIN_SOXT__
#include <GL/glx.h>
#endif // __COIN_SOXT__
#ifdef __COIN_SOGTK__
#include <gtk/gtk.h>
#endif // __COIN_SOGTK__
#ifdef __COIN_SOWIN__
#include <windows.h>
class SoWinGLArea;
#endif // __COIN_SOWIN__
#ifdef __COIN_SOMAC__
// WORK-IN-PROGRESS
#endif // __COIN_SOMAC__

enum GLModes {
  SO_GL_RGB      = 0x01, SO_GLX_RGB      = SO_GL_RGB,
  SO_GL_DOUBLE   = 0x02, SO_GLX_DOUBLE   = SO_GL_DOUBLE,
  SO_GL_ZBUFFER  = 0x04, SO_GLX_ZBUFFER  = SO_GL_ZBUFFER,
  SO_GL_OVERLAY  = 0x08, SO_GLX_OVERLAY  = SO_GL_OVERLAY,
  SO_GL_STEREO   = 0x10, SO_GLX_STEREO   = SO_GL_STEREO
};

// *************************************************************************

class SOGTK_DLL_API SoGtkGLWidget : public SoGtkComponent {
  SOGTK_OBJECT_ABSTRACT_HEADER(SoGtkGLWidget, SoGtkComponent);
  

public:

  void setBorder(const SbBool enable);
  SbBool isBorder(void) const;

  virtual void setDoubleBuffer(const SbBool enable);
  SbBool isDoubleBuffer(void) const;

  void setDrawToFrontBufferEnable(const SbBool enable);
  SbBool isDrawToFrontBufferEnable(void) const;
  
  void setQuadBufferStereo(const SbBool enable);
  SbBool isQuadBufferStereo(void) const;

  void setOverlayRender(const SbBool onoff);
  SbBool isOverlayRender(void) const;

  GtkWidget * getNormalWidget(void) const;
  GtkWidget * getOverlayWidget(void) const;

#ifdef __COIN_SOQT__
  const QGLContext * getNormalContext(void);
  const QGLContext * getOverlayContext(void);
#endif // __COIN_SOQT__
#ifdef __COIN_SOXT__
  Window getNormalWindow(void);
  Window getOverlayWindow(void);
  GLXContext getNormalContext(void);
  GLXContext getOverlayContext(void);

  int getOverlayColorMapSize(void);
  int getColorMapSize(void);

  virtual void setNormalVisual(XVisualInfo * visual);
  XVisualInfo * getNormalVisual(void);
  virtual void setOverlayVisual(XVisualInfo * visual);
  XVisualInfo * getOverlayVisual(void);
#endif // __COIN_SOXT__
#ifdef __COIN_SOWIN__
  HWND getNormalWindow(void) const;
  HWND getOverlayWindow(void) const;
  HDC getNormalDC(void) const;
  HDC getOverlayDC(void) const;
  HGLRC getNormalContext(void) const;
  HGLRC getOverlayContext(void) const;

  void setStealFocus(SbBool doStealFocus);

  virtual void setNormalVisual(PIXELFORMATDESCRIPTOR * vis);
  PIXELFORMATDESCRIPTOR * getNormalVisual(void);
  virtual void setOverlayVisual(PIXELFORMATDESCRIPTOR * vis);
  PIXELFORMATDESCRIPTOR * getOverlayVisual(void);
  virtual void setPixelFormat(int format);
  int getPixelFormat(void);
#endif // __COIN_SOWIN__
#ifdef __COIN_SOMAC__
  // WORK-IN-PROGRESS
#endif // __COIN_SOMAC__

  SbBool hasOverlayGLArea(void) const;
  SbBool hasNormalGLArea(void) const;

  unsigned long getOverlayTransparentPixel(void);

  // OpenGL query functions.
  void getPointSizeLimits(SbVec2f & range, float & granularity);
  void getLineWidthLimits(SbVec2f & range, float & granularity);


protected:
  SoGtkGLWidget(GtkWidget * const parent = NULL,
                  const char * const name = NULL,
                  const SbBool embed = TRUE,
                  const int glmodes = SO_GL_RGB,
                  const SbBool build = TRUE);
  ~SoGtkGLWidget();

  virtual void processEvent(GdkEvent * event);

  GtkWidget * buildWidget(GtkWidget * parent);

  // FIXME: this was public in SoXtGLWidget. 20011012 mortene.
  GtkWidget * getGLWidget(void) const;

  virtual void redraw(void) = 0;
  virtual void redrawOverlay(void);

  virtual void initGraphic(void);
  virtual void initOverlayGraphic(void);

  virtual void sizeChanged(const SbVec2s & size);
  virtual void widgetChanged(GtkWidget * w);

  void setGLSize(const SbVec2s size);
  SbVec2s getGLSize(void) const;
  float getGLAspectRatio(void) const;

  // old aliases
  void setGlxSize(const SbVec2s size) { this->setGLSize(size); }
  SbVec2s getGlxSize(void) const { return this->getGLSize(); }
  float getGlxAspectRatio(void) const { return this->getGLAspectRatio(); }
  void setStereoBuffer(SbBool flag) { this->setQuadBufferStereo(flag); }
  // FIXME: is this really correct? 20011012 mortene.
  SbBool isStereoBuffer(void) const { return this->isQuadBufferStereo(); }

  SbBool isRGBMode(void);

  SbBool waitForExpose;
  SbBool drawToFrontBuffer;

#ifdef __COIN_SOQT__
  virtual bool eventFilter(QObject * obj, QEvent * e);
  static void eventHandler(QWidget *, void *, QEvent *, bool *);
#endif // __COIN_SOQT__
#ifdef __COIN_SOXT__
  static void eventHandler(Widget, void *, XEvent *, Boolean *);
  int getDisplayListShareGroup(GLXContext context);
  Widget getGlxMgrWidget(void);
#endif // __COIN_SOXT__
#ifdef __COIN_SOGTK__
  virtual void afterRealizeHook(void);
  virtual SbBool eventFilter(GtkWidget * object, GdkEvent * event);
  static gint eventHandler(GtkWidget * object, GdkEvent * event, gpointer closure);
#endif // __COIN_SOGTK__
#ifdef __COIN_SOWIN__
  static LRESULT CALLBACK glWidgetProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
  static LRESULT CALLBACK mgrWidgetProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
  static LRESULT eventHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
  int getDisplayListShareGroup(HGLRC ctx);
  HWND getManagerWidget(void);
  HWND parent;
  HWND toplevel;
  SbBool makeNormalCurrent(void);
  SbBool swapNormalBuffers(void);
  SbBool swapOverlayBuffers(void);
#endif // __COIN_SOWIN__
#ifdef __COIN_SOMAC__
  // WORK-IN-PROGRESS
#endif // __COIN_SOMAC__

  void glLockNormal(void);
  void glUnlockNormal(void);

  void glLockOverlay(void);
  void glUnlockOverlay(void);

  void glSwapBuffers(void);
  void glFlushBuffer(void);

  virtual SbBool glScheduleRedraw(void);

#ifdef __COIN_SOQT__
private slots:
  void gl_init(void);
  void gl_reshape(int, int);
  void gl_exposed(void);
#endif // __COIN_SOQT__

private:
  class SoGtkGLWidgetP * pimpl;
  friend class SoGtkGLWidgetP; // FIXME: only used by So[Gtk|Win]GLWidgetP -- get rid of it? 20011012 mortene.

  void buildGLWidget(void); // FIXME: could we get rid of this? 20011012 mortene.

}; // class SoGtkGLWidget

// *************************************************************************

#endif // ! SOGTK_GLWIDGET_H
