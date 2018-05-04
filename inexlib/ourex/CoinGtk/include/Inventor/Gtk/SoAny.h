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

#ifndef SOANY_H
#define SOANY_H

#include <Inventor/SbPList.h>
#include <Inventor/Gtk/SoGtk.h>

// ************************************************************************

class SoAny {
public:
  // use the singleton design pattern for access
  static SoAny * si(void);

  const char * getenv(const char * e);

  void * getSharedGLContext(void * display, void * screen);
  void registerGLContext(void * context, void * display, void * screen);
  void unregisterGLContext(void * context);
  int getSharedCacheContextId(void * context);

  SoGtk::FatalErrorCB * setFatalErrorHandler(SoGtk::FatalErrorCB * cb,
                                               void * userdata);
  SbBool invokeFatalErrorHandler(SbString errmsg, SoGtk::FatalErrors errcode);


  typedef void InternalFatalErrorCB(void * userdata);
  void addInternalFatalErrorHandler(SoAny::InternalFatalErrorCB * cb,
                                    void * ud);

private:
  SoAny(void);
  ~SoAny();
  static void cleanup_si(void);

  // (use SbPList instances to be able to compile properly on SGI /
  // TGS Inventor aswell as Coin)
  SbPList cclist;
  SbPList internalfehandlers, internalfedata;

  SoGtk::FatalErrorCB * fatalcb;
  void * userdata;
}; // class SoAny

// ************************************************************************

#endif // ! SOANY_H
