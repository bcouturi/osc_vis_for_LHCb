/**************************************************************************
 *
 *  This file is part of the Coin SoGtk GUI binding library.
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License version
 *  2.1 as published by the Free Software Foundation.  See the file
 *  LICENSE.LGPL at the root directory of the distribution for all the
 *  details.
 *
 *  If you want to use Coin SoGtk for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition License.
 *
 *  Systems in Motion, Prof Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
 **************************************************************************/

// 

#ifndef SOGTK_FULLVIEWER_H
#define SOGTK_FULLVIEWER_H

#include <Inventor/Gtk/viewers/SoGtkViewer.h>

// FIXME: get rid of non-templatized code. 20020108 mortene.
#ifdef __COIN_SOWIN__
typedef void AppPushButtonCB(HWND hwnd, int id, void * buttonData, void * userData);
typedef void RedrawAppPushButtonCB(LPDRAWITEMSTRUCT lpdis, void * userData);
typedef void CreateAppPushButtonCB(LPMEASUREITEMSTRUCT lpmis, void * userData);
#endif // ! __COIN_SOWIN__


class SoGtkPopupMenu;

// FIXME: do the following 2 enums really have to be public? 20020111 mortene.

enum SoGtkFullViewerPopupMenuItem {
  // FIXME: there's code in (some of) the popupmenu widgets which
  // assumes the root id is equal to 0.  Yup, lousy design. :-/
  // Anyway, don't change it until those cases are fixed.
  // 20010810 mortene.
  ROOT_MENU = 0,
  ROOT_MENU_TITLE,

  FUNCTIONS_MENU,
  HELP_ITEM,
  HOME_ITEM,
  SET_HOME_ITEM,
  VIEW_ALL_ITEM,
  SEEK_ITEM,
  COPY_VIEW_ITEM,
  PASTE_VIEW_ITEM,

  DRAWSTYLES_MENU,
  DRAWSTYLES_STILL_MENU,
  AS_IS_ITEM,
  HIDDEN_LINE_ITEM,
  WIREFRAME_OVERLAY_ITEM,
  NO_TEXTURE_ITEM,
  LOW_RESOLUTION_ITEM,
  WIREFRAME_ITEM,
  POINTS_ITEM,
  BOUNDING_BOX_ITEM,

  DRAWSTYLES_ANIMATING_MENU,
  MOVE_SAME_AS_STILL_ITEM,
  MOVE_NO_TEXTURE_ITEM,
  MOVE_LOW_RES_ITEM,
  MOVE_WIREFRAME_ITEM,
  MOVE_LOW_RES_WIREFRAME_ITEM,
  MOVE_POINTS_ITEM,
  MOVE_LOW_RES_POINTS_ITEM,
  MOVE_BOUNDING_BOX_ITEM,

  TRANSPARENCY_TYPE_MENU,
  SCREEN_DOOR_TRANSPARENCY_ITEM,
  ADD_TRANSPARENCY_ITEM,
  DELAYED_ADD_TRANSPARENCY_ITEM,
  SORTED_OBJECT_ADD_TRANSPARENCY_ITEM,
  BLEND_TRANSPARENCY_ITEM,
  DELAYED_BLEND_TRANSPARENCY_ITEM,
  SORTED_OBJECT_BLEND_TRANSPARENCY_ITEM,
  // These two are Coin extensions.
  SORTED_OBJECT_SORTED_TRIANGLE_ADD_TRANSPARENCY_ITEM,
  SORTED_OBJECT_SORTED_TRIANGLE_BLEND_TRANSPARENCY_ITEM,

  RENDER_BUFFER_TYPE_MENU,
  SINGLE_BUFFER_ITEM,
  DOUBLE_BUFFER_ITEM,
  INTERACTIVE_BUFFER_ITEM,

  EXAMINING_ITEM,
  DECORATION_ITEM,
  HEADLIGHT_ITEM,
  FULLSCREEN_ITEM,
  PREFERENCES_ITEM,

  STEREO_MENU,
  STEREO_OFF_ITEM,
  STEREO_ANAGLYPH_ITEM,
  STEREO_QUADBUFFER_ITEM
};

enum SoGtkViewerPopupMenuRadioGroup {
  STILL_GROUP = 1,
  MOVE_GROUP,
  TRANSPARENCY_GROUP,
  BUFFER_GROUP,
  STEREO_GROUP
};


// *************************************************************************

class SOGTK_DLL_API SoGtkFullViewer : public SoGtkViewer {
  SOGTK_OBJECT_ABSTRACT_HEADER(SoGtkFullViewer, SoGtkViewer);
  

public:
  enum BuildFlag {
    BUILD_NONE       = 0x00,
    BUILD_DECORATION = 0x01,
    BUILD_POPUP      = 0x02,
    BUILD_ALL        = (BUILD_DECORATION | BUILD_POPUP)
  };

  void setDecoration(const SbBool on);
  SbBool isDecoration(void) const;

  void setPopupMenuEnabled(const SbBool on);
  SbBool isPopupMenuEnabled(void) const;

  GtkWidget * getAppPushButtonParent(void) const;
  void addAppPushButton(GtkWidget * newButton);
  void insertAppPushButton(GtkWidget * newButton, int index);
  void removeAppPushButton(GtkWidget * oldButton);
  int findAppPushButton(GtkWidget * oldButton) const;
  int lengthAppPushButton(void) const;

  GtkWidget * getRenderAreaWidget(void) const;

  virtual void setViewing(SbBool on);
  virtual void setCamera(SoCamera * cam);
  virtual void hide(void);

  virtual void setComponentCursor(const SoGtkCursor & cursor);

protected:
  SoGtkFullViewer(GtkWidget * parent,
                    const char * name,
                    SbBool embed,
                    BuildFlag flag,
                    Type type,
                    SbBool build);
  ~SoGtkFullViewer();

  virtual void sizeChanged(const SbVec2s & size);

  GtkWidget * buildWidget(GtkWidget * parent);

  virtual void buildDecoration(GtkWidget * parent);
  virtual GtkWidget * buildLeftTrim(GtkWidget * parent);
  virtual GtkWidget * buildBottomTrim(GtkWidget * parent);
  virtual GtkWidget * buildRightTrim(GtkWidget * parent);
  GtkWidget * buildAppButtons(GtkWidget * parent);
  GtkWidget * buildViewerButtons(GtkWidget * parent);
  virtual void createViewerButtons(GtkWidget * parent, SbPList * buttonlist);

  virtual void buildPopupMenu(void);
  void setPopupMenuString(const char * title);
  void openPopupMenu(const SbVec2s position);

  void setPrefSheetString(const char * title);

  virtual void leftWheelStart(void);
  virtual void leftWheelMotion(float);
  virtual void leftWheelFinish(void);
  float getLeftWheelValue(void) const;
  void setLeftWheelValue(const float value);

  virtual void bottomWheelStart(void);
  virtual void bottomWheelMotion(float);
  virtual void bottomWheelFinish(void);
  float getBottomWheelValue(void) const;
  void setBottomWheelValue(const float value);

  virtual void rightWheelStart(void);
  virtual void rightWheelMotion(float);
  virtual void rightWheelFinish(void);
  float getRightWheelValue(void) const;
  void setRightWheelValue(const float value);

  void setLeftWheelString(const char * const name);
  GtkWidget * getLeftWheelLabelWidget(void) const;
  void setBottomWheelString(const char * const name);
  GtkWidget * getBottomWheelLabelWidget(void) const;
  void setRightWheelString(const char * const name);
  GtkWidget * getRightWheelLabelWidget(void) const;

  virtual SbBool processSoEvent(const SoEvent * const event);

  virtual void openViewerHelpCard(void);

protected:
  GtkWidget * leftDecoration;
  GtkWidget * rightDecoration;
  GtkWidget * bottomDecoration;

  GtkWidget * leftWheelLabel;
  char * leftWheelStr;
  float leftWheelVal;

  GtkWidget * rightWheelLabel;
  char * rightWheelStr;
  float rightWheelVal;

  GtkWidget * bottomWheelLabel;
  char * bottomWheelStr;
  float bottomWheelVal;

  SoGtkPopupMenu * prefmenu;

private:
  SoGtkPopupMenu * setupStandardPopupMenu(void);
  void prepareMenu(SoGtkPopupMenu * menu);

  void setDrawStyleMenuActivation(SoGtkViewer::DrawType type,
                                  SoGtkViewer::DrawStyle value);

  void drawstyleActivated(int menuitemid);

  void menuSelection(int menuitemid);
  static void menuSelectionCallback(int menuitemid, void * userdata);

  SbString popupmenutitle;


  // Private class for implementation hiding. The idiom we're using is
  // a variant of what is known as the "Cheshire Cat", and is also
  // described as the "Bridge" pattern in �Design Patterns� by Gamma
  // et al (aka The Gang Of Four).
  class SoGtkFullViewerP * pimpl;
  friend class SoGtkFullViewerP;


// FIXME: get rid of non-templatized code. 20020108 mortene.

#ifdef __COIN_SOQT__
protected:
  GtkWidget * leftWheel;
  GtkWidget * rightWheel;
  GtkWidget * bottomWheel;

  virtual bool eventFilter(QObject * obj, QEvent * e);
  virtual void afterRealizeHook(void);
  virtual GtkWidget * makeSubPreferences(GtkWidget * parent);

private:
  GtkWidget * makePreferencesWindow(void);
  GtkWidget * makeSeekPreferences(GtkWidget * parent);
  GtkWidget * makeSeekDistancePreferences(GtkWidget * parent);
  GtkWidget * makeZoomPreferences(GtkWidget * parent);
  GtkWidget * makeAutoclipPreferences(GtkWidget * parent);

  void showDecorationWidgets(SbBool onOff);

  void layoutAppButtons(GtkWidget * form);

  void setCameraZoom(const float zoom);
  float getCameraZoom(void);
  void setZoomSliderPosition(float zoom);
  void setZoomFieldString(float zoom);

  void setEnabledClippingWidgets(SbBool flag);

private slots:
  // Thumbwheels.
  void leftWheelPressed(void);
  void leftWheelChanged(float value);
  void leftWheelReleased(void);
  void rightWheelPressed(void);
  void rightWheelChanged(float value);
  void rightWheelReleased(void);
  void bottomWheelPressed(void);
  void bottomWheelChanged(float value);
  void bottomWheelReleased(void);

  // Button row.
  void interactbuttonClicked();
  void viewbuttonClicked();
  void helpbuttonClicked();
  void homebuttonClicked();
  void sethomebuttonClicked();
  void viewallbuttonClicked();
  void seekbuttonClicked();

  // Menu items.
  void selectedViewing();
  void selectedDecoration();
  void selectedHeadlight();
  void copyviewSelected();
  void pasteviewSelected();
  void selectedPrefs();

  // Pref sheet.
  //  seek settings
  void seekAnimationTimeChanged(const char *);
  void seekAnimationTimeChanged(const QString &);
  void seekDetailToggled(int id);
  void seekDistanceWheelChanged(float);
  void seekDistanceEdit();
  void seekDistanceTypeToggle(int id);
  //  zoom settings
  void zoomSliderMoved(int val);
  void zoomFieldChanged();
  void zoomRangeChanged1();
  void zoomRangeChanged2();
  //  clipping settings
  void clippingToggled(bool flag);
  void nearclippingwheelMoved(float val);
  void farclippingwheelMoved(float val);
  void nearclipEditPressed();
  void farclipEditPressed();

  // Generic slots.
  void increaseInteractiveCount();
  void decreaseInteractiveCount();
#endif // __COIN_SOQT__

#ifdef __COIN_SOXT__ // FIXME: get rid of non-templatized code. 20020108 mortene.
protected:
  GtkWidget * leftWheel;
  GtkWidget * rightWheel;
  GtkWidget * bottomWheel;

  Widget buildAppButtonsForm(Widget parent);

  Widget buildFunctionsSubmenu(Widget popup);
  Widget buildDrawStyleSubmenu(Widget popup);

  static void leftWheelStartCB(Widget, XtPointer, XtPointer);
  static void leftWheelMotionCB(Widget, XtPointer, XtPointer);
  static void leftWheelFinishCB(Widget, XtPointer, XtPointer);

  static void bottomWheelStartCB(Widget, XtPointer, XtPointer);
  static void bottomWheelMotionCB(Widget, XtPointer, XtPointer);
  static void bottomWheelFinishCB(Widget, XtPointer, XtPointer);

  static void rightWheelStartCB(Widget, XtPointer, XtPointer);
  static void rightWheelMotionCB(Widget, XtPointer, XtPointer);
  static void rightWheelFinishCB(Widget, XtPointer, XtPointer);

  char * prefstring;
  char * popupTitle;
  SbBool popupEnabled;
  SbPList * viewerButtonWidgets;

  Pixmap createPixmapFromXpmData(Widget button, char ** xpm);
  Pixmap createInsensitivePixmapFromXpmData(Widget button, char ** xpm);
  Widget prefshell;
  Widget prefsheet;

  virtual void createPrefSheet(void);
  void createPrefSheetShellAndForm(Widget & shell, Widget & form);
  void createDefaultPrefSheetParts(Widget * widgets, int & num, Widget form);
  void layoutPartsAndMapPrefSheet(Widget * widgets, int num, Widget form, Widget shell);

  Widget createSeekPrefSheetGuts(Widget parent);
  Widget createSeekDistPrefSheetGuts(Widget parent);
  Widget createZoomPrefSheetGuts(Widget parent);
  Widget createClippingPrefSheetGuts(Widget parent);
  Widget createStereoPrefSheetGuts(Widget parent);
  Widget createSpeedPrefSheetGuts(Widget parent);

  Widget createFramedSeekPrefSheetGuts(Widget parent);
  Widget createFramedSeekDistPrefSheetGuts(Widget parent);
  Widget createFramedZoomPrefSheetGuts(Widget parent);
  Widget createFramedClippingPrefSheetGuts(Widget parent);
  Widget createFramedStereoPrefSheetGuts(Widget parent);
  Widget createFramedSpeedPrefSheetGuts(Widget parent);

  void prefSheetDestroy(void);
  static void prefSheetDestroyCB(Widget, XtPointer, XtPointer);
  void seekPrefSheetField(void);
  static void seekPrefSheetFieldCB(Widget, XtPointer, XtPointer);
  void seekPrefSheetToggle1(void);
  static void seekPrefSheetToggle1CB(Widget, XtPointer, XtPointer);
  void seekPrefSheetToggle2(void);
  static void seekPrefSheetToggle2CB(Widget, XtPointer, XtPointer);

private:
  void selectedViewing(void);
  void selectedDecoration(void);
  void selectedHeadlight(void);
  void selectedPrefs(void);
  void interactbuttonClicked(Boolean set);
  static void interactbuttonCB(Widget, XtPointer, XtPointer);
  void examinebuttonClicked(Boolean set);
  static void examinebuttonCB(Widget, XtPointer, XtPointer);
  void helpbuttonClicked(void);
  static void helpbuttonCB(Widget, XtPointer, XtPointer);
  void homebuttonClicked(void);
  static void homebuttonCB(Widget, XtPointer, XtPointer);
  void sethomebuttonClicked(void);
  static void sethomebuttonCB(Widget, XtPointer, XtPointer);
  void viewallbuttonClicked(void);
  static void viewallbuttonCB(Widget, XtPointer, XtPointer);
  void seekbuttonClicked(void);
  static void seekbuttonCB(Widget, XtPointer, XtPointer);
  void copyviewSelected(void);
  void pasteviewSelected(void);

  Widget seektimefield;
  Widget pointtoggle, objecttoggle;
  void seektimechanged(void);
  static void seektimechangedCB(Widget, XtPointer, XtPointer);
  void pointtoggled(void);
  static void pointtoggledCB(Widget, XtPointer, XtPointer);
  void objecttoggled(void);
  static void objecttoggledCB(Widget, XtPointer, XtPointer);
  Widget seekdistancefield;
  Widget percenttoggle, absolutetoggle;
  void seekdistancechanged(void);
  static void seekdistancechangedCB(Widget, XtPointer, XtPointer);
  void percenttoggled(void);
  static void percenttoggledCB(Widget, XtPointer, XtPointer);
  void absolutetoggled(void);
  static void absolutetoggledCB(Widget, XtPointer, XtPointer);

  SbVec2f zoomrange;
  Widget zoomfrom, zoomslider, zoomto, zoomvalue;
  void zoomfromchanged(void);
  static void zoomfromchangedCB(Widget, XtPointer, XtPointer);
  void zoomtochanged(void);
  static void zoomtochangedCB(Widget, XtPointer, XtPointer);
  void zoomsliderchanged(void);
  static void zoomsliderchangedCB(Widget, XtPointer, XtPointer);
  void zoomvaluechanged(void);
  static void zoomvaluechangedCB(Widget, XtPointer, XtPointer);

  void setCameraZoom(float zoom);
  float getCameraZoom(void);

  // clipping preferences widgets
  Widget autocliptoggle, nearwheel, farwheel, nearvalue, farvalue;

  void autocliptoggled(void);
  static void autocliptoggledCB(Widget, XtPointer, XtPointer);
  void nearwheelvaluechanged(void);
  static void nearwheelvaluechangedCB(Widget, XtPointer, XtPointer);
  void farwheelvaluechanged(void);
  static void farwheelvaluechangedCB(Widget, XtPointer, XtPointer);
  void nearvaluechanged(void);
  static void nearvaluechangedCB(Widget, XtPointer, XtPointer);
  void farvaluechanged(void);
  static void farvaluechangedCB(Widget, XtPointer, XtPointer);

  Widget stereotoggle;
  void stereotoggled(void);
  static void stereotoggledCB(Widget, XtPointer, XtPointer);

  void increaseInteractiveCount(void);
  static void increaseInteractiveCountCB(Widget, XtPointer, XtPointer);
  void decreaseInteractiveCount(void);
  static void decreaseInteractiveCountCB(Widget, XtPointer, XtPointer);

  Widget incspeedbutton, decspeedbutton;
  void speedInc(void);
  static void speedIncCB(Widget, XtPointer, XtPointer);
  void speedDec(void);
  static void speedDecCB(Widget, XtPointer, XtPointer);

  SbBool decorations;

  Widget viewerbase, canvas;

  Widget mainlayout;
  void showDecorationWidgets(SbBool enable);

  Widget appButtonsForm;
  SbPList * appButtonsList;
  void layoutAppPushButtons(Widget parent);

  Widget viewerButtonsForm;

  struct {
    Widget pick, view, help, home, set_home, view_all, seek;
  } viewerbuttons;

  struct {
    Pixmap pick, pick_ins;
    Pixmap view, view_ins;
    Pixmap help, help_ins;
    Pixmap home, home_ins;
    Pixmap set_home, set_home_ins;
    Pixmap view_all, view_all_ins;
    Pixmap seek, seek_ins;
  } pixmaps;

  struct {
    Widget leftwheel, bottomwheel, rightwheel;
  } labels;

  float seekdistance;
  SbBool seekdistaspercentage;
#endif // __COIN_SOXT__

#ifdef __COIN_SOGTK__ // FIXME: get rid of non-templatized code. 20020108 mortene.
protected:
  GtkWidget * leftWheel;
  GtkWidget * rightWheel;
  GtkWidget * bottomWheel;

  virtual SbBool eventFilter(GtkWidget * object, GdkEvent * event);
  virtual GtkWidget * makeSubPreferences(GtkWidget * parent);

private:
  // hooks for SoAnyFullViewer
  // (FIXME: SoAnyFullViewer has been phased out. 20020111 mortene.)

  // menu selections
  void selectedPrefs(void);
  void selectedViewing(void);
  void selectedDecoration(void);
  void selectedHeadlight(void);
  void copyviewSelected(void);
  void pasteviewSelected(void);

  // button selections
  void interactbuttonClicked(void);
  void viewbuttonClicked(void);
  void helpbuttonClicked(void);
  void homebuttonClicked(void);
  void sethomebuttonClicked(void);
  void viewallbuttonClicked(void);
  void seekbuttonClicked(void);
#endif // __COIN_SOGTK__

#ifdef __COIN_SOWIN__ // FIXME: get rid of non-templatized code. 20020108 mortene.
public:
  enum ButtonId {
    VIEWERBUTTON_PICK,
    VIEWERBUTTON_VIEW,
    VIEWERBUTTON_HELP,
    VIEWERBUTTON_HOME,
    VIEWERBUTTON_SET_HOME,
    VIEWERBUTTON_VIEW_ALL,
    VIEWERBUTTON_SEEK
  };

  static void setDoButtonBar(SbBool enable);
  static SbBool isDoButtonBar(void);

  void addAppPushButtonCallback(AppPushButtonCB * callback,
                                 void * data = NULL);
  void addRedrawAppPushButtonCallback(RedrawAppPushButtonCB * callback,
                                       void * data = NULL);
  void addCreateAppPushButtonCallback(CreateAppPushButtonCB * callback,
                                       void * data = NULL);

  virtual void buildViewerButtonsEx(HWND parent, int x, int y, int size);
  virtual void destroyPopupMenu(void);
  virtual int displayPopupMenu(int x, int y, HWND owner);

  virtual void selectedPrefs(void);

  virtual LRESULT onCommand(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
  virtual LRESULT onDrawItem(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
  virtual LRESULT onMeasureItem(HWND window, UINT message, WPARAM wparam, LPARAM lparam);

  void seekbuttonClicked(void);

  class SoFieldSensor * zoomSensor;

  class SoWinThumbWheel * rightWheel;
  class SoWinThumbWheel * bottomWheel;
  class SoWinThumbWheel * leftWheel;

  SbVec2f zoomrange;
  class SoWinViewerPrefSheet * prefsheet;

  SbPList * viewerButtonList;
  SbPList * appButtonList;

  HWND viewerWidget;
  HWND renderAreaWidget;

protected:
  virtual void createPrefSheet(void);
  void showDecorationWidgets(SbBool onOff);

private:
  HWND buildLeftWheel(HWND parent);
  HWND buildBottomWheel(HWND parent);
  HWND buildRightWheel(HWND parent);

  float getCameraZoom(void);
  void setCameraZoom(float zoom);

  // FIXME: quick hack done while merging all the header file
  // definitions for So*FullViewer. 20020111 mortene.
  friend class SoWinViewerPrefSheet;
#endif // __COIN_SOWIN__
};

// *************************************************************************

#endif // ! SOGTK_FULLVIEWER_H
