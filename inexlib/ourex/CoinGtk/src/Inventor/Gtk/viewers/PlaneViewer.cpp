/**************************************************************************
 *
 *  This file is part of the Coin SoGtk GUI binding library.
 *  Copyright (C) 2000 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation.  See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin SoGtk for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition License.
 *
 *  Systems in Motion, Prof Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
 **************************************************************************/

#if SOGTK_DEBUG
static const char rcsid[] =
  "";
#endif // SOGTK_DEBUG

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/projectors/SbPlaneProjector.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoOrthographicCamera.h>

#include <Inventor/Gtk/widgets/gtkthumbwheel.h>

#include <sogtkdefs.h>
#include <Inventor/Gtk/viewers/SoGtkPlaneViewer.h>
#include <Inventor/Gtk/widgets/SoGtkViewerButtonP.h>

#include <Inventor/Gtk/SoGtkCursor.h>
#include <Inventor/Gtk/common/pixmaps/x.xpm>
#include <Inventor/Gtk/common/pixmaps/y.xpm>
#include <Inventor/Gtk/common/pixmaps/z.xpm>
#include <Inventor/Gtk/common/pixmaps/perspective.xpm>
#include <Inventor/Gtk/common/pixmaps/ortho.xpm>

// *************************************************************************

/*!
  \class SoGtkPlaneViewer Inventor/Gtk/viewers/SoGtkPlaneViewer.h
  \brief The SoGtkPlaneViewer class is for examining 3D models by moving the camera in orthogonal planes.
  \ingroup components viewers

  FIXME: write doc
*/

// *************************************************************************

class SoGtkPlaneViewerP {
public:
  SoGtkPlaneViewerP(SoGtkPlaneViewer * publ);
  ~SoGtkPlaneViewerP(void);

  SbPlaneProjector * projector;
  static void visibilityCB(void * data, SbBool visible);

  static SoGtkViewerButton SoGtkPlaneViewerButtons[];
  GdkPixmap * orthopixmap, * perspectivepixmap;
  GdkBitmap * orthomask, * perspectivemask;

  GtkWidget * cameratogglebutton;
  static void xbuttonCB(GtkWidget *, gpointer);
  static void ybuttonCB(GtkWidget *, gpointer);
  static void zbuttonCB(GtkWidget *, gpointer);
  static void camerabuttonCB(GtkWidget *, gpointer);

private:
  SoGtkPlaneViewer * pub;
};

#define PUBLIC(ptr) (ptr->pub)
#define PRIVATE(ptr) (ptr->pimpl)

#define THIS (PRIVATE(this))

// *************************************************************************

enum {
  X_BUTTON = 0,
  Y_BUTTON = 1,
  Z_BUTTON = 2,
  CAMERA_BUTTON = 3,

  FIRST_BUTTON = 0,
  LAST_BUTTON = CAMERA_BUTTON
};

struct SoGtkViewerButton
SoGtkPlaneViewerP::SoGtkPlaneViewerButtons[] = {
  { // plane X button
    N_("x"), "X",
    (GtkSignalFunc) SoGtkPlaneViewerP::xbuttonCB,
    x_xpm
  },
  { // plane Y button
    N_("y"), "Y",
    (GtkSignalFunc) SoGtkPlaneViewerP::ybuttonCB,
    y_xpm
  },
  { // plane Z button
    N_("z"), "Z",
    (GtkSignalFunc) SoGtkPlaneViewerP::zbuttonCB,
    z_xpm
  },
  { // camera type button
    N_("camera"), "C",
    (GtkSignalFunc) SoGtkPlaneViewerP::camerabuttonCB,
    perspective_xpm
  }
}; // SoGtkPlaneViewerButtons[]

// *************************************************************************

SOGTK_OBJECT_SOURCE(SoGtkPlaneViewer);

// *************************************************************************

/*!
  Public default constructor.
*/

SoGtkPlaneViewer::SoGtkPlaneViewer(// public
  GtkWidget * parent,
  const char * const name,
  const SbBool embed,
  const SoGtkFullViewer::BuildFlag flag,
  const SoGtkViewer::Type type)
: inherited(parent, name, embed, flag, type, FALSE)
{
  this->constructor(TRUE);
} // SoGtkPlaneViewer()

/*!
  Protected destructor used when deriving the SoGtkPlaneViewer component class.
*/

SoGtkPlaneViewer::SoGtkPlaneViewer(// protected
  GtkWidget * parent,
  const char * const name,
  const SbBool embed,
  const SoGtkFullViewer::BuildFlag flag,
  const SoGtkViewer::Type type,
  const SbBool build)
: inherited(parent, name, embed, flag, type, FALSE)
{
  this->constructor(build);
} // SoGtkPlaneViewer()

/*!
  Common code for all constructors.
*/

void
SoGtkPlaneViewer::constructor(// private
  const SbBool build)
{
  this->commonConstructor(); // set up generic stuff

  this->pimpl = new SoGtkPlaneViewerP(this);

  this->addVisibilityChangeCallback(SoGtkPlaneViewerP::visibilityCB, this);

  this->setClassName("SoGtkPlaneViewer");

  this->setPopupMenuString(_("Plane Viewer"));
  this->setPrefSheetString(_("Plane Viewer Preference Sheet")); 

  this->setLeftWheelString(_("Translate Y"));
  this->setBottomWheelString(_("Translate X"));
  this->setRightWheelString(_("Zoom"));

  if (! build) return;
  GtkWidget * viewer = this->buildWidget(this->getParentWidget());
  this->setBaseWidget(viewer);
//  this->setSize(SbVec2s(500, 460));
} // constructor()

/*!
  The destructor.
*/

SoGtkPlaneViewer::~SoGtkPlaneViewer(
  void)
{
  delete this->pimpl;
} // ~SoGtkPlaneViewer()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkPlaneViewer::setViewing(// virtual
  SbBool enable)
{
  inherited::setViewing(enable);
} // setViewing()

/*!
  FIXME: write doc
*/

void
SoGtkPlaneViewer::setCamera(// virtual
  SoCamera * newCamera)
{
  if (newCamera) {
    const SoType camtype = newCamera->getTypeId() ;
    const SbBool orthotype =
      camtype.isDerivedFrom(SoOrthographicCamera::getClassTypeId());

    this->setRightWheelString(orthotype ? _("Zoom") : _("Dolly"));
    if (PRIVATE(this)->cameratogglebutton) {
      GtkWidget * label = GTK_BIN(PRIVATE(this)->cameratogglebutton)->child;
      gtk_pixmap_set(GTK_PIXMAP(label),
        (orthotype ? PRIVATE(this)->orthopixmap : PRIVATE(this)->perspectivepixmap),
        (orthotype ? PRIVATE(this)->orthomask : PRIVATE(this)->perspectivemask));
    }
  }
  inherited::setCamera(newCamera);
} // setCamera()

/*!
  Decide whether or not the mouse pointer cursor should be visible in the
  rendering canvas.
*/
void
SoGtkPlaneViewer::setCursorEnabled(// virtual
  SbBool enable)
{
  inherited::setCursorEnabled(enable);
  this->setCursorRepresentation(this->mode);
} // setCursorEnabled()

// *************************************************************************

/*!
  FIXME: write doc
*/

GtkWidget *
SoGtkPlaneViewer::buildWidget(// protected
  GtkWidget * parent)
{
  GtkWidget * viewer = inherited::buildWidget(parent);
  gtk_thumbwheel_set_range_boundary_handling(
     GTK_THUMBWHEEL(this->leftWheel), GTK_THUMBWHEEL_BOUNDARY_ACCUMULATE);
  gtk_thumbwheel_set_range_boundary_handling(
     GTK_THUMBWHEEL(this->rightWheel), GTK_THUMBWHEEL_BOUNDARY_ACCUMULATE);
  
  return viewer;
} // buildWidget()

// *************************************************************************

/*!
  FIXME: write doc
*/

const char *
SoGtkPlaneViewer::getDefaultWidgetName(// virtual, protected
  void) const
{
  static const char defaultWidgetName[] = "SoGtkPlaneViewer";
  return defaultWidgetName;
}

/*!
  FIXME: write doc
*/

const char *
SoGtkPlaneViewer::getDefaultTitle(// virtual, protected
  void) const
{
  static const char defaultTitle[] = N_("Plane Viewer");
  return _(defaultTitle);
}

/*!
  FIXME: write doc
*/

const char *
SoGtkPlaneViewer::getDefaultIconTitle(// virtual, protected
  void) const
{
  static const char defaultIconTitle[] = N_("Plane Viewer");
  return _(defaultIconTitle);
}

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkPlaneViewer::bottomWheelMotion(// virtual, protected
  float value)
{
  this->translateX(value - this->getBottomWheelValue());
  inherited::bottomWheelMotion(value);
} // bottomWheelMotion()

/*!
  FIXME: write doc
*/

void
SoGtkPlaneViewer::leftWheelMotion(// virtual, protected
  float value)
{
  this->translateY(value - this->getLeftWheelValue());
  inherited::leftWheelMotion(value);
} // leftWheelMotion()

/*!
  FIXME: write doc
*/

void
SoGtkPlaneViewer::rightWheelMotion(// virtual, protected
  float value)
{
  this->zoom(this->getRightWheelValue() - value);
  inherited::rightWheelMotion(value);
} // rightWheelMotion()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkPlaneViewer::processEvent(// virtual, protected
  GdkEvent * event)
{
  // SoDebugError::postInfo("SoGtkPlaneViewer::processEvent", "[invoked]");
  if (SoGtkViewer::processCommonEvents(event))
    return;

  inherited::processEvent(event);
} // processEvent()

/*!
  FIXME: write doc
*/

void
SoGtkPlaneViewer::setSeekMode(// virtual, protected
  SbBool enable)
{
  inherited::setSeekMode(enable);
#if 0
  this->setMode(on ?
    SoGtkPlaneViewer::SEEK_WAIT_MODE :
    SoGtkPlaneViewer::EXAMINE);
#endif
} // setSeekMode()

/*!
  Overload this method to be able to draw the rotation graphics in rotation
  mode.
*/

void
SoGtkPlaneViewer::actualRedraw(// virtual, protected
  void)
{
  inherited::actualRedraw();
} // actualRedraw()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkPlaneViewer::createViewerButtons(// virtual, protected
  GtkWidget * parent,
  SbPList * buttonlist)
{
  inherited::createViewerButtons(parent, buttonlist);

  GtkTooltips * tooltips = gtk_tooltips_new();
  GdkColormap * colormap = gtk_widget_get_colormap(parent);

  const size_t buttons = sizeof(SoGtkPlaneViewerP::SoGtkPlaneViewerButtons)
                         / sizeof(SoGtkViewerButton);
  for (size_t button = 0; button < buttons; button++) {
    GtkWidget * widget = (GtkWidget*) 0 ;

    switch(button)
    {
    case CAMERA_BUTTON :
      PRIVATE(this)->cameratogglebutton = widget = gtk_button_new();
      break ;
    default: 
      widget = gtk_button_new();
      break ;
    }

    GTK_WIDGET_UNSET_FLAGS(widget,GTK_CAN_FOCUS);
#ifdef __APPLE__
    //G.Barrand : on SnowLeoparad with :
    //            port list gtk2 pango :
    //gtk2                           @2.16.6         x11/gtk2
    //pango                          @1.26.0         x11/pango
    // the tooltip induces an infinite loop in :
    ///  SubstLookupSubTable::apply ()
#else
    gtk_tooltips_set_tip(tooltips, widget, 
      _(SoGtkPlaneViewerP::SoGtkPlaneViewerButtons[button].keyword), NULL);
#endif

    GdkPixmap * gdk_pixmap = (GdkPixmap *) 0;
    GdkBitmap * gdk_mask   = (GdkBitmap *) 0;

    switch(button)
    {
    case CAMERA_BUTTON :
      {
        SoType t = this->getCameraType();
        if (t.isDerivedFrom(SoOrthographicCamera::getClassTypeId()))
        {
          gdk_pixmap = PRIVATE(this)->orthopixmap;
          gdk_mask   = PRIVATE(this)->orthomask;
        }
        else if (t.isDerivedFrom(SoPerspectiveCamera::getClassTypeId()))
        {
          gdk_pixmap = PRIVATE(this)->perspectivepixmap;
          gdk_mask   = PRIVATE(this)->perspectivemask;
        }
        else assert(0 && "unsupported cameratype");
      }
      break;
    default:
      gdk_pixmap =
        gdk_pixmap_colormap_create_from_xpm_d((GdkWindow *) 0, colormap,
          &gdk_mask, (GdkColor *) 0,
          SoGtkPlaneViewerP::SoGtkPlaneViewerButtons[button].xpm_data);
      break;
    }


    GtkWidget * label = gtk_pixmap_new(gdk_pixmap, gdk_mask);
    gtk_widget_show(label);

    switch(button)
    {
    case CAMERA_BUTTON:
      break ;
    default :
      gdk_pixmap_unref(gdk_pixmap);
      gdk_bitmap_unref(gdk_mask);
      break ;
    };

    gtk_container_add(GTK_CONTAINER(widget), GTK_WIDGET(label));
    if (SoGtkPlaneViewerP::SoGtkPlaneViewerButtons[button].pressed != NULL) {
      gtk_signal_connect(GTK_OBJECT(widget), "pressed",
        GTK_SIGNAL_FUNC(SoGtkPlaneViewerP::SoGtkPlaneViewerButtons[button].pressed),
        (gpointer) this);
    }
    buttonlist->append(widget);
  }
} // createViewerButtons()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkPlaneViewer::openViewerHelpCard(// virtual, protected
  void)
{
  this->openHelpCard("SoGtkPlaneViewer.help");
} // openViewerHelpCard()

// *************************************************************************

/*!
  \internal

  Set cursor graphics according to mode.
*/
void
SoGtkPlaneViewer::setCursorRepresentation(int mode)
{
  // FIXME: seems to never be called. 20011126 mortene.

  GtkWidget * w = this->getGLWidget();
  assert(w);

  if (! this->isCursorEnabled()) {
    this->setComponentCursor(SoGtkCursor::getBlankCursor());
    return;
  }

  switch ((SoGtkPlaneViewer::PlaneViewerMode) mode) {
//  case SoGtkPlaneViewer::INTERACT:
//  case SoGtkPlaneViewer::EXAMINE:
  case SoGtkPlaneViewer::IDLE_MODE:
    this->setComponentCursor(SoGtkCursor::getBlankCursor());
    break ;
  case SoGtkPlaneViewer::DOLLY_MODE:
    this->setComponentCursor(SoGtkCursor(SoGtkCursor::UPARROW));
    break;
  case SoGtkPlaneViewer::ROTZ_WAIT_MODE:
  case SoGtkPlaneViewer::ROTZ_MODE:
    this->setComponentCursor(SoGtkCursor::getRotateCursor());
    break;
  case SoGtkPlaneViewer::SEEK_WAIT_MODE:
  case SoGtkPlaneViewer::SEEK_MODE:
    this->setComponentCursor(SoGtkCursor(SoGtkCursor::CROSSHAIR));
    break;
  case SoGtkPlaneViewer::TRANSLATE_MODE:
    this->setComponentCursor(SoGtkCursor::getPanCursor());
    break;
  default: 
    assert(0); 
    break;
  }
} // setCursorRepresentation()

// ************************************************************************
//
//  Private implementation
//

/*
*/

SoGtkPlaneViewerP::SoGtkPlaneViewerP(
  SoGtkPlaneViewer * publ)
{
  this->pub = publ;

  this->projector = new SbPlaneProjector;
  SbViewVolume vv;
  vv.ortho(-1, 1, -1, 1, -1, 1);
  this->projector->setViewVolume(vv);

  GdkColormap * colormap = gtk_widget_get_colormap (PUBLIC(this)->getParentWidget());

  this->orthopixmap =
    gdk_pixmap_colormap_create_from_xpm_d((GdkWindow *) 0, colormap,
      &this->orthomask, (GdkColor *) 0, ortho_xpm);

  this->perspectivepixmap =
    gdk_pixmap_colormap_create_from_xpm_d((GdkWindow *) 0, colormap,
      &this->perspectivemask, (GdkColor *) 0, perspective_xpm);
} // SoGtkPlaneViewerP()

/*
*/

SoGtkPlaneViewerP::~SoGtkPlaneViewerP(void)
{
  // Button pixmaps.
  gdk_pixmap_unref(this->orthopixmap);
  gdk_bitmap_unref(this->orthomask);  
  gdk_pixmap_unref(this->perspectivepixmap);
  gdk_bitmap_unref(this->perspectivemask);  

  delete this->projector;
} // ~SoGtkPlaneViewerP()

// ************************************************************************

/*!
  \internal
*/

void
SoGtkPlaneViewerP::visibilityCB(// static
  void * closure,
  SbBool visible)
{
  SoGtkPlaneViewer * viewer = (SoGtkPlaneViewer *) closure;
  /* no animation to toggle */
} // visibilityCB()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoGtkPlaneViewerP::xbuttonCB(
   GtkWidget * button,
   gpointer closure)
{
  assert(closure != NULL);
  SoGtkPlaneViewer * viewer = (SoGtkPlaneViewer *) closure;

  viewer->viewPlaneX();
} // ybuttonCB()

/*
  FIXME: write doc
*/

void
SoGtkPlaneViewerP::ybuttonCB(
   GtkWidget * button,
   gpointer closure)
{
  assert(closure != NULL);
  SoGtkPlaneViewer * viewer = (SoGtkPlaneViewer *) closure;

  viewer->viewPlaneY();
} // ybuttonCB()

/*
  FIXME: write doc
*/

void
SoGtkPlaneViewerP::zbuttonCB(
   GtkWidget * button,
   gpointer closure)
{
  assert(closure != NULL);
  SoGtkPlaneViewer * viewer = (SoGtkPlaneViewer *) closure;

  viewer->viewPlaneZ();
} // zbuttonCB()

/*
  FIXME: write doc
*/

void
SoGtkPlaneViewerP::camerabuttonCB(
   GtkWidget * button,
   gpointer closure)
{
  assert(closure != NULL);
  SoGtkPlaneViewer * viewer = (SoGtkPlaneViewer *) closure;

  if (viewer->getCamera())
    viewer->toggleCameraType();
} // camerabuttonCB()

// ************************************************************************

void
SoGtkPlaneViewer::createPrefSheet(void)
{
  // FIXME: not yet implemented. 20020113 mortene.
//    inherited::createPrefSheet();
}

// ************************************************************************

#if SOGTK_DEBUG
static const char * getSoGtkPlaneViewerRCSId(void) { return rcsid; }
#endif // SOGTK_DEBUG

