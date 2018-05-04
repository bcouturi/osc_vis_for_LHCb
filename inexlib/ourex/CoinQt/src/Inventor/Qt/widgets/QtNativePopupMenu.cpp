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

/*!
  \class QtNativePopupMenu Inventor/Qt/widgets/QtNativePopupMenu.h
  \brief The QtNativePopupMenu class implements a common interface for popup
  menu management for all the Coin GUI toolkit libraries.
*/

// *************************************************************************

// FIXME: get rid of this define. We should fix up the compile issues
// wrt Qt 4 properly. 20050629 mortene.
//G.Barrand_Qt4 #define QT3_SUPPORT

#include <QtCore/qmetaobject.h> //G.Barrand QtCore

// *************************************************************************

// Take care of class name incompatibilities between Qt 3 and Qt 4.

#if QT_VERSION < 0x040000 // pre Qt 4
#include <qpopupmenu.h>
#define QPOPUPMENU_CLASS QPopupMenu
#elif QT_VERSION < 0x050000 // pre Qt 5
#include <QtGui/qmenu.h> //G.Barrand QtGui
#define QPOPUPMENU_CLASS QMenu
#else
#include <QtWidgets/qmenu.h> //G.Barrand QtGui
#define QPOPUPMENU_CLASS QMenu
#endif

// ************************************************************************

#include <Inventor/SoLists.h>
#include <Inventor/errors/SoDebugError.h>

#include <Inventor/Qt/widgets/QtNativePopupMenu.h>
#include <Inventor/Qt/widgets/moc_QtNativePopupMenu.icc>

#include <soqtdefs.h>

// *************************************************************************

struct MenuRecord {
  int menuid;
  char * name;
  char * title;
  QPOPUPMENU_CLASS * menu;
  QPOPUPMENU_CLASS * parent;
}; // struct MenuRecord

struct ItemRecord {
  int itemid;
  int flags;
  char * name;
  char * title;
  QPOPUPMENU_CLASS * parent;
  QAction* action;
}; // struct ItemRecord

#define ITEM_MARKED       0x0001
#define ITEM_SEPARATOR    0x0002
#define ITEM_ENABLED      0x0004

// *************************************************************************

/*!
  The constructor.
*/

QtNativePopupMenu::QtNativePopupMenu(
  void)
{
  this->menus = new SbPList;
  this->items = new SbPList;
} // QtNativePopupMenu()

/*!
  Destructor.
*/

QtNativePopupMenu::~QtNativePopupMenu()
{
  const int numMenus = this->menus->getLength();

  int i;
  for (i = 0; i < numMenus; i++) {
    MenuRecord * rec = (MenuRecord *) (*this->menus)[i];
    delete [] rec->name;
    delete [] rec->title;
    // If top-level QPopupMenu is not attached to any parent instance,
    // we dealloc it ourselves.
    if (rec->parent == NULL) delete rec->menu;
    delete rec;
  }

  const int numItems = this->items->getLength();
  for (i = 0; i < numItems; i++) {
    ItemRecord * rec = (ItemRecord *) (*this->items)[i];
    delete [] rec->name;
    delete [] rec->title;
    delete rec->action;
    delete rec;
  }
} // ~QtNativePopupMenu()

// *************************************************************************

/*!
*/

int
QtNativePopupMenu::newMenu(
  const char * name,
  int menuid)
{
  // FIXME: this function is the same in the other So-libraries --
  // move to common abstraction layer SoGuiPopupMenu of
  // possible. 20031012 mortene.

  int id = menuid;
  if (id == -1) {
    id = 1;
    while (this->getMenuRecord(id) != NULL) id++;
  } else {
    assert(this->getMenuRecord(id) == NULL &&
            "requested menuid already taken");
  }

  // id contains ok ID
  MenuRecord * rec = this->createMenuRecord(name);
  rec->menuid = id;
  this->menus->append((void *) rec);
  return id;
} // newMenu()

/*!
*/

int
QtNativePopupMenu::getMenu(
  const char * name)
{
  const int numMenus = this->menus->getLength();
  int i;
  for (i = 0; i < numMenus; i++) {
    MenuRecord * rec = (MenuRecord *) (*this->menus)[i];
    if (strcmp(rec->name, name) == 0)
      return rec->menuid;
  }
  return -1;
} // getMenu()

/*!
*/

void
QtNativePopupMenu::setMenuTitle(
  int menuid,
  const char * title)
{
  MenuRecord * rec = this->getMenuRecord(menuid);
  assert(rec && "no such menu");
  delete [] rec->title;
  rec->title = strcpy(new char [strlen(title)+1], title);
#if QT_VERSION >= 200
  if (rec->parent) {
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
    rec->parent->changeItem(rec->menuid, QString(rec->title));
#else
    //G.Barrand_Qt4 : FIXME
#endif
  }
#else // Qt version < 2.0.0
  // This QMenuData::changeItem() method is being obsoleted from Qt
  // from version 2.0.0 onwards.
  if (rec->parent)
    rec->parent->changeItem(rec->title, rec->menuid);
#endif // Qt version < 2.0.0
} // setMenuTitle()

/*!
*/

const char *
QtNativePopupMenu::getMenuTitle(
  int menuid)
{
  MenuRecord * rec = this->getMenuRecord(menuid);
  assert(rec && "no such menu");
  return rec->title;
} // getMenuTitle()

// *************************************************************************

/*!
*/

int
QtNativePopupMenu::newMenuItem(
  const char * name,
  int itemid)
{
  int id = itemid;
  if (id == -1) {
    id = 1;
    while (this->getItemRecord(id) != NULL) id++;
  } else {
    assert(this->getItemRecord(itemid) == NULL &&
            "requested itemid already taken");
  }
  ItemRecord * rec = createItemRecord(name);
  rec->itemid = id;
  this->items->append(rec);
  return id;
} // newMenuItem()

/*!
*/

int
QtNativePopupMenu::getMenuItem(
  const char * name)
{
  const int numItems = this->items->getLength();
  int i;
  for (i = 0; i < numItems; i++) {
    ItemRecord * rec = (ItemRecord *) (*this->items)[i];
    if (strcmp(rec->name, name) == 0)
      return rec->itemid;
  }
  return -1;
} // getMenuItem()

/*!
*/

void
QtNativePopupMenu::setMenuItemTitle(
  int itemid,
  const char * title)
{
  ItemRecord * rec = this->getItemRecord(itemid);
  assert(rec && "no such menu");
  delete [] rec->title;
  rec->title = strcpy(new char [strlen(title)+1], title);
#if QT_VERSION >= 200
  if (rec->parent) {
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
    rec->parent->changeItem(rec->itemid, QString(rec->title));
#else
    //G.Barrand_Qt4 : FIXME
#endif
  }
#else // Qt version < 2.0.0
  // This QMenuData::changeItem() method is being obsoleted from Qt
  // from version 2.0.0 onwards.
  if (rec->parent)
    rec->parent->changeItem(rec->title, rec->itemid);
#endif // Qt version < 2.0.0
} // setMenuItemTitle()

/*!
*/

const char *
QtNativePopupMenu::getMenuItemTitle(
  int itemid)
{
  ItemRecord * rec = this->getItemRecord(itemid);
  assert(rec && "no such menu");
  return rec->title;
} // getMenuItemTitle()

/*!
*/

void
QtNativePopupMenu::setMenuItemEnabled(int itemid,
                                      SbBool enabled)
{
  ItemRecord * rec = this->getItemRecord(itemid);
  if (rec) {
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
    rec->parent->setItemEnabled(rec->itemid, enabled ? true : false);
#else
    //G.Barrand_Qt4 : FIXME
#endif
    return;
  }
  MenuRecord * mrec = this->getMenuRecord(itemid);
  assert(mrec && "no such menu");
  assert(mrec->parent && "a menuitem must have a parent to be enabled/disabled");
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
  mrec->parent->setItemEnabled(mrec->menuid, enabled ? true : false);
#else
  //G.Barrand_Qt4 : FIXME
#endif
} // setMenuItemEnabled()

/*!
*/

SbBool
QtNativePopupMenu::getMenuItemEnabled(int itemid) 
{
  ItemRecord * rec = this->getItemRecord(itemid);
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
  if (rec) return rec->parent->isItemEnabled(rec->itemid) ? TRUE : FALSE;
#else
  //G.Barrand_Qt4 : FIXME
  return FALSE;
#endif

  MenuRecord * mrec = this->getMenuRecord(itemid);
  assert(mrec && "no such menu");
  assert(mrec->parent && "a menuitem must have a parent to be enabled/disabled");
  
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
  return mrec->parent->isItemEnabled(mrec->menuid) ? TRUE : FALSE;
#else
  //G.Barrand_Qt4 : FIXME
  return FALSE;
#endif
} // getMenuItemEnabled()

/*!
*/

void
QtNativePopupMenu::_setMenuItemMarked(int itemid, SbBool marked)
{
  ItemRecord * rec = this->getItemRecord(itemid);
  if (rec == NULL)
    return;
  if (marked)
    rec->flags |= ITEM_MARKED;
  else
    rec->flags &= ~ITEM_MARKED;
  if (rec->parent != NULL) {
#if QT_VERSION >= 0x040000 // Qt 4.*
    // FIXME: is this really safe? (20050727 frodo)
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
    QAction * action = (QAction *) rec->parent->findItem(itemid);
    if (action) action->setChecked(marked ? true : false);
#else
    //G.Barrand_Qt4 : FIMXE
#endif
#else
    rec->parent->setItemChecked(rec->itemid, marked ? true : false);
#endif
  }
}

/*!
*/

SbBool
QtNativePopupMenu::getMenuItemMarked(
  int itemid)
{
  ItemRecord * rec = this->getItemRecord(itemid);
  assert(rec && "no such menu");
  if (rec->parent == NULL)
    return (rec->flags & ITEM_MARKED) ? TRUE : FALSE;
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
  return rec->parent->isItemChecked(rec->itemid) ? TRUE : FALSE;
#else
  //G.Barrand_Qt4 : FIXME
  return FALSE;
#endif
} // getMenuItemMarked()

// *************************************************************************

/*!
*/

void
QtNativePopupMenu::addMenu(int menuid,
                           int submenuid,
                           int pos)
{
  MenuRecord * super = this->getMenuRecord(menuid);
  MenuRecord * sub = this->getMenuRecord(submenuid);
  assert(super && sub && "no such menu");

  // disconnect the submenu from the activated() signal. In Qt 4.0
  // only the parent menu should be connected, since both the parent
  // menu and the submenu will generate a call to
  // itemActivation(). Multiple calls to itemActivation() causes a
  // segfault when selecting Quadbuffer stereo, at least when it's not
  // supported. (20050726 frodo)
#if QT_VERSION >= 0x040000 // Qt 4.*
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
  QObject::disconnect(sub->menu, SIGNAL(activated(int)),
                      this, SLOT(itemActivation(int)));
#else
  QObject::disconnect(sub->menu, SIGNAL(triggered(QAction*)),
                      this, SLOT(itemActivation(QAction*)));
#endif
#endif

#ifdef QT3_SUPPORT //G.Barrand_Qt4 
  if (pos == -1)
    super->menu->insertItem(QString(sub->title), sub->menu, sub->menuid);
  else 
    super->menu->insertItem(QString(sub->title),
                             sub->menu, sub->menuid, pos);
#else
  super->menu->addAction(sub->menu->menuAction());
#endif
  sub->parent = super->menu;
} // addMenu()

/*!
*/

void
QtNativePopupMenu::addMenuItem(int menuid,
                               int itemid,
                               int pos)
{
  MenuRecord * menu = this->getMenuRecord(menuid);
  assert(menu && "invalid parent menu id");
  ItemRecord * item = this->getItemRecord(itemid);
  assert(item && "invalid child menu id");

#ifdef QT3_SUPPORT //G.Barrand_Qt4 
  if (pos == -1)
    menu->menu->insertItem(QString(item->title), item->itemid);
  else
    menu->menu->insertItem(QString(item->title), item->itemid, pos);
#else
  item->action = menu->menu->addAction(item->title);
#endif
  item->parent = menu->menu;

#if QT_VERSION >= 0x040000 // Qt 4.*
  // FIXME: is this really safe? (20050726 frodo)
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
  QAction * action = (QAction *) item->parent->findItem(itemid);
  if (action) action->setCheckable(true);
#else
  //G.Barrand_Qt4 : FIXME
#endif
#endif // Qt 4.*
  
  if (item->flags & ITEM_MARKED) {
#if QT_VERSION >= 0x040000 // Qt 4.*
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
    if (action) action->setChecked(true);
#else
    //G.Barrand_Qt4 : FIXME
#endif
#else
    item->parent->setItemChecked(item->itemid, true);
#endif
  }
} // addMenuItem()

void
QtNativePopupMenu::addSeparator(int menuid,
                                int pos)
{
  MenuRecord * menu = this->getMenuRecord(menuid);
  assert(menu && "no such menu");
  
  ItemRecord * rec = createItemRecord("separator");
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
  menu->menu->insertSeparator(pos);
#else
  //G.Barrand_Qt4 : FIXME
#endif
  rec->flags |= ITEM_SEPARATOR;
  this->items->append(rec);
} // addSeparator()

/*!
  This method removes the submenu with the given \a menuid.

  A removed menu can be attached again later - its menuid will still be
  allocated.
*/

void
QtNativePopupMenu::removeMenu(int menuid)
{
  MenuRecord * rec = this->getMenuRecord(menuid);
  assert(rec && "no such menu");

  if (rec->menuid == 0) {
#if SOQT_DEBUG
    SoDebugError::postInfo("QtNativePopupMenu::RemoveMenu", "can't remove root");
#endif // SOQT_DEBUG
    return;
  }
  if (rec->parent == NULL) {
#if SOQT_DEBUG
    SoDebugError::postInfo("QtNativePopupMenu::RemoveMenu", "menu not attached");
#endif // SOQT_DEBUG
    return;
  }
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
  rec->parent->removeItem(rec->menuid);
#else
  //G.Barrand_Qt4 : FIXME
#endif
  rec->parent = NULL;
} // removeMenu()

/*!
  This method removes the menu item with the given \a itemid.

  A removed menu item can be attached again later - its itemid will still
  be allocated.
*/

void
QtNativePopupMenu::removeMenuItem(int itemid)
{
  ItemRecord * rec = this->getItemRecord(itemid);
  assert(rec && "no such item");

  if (rec->parent == NULL) {
#if SOQT_DEBUG
    SoDebugError::postInfo("QtNativePopupMenu::RemoveMenu", "item not attached");
#endif // SOQT_DEBUG
    return;
  }
#ifdef QT3_SUPPORT //G.Barrand_Qt4 
  rec->parent->removeItem(rec->itemid);
#else
  //G.Barrand_Qt4 : FIXME
#endif
  rec->parent = NULL;
} // removeMenuItem()

// *************************************************************************

// Doc in superclass.
void
QtNativePopupMenu::popUp(QWidget * inside, int x, int y)
{
  MenuRecord * rec = this->getMenuRecord(0);

  // Use exec() and not popup(). popup() doesn't seem to work properly
  // with a GL widget (the GL-redraw seems to overwrite the popup
  // widget or something). pederb, 2003-10-01 
  //
  // An additional piece of information: the popup-menu misbehavior is
  // that at first attempt just a "shadow" comes up, and then on the
  // second RMB-press the menu actually comes up. This problem has
  // only been seen on a few Windows 2000 platforms, though, so in
  // case we ever need to revert to using popup() again, make sure the
  // code is properly tested over a wide range of platforms.
  // mortene, 2003-10-01
  //
  // Ignore return value. We use callbacks.
  (void) rec->menu->exec(inside->mapToGlobal(QPoint(x, y)));
}

// *************************************************************************

/*!
*/

MenuRecord *
QtNativePopupMenu::getMenuRecord(
  int menuid)
{
  const int numMenus = this->menus->getLength();
  int i;
  for (i = 0; i < numMenus; i++)
    if (((MenuRecord *) (*this->menus)[i])->menuid == menuid)
      return (MenuRecord *) (*this->menus)[i];
  return (MenuRecord *) NULL;
} // getMenuRecord()

/*!
*/
ItemRecord *
QtNativePopupMenu::getItemRecord(int itemid)
{
  const int numItems = this->items->getLength();
  for (int i = 0; i < numItems; i++) {
    const int recid = ((ItemRecord *) (*this->items)[i])->itemid;
    if (recid == itemid) { return (ItemRecord *) (*this->items)[i]; }
  }

  return (ItemRecord *) NULL;
}

// *************************************************************************

/*!
*/

MenuRecord *
QtNativePopupMenu::createMenuRecord(
  const char * name)
{
  MenuRecord * rec = new MenuRecord;
  rec->menuid = -1;
  rec->name = strcpy(new char [strlen(name)+1], name);
  rec->title = strcpy(new char [strlen(name)+1], name);

#if QT_VERSION >= 0x040000 // Qt 4.*
  rec->menu = new QPOPUPMENU_CLASS(QString(name));
#else
  rec->menu = new QPOPUPMENU_CLASS((QWidget *) NULL, name);
#endif

#ifdef QT3_SUPPORT //G.Barrand_Qt4 
  QObject::connect(rec->menu, SIGNAL(activated(int)),
                   this, SLOT(itemActivation(int)));
#else
  //G.Barrand_Qt4 : FIXME
  QObject::connect(rec->menu, SIGNAL(triggered(QAction*)),
                   this, SLOT(itemActivation(QAction*)));
#endif

  rec->parent = NULL;
  return rec;
} // create()

/*!
*/

ItemRecord *
QtNativePopupMenu::createItemRecord(
  const char * name)
{
  ItemRecord * rec = new ItemRecord;
  rec->itemid = -1;
  rec->flags = 0;
  rec->name = strcpy(new char [strlen(name)+1], name);
  rec->title = strcpy(new char [strlen(name)+1], name);
  rec->parent = NULL;
  rec->action = 0;
  return rec;
} // create()

// *************************************************************************

#ifdef QT3_SUPPORT //G.Barrand_Qt4 
void
QtNativePopupMenu::itemActivation(// private slot
  int itemid)
{
  inherited::invokeMenuSelection(itemid);
} // menuSelection()
#else

ItemRecord *
QtNativePopupMenu::getItemRecord(QAction* action)
{
  const int numItems = this->items->getLength();
  for (int i = 0; i < numItems; i++) {
    QAction* act = ((ItemRecord *) (*this->items)[i])->action;
    if (act == action) { return (ItemRecord *) (*this->items)[i]; }
  }

  return (ItemRecord *) NULL;
}


void QtNativePopupMenu::itemActivation(QAction* action){
  ItemRecord* item = getItemRecord(action);
  if(item) inherited::invokeMenuSelection(item->itemid);
} // menuSelection()
#endif

// *************************************************************************
