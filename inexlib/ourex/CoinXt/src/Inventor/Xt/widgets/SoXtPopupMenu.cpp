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

/*!
  \class SoXtPopupMenu Inventor/Xt/widgets/SoXtPopupMenu.h
  \brief The SoXtPopupMenu class implements a common interface for popup
  menu management for all the Coin GUI toolkit libraries.

  The point of this class is to enable client code to be exactly the
  same for all GUI libraries, even without being template-based.
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <assert.h>

#include <Inventor/SoLists.h>
#include <Inventor/errors/SoDebugError.h>

#include <soxtdefs.h>
#include <Inventor/Xt/widgets/SoXtPopupMenu.h>
#include <Inventor/Xt/widgets/XtNativePopupMenu.h>
#include <Inventor/Xt/common/SbGuiList.h>

// *************************************************************************

// Documentation for abstract methods.

/*!
  \fn void SoXtPopupMenu::popUp(Widget inside, int x, int y)

  Invokes this popup menu instance.

  The coordinates for the popup will be used relative to the origo of
  the widget argument.
*/

// *************************************************************************

// The private data for the SoXtPopupMenu.

class SoXtPopupMenuP {
public:
  SbGuiList<int> menuitems;
  SbGuiList<int> radiogroups;
  SbPList * callbacks;
  SbPList * userdata;
};


#define PRIVATE(o) (o->pimpl)

// *************************************************************************

/*!
  Instantiation method.

  SoXtPopupMenu is implemented with this "factory" method instead of
  a public constructor to provide a completely generic interface on
  top of the toolkit's native popup menu implementation.
*/
SoXtPopupMenu *
SoXtPopupMenu::createInstance(void)
{
  return new XtNativePopupMenu();
}


/*!
  The class constructor is protected, as SoXtPopupMenu uses a factory
  instantiation method.
*/
SoXtPopupMenu::SoXtPopupMenu(void)
{
  PRIVATE(this) = new SoXtPopupMenuP;

  PRIVATE(this)->callbacks = NULL;
  PRIVATE(this)->userdata = NULL;
} // SoXtPopupMenu()

/*!
  Destructor.
*/
SoXtPopupMenu::~SoXtPopupMenu()
{
  delete PRIVATE(this)->callbacks;
  delete PRIVATE(this)->userdata;
} // ~SoXtPopupMenu()

// *************************************************************************

/*!
*/

void
SoXtPopupMenu::setMenuItemMarked(
  int itemid,
  SbBool marked)
{
  // forward to native implementation
  this->_setMenuItemMarked(itemid, marked);
} // setMenuItemMarked()

// *************************************************************************

/*!
*/

int
SoXtPopupMenu::newRadioGroup(
  int groupid)
{
  const int numGroupedItems = PRIVATE(this)->radiogroups.getLength();
  int id = groupid;
  if (id != -1) {
    SbBool hit = FALSE;
    int i;
    for (i = 0; i < numGroupedItems && hit == FALSE; i++) {
      if (PRIVATE(this)->radiogroups[i] == id)
        hit = TRUE;
    }
    if (hit == TRUE) {
#if SOXT_DEBUG
      SoDebugError::postInfo("SoXtPopupMenu::NewRadioGroup",
        "requested group id already taken");
#endif // SOXT_DEBUG
      return -1; 
    }
  } else {
    id = 0;
    SbBool found;
    do {
      id++;
      found = FALSE;
      int i;
      for (i = 0; i < numGroupedItems && found == FALSE; i++) {
        if (PRIVATE(this)->radiogroups[i] == id)
          found = TRUE;
      }
    } while (found == TRUE);
  }
  // id is OK here
  PRIVATE(this)->menuitems.append(-1); // fake menu item
  PRIVATE(this)->radiogroups.append(id); // register id in array
  return id;
} // newRadioGroup()

/*!
*/

int
SoXtPopupMenu::getRadioGroup(
  int itemid)
{
  assert(itemid != -1);
  const int numGroupedItems = PRIVATE(this)->radiogroups.getLength();
  int i;
  for (i = 0; i < numGroupedItems; i++) {
    if (PRIVATE(this)->menuitems[i] == itemid)
      return PRIVATE(this)->radiogroups[i];
  }
  return -1;
} // getRadioGroup()

/*!
*/

int
SoXtPopupMenu::getRadioGroupSize(
  int groupid)
{
  int count = 0;
  const int numItems = PRIVATE(this)->radiogroups.getLength();
  int i;
  for (i = 0; i < numItems; i++) {
    if (PRIVATE(this)->radiogroups[i] == groupid &&
         PRIVATE(this)->menuitems[i] != -1)
      count++;
  }
  return count;
} // getRadioGroupSize()

/*!
*/

void
SoXtPopupMenu::addRadioGroupItem(
  int groupid,
  int itemid)
{
  PRIVATE(this)->menuitems.append(itemid);
  PRIVATE(this)->radiogroups.append(groupid);
} // addRadioGroupItem()

/*!
*/

void
SoXtPopupMenu::removeRadioGroupItem(
  int itemid)
{
  assert(itemid != -1);
  const int numGroupedItems = PRIVATE(this)->menuitems.getLength();
  int i;
  for (i = 0; i < numGroupedItems; i++) {
    if (PRIVATE(this)->menuitems[i] == itemid) {
      PRIVATE(this)->menuitems.remove(i);
      PRIVATE(this)->radiogroups.remove(i);
      return;
    }
  }
} // removeRadioGroupItem()

void
SoXtPopupMenu::setRadioGroupMarkedItem(
  int itemid)
{
  const int numItems = PRIVATE(this)->menuitems.getLength();
  int i;
  int groupid = -1;
  for (i = 0; i < numItems && groupid == -1; i++) {
    if (PRIVATE(this)->menuitems[i] == itemid)
      groupid = PRIVATE(this)->radiogroups[i];
  }
  if (groupid == -1)
    return;
  for (i = 0; i < numItems; i++) {
    if (PRIVATE(this)->radiogroups[i] == groupid) {
      int item = PRIVATE(this)->menuitems[i];
      if ((item != -1) && (item != itemid)) {
        if ( this->getMenuItemMarked(item) ) {
          this->setMenuItemMarked(item, FALSE);
        }
      }
    }
  }
} // setRadioGroupItemEnabled()

int
SoXtPopupMenu::getRadioGroupMarkedItem(
  int groupid)
{
  const int numItems = PRIVATE(this)->menuitems.getLength();
  int i;
  for (i = 0; i < numItems; i++) {
    if (PRIVATE(this)->radiogroups[i] == groupid) {
      int item = PRIVATE(this)->menuitems[i];
      if (item != -1 && this->getMenuItemMarked(item))
        return item;
    }
  }
  return -1;
} // setRadioGroupItemEnabled()

// *************************************************************************

/*!
*/

void
SoXtPopupMenu::addMenuSelectionCallback(
  SoXtMenuSelectionCallback * callback,
  void * user)
{
  if (PRIVATE(this)->callbacks == NULL) {
    PRIVATE(this)->callbacks = new SbPList;
    PRIVATE(this)->userdata = new SbPList;
  }
  PRIVATE(this)->callbacks->append((void *) callback);
  PRIVATE(this)->userdata->append(user);
} // addMenuSelectionCallback()

/*!
*/

void
SoXtPopupMenu::removeMenuSelectionCallback(
  SoXtMenuSelectionCallback * callback,
  void * user)
{
  if (PRIVATE(this)->callbacks == NULL) {
#if SOXT_DEBUG
    SoDebugError::postInfo("SoXtPopupMenu::RemoveMenuSelectionCallback",
      "no callbacks at all");
#endif // SOXT_DEBUG
    return;
  }
  int numCallbacks = PRIVATE(this)->callbacks->getLength();
  int i;
  for (i = 0; i < numCallbacks; i++) {
    if ((*PRIVATE(this)->callbacks)[i] == (void *) callback &&
         (*PRIVATE(this)->userdata)[i] == user) {
      PRIVATE(this)->callbacks->remove(i);
      PRIVATE(this)->userdata->remove(i);
      i--;
      numCallbacks--;
    }
  }
} // removeMenuSelectionCallback()

/*!
*/

void
SoXtPopupMenu::invokeMenuSelection(
  int itemid)
{
  if (PRIVATE(this)->callbacks == NULL)
    return;
  const int numCallbacks = PRIVATE(this)->callbacks->getLength();
  int i;
  for (i = 0; i < numCallbacks; i++) {
    SoXtMenuSelectionCallback * cb = (SoXtMenuSelectionCallback *)
      (*PRIVATE(this)->callbacks)[i];
    (*cb)(itemid, (*PRIVATE(this)->userdata)[i]);
  }
} // invokeMenuSelection()

// *************************************************************************

