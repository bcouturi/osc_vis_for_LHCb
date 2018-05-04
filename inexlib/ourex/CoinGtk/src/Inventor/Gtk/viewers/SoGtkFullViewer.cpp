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

// This file contains the generic, "templatize-able" parts of the
// So*FullViewer sourcecode.


// *************************************************************************

#include <Inventor/Gtk/viewers/SoGtkFullViewer.h>
#include <Inventor/Gtk/widgets/SoGtkPopupMenu.h>
#include <sogtkdefs.h>

#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/errors/SoDebugError.h>

/*!
  Set title of popup menu.
*/
void
SoGtkFullViewer::setPopupMenuString(const char * name)
{
  this->popupmenutitle = name;
  if (this->prefmenu) { this->prefmenu->setMenuTitle(ROOT_MENU, name); }
}

// ************************************************************************

SoGtkPopupMenu *
SoGtkFullViewer::setupStandardPopupMenu(
  void)
{
  SoGtkPopupMenu * menu = SoGtkPopupMenu::createInstance();

  menu->newMenu("rootmenu", ROOT_MENU);
  // FIXME: none of the So*-toolkits so far uses the rootmenu title to
  // set up a title string at the top of the root popupmenu. 20010810 mortene.
  menu->setMenuTitle(ROOT_MENU, this->popupmenutitle.getString());

#if 0 // OBSOLETED: this looks ugly and confusing for the user. 20010810 mortene.
  // Simple hack for a common / generic way of setting up a title for
  // the popupmenu.
  menu->newMenuItem("rootmenutitle", ROOT_MENU_TITLE);
  menu->setMenuItemTitle(ROOT_MENU_TITLE, this->popupmenutitle.getString());
  menu->addMenuItem(ROOT_MENU, ROOT_MENU_TITLE);
  menu->setMenuItemEnabled(ROOT_MENU_TITLE, FALSE);
  menu->addSeparator(ROOT_MENU);
#endif // OBSOLETED

  menu->newMenu("functionsmenu", FUNCTIONS_MENU);
  menu->setMenuTitle(FUNCTIONS_MENU, _("Functions"));
  menu->newMenu("drawstylesmenu", DRAWSTYLES_MENU);
  menu->setMenuTitle(DRAWSTYLES_MENU, _("Draw Styles"));
  menu->newMenu("stereomenu", STEREO_MENU);
  menu->setMenuTitle(STEREO_MENU, _("Stereo Viewing"));
  menu->newMenuItem("examining", EXAMINING_ITEM);
  menu->setMenuItemTitle(EXAMINING_ITEM, _("Viewing"));
  menu->newMenuItem("decoration", DECORATION_ITEM);
  menu->setMenuItemTitle(DECORATION_ITEM, _("Decorations"));
  menu->newMenuItem("headlight", HEADLIGHT_ITEM);
  menu->setMenuItemTitle(HEADLIGHT_ITEM, _("Headlight"));
  menu->newMenuItem("fullscreen", FULLSCREEN_ITEM);
  menu->setMenuItemTitle(FULLSCREEN_ITEM, _("Fullscreen"));
  menu->newMenuItem("preferences", PREFERENCES_ITEM);
  menu->setMenuItemTitle(PREFERENCES_ITEM, _("Preferences..."));

  menu->addMenu(ROOT_MENU, FUNCTIONS_MENU);
  menu->addMenu(ROOT_MENU, DRAWSTYLES_MENU);
  menu->addMenu(ROOT_MENU, STEREO_MENU);
  menu->addMenuItem(ROOT_MENU, EXAMINING_ITEM);
  menu->addMenuItem(ROOT_MENU, DECORATION_ITEM);
  menu->addMenuItem(ROOT_MENU, HEADLIGHT_ITEM);
  menu->addMenuItem(ROOT_MENU, FULLSCREEN_ITEM);
  menu->addSeparator(ROOT_MENU);
  menu->addMenuItem(ROOT_MENU, PREFERENCES_ITEM);

  menu->newMenuItem("help", HELP_ITEM);
  menu->setMenuItemTitle(HELP_ITEM, _("Help"));
  menu->newMenuItem("home", HOME_ITEM);
  menu->setMenuItemTitle(HOME_ITEM, _("Home"));
  menu->newMenuItem("set_home", SET_HOME_ITEM);
  menu->setMenuItemTitle(SET_HOME_ITEM, _("Set Home"));
  menu->newMenuItem("view_all", VIEW_ALL_ITEM);
  menu->setMenuItemTitle(VIEW_ALL_ITEM, _("View All"));
  menu->newMenuItem("seek", SEEK_ITEM);
  menu->setMenuItemTitle(SEEK_ITEM, _("Seek"));
  menu->newMenuItem("copy_view", COPY_VIEW_ITEM);
  menu->setMenuItemTitle(COPY_VIEW_ITEM, _("Copy View"));
  menu->newMenuItem("paste_view", PASTE_VIEW_ITEM);
  menu->setMenuItemTitle(PASTE_VIEW_ITEM, _("Paste View"));

  menu->addMenuItem(FUNCTIONS_MENU, HELP_ITEM);
  menu->addMenuItem(FUNCTIONS_MENU, HOME_ITEM);
  menu->addMenuItem(FUNCTIONS_MENU, SET_HOME_ITEM);
  menu->addMenuItem(FUNCTIONS_MENU, VIEW_ALL_ITEM);
  menu->addMenuItem(FUNCTIONS_MENU, SEEK_ITEM);
  menu->addSeparator(FUNCTIONS_MENU);
  menu->addMenuItem(FUNCTIONS_MENU, COPY_VIEW_ITEM);
  menu->addMenuItem(FUNCTIONS_MENU, PASTE_VIEW_ITEM);

  menu->newMenu("drawstylesstill", DRAWSTYLES_STILL_MENU);
  menu->setMenuTitle(DRAWSTYLES_STILL_MENU, _("Still Drawstyle"));

  menu->newMenuItem("as_is", AS_IS_ITEM);
  menu->setMenuItemTitle(AS_IS_ITEM, _("as is"));
  menu->newMenuItem("hidden_line", HIDDEN_LINE_ITEM);
  menu->setMenuItemTitle(HIDDEN_LINE_ITEM, _("hidden line"));
  menu->newMenuItem("wireframe_overlay", WIREFRAME_OVERLAY_ITEM);
  menu->setMenuItemTitle(WIREFRAME_OVERLAY_ITEM, _("wireframe overlay"));
  menu->newMenuItem("no_textures", NO_TEXTURE_ITEM);
  menu->setMenuItemTitle(NO_TEXTURE_ITEM, _("no texture"));
  menu->newMenuItem("low_resolution", LOW_RESOLUTION_ITEM);
  menu->setMenuItemTitle(LOW_RESOLUTION_ITEM, _("low resolution"));
  menu->newMenuItem("wireframe", WIREFRAME_ITEM);
  menu->setMenuItemTitle(WIREFRAME_ITEM, _("wireframe"));
  menu->newMenuItem("points", POINTS_ITEM);
  menu->setMenuItemTitle(POINTS_ITEM, _("points"));
  menu->newMenuItem("bounding_box", BOUNDING_BOX_ITEM);
  menu->setMenuItemTitle(BOUNDING_BOX_ITEM, _("bounding box (no depth)"));

  menu->newRadioGroup(STILL_GROUP);
  menu->addRadioGroupItem(STILL_GROUP, AS_IS_ITEM);
  menu->addRadioGroupItem(STILL_GROUP, HIDDEN_LINE_ITEM);
  menu->addRadioGroupItem(STILL_GROUP, WIREFRAME_OVERLAY_ITEM);
  menu->addRadioGroupItem(STILL_GROUP, NO_TEXTURE_ITEM);
  menu->addRadioGroupItem(STILL_GROUP, LOW_RESOLUTION_ITEM);
  menu->addRadioGroupItem(STILL_GROUP, WIREFRAME_ITEM);
  menu->addRadioGroupItem(STILL_GROUP, POINTS_ITEM);
  menu->addRadioGroupItem(STILL_GROUP, BOUNDING_BOX_ITEM);

  menu->addMenuItem(DRAWSTYLES_STILL_MENU, AS_IS_ITEM);
  menu->addMenuItem(DRAWSTYLES_STILL_MENU, HIDDEN_LINE_ITEM);
  menu->addMenuItem(DRAWSTYLES_STILL_MENU, WIREFRAME_OVERLAY_ITEM);
  menu->addMenuItem(DRAWSTYLES_STILL_MENU, NO_TEXTURE_ITEM);
  menu->addMenuItem(DRAWSTYLES_STILL_MENU, LOW_RESOLUTION_ITEM);
  menu->addMenuItem(DRAWSTYLES_STILL_MENU, WIREFRAME_ITEM);
  menu->addMenuItem(DRAWSTYLES_STILL_MENU, POINTS_ITEM);
  menu->addMenuItem(DRAWSTYLES_STILL_MENU, BOUNDING_BOX_ITEM);

  menu->newMenu("drawstylesanimation", DRAWSTYLES_ANIMATING_MENU);
  menu->setMenuTitle(DRAWSTYLES_ANIMATING_MENU, _("Animating Drawstyle"));

  menu->newMenuItem("move_same_as_still", MOVE_SAME_AS_STILL_ITEM);
  menu->setMenuItemTitle(MOVE_SAME_AS_STILL_ITEM, _("same as still"));
  menu->newMenuItem("move_no_texture", MOVE_NO_TEXTURE_ITEM);
  menu->setMenuItemTitle(MOVE_NO_TEXTURE_ITEM, _("no texture"));
  menu->newMenuItem("move_low_res", MOVE_LOW_RES_ITEM);
  menu->setMenuItemTitle(MOVE_LOW_RES_ITEM, _("low resolution"));
  menu->newMenuItem("move_wireframe", MOVE_WIREFRAME_ITEM);
  menu->setMenuItemTitle(MOVE_WIREFRAME_ITEM, _("wireframe"));
  menu->newMenuItem("move_low_res_wireframe", MOVE_LOW_RES_WIREFRAME_ITEM);
  menu->setMenuItemTitle(MOVE_LOW_RES_WIREFRAME_ITEM,
    _("low res wireframe (no depth)"));
  menu->newMenuItem("move_points", MOVE_POINTS_ITEM);
  menu->setMenuItemTitle(MOVE_POINTS_ITEM, _("points"));
  menu->newMenuItem("move_low_res_points", MOVE_LOW_RES_POINTS_ITEM);
  menu->setMenuItemTitle(MOVE_LOW_RES_POINTS_ITEM,
    _("low res points (no depth)"));
  menu->newMenuItem("move_bounding_box", MOVE_BOUNDING_BOX_ITEM);
  menu->setMenuItemTitle(MOVE_BOUNDING_BOX_ITEM, _("bounding box (no depth)"));

  menu->newRadioGroup(MOVE_GROUP);
  menu->addRadioGroupItem(MOVE_GROUP, MOVE_SAME_AS_STILL_ITEM);
  menu->addRadioGroupItem(MOVE_GROUP, MOVE_NO_TEXTURE_ITEM);
  menu->addRadioGroupItem(MOVE_GROUP, MOVE_LOW_RES_ITEM);
  menu->addRadioGroupItem(MOVE_GROUP, MOVE_WIREFRAME_ITEM);
  menu->addRadioGroupItem(MOVE_GROUP, MOVE_LOW_RES_WIREFRAME_ITEM);
  menu->addRadioGroupItem(MOVE_GROUP, MOVE_POINTS_ITEM);
  menu->addRadioGroupItem(MOVE_GROUP, MOVE_LOW_RES_POINTS_ITEM);
  menu->addRadioGroupItem(MOVE_GROUP, MOVE_BOUNDING_BOX_ITEM);

  menu->addMenuItem(DRAWSTYLES_ANIMATING_MENU, MOVE_SAME_AS_STILL_ITEM);
  menu->addMenuItem(DRAWSTYLES_ANIMATING_MENU, MOVE_NO_TEXTURE_ITEM);
  menu->addMenuItem(DRAWSTYLES_ANIMATING_MENU, MOVE_LOW_RES_ITEM);
  menu->addMenuItem(DRAWSTYLES_ANIMATING_MENU, MOVE_WIREFRAME_ITEM);
  menu->addMenuItem(DRAWSTYLES_ANIMATING_MENU, MOVE_LOW_RES_WIREFRAME_ITEM);
  menu->addMenuItem(DRAWSTYLES_ANIMATING_MENU, MOVE_POINTS_ITEM);
  menu->addMenuItem(DRAWSTYLES_ANIMATING_MENU, MOVE_LOW_RES_POINTS_ITEM);
  menu->addMenuItem(DRAWSTYLES_ANIMATING_MENU, MOVE_BOUNDING_BOX_ITEM);

  menu->newMenu("transparencytype", TRANSPARENCY_TYPE_MENU);
  menu->setMenuTitle(TRANSPARENCY_TYPE_MENU, _("Transparency Type"));
  menu->newRadioGroup(TRANSPARENCY_GROUP);

  { // Fill in menu items in radiomenu with transparency selections.

    struct menutransparencyitem {
      const char * name, * title;
      int id;
    };

    struct menutransparencyitem items[] = {
      { "screen_door", _("screen door"), SCREEN_DOOR_TRANSPARENCY_ITEM },
      { "add", _("add"), ADD_TRANSPARENCY_ITEM },
      { "delayed_add", _("delayed add"), DELAYED_ADD_TRANSPARENCY_ITEM },
      { "sorted_object_add", _("sorted object add"), SORTED_OBJECT_ADD_TRANSPARENCY_ITEM },
      { "blend", _("blend"), BLEND_TRANSPARENCY_ITEM },
      { "delayed_blend", _("delayed blend"), DELAYED_BLEND_TRANSPARENCY_ITEM },
      { "sorted_object_blend", _("sorted object blend"), SORTED_OBJECT_BLEND_TRANSPARENCY_ITEM }
#ifdef __COIN__ // Coin extensions
      ,
      { "sorted_object_sorted_triangle_add", _("sorted object sorted triangle add"), SORTED_OBJECT_SORTED_TRIANGLE_ADD_TRANSPARENCY_ITEM },
      { "sorted_object_sorted_triangle_blend", _("sorted object sorted triangle blend"), SORTED_OBJECT_SORTED_TRIANGLE_BLEND_TRANSPARENCY_ITEM }
#endif // __COIN__
    };

    int nritems = sizeof(items) / sizeof(struct menutransparencyitem);
    for (int i = 0; i < nritems; i++) {
      menu->newMenuItem(items[i].name, items[i].id);
      menu->setMenuItemTitle(items[i].id, items[i].title);
      menu->addRadioGroupItem(TRANSPARENCY_GROUP, items[i].id);
      menu->addMenuItem(TRANSPARENCY_TYPE_MENU, items[i].id);
    }
  }

  menu->newMenuItem("stereooff", STEREO_OFF_ITEM);
  menu->newMenuItem("stereoredcyan", STEREO_ANAGLYPH_ITEM);
  menu->newMenuItem("stereoquadbuffer", STEREO_QUADBUFFER_ITEM);
  menu->setMenuItemTitle(STEREO_OFF_ITEM, _("Off"));
  menu->setMenuItemTitle(STEREO_ANAGLYPH_ITEM, _("Red/Cyan"));
  menu->setMenuItemTitle(STEREO_QUADBUFFER_ITEM, _("Quad buffer"));

  menu->newRadioGroup(STEREO_GROUP);
  menu->addRadioGroupItem(STEREO_GROUP, STEREO_OFF_ITEM);
  menu->addRadioGroupItem(STEREO_GROUP, STEREO_ANAGLYPH_ITEM);
  menu->addRadioGroupItem(STEREO_GROUP, STEREO_QUADBUFFER_ITEM);

  menu->addMenuItem(STEREO_MENU, STEREO_OFF_ITEM);
  menu->addMenuItem(STEREO_MENU, STEREO_ANAGLYPH_ITEM);
  menu->addMenuItem(STEREO_MENU, STEREO_QUADBUFFER_ITEM);

  menu->newMenu("renderbuffertype", RENDER_BUFFER_TYPE_MENU);
  menu->setMenuTitle(RENDER_BUFFER_TYPE_MENU, _("Render Buffer Type"));

  menu->newMenuItem("single_buffer", SINGLE_BUFFER_ITEM);
  menu->setMenuItemTitle(SINGLE_BUFFER_ITEM, _("single"));
  menu->newMenuItem("double_buffer", DOUBLE_BUFFER_ITEM);
  menu->setMenuItemTitle(DOUBLE_BUFFER_ITEM, _("double"));
  menu->newMenuItem("interactive_buffer", INTERACTIVE_BUFFER_ITEM);
  menu->setMenuItemTitle(INTERACTIVE_BUFFER_ITEM, _("interactive"));

  menu->newRadioGroup(BUFFER_GROUP);
  menu->addRadioGroupItem(BUFFER_GROUP, SINGLE_BUFFER_ITEM);
  menu->addRadioGroupItem(BUFFER_GROUP, DOUBLE_BUFFER_ITEM);
  menu->addRadioGroupItem(BUFFER_GROUP, INTERACTIVE_BUFFER_ITEM);

  menu->addMenuItem(RENDER_BUFFER_TYPE_MENU, SINGLE_BUFFER_ITEM);
  menu->addMenuItem(RENDER_BUFFER_TYPE_MENU, DOUBLE_BUFFER_ITEM);
  menu->addMenuItem(RENDER_BUFFER_TYPE_MENU, INTERACTIVE_BUFFER_ITEM);

  menu->addMenu(DRAWSTYLES_MENU, DRAWSTYLES_STILL_MENU);
  menu->addMenu(DRAWSTYLES_MENU, DRAWSTYLES_ANIMATING_MENU);
  menu->addMenu(DRAWSTYLES_MENU, TRANSPARENCY_TYPE_MENU);
  menu->addMenu(DRAWSTYLES_MENU, RENDER_BUFFER_TYPE_MENU);

  int toggle;
  toggle = menu->newRadioGroup();
  menu->addRadioGroupItem(toggle, EXAMINING_ITEM);
  toggle = menu->newRadioGroup();
  menu->addRadioGroupItem(toggle, DECORATION_ITEM);
  toggle = menu->newRadioGroup();
  menu->addRadioGroupItem(toggle, HEADLIGHT_ITEM);
  toggle = menu->newRadioGroup();
  menu->addRadioGroupItem(toggle, FULLSCREEN_ITEM);

  menu->addMenuSelectionCallback(SoGtkFullViewer::menuSelectionCallback,
    (void *) this);

  this->prefmenu = menu;
  return menu;
} // buildPopupMenu()

// ************************************************************************

void
SoGtkFullViewer::prepareMenu(SoGtkPopupMenu * menu)
{
  //// Misc. //////////////////////////////////////////////////////////
  menu->setMenuItemMarked(DECORATION_ITEM, this->isDecoration());
  menu->setMenuItemMarked(EXAMINING_ITEM, this->isViewing());
  menu->setMenuItemMarked(HEADLIGHT_ITEM, this->isHeadlight());
  menu->setMenuItemMarked(FULLSCREEN_ITEM, this->isFullScreen());

  //// Basic drawstyles. //////////////////////////////////////////////
  this->setDrawStyleMenuActivation(SoGtkFullViewer::STILL,
                                   this->getDrawStyle(SoGtkFullViewer::STILL));
  this->setDrawStyleMenuActivation(SoGtkFullViewer::INTERACTIVE,
                                   this->getDrawStyle(SoGtkFullViewer::INTERACTIVE));

  //// Transparency setting. //////////////////////////////////////////
  switch (this->getTransparencyType()) {
  case SoGLRenderAction::SCREEN_DOOR:
    menu->setMenuItemMarked(SCREEN_DOOR_TRANSPARENCY_ITEM, TRUE);
    break;
  case SoGLRenderAction::ADD:
    menu->setMenuItemMarked(ADD_TRANSPARENCY_ITEM, TRUE);
    break;
  case SoGLRenderAction::DELAYED_ADD:
    menu->setMenuItemMarked(DELAYED_ADD_TRANSPARENCY_ITEM, TRUE);
    break;
  case SoGLRenderAction::SORTED_OBJECT_ADD:
    menu->setMenuItemMarked(SORTED_OBJECT_ADD_TRANSPARENCY_ITEM, TRUE);
    break;
  case SoGLRenderAction::BLEND:
    menu->setMenuItemMarked(BLEND_TRANSPARENCY_ITEM, TRUE);
    break;
  case SoGLRenderAction::DELAYED_BLEND:
    menu->setMenuItemMarked(DELAYED_BLEND_TRANSPARENCY_ITEM, TRUE);
    break;
  case SoGLRenderAction::SORTED_OBJECT_BLEND:
    menu->setMenuItemMarked(SORTED_OBJECT_BLEND_TRANSPARENCY_ITEM, TRUE);
    break;
#ifdef __COIN__  // Coin extensions
  case SoGLRenderAction::SORTED_OBJECT_SORTED_TRIANGLE_ADD:
    menu->setMenuItemMarked(SORTED_OBJECT_SORTED_TRIANGLE_ADD_TRANSPARENCY_ITEM, TRUE);
    break;
  case SoGLRenderAction::SORTED_OBJECT_SORTED_TRIANGLE_BLEND:
    menu->setMenuItemMarked(SORTED_OBJECT_SORTED_TRIANGLE_BLEND_TRANSPARENCY_ITEM, TRUE);
    break;
#endif // __COIN__
  default: assert(FALSE && "unknown transparency type");
  }

  //// Stereo viewing. ////////////////////////////////////////////////
  SbBool stereo = this->isStereoViewing();
  SbBool quadbuffer = this->isQuadBufferStereo();
 if (!stereo)
  menu->setMenuItemMarked(STEREO_OFF_ITEM, TRUE);
 if (stereo && !quadbuffer)
  menu->setMenuItemMarked(STEREO_ANAGLYPH_ITEM, TRUE);
 if (stereo && quadbuffer)
  menu->setMenuItemMarked(STEREO_QUADBUFFER_ITEM, TRUE);

  //// GL canvas settings. ////////////////////////////////////////////
  switch (this->getBufferingType()) {
  case SoGtkViewer::BUFFER_SINGLE:
    menu->setMenuItemMarked(SINGLE_BUFFER_ITEM, TRUE);
    break;
  case SoGtkViewer::BUFFER_DOUBLE:
    menu->setMenuItemMarked(DOUBLE_BUFFER_ITEM, TRUE);
    break;
  case SoGtkViewer::BUFFER_INTERACTIVE:
    menu->setMenuItemMarked(INTERACTIVE_BUFFER_ITEM, TRUE);
    break;
  default:
    assert(0 && "unsupported buffer type");
    break;
  }
}

// ************************************************************************

void
SoGtkFullViewer::menuSelection(// virtual
  int menuitemid)
{
  switch (menuitemid) {
  case -1:
    // means no item was selected
#if SOGTK_DEBUG
    SoDebugError::postInfo("SoGtkFullViewer::menuSelection",
      "-1 not appropriate on callback usage");
#endif // SOGTK_DEBUG
    break;

  case EXAMINING_ITEM:
    this->setViewing(this->isViewing() ? FALSE : TRUE);
    break;
  case DECORATION_ITEM:
    this->setDecoration(this->isDecoration() ? FALSE : TRUE);
    break;
  case HEADLIGHT_ITEM:
    this->setHeadlight(this->isHeadlight() ? FALSE : TRUE);
    break;
  case FULLSCREEN_ITEM:
    (void)this->setFullScreen(this->isFullScreen() ? FALSE : TRUE);
    break;
  case PREFERENCES_ITEM:
    this->selectedPrefs();
    break;

  case HELP_ITEM:
    this->openViewerHelpCard();
    break;
  case HOME_ITEM:
    this->resetToHomePosition();
    break;
  case SET_HOME_ITEM:
    this->saveHomePosition();
    break;
  case VIEW_ALL_ITEM:
    this->viewAll();
    break;
  case SEEK_ITEM:
    this->seekbuttonClicked();
    break;
  case COPY_VIEW_ITEM:
    this->copyView(SbTime::getTimeOfDay());

    break;
  case PASTE_VIEW_ITEM:
    this->pasteView(SbTime::getTimeOfDay());
    break;

  case AS_IS_ITEM:
  case HIDDEN_LINE_ITEM:
  case WIREFRAME_OVERLAY_ITEM:
  case NO_TEXTURE_ITEM:
  case LOW_RESOLUTION_ITEM:
  case WIREFRAME_ITEM:
  case POINTS_ITEM:
  case BOUNDING_BOX_ITEM:
    this->drawstyleActivated(menuitemid);
    break;

  case MOVE_SAME_AS_STILL_ITEM:
  case MOVE_NO_TEXTURE_ITEM:
  case MOVE_LOW_RES_ITEM:
  case MOVE_WIREFRAME_ITEM:
  case MOVE_LOW_RES_WIREFRAME_ITEM:
  case MOVE_POINTS_ITEM:
  case MOVE_LOW_RES_POINTS_ITEM:
  case MOVE_BOUNDING_BOX_ITEM:
    this->drawstyleActivated(menuitemid);
    break;

  case SCREEN_DOOR_TRANSPARENCY_ITEM:
  case ADD_TRANSPARENCY_ITEM:
  case DELAYED_ADD_TRANSPARENCY_ITEM:
  case SORTED_OBJECT_ADD_TRANSPARENCY_ITEM:
  case BLEND_TRANSPARENCY_ITEM:
  case DELAYED_BLEND_TRANSPARENCY_ITEM:
  case SORTED_OBJECT_BLEND_TRANSPARENCY_ITEM:
  case SORTED_OBJECT_SORTED_TRIANGLE_ADD_TRANSPARENCY_ITEM:
  case SORTED_OBJECT_SORTED_TRIANGLE_BLEND_TRANSPARENCY_ITEM:
    this->drawstyleActivated(menuitemid);
    this->scheduleRedraw();
    break;

  case SINGLE_BUFFER_ITEM:
  case DOUBLE_BUFFER_ITEM:
  case INTERACTIVE_BUFFER_ITEM:
    this->drawstyleActivated(menuitemid);
    break;

  case STEREO_OFF_ITEM:
    this->setStereoViewing(FALSE);
    this->setQuadBufferStereo(FALSE);
    break;
  case STEREO_ANAGLYPH_ITEM:
    this->setStereoViewing(TRUE);
    this->setQuadBufferStereo(FALSE);
    break;
  case STEREO_QUADBUFFER_ITEM:
    this->setStereoViewing(TRUE);
    this->setQuadBufferStereo(TRUE);
    if (!this->isQuadBufferStereo()) {
      this->setStereoViewing(FALSE);
      this->prefmenu->setMenuItemMarked(STEREO_OFF_ITEM, TRUE);
    }
    break;

  default:
    SoDebugError::postInfo("SoGtkFullViewer::menuSelection",
                           "popup menu handling for item %d is not implemented", 
                           menuitemid);
    break;
  } // switch (menuitemid)
} // menuSelection()


// ************************************************************************

void
SoGtkFullViewer::menuSelectionCallback(// static
  int menuitemid,
  void * userdata)
{
  SoGtkFullViewer * viewer = (SoGtkFullViewer *) userdata;
  viewer->menuSelection(menuitemid);
} // menuSelectionCallback()

// ************************************************************************

/*!
 \internal
*/

void
SoGtkFullViewer::setDrawStyleMenuActivation(
  SoGtkViewer::DrawType type,
  SoGtkViewer::DrawStyle value)
{
  assert(this->prefmenu != NULL);

  switch (type) {
  case SoGtkViewer::STILL:
    switch (value) {
    case SoGtkViewer::VIEW_AS_IS:
      this->prefmenu->setMenuItemMarked(AS_IS_ITEM, TRUE);
      break;
    case SoGtkViewer::VIEW_HIDDEN_LINE:
      this->prefmenu->setMenuItemMarked(HIDDEN_LINE_ITEM, TRUE);
      break;
    case SoGtkViewer::VIEW_WIREFRAME_OVERLAY:
      this->prefmenu->setMenuItemMarked(WIREFRAME_OVERLAY_ITEM, TRUE);
      break;
    case SoGtkViewer::VIEW_NO_TEXTURE:
      this->prefmenu->setMenuItemMarked(NO_TEXTURE_ITEM, TRUE);
      break;
    case SoGtkViewer::VIEW_LOW_COMPLEXITY:
      this->prefmenu->setMenuItemMarked(LOW_RESOLUTION_ITEM, TRUE);
      break;
    case SoGtkViewer::VIEW_LOW_RES_LINE:
#if SOGTK_DEBUG
      SoDebugError::postWarning("SoGtkFullViewer::setDrawStyleMenuActivation",
                                "Use VIEW_LINE, not VIEW_LOW_RES_LINE for the STILL drawstyle.");
#endif // debug
    case SoGtkViewer::VIEW_LINE:
      this->prefmenu->setMenuItemMarked(WIREFRAME_ITEM, TRUE);
      break;
    case SoGtkViewer::VIEW_LOW_RES_POINT:
#if SOGTK_DEBUG
      SoDebugError::postWarning("SoGtkFullViewer::setDrawStyleMenuActivation",
                                "Use VIEW_POINT, not VIEW_LOW_RES_POINT for the STILL drawstyle.");
#endif // debug
    case SoGtkViewer::VIEW_POINT:
      this->prefmenu->setMenuItemMarked(POINTS_ITEM, TRUE);
      break;
    case SoGtkViewer::VIEW_BBOX:
      this->prefmenu->setMenuItemMarked(BOUNDING_BOX_ITEM, TRUE);
      break;
    default:
      assert(0 && "unsupported default switch-case");
      break;
    } // switch (value)
    break;

  case SoGtkViewer::INTERACTIVE:
    switch (value) {
    case SoGtkViewer::VIEW_SAME_AS_STILL:
      this->prefmenu->setMenuItemMarked(MOVE_SAME_AS_STILL_ITEM, TRUE);
      break;
    case SoGtkViewer::VIEW_NO_TEXTURE:
      this->prefmenu->setMenuItemMarked(MOVE_NO_TEXTURE_ITEM, TRUE);
      break;
    case SoGtkViewer::VIEW_LOW_COMPLEXITY:
      this->prefmenu->setMenuItemMarked(MOVE_LOW_RES_ITEM, TRUE);
      break;
    case SoGtkViewer::VIEW_LINE:
      this->prefmenu->setMenuItemMarked(MOVE_WIREFRAME_ITEM, TRUE);
      break;
    case SoGtkViewer::VIEW_LOW_RES_LINE:
      this->prefmenu->setMenuItemMarked(MOVE_LOW_RES_WIREFRAME_ITEM, TRUE);
      break;
    case SoGtkViewer::VIEW_POINT:
      this->prefmenu->setMenuItemMarked(MOVE_POINTS_ITEM, TRUE);
      break;
    case SoGtkViewer::VIEW_LOW_RES_POINT:
      this->prefmenu->setMenuItemMarked(MOVE_LOW_RES_POINTS_ITEM, TRUE);
      break;
    case SoGtkViewer::VIEW_BBOX:
      this->prefmenu->setMenuItemMarked(MOVE_BOUNDING_BOX_ITEM, TRUE);
      break;
    default:
      assert(0 && "unsupported default switch-case");
      break;
    } // switch (value)
    break;

  default:
    assert(0 && "unsupported default switch-case");
    break;
  } // switch (type)
} // setDrawStyleMenuActivation()

// ************************************************************************

/*!
  \internal
*/

void
SoGtkFullViewer::drawstyleActivated(
  int menuitemid)
{
  switch (menuitemid) {
  case SINGLE_BUFFER_ITEM:
    this->setBufferingType(SoGtkViewer::BUFFER_SINGLE);
    return;
  case DOUBLE_BUFFER_ITEM:
    this->setBufferingType(SoGtkViewer::BUFFER_DOUBLE);
    return;
  case INTERACTIVE_BUFFER_ITEM:
    this->setBufferingType(SoGtkViewer::BUFFER_INTERACTIVE);
    return;
  default:
    break;
  } // switch (menuitemid)

  switch (menuitemid) {
  case SCREEN_DOOR_TRANSPARENCY_ITEM:
    this->setTransparencyType(SoGLRenderAction::SCREEN_DOOR);
    return;
  case ADD_TRANSPARENCY_ITEM:
    this->setTransparencyType(SoGLRenderAction::ADD);
    return;
  case DELAYED_ADD_TRANSPARENCY_ITEM:
    this->setTransparencyType(SoGLRenderAction::DELAYED_ADD);
    return;
  case SORTED_OBJECT_ADD_TRANSPARENCY_ITEM:
    this->setTransparencyType(SoGLRenderAction::SORTED_OBJECT_ADD);
    return;
  case BLEND_TRANSPARENCY_ITEM:
    this->setTransparencyType(SoGLRenderAction::BLEND);
    return;
  case DELAYED_BLEND_TRANSPARENCY_ITEM:
    this->setTransparencyType(SoGLRenderAction::DELAYED_BLEND);
    return;
  case SORTED_OBJECT_BLEND_TRANSPARENCY_ITEM:
    this->setTransparencyType(SoGLRenderAction::SORTED_OBJECT_BLEND);
    return;
#ifdef __COIN__ // Coin extensions
  case SORTED_OBJECT_SORTED_TRIANGLE_ADD_TRANSPARENCY_ITEM:
    this->setTransparencyType(SoGLRenderAction::SORTED_OBJECT_SORTED_TRIANGLE_ADD);
    return;
  case SORTED_OBJECT_SORTED_TRIANGLE_BLEND_TRANSPARENCY_ITEM:
    this->setTransparencyType(SoGLRenderAction::SORTED_OBJECT_SORTED_TRIANGLE_BLEND);
    return;
#endif // __COIN__
  default:
    break;
  } // switch (menuitemid)

  SoGtkViewer::DrawStyle val = SoGtkViewer::VIEW_AS_IS;
  switch (menuitemid) {
  case AS_IS_ITEM:
    val = SoGtkViewer::VIEW_AS_IS;
    break;
  case HIDDEN_LINE_ITEM:
    val = SoGtkViewer::VIEW_HIDDEN_LINE;
    break;
  case WIREFRAME_OVERLAY_ITEM:
    val = SoGtkViewer::VIEW_WIREFRAME_OVERLAY;
    break;
  case NO_TEXTURE_ITEM:
    val = SoGtkViewer::VIEW_NO_TEXTURE;
    break;
  case LOW_RESOLUTION_ITEM:
    val = SoGtkViewer::VIEW_LOW_COMPLEXITY;
    break;
  case WIREFRAME_ITEM:
    val = SoGtkViewer::VIEW_LINE;
    break;
  case POINTS_ITEM:
    val = SoGtkViewer::VIEW_POINT;
    break;
  case BOUNDING_BOX_ITEM:
    val = SoGtkViewer::VIEW_BBOX;
    break;

  case MOVE_SAME_AS_STILL_ITEM:
    val = SoGtkViewer::VIEW_SAME_AS_STILL;
    break;
  case MOVE_NO_TEXTURE_ITEM:
    val = SoGtkViewer::VIEW_NO_TEXTURE;
    break;
  case MOVE_LOW_RES_ITEM:
    val = SoGtkViewer::VIEW_LOW_COMPLEXITY;
    break;
  case MOVE_WIREFRAME_ITEM:
    val = SoGtkViewer::VIEW_LINE;
    break;
  case MOVE_LOW_RES_WIREFRAME_ITEM:
    val = SoGtkViewer::VIEW_LOW_RES_LINE;
    break;
  case MOVE_POINTS_ITEM:
    val = SoGtkViewer::VIEW_POINT;
    break;
  case MOVE_LOW_RES_POINTS_ITEM:
    val = SoGtkViewer::VIEW_LOW_RES_POINT;
    break;
  case MOVE_BOUNDING_BOX_ITEM:
    val = SoGtkViewer::VIEW_BBOX;
    break;

  default:
    assert(0);
    break;
  } // switch (menuitemid)

  SoGtkViewer::DrawType type = SoGtkViewer::STILL;

  switch (menuitemid) {
  case AS_IS_ITEM:
  case HIDDEN_LINE_ITEM:
  case WIREFRAME_OVERLAY_ITEM:
  case NO_TEXTURE_ITEM:
  case LOW_RESOLUTION_ITEM:
  case WIREFRAME_ITEM:
  case POINTS_ITEM:
  case BOUNDING_BOX_ITEM:
    type = SoGtkViewer::STILL;
    break;

  case MOVE_SAME_AS_STILL_ITEM:
  case MOVE_NO_TEXTURE_ITEM:
  case MOVE_LOW_RES_ITEM:
  case MOVE_WIREFRAME_ITEM:
  case MOVE_LOW_RES_WIREFRAME_ITEM:
  case MOVE_POINTS_ITEM:
  case MOVE_LOW_RES_POINTS_ITEM:
  case MOVE_BOUNDING_BOX_ITEM:
    type = SoGtkViewer::INTERACTIVE;
    break;

  default:
    assert(0);
    break;
  } // switch (menuitemid)

  this->setDrawStyle(type, val);
} // drawstyleActivated()

// ************************************************************************

// Documented in superclass.
SbBool
SoGtkFullViewer::processSoEvent(const SoEvent * const ev)
{
  // We're in "interact" mode (ie *not* the camera modification mode),
  // so don't handle the event here. It should either be forwarded to
  // the scenegraph, or caught by SoGtkViewer::processSoEvent() if
  // it's an ESC press (to switch modes).
  if (!this->isViewing()) { return inherited::processSoEvent(ev); }

  // Note: with the original SGI InventorXt, the popup menu is also
  // activated by RMB clicks in non-viewing mode. We want to allow RMB
  // clicks to pass through to the scenegraph in non-viewing mode, so
  // we don't copy that behavior.

  if (ev->getTypeId().isDerivedFrom(SoMouseButtonEvent::getClassTypeId())) {
    SoMouseButtonEvent * const e = (SoMouseButtonEvent *) ev;
    if ((e->getButton() == SoMouseButtonEvent::BUTTON2)) {
      if (this->isPopupMenuEnabled()) {
        if (e->getState() == SoButtonEvent::DOWN) {
          this->openPopupMenu(e->getPosition());
        }
        // Steal all RMB-events if the viewer uses the popup-menu.
        return TRUE;
      }
    }
  }

  return inherited::processSoEvent(ev);
}


// FIXME: get rid of this hack and make this file truly
// "generic". 20020109 mortene.
#ifndef __COIN_SOWIN__

// *************************************************************************

// Doc in superclass.
void
SoGtkFullViewer::setComponentCursor(const SoGtkCursor & cursor)
{
  // Overridden to apply the new cursor only for the rendering canvas
  // widget. Otherwise, the default SoGtkComponent
  // setComponentCursor() method will set the cursor for the top-most
  // parent widget, which makes it affect all sub-widgets, like the
  // decorations stuff.

  SoGtkComponent::setWidgetCursor(this->getGLWidget(), cursor);
}

// *************************************************************************

/*!
  This method returns the native widget for the label below the left
  thumb wheel.
*/

GtkWidget *
SoGtkFullViewer::getLeftWheelLabelWidget(void) const
{
  return this->leftWheelLabel;
}

/*!
  This method returns the native widget for the label below the right thumb
  wheel.
*/
GtkWidget *
SoGtkFullViewer::getRightWheelLabelWidget(void) const
{
  return this->rightWheelLabel;
}

/*!
  This method returns the native widget for the label beside the
  bottom thumb wheel.
*/
GtkWidget *
SoGtkFullViewer::getBottomWheelLabelWidget(void) const
{
  return this->bottomWheelLabel;
}

// *************************************************************************

#endif // ! __COIN_SOWIN__
