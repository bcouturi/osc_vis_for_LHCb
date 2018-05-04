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
// 

#ifndef SOGTK_POPUPMENU_H
#define SOGTK_POPUPMENU_H

#include <Inventor/SbBasic.h>

#include <Inventor/Gtk/SoGtkComponent.h>

// *************************************************************************

typedef void SoGtkMenuSelectionCallback(int itemid, void * user);

class SOGTK_DLL_API SoGtkPopupMenu { // abstract interface class
public:
  static SoGtkPopupMenu * createInstance(void);
  virtual ~SoGtkPopupMenu();

  virtual int newMenu(const char * name, int menuid = -1) = 0;
  virtual int getMenu(const char * name) = 0;
  virtual void setMenuTitle(int id, const char * title) = 0;
  virtual const char * getMenuTitle(int id) = 0;

  virtual int newMenuItem(const char * name, int itemid = -1) = 0;
  virtual int getMenuItem(const char * name) = 0;
  virtual void setMenuItemTitle(int itemid, const char * title) = 0;
  virtual const char * getMenuItemTitle(int itemid) = 0;
  virtual void setMenuItemEnabled(int itemid, SbBool enabled) = 0;
  virtual SbBool getMenuItemEnabled(int itemid) = 0;
  void setMenuItemMarked(int itemid, SbBool marked);
  virtual SbBool getMenuItemMarked(int itemid) = 0;

  virtual void addMenu(int menuid, int submenuid, int pos = -1) = 0;
  virtual void addMenuItem(int menuid, int itemid, int pos = -1) = 0;
  virtual void addSeparator(int menuid, int pos = -1) = 0;
  virtual void removeMenu(int menuid) = 0;
  virtual void removeMenuItem(int itemid) = 0;

  virtual void popUp(GtkWidget * inside, int x, int y) = 0;

  int newRadioGroup(int groupid = -1);
  int getRadioGroup(int itemid);
  int getRadioGroupSize(int groupid);
  void addRadioGroupItem(int groupid, int itemid);
  void removeRadioGroupItem(int itemid);

  void setRadioGroupMarkedItem(int itemid);
  int getRadioGroupMarkedItem(int groupid);

  void addMenuSelectionCallback(SoGtkMenuSelectionCallback * callback,
                                void * data);
  void removeMenuSelectionCallback(SoGtkMenuSelectionCallback * callback,
                                   void * data);

protected:
  SoGtkPopupMenu(void);

  virtual void _setMenuItemMarked(int itemid, SbBool marked) = 0;

  void invokeMenuSelection(int itemid);

private:
  class SoGtkPopupMenuP * pimpl;

}; // class SoGtkPopupMenu

// *************************************************************************

#endif // ! SOGTK_POPUPMENU_H
