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

// @configure_input@

/* For the Doxygen tool. */

/*! \defgroup misc Miscellaneous Classes */
/*! \defgroup devices Xt Device Classes */
/*! \defgroup components Xt Components */
/*! \defgroup viewers Xt Viewer Components */


// FIXME: the code example shouldn't be duplicated here -- it's
// already part of the SoQt mainpage-doc. 20020806 mortene.

/*!
  \class SoXt SoXt.h Inventor/Xt/SoXt.h
  \brief The SoXt class takes care of Xt initialization and event dispatching.
  \ingroup misc

  This is the "application-wide" class with solely static methods
  handling initialization and event processing tasks. You must use
  this class in any application built on top of the SoXt
  library.

  Typical usage is as follows (complete application code):

  \code
  #include <Inventor/Xt/SoXt.h>
  #include <Inventor/Xt/viewers/SoXtExaminerViewer.h>
  #include <Inventor/nodes/SoBaseColor.h>
  #include <Inventor/nodes/SoCone.h>
  #include <Inventor/nodes/SoSeparator.h>
  
  int
  main(int argc, char ** argv)
  {
    // Initialize SoXt and Inventor API libraries. This returns a main
    // window to use.
    Widget mainwin = SoXt::init(argc, argv, argv[0]);
  
    // Make a dead simple scene graph, only containing a single
    // yellow cone under the scenegraph root.
    SoSeparator * root = new SoSeparator;
    root->ref();

    SoBaseColor * col = new SoBaseColor;
    col->rgb = SbColor(1, 1, 0);
    root->addChild(col);

    root->addChild(new SoCone);
  
    // Use one of the convenient viewer classes.
    SoXtExaminerViewer * eviewer = new SoXtExaminerViewer(mainwin);
    eviewer->setSceneGraph(root);
    eviewer->show();
  
    // Pop up the main window.
    SoXt::show(mainwin);
    // Loop until exit.
    SoXt::mainLoop();

    // Clean up resources.
    delete eviewer;
    root->unref();

    return 0;
  }
  \endcode

  And voila:

  <center>
  <img src="http://doc.coin3d.org/images/SoLibs/general/sogui-class-example.png">
  </center>

  \sa SoXtComponent
*/

#include <stdlib.h>
#include <string.h>
#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/SoGuiP.h>
#include <Inventor/Xt/SoAny.h>


// Default constructor and the destructor is private (SoXt class
// consists solely of static functions).
SoXt::SoXt(void) { }
SoXt::~SoXt() { }


// *************************************************************************

/*!
  This function initializes the SoXt library, as well as the
  Inventor API. The latter is done by calling \a SoDB::init(), \a
  SoNodeKit::init() and \a SoInteraction::init().

  The returned Widget is a toplevel shell widget for the
  application, which can be used as a shell for the main component.

  This method is provided for easier porting / compatibility with the
  original SGI Inventor InventorXt library. It just adds dummy \a argc
  and \a argv arguments and calls the SoXt::init() method below.
*/
Widget
SoXt::init(const char * appname, const char * classname)
{
  // If this is SoQt, Qt will keep a reference to the appname string
  // -- so make it guaranteed permanent.
  char * buf = NULL;
  if (appname != NULL) {
    buf = new char[strlen(appname) + 1]; // FIXME: minor memleak, free on app-exit. 20020117 mortene.
    (void)strcpy(buf, appname);
  }

  // Fake argc and argv setup, forward to real init routine.
  static char *array[1]; // use static array to avoid memory corruption in Qt
  array[0] = buf;
  int argc = (buf != NULL) ? 1 : 0;
  return SoXt::init(argc, array, appname, classname);
}

/*!
  \fn Widget SoXt::init(int & argc, char ** argv, const char * appname, const char * classname)

  This function initializes the SoXt library, as well as the
  Inventor API. The latter is done by calling \a SoDB::init(), \a
  SoNodeKit::init() and \a SoInteraction::init().

  Creates an application framework and constructs and returns a main
  widget for you.

  The returned Widget is a toplevel shell widget for the
  application, which can be used as a shell for the main component.
*/

/*!
  \fn void SoXt::init(Widget toplevelwidget)

  This function initializes the SoXt library, as well as the
  Inventor API. The latter is done by calling \a SoDB::init(), \a
  SoNodeKit::init() and \a SoInteraction::init().

  Assumes you are creating your own application framework and main
  widget.  \a toplevelwidget should be your application's main widget.
*/

/*!
  \fn void SoXt::done(void)

  Cleans up all static data allocated by the SoXt library on
  initialization.

  Is called implicitly from the end of SoXt::mainLoop(), so the
  application programmer don't have to use it explicitly unless she
  has taken control over the native event loop herself, and is
  therefore not using SoXt::mainLoop().

  (Even then it is usually not necessary to call this method, as the
  operating system should take care of cleaning up resource use when
  an application exits. This method is mainly provided as a manner to
  be able to clean up in "unusual" run-time environments, for instance
  if SoXt is used as a component in a browser plug-in.)

  Nevertheless, it should never be invoked more than \e once, and that
  is just before application exit, as it deallocates \e static data
  allocated as "one-off" operations in SoXt::init().

  NOTE: done() is not present in the original API for SGI's InventorXt
  library.
*/

/*!
  \fn void SoXt::mainLoop(void)

  This is the event dispatch loop.

  It doesn't return until application exit is somehow forced, either
  programmatically from the relevant API-call of the native toolkit,
  or when the user closes the application's main widget.

  An important note: be careful about how you handle
  SoXtComponent-derived objects after the application control returns
  from mainLoop(), as SoXt will then have been "cleaned up" with
  regards to various internal resources. So doing for instance
  something like this:

  \code
  SoXt::mainLoop();
  viewer->hide();
  \endcode

  ..spells "undefined behavior, expect a crash".

  \e Deleting a component after mainLoop() returns is allowed, though,
  and also necessary to avoid getting reports of possible memory leaks
  from memleak checkers.
*/

/*!
  \fn void SoXt::exitMainLoop(void)

  This function will make the main event loop finish looping.

  NOTE: exitMainLoop() is not present in the original API for SGI's
  InventorXt library.
*/

/*!
  \fn Widget SoXt::getTopLevelWidget(void)

  Returns the Widget which is the main widget for the
  application. When this widget gets closed, SoXt::mainLoop() will
  return (unless the close event is caught by the user).

  \sa getShellWidget()
*/

// *************************************************************************

/*!
  This method will fill in the integers pointed to by the arguments
  with the corresponding part of the version release number of the
  SoXt library.

  A \c NULL pointer will make that part of the version number be ignored.

  This method is not part of the original InventorXt API from SGI.
*/
void
SoXt::getVersionInfo(int * major, int * minor, int * micro)
{
  if (major) { *major = SOXT_MAJOR_VERSION; }
  if (minor) { *minor = SOXT_MINOR_VERSION; }
  if (micro) { *micro = SOXT_MICRO_VERSION; }
}

/*!
  This method returns a string containing the version id of the
  library.

  This method is not part of the original InventorXt API from SGI.
*/
const char *
SoXt::getVersionString(void)
{
  static const char version[] = SOXT_VERSION;
  return version;
}

// *************************************************************************

/*!
  \typedef void SoXt::FatalErrorCB(const SbString errmsg, SoXt::FatalErrors errcode, void * userdata)

  An application function callback for handling fatal errors should be
  of this type.

  The first argument is an error message in English describing the
  details of the error. The second argument is an error code used so
  the application can identify specific conditions. The third argument
  is the userdata pointer passed in to SoXt::setFatalErrorHandler().
*/

/*!
  \enum SoXt::FatalErrors
  Numerical identifiers for classifying the different kinds of possible
  fatal errors.
*/

/*!
  \var SoXt::FatalErrors SoXt::NO_OPENGL_CANVAS

  Could not construct \e any valid OpenGL canvas. Something is very
  wrong on the client system.
*/
/*!
  \var SoXt::FatalErrors SoXt::INTERNAL_ASSERT

  An internal error condition that should never happen was
  detected. The most likely cause of this is programmering errors
  within the SoXt library itself.
*/
/*!
  \var SoXt::FatalErrors SoXt::UNSPECIFIED_ERROR

  Signifies that we were not able to specify in any greater detail the
  error condition that came up.
*/

/*!
  Set up a callback to invoke in the case of unexpected fatal error
  conditions within the SoXt library.

  Almost any error condition within the library is handled in a robust
  way through return values indicating errors for the offending calls,
  but there are a few cases that are impossible to handle without
  seriously crippling the functionality.

  (One example is if we fail to find \e any way of making a valid
  OpenGL canvas. This is an indication that something is seriously
  wrong on the end-user's system, and the SoXt library will not work
  properly.)

  In the case of a fatal error, it is expected that the given
  application callback function communicates the problem to the
  end-user and then either exits the application or at least refrains
  from using any part of the SoXt library.

  If no callback is explicitly set up by the application, the SoXt
  library will display an error message to the end-user and then exit
  the application.

  When setting a callback, this method returns a pointer to the
  previous callback function, or \c NULL if none.

  (This is an extension versus the original SGI InventorXt library
  API.)


  On a related note, be aware that the end-user will still be notified
  about non-fatal errors and warning messages through a dialog box. If
  you want to handle these yourself, or if you don't want your
  end-user to see any of these non-critical messages, you will also
  need to install an error handler callback function on the Coin
  library itself:

  \code
    SoDebugError::setHandlerCallback(myErrorHandlerCB, myCBUserData);
  \endcode

  (Please also see the documentation of
  SoDebugError::setHandlerCallback().)

 */
SoXt::FatalErrorCB *
SoXt::setFatalErrorHandler(SoXt::FatalErrorCB * cb, void * userdata)
{
  return SoAny::si()->setFatalErrorHandler(cb, userdata);
}

// *************************************************************************

/*!
  Returns \c TRUE if this binary version of the SoXt library was
  compiled with debug settings on.

  This method was not part of the original SGI InventorXt library, but
  is an extension specific to the Coin project.
*/
SbBool
SoXt::isDebugLibrary(void)
{
  return SOXT_DEBUG != 0 ? TRUE : FALSE;
}

/*!
  Returns \c TRUE if this version of the SoXt library has an
  Application Binary Interface compatible with the given version.

  This method was not part of the original SGI InventorXt library, but
  is an extension specific to the Coin project.
*/
SbBool
SoXt::isCompatible(unsigned int major, unsigned int minor)
{
  if (major != SOXT_MAJOR_VERSION) { return FALSE; }
  if (minor > SOXT_MINOR_VERSION) { return FALSE; }
  return TRUE;
}

/*!
  \enum SoXt::ABIType
  Numerical identifiers to identify how the library was built.
*/
/*!
  \var SoXt::ABIType SoXt::DLL
  The SoXt library was built as a dynamic link library (aka "shared
  library").
*/
/*!
  \var SoXt::ABIType SoXt::LIB
  The SoXt library was built as a static library.
*/
/*!
  \var SoXt::ABIType SoXt::UNKNOWN
  The SoXt introspection mechanisms can not decide how the library
  was built.
*/


/*!
  Returns an indication on how the library was compiled: as a dynamic
  library, or as a static library.

  This method was not part of the original SGI InventorXt library, but
  is an extension specific to the Coin project.
*/
SoXt::ABIType
SoXt::getABIType(void)
{
  // FIXME: replace this ugly shit with a configure-check to test if
  // we're on a DLL-capable platform. 20020118 mortene.
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#ifdef SOXT_MAKE_DLL
  return SoXt::DLL;
#else
  return SoXt::LIB;
#endif
#endif
  // FIXME: this is rather lame, we have enough information in the
  // configure / build process to always know whether we're building
  // static or dynamic. 20020618 mortene.
  return SoXt::UNKNOWN;
}

#if 0 // FIXME: not in use, see larsa's FIXME below. 20020617 mortene.

// *************************************************************************
// These sanity checks are designed to detect common pitfall errors for
// Microsoft Windows linkage with SoXt and Coin.

// FIXME: use an "is-this-a-DLL-capable-platform" configure check
// and remove the system "#if defined" tests below. 20011203 mortene.
// FIXME: I disabled this because it wasn't inlined in the client app
// but compiled into the library by MSVC++ and with SOWIN_DLL undefined,
// the ABI test always tried the LIB_ABI path.  20020126 larsa
#if 0 && (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
#define SOXT_SANITY_CHECK(forward_call)                  \
  if (SoXt::isDebugLibrary() &&                       \
       SoDB::isInitialized())                          \
    SoDebugError::postWarning("SoXt::init",            \
      "unable to verify application linkage sanity "    \
      "because Open Inventor has already been "         \
      "initialized before SoXt::init call");           \
  SOXT_RUNTIME_SANITY_CHECK();                         \
  SOXT_LINKSTYLE_SANITY_CHECK();                       \
  forward_call;                                         \
  SOXT_LINKTIME_SANITY_CHECK()

#define SOXT_RUNTIME_SANITY_CHECK()                    \
  do {                                                  \
    if (!SoXt::isCompatible(SOXT_MAJOR_VERSION,      \
                            SOXT_MINOR_VERSION))      \
      SoXt::abort(SOXT_RUNTIME_MISMATCH);             \
  } while (FALSE)

#ifdef SOXT_DLL
#define SOXT_LINKSTYLE_SANITY_CHECK()                  \
  do {                                                  \
    if (SoXt::getABIType() != SOXT_DLL_ABI)         \
      SoXt::abort(SOXT_LINKSTYLE_MISMATCH);           \
  } while (FALSE)
#else
#define SOXT_LINKSTYLE_SANITY_CHECK()                  \
  do {                                                  \
    if (SoXt::getABIType() != SOXT_LIB_ABI)         \
      SoXt::abort(SOXT_LINKSTYLE_MISMATCH);           \
  } while (FALSE)
#endif

#define SOXT_LINKTIME_SANITY_CHECK()                   \
  do {                                                  \
    if (!SoDB::isInitialized())                       \
      SoXt::abort(SOXT_LINKTIME_MISMATCH);            \
  } while (FALSE)
#else /* ! MS Windows */
#define SOXT_SANITY_CHECK(forward_call)                \
  forward_call
#endif /* ! MS Windows */

#endif // OBSOLETED

#ifndef DOXYGEN_SKIP_THIS

// Abort the application due to some kind of mismatch in the ABI
// settings / configuration. This should hopefully help application
// programmers avoid shooting themselves in the foot by controlling
// certain run-time parameters in the client application versus what
// is expected by the SoXt library.
//
// If you're an application programmer, it is very likely that you
// only need to consider this an internal library method.
//
// This method was not part of the original SGI InventorXt library,
// but is an extension specific to the Coin project.
void
SoGuiP::abort(SoGuiP::ABIError error)
{
  switch (error) {
  case SoGuiP::LINKTIME_MISMATCH:
    SoXt::createSimpleErrorDialog(NULL, "Fatal Error",
                                     "Detected linktime mismatch error.");
    break;

  case SoGuiP::LINKSTYLE_MISMATCH:
    SoXt::createSimpleErrorDialog(NULL, "Fatal Error",
                                     "Detected linkstyle mismatch error (DLL vs. LIB).");
    break;

  case SoGuiP::RUNTIME_MISMATCH:
    SoXt::createSimpleErrorDialog(NULL, "Fatal Error",
                                     "Detected runtime mismatch error (versioning and ABI compatibility).");
    break;

  default:
    // FIXME: shouldn't this rather be an assert? 20020118 mortene.
    SoXt::createSimpleErrorDialog(NULL, "Fatal Error",
                                     "Unknown error in SoXt :(");
    break;
  }

  // FIXME: call fatal error handler in SoAny. 20020118 mortene.
  exit(-1);
}

#endif // DOXYGEN_SKIP_THIS

// *************************************************************************

/*!
  \fn void SoXt::createSimpleErrorDialog(Widget widget, const char * title, const char * string1, const char * string2)


  This is a convenient way for the application programmer to throw up
  an obtrusive application-global error dialog.

  If \a widget is \c NULL, the dialog will be modal for the whole
  application (all windows will be blocked for input). If not,
  only the window for the given \a widget will be blocked.

  \a title is the title of the dialog box. \a string1 and \a string2
  contains the text which will be shown in the dialog box.

  There will only be a single "Ok" button for the user to press and
  continue with the application.
*/

// *************************************************************************
