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

#ifndef SOXT_INTERNALDEFS_H
#define SOXT_INTERNALDEFS_H

/* ********************************************************************** */

#ifndef SOXT_INTERNAL
#error this is a private header file
#endif /* !SOXT_INTERNAL */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

/* ********************************************************************** */

#ifdef __FILE__
#define SOXT_STUB_FILE __FILE__
#else
/* FIXME: avoid this case by redefining the compilation rules to define the
   current filename in some variable.  [20010919 larsa] */
#define SOXT_STUB_FILE ((char *)0L)
#endif

#ifdef __LINE__
#define SOXT_STUB_LINE __LINE__
#else
#define SOXT_STUB_LINE 0
#endif

#ifdef __cplusplus
#ifdef HAVE_CPP_COMPILER_FUNCTION_NAME_VAR
#define SOXT_STUB_FUNC HAVE_CPP_COMPILER_FUNCTION_NAME_VAR
#else
#define SOXT_STUB_FUNC ((const char *)0L)
#endif
#else /* !__cplusplus */
#ifdef HAVE_C_COMPILER_FUNCTION_NAME_VAR
#define SOXT_STUB_FUNC HAVE_C_COMPILER_FUNCTION_NAME_VAR
#else
#define SOXT_STUB_FUNC ((const char *)0L)
#endif
#endif /* !__cplusplus */

#if SOXT_DEBUG

#include <Inventor/errors/SoDebugError.h>

/*
  This stupid thing is here to silence some compilers that complain on
  constant if-expressions.
*/

inline int soxt_not_null(const void * arg) { return (arg != NULL) ? 1 : 0; }


/* Collects common code from SOXT_STUB(), SOXT_STUB()_ONCE and
   SOXT_OBSOLETED(). */

#define SOXT_FUNCTIONIDENTIFIER(SOXTSTR) \
    SbString SOXTSTR(soxt_not_null(SOXT_STUB_FILE) ? SOXT_STUB_FILE : ""); \
    if (SOXTSTR.getLength() > 0) { SOXTSTR += ':'; } \
    SOXTSTR += SbString((int)SOXT_STUB_LINE); SOXTSTR += ':'; \
    SOXTSTR += soxt_not_null(SOXT_STUB_FUNC) ? SOXT_STUB_FUNC : ""

/*
  SOXT_STUB(): a macro that prints out stub information.
  Used where there is functionality missing.
*/

#define SOXT_STUB() \
  do { \
    SOXT_FUNCTIONIDENTIFIER(s); \
    SoDebugError::postWarning(s.getString(), \
                              "STUB: functionality not yet completed"); \
  } while (0)

/*
  SOXT_STUB_ONCE(): a macro that prints out stub information, but only on
  the first pass.  Used where there is functionality missing that may be
  called hundreds of times.
*/

#define SOXT_STUB_ONCE() \
  do { \
    static int first = 1; \
    if (first) { \
      SOXT_FUNCTIONIDENTIFIER(s); \
      SoDebugError::postWarning(s.getString(), \
                                "STUB: functionality not yet completed " \
                                "(first and last warning)"); \
      first = 0; \
    } \
  } while (0)

/*
  SOXT_OBSOLETED(): a macro that prints out information about an obsoleted
  function still being in use.
*/

#define SOXT_OBSOLETED() \
  do { \
    SOXT_FUNCTIONIDENTIFIER(s); \
    SoDebugError::post(s.getString(), \
                       "OBSOLETED: functionality no longer supported"); \
  } while (0)

#else /* !SOXT_DEBUG */

/* snub the stubs */
#define SOXT_STUB()      do { } while (0)
#define SOXT_STUB_ONCE() do { } while (0)
#define SOXT_OBSOLETED() do { } while (0)

#endif /* !SOXT_DEBUG */

/*
  GETTEXT support
*/

#if defined(ENABLE_NLS) && defined(HAVE_GETTEXT)
#include <libintl.h>
#define _(String) dgettext (PACKAGE, String)
#define N_(String) (String)
#else
#define _(String) (String)
#define N_(String) (String)
#endif /* HAVE_GETTEXT && ENABLE_NLS */

/* ********************************************************************** */

#endif /* ! SOXT_INTERNALDEFS_H */
