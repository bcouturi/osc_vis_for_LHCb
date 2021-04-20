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

// *************************************************************************

// Class documentation in common/SoGuiCommon.cpp.in.

// *************************************************************************

// FIXME: share the relevant parts of the mainpage-doc with the other
// So* libraries. (Doxygen supports conditional inclusion / exclusion
// of doc parts.) 20020806 mortene.
//
// UPDATE 20030509 mortene: the mainpage documentation below is now
// duplicated for SoWin.

/*!
  \mainpage

  %SoQt is a library which provides the glue between Systems in
  Motion's \COIN high-level 3D visualization library and Troll Tech's
  \QT 2D user interface library.

  \QT is a C++ toolkit for multiplatform development of 2D user
  interfaces, and also includes other functionality to help
  programmers write multiplatform applications. \QT is currently
  available on X11-based systems (UNIX, Linux and BSDs), MSWindows,
  Mac OS X and embedded systems.

  For more information on the \QT toolkit, see the web site for Troll
  Tech (makers of \QT): <http://www.trolltech.com>.

  By using the combination of \COIN, \QT and %SoQt for your 3D
  applications, you have a framework for writing completely portable
  software across the whole range of UNIX, Linux, Microsoft Windows
  and Mac OS X operating systems. \COIN, \QT and %SoQt makes this
  possible from a 100% common codebase, which means there is a minimum
  of hassle for developers when working on multiplatform software,
  with the resulting large gains in productivity.

  %SoQt, like \COIN and \QT, provides the programmer with a high-level
  application programmer's interface (API) in C++. The library
  primarily includes a class-hierarchy of viewer components of varying
  functionality and complexity, with various modes for the end-user to
  control the 3D-scene camera interaction.

  For a small, completely stand-alone usage example on how to
  initialize the library and set up a viewer instance window, see the
  following code:

  \code
  #include <Inventor/Qt/SoQt.h>
  #include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
  #include <Inventor/nodes/SoBaseColor.h>
  #include <Inventor/nodes/SoCone.h>
  #include <Inventor/nodes/SoSeparator.h>
  
  int
  main(int argc, char ** argv)
  {
    // Initializes SoQt library (and implicitly also the Coin and Qt
    // libraries). Returns a top-level / shell Qt window to use.
    QWidget * mainwin = SoQt::init(argc, argv, argv[0]);
  
    // Make a dead simple scene graph by using the Coin library, only
    // containing a single yellow cone under the scenegraph root.
    SoSeparator * root = new SoSeparator;
    root->ref();

    SoBaseColor * col = new SoBaseColor;
    col->rgb = SbColor(1, 1, 0);
    root->addChild(col);

    root->addChild(new SoCone);
  
    // Use one of the convenient SoQt viewer classes.
    SoQtExaminerViewer * eviewer = new SoQtExaminerViewer(mainwin);
    eviewer->setSceneGraph(root);
    eviewer->show();
  
    // Pop up the main window.
    SoQt::show(mainwin);
    // Loop until exit.
    SoQt::mainLoop();

    // Clean up resources.
    delete eviewer;
    root->unref();

    return 0;
  }
  \endcode

  As compiled and run, this example provides the end-user with a full
  fledged 3D viewer. The viewer automatically contains mouse
  interaction handling logic to let the end-user "examine" the
  3D-model / scene (since this is the SoQtExaminerViewer class), plus
  toolbar controls on the right-side decorations border for often used
  controls:

  <center>
  <img src="http://doc.coin3d.org/images/SoLibs/general/sogui-class-example.png">
  </center>

  The %SoQt library contains several such high-level classes as the
  SoQtExaminerViewer used in the above example. These are primarily
  used for doing Rapid Application Development (RAD) of new concepts
  and ideas for your 3D system.  The "real" application will typically
  use one of the lower-complexity classes higher up in the inheritance
  hierarchy, such as the SoQtRenderArea, which provides the
  application programmer with full control over the user interface
  components and general layout to present for the end-user, as
  suitable for the specific application needs.

  This is how the %SoQt library fits in with the other system
  components:

  <center>
  <img src="http://doc.coin3d.org/images/SoLibs/mainpage/soqt-boxology.png">
  </center>

  As can be seen from the above figure, %SoQt builds on Systems in
  Motion's \COIN library for the 3D graphics, and Troll Tech's \QT
  library for the 2D user interface components and the OpenGL canvas
  binding.

  The additional functionality provided by %SoQt over \COIN and \QT
  is:

  <ul>

  <li>The most convenient management of OpenGL context types, such as
  singlebuffered versus doublebuffered rendering, the use of overlay
  planes, stereo rendering, etc. This is handled through the
  SoQtGLWidget class, which builds on Qt's QGLWidget class, and
  through the SoQtRenderArea class (which contains the main binding
  into the \COIN library's main data structures).</li>

  <li>The translation of native \QT interaction device events (from
  e.g. the mouse or the keyboard) into the \COIN library's event
  types. The translation is done by the SoQtDevice classes, controlled
  by the SoQtRenderArea.

  These "generic" \COIN events are then passed into the 3D scenegraph
  for further processing, for instance by \COIN's 3D user interaction
  components -- like this "trackball manipulator" attached to a simple
  cone:

  <center>
  <img src="http://doc.coin3d.org/images/Coin/draggers/trackball-cone.png">
  </center>
  </li>

  <li>Some abstract viewer classes, like the SoQtViewer and
  SoQtFullViewer, which provides additional services on top of the
  SoQtRenderArea for assisting the application programmer in
  convenient handling of cameras and lightsources in the 3D scene (by
  the SoQtViewer), plus adding the basic, common user interface
  components (by the SoQtFullViewer).</li>

  <li>A set of high-level viewer classes, as has been presented by the
  SoQtExaminerViewer in the above source code example. There are
  currently three different non-abstract viewer classes to choose
  from: the SoQtExaminerViewer (a plain model viewer), the
  SoQtFlyViewer (for fly-throughs in larger 3D scenes) and the
  SoQtPlaneViewer (for CAD-style viewing and interaction along the 3
  principal axes).</li>

  </ul>

  The %SoQt library is "dual-licensed", which means it's available
  either under a Free Software license (specifically the <a
  href="http://www.fsf.org/copyleft/gpl.html">GNU General Public
  License</a>), or a license better suited for the development of
  proprietary / commercial applications: our <a
  href="http://www.coin3d.org">Coin Professional Edition License</a>.

  Note that to use %SoQt under the GNU General Public License, you
  have to comply with that license's restrictions. These restrictions
  are \e not well suited for the development of non-Free software, so
  the availability of %SoQt under the GNU GPL is primarily meant to be
  a service to the Free Software community. We \e strongly advise you
  to invest in the Coin Professional Edition License for using %SoQt
  in proprietary development projects. As a Coin Professional Edition
  License holder, you gain the rights to use the %SoQt library in just
  about any way you like. See the Coin web site at
  <http://www.coin3d.org> for more information on the advantages of
  the Coin Professional Edition License, and how to become a license
  holder.

  For those who are using the implementations of the Inventor API from
  either SGI or TGS, we would like to point out that %SoQt can also be
  used on top of either of those libraries instead of the \COIN
  library from Systems in Motion.

  The %SoQt API is based on and closely matches the InventorXt library
  API, originally developed by SGI. This should make it
  straigthforward to port InventorXt code over to %SoQt, for instance
  to gain greater portability.

  \sa The documentation for the \COIN library: <http://doc.coin3d.org/Coin>.
*/

// *************************************************************************


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

// FIXME: get rid of this define. We should fix up the compile issues
// wrt Qt 4 properly. 20050629 mortene.
//G.Barrand_Qt4 #define QT3_SUPPORT

#include <stdlib.h>
#include <limits.h>

#include <QtCore/qglobal.h> //G.Barrand

#if QT_VERSION < 0x050000 // pre Qt 5
#include <QtGui/qmainwindow.h> //G.Barrand QtGui
#include <QtGui/qmessagebox.h> //G.Barrand QtGui
#include <QtGui/qapplication.h> //G.Barrand QtGui
#else
#include <QtWidgets/qmainwindow.h>
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qapplication.h>
#endif

#include <QtCore/qtimer.h> //G.Barrand QtCore
#include <QtGui/qevent.h> //G.Barrand QtGui
#include <QtCore/qmetaobject.h> //G.Barrand QtCore

#ifdef Q_WS_X11
#include <X11/Xlib.h>
#if QT_VERSION < 0x040000
#else
#include <QtGui/qx11info_x11.h> // for QX11Info
#endif
#endif // Q_WS_X11

#include <Inventor/SoDB.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/nodekits/SoNodeKit.h>
#include <Inventor/SbTime.h>
#include <Inventor/errors/SoDebugError.h>

#include <Inventor/Qt/SoQtP.h>
#include <Inventor/Qt/moc_SoQtP.icc>

#include <Inventor/Qt/SoQt.h>
#include <Inventor/Qt/SoQtObject.h>
#include <Inventor/Qt/devices/SoQtDevice.h>
/* The setting of this define needs to be added manually to
   configure.in for all relevant projects. */
#ifdef HAVE_X11_AVAILABLE
#include <Inventor/Qt/devices/spwinput.h>
#include <X11/Xlib.h>
#endif // HAVE_X11_AVAILABLE

#ifdef HAVE_WIN32_API
#include <windows.h>
#include <Inventor/Qt/devices/spwinput_win32.h>
#endif // HAVE_WIN32_API


#include <Inventor/Qt/SoQtComponent.h>
#include <Inventor/Qt/SoAny.h>
#include <Inventor/Qt/SoQtInternal.h>

#include <Inventor/Qt/nodes/SoGuiNodes.h>
#include <Inventor/Qt/engines/SoGuiEngines.h>

#include <soqtdefs.h>


// *************************************************************************

// The private data for the SoQt class.

#ifndef DOXYGEN_SKIP_THIS // Skip internal classes SoQtP and SoQtApplication.

QWidget * SoQtP::mainwidget = NULL;
QApplication * SoQtP::appobject = NULL;
SbBool SoQtP::madeappobject = FALSE;
QTimer * SoQtP::idletimer = NULL;
QTimer * SoQtP::timerqueuetimer = NULL;
QTimer * SoQtP::delaytimeouttimer = NULL;
SoQtP * SoQtP::slotobj = NULL;
bool SoQtP::didcreatemainwidget = FALSE;

#ifdef HAVE_QT_WIN_USE_SIMPLE_TIMERS
// Qt 3.3 seems to use simple timers by default. Since simple timers
// causes idle sensors to trigger only 100 times per second, we set
// this variable to false in SoQt::init(). pederb, 2004-07-21

// The global qt_win_use_simple_timers variable is at least defined in
// Qt 3.0.5 and 3.3.1. Reading the Qt interest archive, it seems like
// it's ok for application code to modify it though.
Q_EXPORT extern bool qt_win_use_simple_timers;
#endif // HAVE_QT_WIN_USE_SIMPLE_TIMERS

static void soqt_reset_simple_timers(void)
{
#ifdef HAVE_QT_WIN_USE_SIMPLE_TIMERS
  const char * env = SoAny::si()->getenv("SOQT_USE_SIMPLE_TIMERS");
  if (env) {
    qt_win_use_simple_timers = (bool) atoi(env);
  }
  else {
    qt_win_use_simple_timers = false;
  }
#endif // HAVE_QT_WIN_USE_SIMPLE_TIMERS
}

#define ENVVAR_NOT_INITED INT_MAX

int SoQtP::DEBUG_X11SYNC = ENVVAR_NOT_INITED;
const char * SoQtP::SOQT_XSYNC = "SOQT_XSYNC";
int SoQtP::BRIL_X11_SILENCER = ENVVAR_NOT_INITED;
int SoQtP::X11_ERRORHANDLER = ENVVAR_NOT_INITED;
const char * SoQtP::SOQT_X11_ERRORHANDLER = "SOQT_X11_ERRORHANDLER";
SoQtP_XErrorHandler * SoQtP::previous_handler = NULL;

int SoQtP::DEBUG_LISTMODULES = ENVVAR_NOT_INITED;

// We're using the singleton pattern to create a single SoQtP object
// instance (a dynamic object is needed for attaching slots to signals
// -- this is really a workaround for some silliness in the Qt
// design).
SoQtP *
SoQtP::soqt_instance(void)
{
  if (!SoQtP::slotobj) { SoQtP::slotobj = new SoQtP; }
  return SoQtP::slotobj;
}

int
SoQtP::X11Errorhandler(void * d, void * ee)
{
#if defined(Q_WS_X11)

  // This is a hack provided for one of our Coin PEL holders, to
  // silence X11 error output from some erroneous Qt code.
  if (SoQtP::BRIL_X11_SILENCER == ENVVAR_NOT_INITED) {
    const char * env = SoAny::si()->getenv("SOQT_BRIL_X11_SILENCER_HACK");
    SoQtP::BRIL_X11_SILENCER = env ? atoi(env) : 0;
  }
  if (SoQtP::BRIL_X11_SILENCER) { return 0; }


  // Include misc information on the Display to aid further debugging
  // on our end upon bugreports.

  SbString depthsstr = "";
  int num;
  int * depths = XListDepths((Display *)d, DefaultScreen(d), &num);
  if ((depths != NULL) && (num > 0)) {
    depthsstr = "(Available Display depths are:";
    for (int i=0; i < num; i++) {
      depthsstr += ' ';
      depthsstr += SbString(depths[i]);
    }
    depthsstr += ". Default depth is ";
    depthsstr += SbString(DefaultDepth(d, DefaultScreen(d)));
    depthsstr += ".)";
    XFree(depths);
  }

  // Then the instructions:

  SbString instructions = "";
  if (! SoQtP::DEBUG_X11SYNC) {
#ifdef __COIN__
    instructions.sprintf("Set environment variable %s to \"1\" and "
                         "re-run the application in a debugger with a "
                         "breakpoint set on SoQtP::X11Errorhandler() to get a "
                         "valid backtrace. "
                     "Then please forward the following information in an "
                     "e-mail to <coin-bugs@coin3d.org> along with the "
                     "backtrace. ",
                         SoQtP::SOQT_XSYNC);
#else // __COIN__
    instructions = "Set environment variable SOQT_XSYNC to \"1\" and "
                         "re-run the application in a debugger with a "
                         "breakpoint set on SoQtP::X11Errorhandler() to get a "
                         "valid backtrace. "
                     "Then please forward the following information in an "
                     "e-mail to <coin-bugs@coin3d.org> along with the "
                     "backtrace. ";
#endif // ! __COIN__
  }

  SoDebugError::post("SoQtP::X11Errorhandler",
                     "Detected probable Qt bug (or internal SoQt bug). %s %s",
                     instructions.getString(), depthsstr.getString());

  SoDebugError::post("SoQtP::X11Errorhandler",
                     "If you don't want SoQt to catch X11 errors, set the %s "
                     "environment variable to \"0\".",
                     SoQtP::SOQT_X11_ERRORHANDLER);

  SoQtP::previous_handler(d, ee);
#endif // Q_WS_X11
  return -1; // shouldn't get here, the system handler will normally exit
}

// A timer sensor is ready for triggering, so tell the sensor manager
// object to process the queue.
void
SoQtP::slot_timedOutSensor()
{
#if SOQT_DEBUG && 0
    SoDebugError::postInfo("SoQt::timedOutSensor",
                           "processing timer queue");
    SoDebugError::postInfo("SoQt::timedOutSensor",
                           "is %s",
                           SoQtP::delaytimeouttimer->isActive() ?
                           "active" : "inactive");
#endif

  SoDB::getSensorManager()->processTimerQueue();

  // The change callback is _not_ called automatically from
  // SoSensorManager after the process methods, so we need to
  // explicitly trigger it ourselves here.
  SoGuiP::sensorQueueChanged(NULL);
}

#ifdef QT3_SUPPORT //G.Barrand_Qt4 
#else
static void start(QTimer& timer,int msec, bool sshot) {
  timer.setSingleShot(sshot);
  timer.start(msec);
}
#endif

// The system is idle, so we're going to process the queue of delay
// type sensors.
void
SoQtP::slot_idleSensor()
{
#if defined(HAVE_QAPPLICATION_HASPENDINGEVENTS) && 0 // disabled, too much debug
#if SOQT_DEBUG
    SoDebugError::postInfo("SoQt::idleSensor",
                           "processing delay queue, "
                           "QApplication::hasPendingEvents()==%d",
                           SoQtP::appobject->hasPendingEvents());
    SoDebugError::postInfo("SoQt::idleSensor", "is %s",
                           SoQtP::idletimer->isActive() ? "active" : "inactive");
#endif
#endif // HAVE_QAPPLICATION_HASPENDINGEVENTS

#ifdef HAVE_QAPPLICATION_HASPENDINGEVENTS  
  // If there are still events in the Qt event queue, push the
  // idle-timer at the end of the queue again.
  if (SoQtP::appobject->hasPendingEvents()) {
    if (SoQtP::idletimer->isActive()) SoQtP::idletimer->stop();
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
    SoQtP::idletimer->start(0, TRUE);
#else
    start(*(SoQtP::idletimer),0, TRUE);
#endif
    return;
  }
#endif // HAVE_QAPPLICATION_HASPENDINGEVENTS

  SoDB::getSensorManager()->processTimerQueue();
  SoDB::getSensorManager()->processDelayQueue(TRUE);

  // The change callback is _not_ called automatically from
  // SoSensorManager after the process methods, so we need to
  // explicitly trigger it ourselves here.
  SoGuiP::sensorQueueChanged(NULL);
}

// The delay sensor timeout point has been reached, so process the
// delay queue even though the system is not idle (to avoid
// starvation).
void
SoQtP::slot_delaytimeoutSensor()
{
#if SOQT_DEBUG && 0
    SoDebugError::postInfo("SoQt::delaytimeoutSensor",
                           "processing delay queue");
    SoDebugError::postInfo("SoQtP::delaytimeouttimer", "is %s",
                           SoQtP::delaytimeouttimer->isActive() ?
                           "active" : "inactive");
#endif

  SoDB::getSensorManager()->processTimerQueue();
  SoDB::getSensorManager()->processDelayQueue(FALSE);

  // The change callback is _not_ called automatically from
  // SoSensorManager after the process methods, so we need to
  // explicitly trigger it ourselves here.
  SoGuiP::sensorQueueChanged(NULL);
}

// This function gets called whenever something has happened to any of
// the sensor queues. It starts or reschedules a timer which will
// trigger when a sensor is ripe for plucking.
void
SoGuiP::sensorQueueChanged(void *)
{
  // We need three different mechanisms to interface Coin sensor
  // handling with Qt event handling, which are:
  //
  // 1. Detect when the application is idle and then empty the
  // delay-queue completely for delay-sensors -- handled by
  // SoQtP::idletimer.
  //
  // 2. Detect when one or more timer-sensors are ripe and trigger
  // those -- handled by SoQtP::timerqueuetimer.
  //
  // 3. On the "delay-sensor timeout interval", empty all highest
  // priority delay-queue sensors to avoid complete starvation in
  // continually busy applications -- handled by
  // SoQtP::delaytimeouttimer.


  // Allocate Qt timers on first call.

  if (!SoQtP::timerqueuetimer) {
    SoQtP::timerqueuetimer = new QTimer;
    QObject::connect(SoQtP::timerqueuetimer, SIGNAL(timeout()),
                     SoQtP::soqt_instance(), SLOT(slot_timedOutSensor()));
    SoQtP::idletimer = new QTimer;
    QObject::connect(SoQtP::idletimer, SIGNAL(timeout()),
                     SoQtP::soqt_instance(), SLOT(slot_idleSensor()));
    SoQtP::delaytimeouttimer = new QTimer;
    QObject::connect(SoQtP::delaytimeouttimer, SIGNAL(timeout()),
                     SoQtP::soqt_instance(), SLOT(slot_delaytimeoutSensor()));
  }

  SoSensorManager * sm = SoDB::getSensorManager();

  // Set up timer queue timeout if necessary.

  SbTime t;
  if (sm->isTimerSensorPending(t)) {
    SbTime interval = t - SbTime::getTimeOfDay();
    // Qt v2.1.1 (at least) on MSWindows will fail to trigger the
    // timer if the interval is < 0.0.
    //
    // We also want to avoid setting it to 0.0, as that has a special
    // semantic meaning: trigger only when the application is idle and
    // event queue is empty -- which is not what we want to do here.
    //
    // So we clamp it, to a small positive value:
    if (interval.getValue() <= 0.0) { interval.setValue(1.0/5000.0); }

#if SOQT_DEBUG && 0
      SoDebugError::postInfo("SoQt::sensorQueueChanged",
                             "timersensor pending, interval %f",
                             interval.getValue());
#endif

    // Change interval of timerqueuetimer when head node of the
    // timer-sensor queue of SoSensorManager changes. 
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
    if (!SoQtP::timerqueuetimer->isActive())
      SoQtP::timerqueuetimer->start((int)interval.getMsecValue(), TRUE);
    else
      SoQtP::timerqueuetimer->changeInterval((int)interval.getMsecValue());
#else
    if (!SoQtP::timerqueuetimer->isActive())
      start(*(SoQtP::timerqueuetimer),(int)interval.getMsecValue(), TRUE);
    else
      SoQtP::timerqueuetimer->start((int)interval.getMsecValue());
#endif
  }
  // Stop timerqueuetimer if queue is completely empty.
  else if (SoQtP::timerqueuetimer->isActive()) {
    SoQtP::timerqueuetimer->stop();
  }


  // Set up idle notification for delay queue processing if necessary.

  if (sm->isDelaySensorPending()) {
#if SOQT_DEBUG && 0
      SoDebugError::postInfo("SoQt::sensorQueueChanged",
                             "delaysensor pending");
#endif

    // Start idletimer at 0 seconds in the future. -- That means it will 
    // trigger when the Qt event queue has been run through, i.e. when 
    // the application is idle.
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
    if (!SoQtP::idletimer->isActive()) SoQtP::idletimer->start(0, TRUE);
#else
    if (!SoQtP::idletimer->isActive()) start(*(SoQtP::idletimer),0, TRUE);
#endif

    if (!SoQtP::delaytimeouttimer->isActive()) {
      const SbTime & t = SoDB::getDelaySensorTimeout();
      if (t != SbTime::zero()) {
        unsigned long timeout = t.getMsecValue();
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
        SoQtP::delaytimeouttimer->start((int)timeout, TRUE);
#else
        start(*(SoQtP::delaytimeouttimer),(int)timeout, TRUE);
#endif
      }
    }
  }
  else {
    if (SoQtP::idletimer->isActive()) SoQtP::idletimer->stop();
    if (SoQtP::delaytimeouttimer->isActive()) SoQtP::delaytimeouttimer->stop();
  }
}

// *************************************************************************

// We overload the QApplication class to be able to get hold of the
// X11 events directly. (This is necessary to handle Spacetec
// spaceball devices.)
class SoQtApplication : public QApplication {
public:
  SoQtApplication(int& argc, char ** argv) : QApplication(argc, argv) { 
#ifdef HAVE_X11_AVAILABLE
    this->display = NULL;
#endif // HAVE_X11_AVAILABLE
  }
#ifdef HAVE_X11_AVAILABLE
  virtual bool x11EventFilter(XEvent * e) {
    SPW_InputEvent sbEvent;
    if (SPW_TranslateEventX11(this->getDisplay(), e, &sbEvent)) {
      QWidget * focus = this->focusWidget();
      if (!focus) focus = this->activeWindow();
      if (focus) {
        QCustomEvent qevent((QEvent::Type)SoQtInternal::SPACEBALL_EVENT,
                            (void *)&sbEvent);
        QApplication::sendEvent(focus, &qevent);
      }
    }
    return QApplication::x11EventFilter(e);
  }
  Display * getDisplay(void) {
    QWidget * topw = SoQt::getTopLevelWidget();
    if (topw) return topw->x11Display();
    if (this->display == NULL) {
      // Keep a single display-ptr.
      // 
      // This resource is never deallocated explicitly (but of course
      // implicitly by the system on application close-down). This to
      // work around some strange problems with the NVidia-driver 29.60
      // on XFree86 v4 when using XCloseDisplay() -- like doublebuffered
      // visuals not working correctly.
      //
      // Also, XCloseDisplay() has been known to cause crashes when
      // running remotely from some old Mesa version on Red Hat Linux
      // 6.2 onto an IRIX6.5 display. It seems likely that this was
      // caused by a bug in that particular old Mesa version.
      //
      // mortene@sim.no
      this->display = XOpenDisplay(NULL);
    }
    return this->display;
  }
  Display * display;
#endif // HAVE_X11_AVAILABLE

#ifdef HAVE_WIN32_API
  virtual bool winEventFilter(MSG * msg) {
    SPW_InputEvent sbEvent;
    if (SPW_TranslateEventWin32(msg, &sbEvent)) {
      QWidget * focus = this->focusWidget();
      if (!focus) focus = this->activeWindow();
      if (focus) {
        QCustomEvent qevent((QEvent::Type)SoQtInternal::SPACEBALL_EVENT,
                            (void *)&sbEvent);
        QApplication::sendEvent(focus, &qevent);
      }
    }
#if QT_VERSION < 0x030000
    return QApplication::winEventFilter(msg);
#else
    long result = 0;
    return QApplication::winEventFilter(msg, &result);
#endif
  }
#endif // HAVE_WIN32_API
};

#endif // DOXYGEN_SKIP_THIS

// *************************************************************************

#ifdef COIN_IV_EXTENSIONS
#define COIN_IV_EXTENSION(ext)  \
class ext {                     \
public:                         \
static void initClass(void);    \
};                              \

COIN_IV_EXTENSIONS
#undef COIN_IV_EXTENSION
#endif

// documented in common/SoGuiCommon.cpp.in
void
SoQt::init(QWidget * toplevelwidget)
{
  // workaround for Qt bug under Windows. See above for comments
  soqt_reset_simple_timers();

  // "qApp" is a global variable from the Qt library, pointing to the
  // single QApplication instance in a Qt-application.
  if (qApp == NULL) {
    // Set up the QApplication instance which we have derived into a
    // subclass to catch spaceball events.

    // use a static char array to store the dummy argv parameters
    static char * dummyargv[1];
    dummyargv[0] = (char*)"SoQt"; //G.Barrand : ?
    static int argc = 1;
    SoQtP::appobject = new SoQtApplication(argc, (char **) dummyargv);
    SoQtP::madeappobject = TRUE;
  }
  else {
    // The user already set one up for us.
    //
    // FIXME: should somehow warn about the fact that spaceball events
    // will not be caught, if a spaceball is attempted used. 20020619 mortene.
    SoQtP::appobject = qApp;
  }

  // This init()-method is called by the other 2 init()'s, so place
  // common code here.

  // Must do this here so SoDebugError is initialized before it could
  // be attempted used.
  if (!SoDB::isInitialized()) { SoDB::init(); }
  SoNodeKit::init();
  SoInteraction::init();

  if (SOQT_DEBUG && SoQtP::mainwidget) {
    SoDebugError::postWarning("SoQt::init",
                              "This method should be called only once.");
    return;
  }

  SoQtObject::init();
#ifdef COIN_IV_EXTENSIONS
#define COIN_IV_EXTENSION(ext) ext::initClass();
  COIN_IV_EXTENSIONS
#undef COIN_IV_EXTENSION
#endif

  SoGuiNodes::initClasses();
  SoGuiEngines::initClasses();

  // This is _extremely_ useful for debugging X errors: activate this
  // code (set the SOQT_XSYNC environment variable on your system to
  // "1") and SOQT_X11_ERRORHANDLER to "1", then rerun the application
  // code in a debugger with a breakpoint set at
  // SoQtP::X11Errorhandler(). Now you can backtrace to the exact
  // source location of the failing X request.
  //
  // Note that we changed this from being opt-out to opt-in, due to
  // what seemed like false negatives, and the fact that bad code from
  // Qt would look like SoQt bugs.
#ifdef Q_WS_X11
  if (SoQtP::X11_ERRORHANDLER == ENVVAR_NOT_INITED) {
    const char * env = SoAny::si()->getenv(SoQtP::SOQT_X11_ERRORHANDLER);
    SoQtP::X11_ERRORHANDLER = env ? atoi(env) : 0;
  }
      
  if (SoQtP::X11_ERRORHANDLER) {
    // Intervene upon X11 errors.
    SoQtP::previous_handler = (SoQtP_XErrorHandler*)XSetErrorHandler((XErrorHandler)SoQtP::X11Errorhandler);

    if (SoQtP::DEBUG_X11SYNC == ENVVAR_NOT_INITED) {
      const char * env = SoAny::si()->getenv(SoQtP::SOQT_XSYNC);
      SoQtP::DEBUG_X11SYNC = env ? atoi(env) : 0;
      if (SoQtP::DEBUG_X11SYNC) {
        // FIXME: SoDebugError::initClass() not yet invoked! 20021021 mortene.
        SoDebugError::postInfo("SoQt::init", "Turning on X synchronization.");
        Display * d;
#if QT_VERSION < 0x040000 // pre Qt 4
        d = qt_xdisplay();
#else // Qt 4.0.0+
        d = QX11Info::display();
#endif
        XSynchronize(d, True);
      }
    }
  }
#endif // Q_WS_X11

  // This should prove helpful for debugging the pervasive problem
  // under Win32 with loading multiple instances of the same library.
  if (SoQtP::DEBUG_LISTMODULES == ENVVAR_NOT_INITED) {
    const char * env = SoAny::si()->getenv("SOGUI_DEBUG_LISTMODULES");
    SoQtP::DEBUG_LISTMODULES = env ? atoi(env) : 0;
    if (SoQtP::DEBUG_LISTMODULES) { SoAny::listWin32ProcessModules(); }
  }
  
  SoDB::getSensorManager()->setChangedCallback(SoGuiP::sensorQueueChanged, NULL);
  SoQtP::mainwidget = toplevelwidget;
}

// documented in common/SoGuiCommon.cpp.in
QWidget *
SoQt::init(int & argc, char ** argv, const char * appname, const char * classname)
{
  // Must do this here so SoDebugError is initialized before it could
  // be attempted used.
  if (!SoDB::isInitialized()) { SoDB::init(); }

  // these aren't necessary - they are initialized in the SoQt::init method above
  // which is invoked below
  // SoNodeKit::init();
  // SoInteraction::init();

  if (SOQT_DEBUG && (SoQtP::appobject || SoQtP::mainwidget)) {
    SoDebugError::postWarning("SoQt::init",
                              "This method should be called only once.");
    return SoQtP::mainwidget;
  }

  // "qApp" is a global variable from the Qt library, pointing to the
  // single QApplication instance in a Qt-application.
  if (qApp == NULL) {
    // Set up the QApplication instance which we have derived into a
    // subclass to catch spaceball events.
    SoQtP::appobject = new SoQtApplication(argc, argv);
    SoQtP::madeappobject = TRUE;
  }
  else {
    // The user already set one up for us.
    //
    // FIXME: should somehow warn about the fact that spaceball events
    // will not be caught, if a spaceball is attempted used. 20020619 mortene.
    SoQtP::appobject = qApp;
  }

#ifdef QT3_SUPPORT //G.Barrand_Qt4 
  QWidget * mainw = new QWidget(NULL, classname);
#else
  QWidget * mainw = new QWidget(NULL);
#if QT_VERSION < 0x050000 // pre Qt 5
  mainw->setObjectName(QString::fromAscii(classname)); //G.Barrand_Qt4
#else
  mainw->setObjectName(QString::fromLatin1(classname));
#endif
#endif
  SoQtP::didcreatemainwidget = TRUE;
  SoQt::init(mainw);

  if (appname) { 
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
    SoQtP::mainwidget->setCaption(appname); 
#else
    SoQtP::mainwidget->setWindowTitle(appname); 
#endif
  }
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
  SoQtP::appobject->setMainWidget(SoQtP::mainwidget);
#else
  //G.Barrand_Qt4 ???
#endif
  return SoQtP::mainwidget;
}

// documented in common/SoGuiCommon.cpp.in
void
SoQt::mainLoop(void)
{
  (void) qApp->exec();

  // The invocation of SoQt::done() used to be triggered from
  // atexit(), but this was changed as we were getting mysterious
  // crashes. No wonder, perhaps, as for all we know the Qt library is
  // cleaned up _before_ our atexit() methods are invoked.
  SoQt::done();
}

// documented in common/SoGuiCommon.cpp.in
void
SoQt::exitMainLoop(void)
{
  qApp->exit(0);
}

// documented in common/SoGuiCommon.cpp.in
void
SoQt::done(void)
{
  // To avoid getting any further invokations of
  // SoGuiP::sensorQueueChanged() (which would re-allocate the timers
  // we destruct below). This could for instance happen when
  // de-coupling the scenegraph camera, triggering a notification
  // chain through the scenegraph.
  SoDB::getSensorManager()->setChangedCallback(NULL, NULL);

  delete SoQtP::timerqueuetimer; SoQtP::timerqueuetimer = NULL;
  delete SoQtP::idletimer; SoQtP::idletimer = NULL;
  delete SoQtP::delaytimeouttimer; SoQtP::delaytimeouttimer = NULL;

  if (SoQtP::didcreatemainwidget) {
    delete SoQtP::mainwidget;
    SoQtP::mainwidget = NULL;
    SoQtP::didcreatemainwidget = FALSE;
  }

  delete SoQtP::slotobj; SoQtP::slotobj = NULL;

  // Notice that by default we *don't* delete the QApplication object,
  // even though it originated with us, as it might be used someplace
  // else in the application.
  //
  // Because there are situations where it is useful to have it
  // deleted, we provide this possibility through setting an envvar
  // (so the app programmer must actively seek out and know what she
  // is doing):

  if (SoQtP::madeappobject) {
    const char * env = SoAny::si()->getenv("SOQT_DELETE_QAPPLICATION");
    if (env && atoi(env) > 0) {
      delete SoQtP::appobject; SoQtP::appobject = NULL;
    }
  }
}


// Documented in common/SoGuiCommon.cpp.in.
QWidget *
SoQt::getTopLevelWidget(void)
{
  return SoQtP::mainwidget;
}

/*!
  Returns a pointer to the Qt QWidget which is the top level widget for the
  given QWidget \a w. This is just a convenience function provided for
  easier porting of Open Inventor applications based on SoXt components,
  as you can do the same thing by calling the QWidget::topLevelWidget()
  method directly on \a w.

  \sa getTopLevelWidget()
*/

QWidget *
SoQt::getShellWidget(const QWidget * w)
{
  assert(w != NULL && "called with NULL pointer");
  return w->topLevelWidget();
}

/*!
  This method is provided for easier porting/compatibility with the
  Open Inventor SoXt component classes. It will call QWidget::show() and
  QWidget::raise() on the provided \a widget pointer.

  \sa hide()
*/

void
SoQt::show(QWidget * const widget)
{
  assert(widget && "called with NULL pointer");
  widget->show();
  widget->raise();
}

/*!
  This method is provided for easier porting/compatibility with the
  Open Inventor SoXt component classes. It will call QWidget::hide() on the
  provided \a widget pointer.

  \sa show()
*/

void
SoQt::hide(QWidget * const widget)
{
  assert(widget && "called with NULL pointer");
  widget->hide();
}

/*!
  This method is provided for easier porting of applications based on the
  Open Inventor SoXt component classes. It will call QWidget::resize() on the
  provided \a w widget pointer.

  \sa getWidgetSize()
*/

void
SoQt::setWidgetSize(QWidget * const w, const SbVec2s size)
{
  if (SOQT_DEBUG && !w) {
    SoDebugError::postWarning("SoQt::setWidgetSize",
                              "Called with NULL pointer.");
    return;
  }
  if (SOQT_DEBUG && ((size[0] <= 0) || (size[1] <= 0))) {
    SoDebugError::postWarning("SoQt::setWidgetSize",
                              "Called with invalid dimension(s): (%d, %d).",
                              size[0], size[1]);
    return;
  }

#if 0 // debug
  SoDebugError::postInfo("SoQt::setWidgetSize",
                         "resize %p: (%d, %d)",
                         w, size[0], size[1]);
#endif // debug

  w->resize(size[0], size[1]);
}

/*!
  This method is provided for easier porting/compatibility with the
  Open Inventor SoXt component classes. It will do the same as calling
  QWidget::size() (except that we're returning an SbVec2s).

  \sa setWidgetSize()
*/

SbVec2s
SoQt::getWidgetSize(const QWidget * w)
{
  if (SOQT_DEBUG && !w) {
    SoDebugError::postWarning("SoQt::getWidgetSize",
                              "Called with NULL pointer.");
    return SbVec2s(0, 0);
  }

  return SbVec2s(w->width(), w->height());
}

// Documented in common/SoGuiCommon.cpp.in.
//
// Just a simple wrap-around for the Qt QMessageBox::warning() call,
// provided for easier porting from applications using the Open
// Inventor SoXt component classes.
void
SoQt::createSimpleErrorDialog(QWidget * widget,
                              const char * title,
                              const char * string1,
                              const char * string2)
{
  if (SOQT_DEBUG && !title) {
    SoDebugError::postWarning("SoQt::createSimpleErrorDialog",
                              "Called with NULL title pointer.");
  }
  if (SOQT_DEBUG && !string1) {
    SoDebugError::postWarning("SoQt::createSimpleErrorDialog",
                              "Called with NULL error string pointer.");
  }

  SbString t(title ? title : "");
  SbString errstr(string1 ? string1 : "");

  if (string2) {
    errstr += '\n';
    errstr += string2;
  }

  (void)QMessageBox::warning(widget, t.getString(), errstr.getString());
}

// *************************************************************************
