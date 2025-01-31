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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/elements/SoLazyElement.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include <Inventor/Xt/common/gl.h>
#include <Inventor/Xt/SoAny.h>
#include <Inventor/Xt/nodes/SoGuiFrame.h>
#include <assert.h>

// *************************************************************************

class Frame {
public:
  SoGuiFrame * api;

  SoCoordinate3 * coords;
  SoMaterial * light;
  SoMaterial * shade;

  SoFieldSensor * size_sensor;
  SoFieldSensor * width_sensor;
  static void size_updated_cb(void * closure, SoSensor * sensor);

  SoFieldSensor * design_sensor;
  static void design_updated_cb(void * closure, SoSensor * sensor);
  SoFieldSensor * complement_sensor;

  static const char * scene[];
};

// *************************************************************************

#define PRIVATE(obj) ((Frame *)obj->internals)

void
SoGuiFrame::initClass(void)
{
  SO_KIT_INIT_CLASS(SoGuiFrame, SoBaseKit, "BaseKit");
}

SO_KIT_SOURCE(SoGuiFrame);

SoGuiFrame::SoGuiFrame(void)
{
  this->internals = new Frame;
  PRIVATE(this)->api = this;

  SO_KIT_CONSTRUCTOR(SoGuiFrame);

  SO_KIT_ADD_FIELD(size, (SbVec3f(0.0f, 0.0f, 0.0f)));
  SO_KIT_ADD_FIELD(design, (SoGuiFrame::BLACK));
  SO_KIT_ADD_FIELD(width, (1.0f));
  SO_KIT_ADD_FIELD(complement, (FALSE));

  SO_KIT_DEFINE_ENUM_VALUE(FrameDesign, BLACK);
  SO_KIT_DEFINE_ENUM_VALUE(FrameDesign, COLOR);
  SO_KIT_DEFINE_ENUM_VALUE(FrameDesign, EMBOSS);

  SO_KIT_SET_SF_ENUM_TYPE(design, FrameDesign);

  SO_KIT_ADD_CATALOG_ENTRY(scene, SoSeparator, TRUE, this, "", FALSE);

  SO_KIT_INIT_INSTANCE();

  SoNode * node = SoAny::loadSceneGraph(Frame::scene);
  assert(node);

  this->setAnyPart("scene", node);

  PRIVATE(this)->coords = (SoCoordinate3 *) SoAny::scanSceneForName(node, "coordinates");
  PRIVATE(this)->light = (SoMaterial *) SoAny::scanSceneForName(node, "light");
  PRIVATE(this)->shade = (SoMaterial *) SoAny::scanSceneForName(node, "shade");

  assert(PRIVATE(this)->coords);
  assert(PRIVATE(this)->light);
  assert(PRIVATE(this)->shade);
  
  PRIVATE(this)->coords->ref();
  PRIVATE(this)->light->ref();
  PRIVATE(this)->shade->ref();

  PRIVATE(this)->size_sensor = new SoFieldSensor(Frame::size_updated_cb, PRIVATE(this));
  PRIVATE(this)->size_sensor->attach(&(this->size));
  PRIVATE(this)->width_sensor = new SoFieldSensor(Frame::size_updated_cb, PRIVATE(this));
  PRIVATE(this)->width_sensor->attach(&(this->size));
  PRIVATE(this)->design_sensor = new SoFieldSensor(Frame::design_updated_cb, PRIVATE(this));
  PRIVATE(this)->design_sensor->attach(&(this->design));
  PRIVATE(this)->complement_sensor = new SoFieldSensor(Frame::design_updated_cb, PRIVATE(this));
  PRIVATE(this)->complement_sensor->attach(&(this->complement));
  this->design.touch();
}

SoGuiFrame::~SoGuiFrame(void)
{
  PRIVATE(this)->coords->unref();
  PRIVATE(this)->light->unref();
  PRIVATE(this)->shade->unref();
  delete PRIVATE(this)->size_sensor;
  delete PRIVATE(this)->width_sensor;
  delete PRIVATE(this)->design_sensor;
  delete PRIVATE(this)->complement_sensor;
  Frame * obj = PRIVATE(this);
  delete obj;
}

#undef PRIVATE

// *************************************************************************

#define PUBLIC(obj) ((SoGuiFrame *)obj->api)

const char *
Frame::scene[] =
{
  "#Inventor V2.1 ascii",
  "",
  "Separator {",
  "  DEF coordinates Coordinate3 {",
  "    point [",
  "      0 0 0,",
  "      1 0 0,",
  "      1 1 0,",
  "      0 1 0,",
  "      -1 -1 0,",
  "      2 -1 0,",
  "      2 2 0,",
  "      -1 2 0",
  "    ]",
  "  }",
  // shadow
  "  DEF shade Material {",
  "    diffuseColor 0.9 0.9 0.9",
  "  }",
  "  IndexedFaceSet {",
  "    coordIndex [",
  "      4 5 1 -1",
  "      4 1 0 -1",
  "      5 6 2 -1",
  "      5 2 1 -1",
  "    ]",
  "  }",
  // light
  "  DEF light Material {",
  "    diffuseColor 0.5 0.5 0.5",
  "  }",
  "  IndexedFaceSet {",
  "    coordIndex [",
  "      6 7 3 -1",
  "      6 3 2 -1",
  "      7 4 0 -1",
  "      7 0 3 -1",
  "    ]",
  "  }",
  "}",
  NULL
};

void
Frame::size_updated_cb(void * closure, SoSensor * sensor)
{
  assert(closure != NULL);
  Frame * me = (Frame *) closure;
  SoCoordinate3 * coords = me->coords;
  assert(coords);
  SbVec3f size = PUBLIC(me)->size.getValue();
  float width = PUBLIC(me)->width.getValue();
  SbBool save = coords->point.enableNotify(FALSE);
  coords->point.set1Value(1, SbVec3f(size[0], 0.0f, 0.0f));
  coords->point.set1Value(2, SbVec3f(size[0], size[1], 0.0f));
  coords->point.set1Value(3, SbVec3f(0.0f, size[1], 0.0f));
  coords->point.set1Value(4, SbVec3f(-width, -width, 0.0f));
  coords->point.set1Value(5, SbVec3f(size[0]+width, -width, 0.0f));
  coords->point.set1Value(6, SbVec3f(size[0]+width, size[1]+width, 0.0f));
  coords->point.set1Value(7, SbVec3f(-width, size[1]+width, 0.0f));
  coords->point.enableNotify(save);
  if ( save ) coords->point.touch();
}

void
Frame::design_updated_cb(void * closure, SoSensor * sensor)
{
  assert(closure != NULL);
  Frame * me = (Frame *) closure;
  SoMaterial * light = me->light;
  SoMaterial * shade = me->shade;
  assert(light);
  assert(shade);
  switch ( PUBLIC(me)->design.getValue() ) {
  case SoGuiFrame::EMBOSS:
    if ( PUBLIC(me)->complement.getValue() ) {
      light->diffuseColor.setValue(SbColor(0.5f, 0.5f, 0.5f));
      shade->diffuseColor.setValue(SbColor(0.9f, 0.9f, 0.9f));
    } else {
      light->diffuseColor.setValue(SbColor(0.9f, 0.9f, 0.9f));
      shade->diffuseColor.setValue(SbColor(0.5f, 0.5f, 0.5f));
    }
    break;
  case SoGuiFrame::BLACK:
    if ( PUBLIC(me)->complement.getValue() ) {
      light->diffuseColor.setValue(SbColor(1.0f, 0.0f, 0.0f));
      shade->diffuseColor.setValue(SbColor(1.0f, 0.0f, 0.0f));
    } else {
      light->diffuseColor.setValue(SbColor(0.0f, 0.0f, 0.0f));
      shade->diffuseColor.setValue(SbColor(0.0f, 0.0f, 0.0f));
    }
    break;
  default:
    break;
  }
}

#undef PUBLIC

// *************************************************************************
