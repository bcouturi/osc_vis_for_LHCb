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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <assert.h>

#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoNodeSensor.h>
#ifdef HAVE_SOVRMLMATERIAL
#include <Inventor/VRMLnodes/SoVRMLMaterial.h>
#endif // HAVE_SOVRMLMATERIAL

#include <Inventor/Qt/SoAny.h>
#include <Inventor/Qt/nodes/SoGuiPane.h>
#include <Inventor/Qt/nodes/SoGuiImage.h>
#include <Inventor/Qt/nodes/SoGuiSceneTexture2.h>
#include <Inventor/Qt/nodes/SoGuiSlider1.h>
#include <Inventor/Qt/nodes/SoGuiToggleButton.h>
#include <Inventor/Qt/nodes/SoGuiRadioButton.h>
#include <Inventor/Qt/nodes/SoGuiColorEditor.h>

#include <Inventor/Qt/nodes/SoGuiMaterialEditor.h>

// *************************************************************************

class MaterialEditor {
public:
  MaterialEditor(void);
  ~MaterialEditor(void);

  SoGuiMaterialEditor * api;

  SoMaterial * material;
  SoNodeSensor * material_sensor;
  static void material_cb(void * closure, SoSensor * sensor);

  SoFieldSensor * material_field_sensor;
  static void material_field_cb(void * closure, SoSensor * sensor);

  SoGuiToggleButton * button1;
  SoGuiToggleButton * button2;
  SoGuiToggleButton * button3;
  SoGuiToggleButton * button4;
  SoFieldSensor * button1_sensor;
  SoFieldSensor * button2_sensor;
  SoFieldSensor * button3_sensor;
  SoFieldSensor * button4_sensor;
  static void button1_cb(void * closure, SoSensor * sensor);
  static void button2_cb(void * closure, SoSensor * sensor);
  static void button3_cb(void * closure, SoSensor * sensor);
  static void button4_cb(void * closure, SoSensor * sensor);
  SoGuiRadioButton * radio1;
  SoGuiRadioButton * radio2;
  SoGuiRadioButton * radio3;
  SoGuiRadioButton * radio4;

  SoGuiSlider1 * slider1;
  SoGuiSlider1 * slider2;
  SoGuiSlider1 * slider3;
  SoGuiSlider1 * slider4;
  SoGuiSlider1 * slider5;
  SoGuiSlider1 * slider6;
  SoFieldSensor * ambient_slider_sensor;
  SoFieldSensor * diffuse_slider_sensor;
  SoFieldSensor * specular_slider_sensor;
  SoFieldSensor * emissive_slider_sensor;
  SoFieldSensor * shininess_slider_sensor;
  SoFieldSensor * transparency_slider_sensor;
  static void ambient_slider_cb(void * closure, SoSensor * sensor);
  static void diffuse_slider_cb(void * closure, SoSensor * sensor);
  static void specular_slider_cb(void * closure, SoSensor * sensor);
  static void emissive_slider_cb(void * closure, SoSensor * sensor);
  static void shininess_slider_cb(void * closure, SoSensor * sensor);
  static void transparency_slider_cb(void * closure, SoSensor * sensor);

  SoGuiColorEditorOpenCB * openeditorcb;
  SoGuiColorEditorCloseCB * closeeditorcb;
  void * cbclosure;

  static SoGuiColorEditor * coloreditoropen_cb(void * closure);
  static void coloreditorclose_cb(void * closure, SoGuiColorEditor * editor);

  SbBool coloreditoropen;
  SoGuiColorEditor * coloreditor;
  SoFieldSensor * coloreditor_sensor;
  static void coloreditor_cb(void * closure, SoSensor * sensor);

  void checkColorEditorOpenStatus(void);

  static const char * texturescene[];
  static const char * editorscene[];
};

// *************************************************************************

#define PRIVATE(obj) ((MaterialEditor *) ((SoGuiMaterialEditor *) obj)->internals)

void
SoGuiMaterialEditor::initClass(void)
{
  SO_KIT_INIT_CLASS(SoGuiMaterialEditor, SoBaseKit, "BaseKit");
}

SO_KIT_SOURCE(SoGuiMaterialEditor);

SoGuiMaterialEditor::SoGuiMaterialEditor(void)
{
  this->internals = new MaterialEditor;
  PRIVATE(this)->api = this;

  SO_KIT_CONSTRUCTOR(SoGuiMaterialEditor); 

  SO_KIT_ADD_FIELD(material, (NULL));
  SO_KIT_ADD_FIELD(index, (0));
  SO_KIT_ADD_FIELD(update, (CONTINUOUS));
      
  SO_KIT_DEFINE_ENUM_VALUE(Update, CONTINUOUS);
  SO_KIT_DEFINE_ENUM_VALUE(Update, AFTER_ACCEPT);

  SO_KIT_SET_SF_ENUM_TYPE(update, Update);

  SO_KIT_ADD_CATALOG_ENTRY(root, SoGuiPane, TRUE, this, "", FALSE);

  SO_KIT_INIT_INSTANCE(); 

  SoNode * scene = SoAny::loadSceneGraph(MaterialEditor::editorscene);
  assert(scene != NULL);
  assert(scene->isOfType(SoGuiPane::getClassTypeId()));
  scene->ref();

  SoGuiImage * sphereimage =
    (SoGuiImage *) SoAny::scanSceneForName(scene, "image");
  assert(sphereimage != NULL);
  scene->unrefNoDelete();
  SbBool ok = this->setAnyPart("root", scene);
  assert(ok);

  PRIVATE(this)->button1 = (SoGuiToggleButton *) SoAny::scanSceneForName(scene, "tbutton1");
  PRIVATE(this)->button2 = (SoGuiToggleButton *) SoAny::scanSceneForName(scene, "tbutton2");
  PRIVATE(this)->button3 = (SoGuiToggleButton *) SoAny::scanSceneForName(scene, "tbutton3");
  PRIVATE(this)->button4 = (SoGuiToggleButton *) SoAny::scanSceneForName(scene, "tbutton4");
  assert(PRIVATE(this)->button1);
  assert(PRIVATE(this)->button2);
  assert(PRIVATE(this)->button3);
  assert(PRIVATE(this)->button4);
  PRIVATE(this)->radio1 = (SoGuiRadioButton *) SoAny::scanSceneForName(scene, "rbutton1");
  PRIVATE(this)->radio2 = (SoGuiRadioButton *) SoAny::scanSceneForName(scene, "rbutton2");
  PRIVATE(this)->radio3 = (SoGuiRadioButton *) SoAny::scanSceneForName(scene, "rbutton3");
  PRIVATE(this)->radio4 = (SoGuiRadioButton *) SoAny::scanSceneForName(scene, "rbutton4");
  assert(PRIVATE(this)->radio1);
  assert(PRIVATE(this)->radio2);
  assert(PRIVATE(this)->radio3);
  assert(PRIVATE(this)->radio4);

  PRIVATE(this)->slider1 = (SoGuiSlider1 *) SoAny::scanSceneForName(scene, "slider1");
  PRIVATE(this)->slider2 = (SoGuiSlider1 *) SoAny::scanSceneForName(scene, "slider2");
  PRIVATE(this)->slider3 = (SoGuiSlider1 *) SoAny::scanSceneForName(scene, "slider3");
  PRIVATE(this)->slider4 = (SoGuiSlider1 *) SoAny::scanSceneForName(scene, "slider4");
  PRIVATE(this)->slider5 = (SoGuiSlider1 *) SoAny::scanSceneForName(scene, "slider5");
  PRIVATE(this)->slider6 = (SoGuiSlider1 *) SoAny::scanSceneForName(scene, "slider6");
  assert(PRIVATE(this)->slider1);
  assert(PRIVATE(this)->slider2);
  assert(PRIVATE(this)->slider3);
  assert(PRIVATE(this)->slider4);
  assert(PRIVATE(this)->slider5);
  assert(PRIVATE(this)->slider6);
  PRIVATE(this)->slider1->setSurfaceColor(SbColor(0,0,0), SbColor(1,1,1));
  PRIVATE(this)->slider2->setSurfaceColor(SbColor(0,0,0), SbColor(1,1,1));
  PRIVATE(this)->slider3->setSurfaceColor(SbColor(0,0,0), SbColor(1,1,1));
  PRIVATE(this)->slider4->setSurfaceColor(SbColor(0,0,0), SbColor(1,1,1));
  PRIVATE(this)->slider5->setSurfaceColor(SbColor(0,0,0), SbColor(1,1,1));
  PRIVATE(this)->slider6->setSurfaceColor(SbColor(0,0,0), SbColor(1,1,1));

  SoNode * textureroot = SoAny::loadSceneGraph(MaterialEditor::texturescene);
  assert(textureroot != NULL);
  assert(textureroot->isOfType(SoSeparator::getClassTypeId()));
  SoNode * texture = ((SoSeparator *) textureroot)->getChild(0);
  assert(texture != NULL);
  assert(texture->isOfType(SoGuiSceneTexture2::getClassTypeId()));

  SoNode * subscene = ((SoGuiSceneTexture2 *) texture)->scene.getValue();
  assert(subscene);
  subscene->ref();
  PRIVATE(this)->material = (SoMaterial *) SoAny::scanSceneForName(subscene, "material");
  subscene->unrefNoDelete();
  assert(PRIVATE(this)->material);

  ok = sphereimage->setPart("texture", texture);
  assert(ok);

  PRIVATE(this)->ambient_slider_sensor = new SoFieldSensor(MaterialEditor::ambient_slider_cb, PRIVATE(this));
  PRIVATE(this)->diffuse_slider_sensor = new SoFieldSensor(MaterialEditor::diffuse_slider_cb, PRIVATE(this));
  PRIVATE(this)->specular_slider_sensor = new SoFieldSensor(MaterialEditor::specular_slider_cb, PRIVATE(this));
  PRIVATE(this)->emissive_slider_sensor = new SoFieldSensor(MaterialEditor::emissive_slider_cb, PRIVATE(this));
  PRIVATE(this)->shininess_slider_sensor = new SoFieldSensor(MaterialEditor::shininess_slider_cb, PRIVATE(this));
  PRIVATE(this)->transparency_slider_sensor = new SoFieldSensor(MaterialEditor::transparency_slider_cb, PRIVATE(this));
  PRIVATE(this)->ambient_slider_sensor->attach(&(PRIVATE(this)->slider1->value));
  PRIVATE(this)->diffuse_slider_sensor->attach(&(PRIVATE(this)->slider2->value));
  PRIVATE(this)->specular_slider_sensor->attach(&(PRIVATE(this)->slider3->value));
  PRIVATE(this)->emissive_slider_sensor->attach(&(PRIVATE(this)->slider4->value));
  PRIVATE(this)->shininess_slider_sensor->attach(&(PRIVATE(this)->slider5->value));
  PRIVATE(this)->transparency_slider_sensor->attach(&(PRIVATE(this)->slider6->value));

  PRIVATE(this)->button1_sensor = new SoFieldSensor(MaterialEditor::button1_cb, PRIVATE(this));
  PRIVATE(this)->button2_sensor = new SoFieldSensor(MaterialEditor::button2_cb, PRIVATE(this));
  PRIVATE(this)->button3_sensor = new SoFieldSensor(MaterialEditor::button3_cb, PRIVATE(this));
  PRIVATE(this)->button4_sensor = new SoFieldSensor(MaterialEditor::button4_cb, PRIVATE(this));
  PRIVATE(this)->button1_sensor->attach(&(PRIVATE(this)->button1->on));
  PRIVATE(this)->button2_sensor->attach(&(PRIVATE(this)->button2->on));
  PRIVATE(this)->button3_sensor->attach(&(PRIVATE(this)->button3->on));
  PRIVATE(this)->button4_sensor->attach(&(PRIVATE(this)->button4->on));

  PRIVATE(this)->material_sensor = new SoNodeSensor(MaterialEditor::material_cb, PRIVATE(this));
  PRIVATE(this)->material_sensor->attach(PRIVATE(this)->material);
  PRIVATE(this)->material->touch();

  PRIVATE(this)->material_field_sensor = new SoFieldSensor(MaterialEditor::material_field_cb, PRIVATE(this));
  PRIVATE(this)->material_field_sensor->attach(&(this->material));

  this->setColorEditorCallbacks(MaterialEditor::coloreditoropen_cb, MaterialEditor::coloreditorclose_cb, PRIVATE(this));
}

SoGuiMaterialEditor::~SoGuiMaterialEditor(void)
{
  MaterialEditor * obj = PRIVATE(this);
  delete obj;
}

void
SoGuiMaterialEditor::setColorEditorCallbacks(
  SoGuiColorEditorOpenCB * open,
  SoGuiColorEditorCloseCB * close,
  void * closure)
{
  PRIVATE(this)->openeditorcb = open;
  PRIVATE(this)->closeeditorcb = close;
  PRIVATE(this)->cbclosure = closure;
}

SoMaterial *
SoGuiMaterialEditor::getSphereMaterialNode(void) const
{
  return PRIVATE(this)->material;
}

// *************************************************************************
// MaterialEditor
// *************************************************************************

#define PUBLIC(obj) (((MaterialEditor *) obj)->api)

// +--------------------------------------------------------------------+
// |                   |  o x  Ambient    |======================| 0.00 |
// |                   |  o x  Diffuse    |======================| 0.00 |
// |                   |  o x  Specular   |======================| 0.00 |
// |                   |  o x  Emissive   |======================| 0.00 |
// |                   |                                                |
// |                   |       Shininess  |======================| 0.00 |
// |                   |       Transp     |======================| 0.00 |
// +--------------------------------------------------------------------+
//                                                          [nice - Ed]

const char *
MaterialEditor::texturescene[] =
{
  "#Inventor V2.1 ascii",
  "",
  "SoGuiSceneTexture2 {",
  "  scene",
  "    Separator {",
  "      OrthographicCamera {",
  "        position 0 0 2",
  "        nearDistance 1",
  "        farDistance 3",
  "        height 2",
  "      }",
  "      LightModel { model BASE_COLOR }",
  "      BaseColor {",
  "        rgb [ 0.3 0.3 0.3,  0.6 0.6 0.6,  0.6 0.6 0.6,  0.3 0.3 0.3 ]",
  "      }",
  "      MaterialBinding { value PER_FACE }",
  "      Coordinate3 {",
  "        point [",
  "          -3  3  0,  0  3  0,  3  3  0,",
  "          -3  0  0,  0  0  0,  3  0  0,",
  "          -3 -3  0,  0 -3  0,  3 -3  0",
  "        ]",
  "      }",
  "      QuadMesh {",
  "        verticesPerColumn 3",
  "        verticesPerRow 3",
  "      }",
  "      LightModel { model PHONG }",
  "      DirectionalLight { direction  0.556 -0.623 -0.551 }",
  "      DirectionalLight { direction -0.556 -0.623 -0.551 }",
  "      DEF material Material { }",
  "      Complexity { value 0.8 }",
  "      ShapeHints { vertexOrdering COUNTERCLOCKWISE shapeType SOLID }",
  "      Sphere { radius 0.85 }",
  "    }",
  "  }",
  "}",
  NULL
};

// FIXME: add labels
// FIXME: adjust geometry

const char *
MaterialEditor::editorscene[] =
{
  "#Inventor V2.1 ascii",
  "",
  "DEF pane SoGuiPane {",
  "  worldSize 1 1 0",
  "  objectSize 550 300 0",
  "  SoGuiPosition {",
  "    position = DEF wm Calculator {",
  "      A = USE pane.objectSize",
  "      expression [",
  "        \"ta = (A[0] / 2 < A[1]) ? (A[0]/2)-20 : A[1]-20\"",
  "        \"tb = (A[1] - ta) / 2\"",
  "        \"oA = vec3f(10, tb, 0)\"", // image position
  "        \"oB = vec3f(ta, ta, 0)\"", // image size
  "        \"oC = vec3f(ta + 30, A[1] - 25, 0)\"", // radio1 position
  "        \"oD = vec3f(0, -30, 0)\"", // ratio translation
  "      ]",
  "    } . oA",
  "  }",
  "  DEF image SoGuiImage { size = USE wm.oB }",
  "  SoGuiFrame {",
  "    size = USE image.size",
  "    design EMBOSS",
  "    width 2",
  "    complement TRUE",
  "  }",
  "  SoGuiPosition { position = USE wm.oC }",
  "  DEF rbutton1 SoGuiRadioButton {",
  "    size 10 10 0",
  "    on = DEF rbuttons SoGuiRadioGroup {",
  "      in0 = USE rbutton1.on",
  "      in1 = DEF rbutton2 SoGuiRadioButton { size 10 10 0 on = USE rbuttons.out1 } . on",
  "      in2 = DEF rbutton3 SoGuiRadioButton { size 10 10 0 on = USE rbuttons.out2 } . on",
  "      in3 = DEF rbutton4 SoGuiRadioButton { size 10 10 0 on = USE rbuttons.out3 } . on",
  "    } . out0",
  "  }",
  "  SoGuiFrame {",
  "    size = USE rbutton1.size",
  "    width 2",
  "    complement = USE rbutton1.on",
  "  }",
  "  SoGuiTranslation { translation = USE wm.oD }",
  "  USE rbutton2",
  "  SoGuiFrame {",
  "    size = USE rbutton2.size",
  "    width 2",
  "    complement = USE rbutton2.on",
  "  }",
  "  SoGuiTranslation { translation = USE wm.oD }",
  "  USE rbutton3",
  "  SoGuiFrame {",
  "    size = USE rbutton3.size",
  "    width 2",
  "    complement = USE rbutton3.on",
  "  }",
  "  SoGuiTranslation { translation = USE wm.oD }",
  "  USE rbutton4",
  "  SoGuiFrame {",
  "    size = USE rbutton4.size",
  "    width 2",
  "    complement = USE rbutton4.on",
  "  }",
  "  SoGuiPosition {",
  "    position = DEF wm2 Calculator {",
  "      A = USE pane.objectSize",
  "      expression [",
  "        \"ta = (A[0] / 2 < A[1]) ? (A[0]/2)-20 : A[1]-20\"",
  "        \"oA = vec3f(ta + 60, A[1] - 25, 0)\"",
  "        \"oB = vec3f(ta + 90, A[1] - 25, 0)\"",
  "        \"oC = vec3f(ta - 100, 15, 0)\"",
  "      ]",
  "    } . oA",
  "  }",
  "  DEF tbutton1 SoGuiToggleButton {",
  "    size 10 10 0"
  "    on = USE rbuttons.out0",
  "  }",
  "  SoGuiFrame {",
  "    size = USE tbutton1.size",
  "    width 2",
  "    complement = USE tbutton1.on",
  "    design EMBOSS",
  "  }",
  "  SoGuiTranslation { translation = USE wm.oD }",
  "  DEF tbutton2 SoGuiToggleButton {",
  "    size 10 10 0"
  "    on = USE rbuttons.out1",
  "  }",
  "  SoGuiFrame {",
  "    size = USE tbutton2.size",
  "    width 2",
  "    complement = USE tbutton2.on",
  "    design EMBOSS",
  "  }",
  "  SoGuiTranslation { translation = USE wm.oD }",
  "  DEF tbutton3 SoGuiToggleButton {",
  "    size 10 10 0"
  "    on = USE rbuttons.out2",
  "  }",
  "  SoGuiFrame {",
  "    size = USE tbutton3.size",
  "    width 2",
  "    complement = USE tbutton3.on",
  "    design EMBOSS",
  "  }",
  "  SoGuiTranslation { translation = USE wm.oD }",
  "  DEF tbutton4 SoGuiToggleButton {",
  "    size 10 10 0"
  "    on = USE rbuttons.out3",
  "  }",
  "  SoGuiFrame {",
  "    size = USE tbutton4.size",
  "    width 2",
  "    complement = USE tbutton4.on",
  "    design EMBOSS",
  "  }",

  "  SoGuiPosition { position = USE wm2.oB }",
  "  DEF sliderframe SoGuiFrame {",
  "    size = USE wm2.oC",
  "    width 2",
  "    complement TRUE",
  "    design EMBOSS",
  "  }",
  "  DEF slider1 SoGuiSlider1 { size = USE sliderframe.size }",
  "  SoGuiTranslation { translation = USE wm.oD }",
  "  USE sliderframe",
  "  DEF slider2 SoGuiSlider1 { size = USE sliderframe.size }",
  "  SoGuiTranslation { translation = USE wm.oD }",
  "  USE sliderframe",
  "  DEF slider3 SoGuiSlider1 { size = USE sliderframe.size }",
  "  SoGuiTranslation { translation = USE wm.oD }",
  "  USE sliderframe",
  "  DEF slider4 SoGuiSlider1 { size = USE sliderframe.size }",
  "  SoGuiTranslation { translation = USE wm.oD }",
  "  SoGuiTranslation { translation = USE wm.oD }",
  "  USE sliderframe",
  "  DEF slider5 SoGuiSlider1 { size = USE sliderframe.size }",
  "  SoGuiTranslation { translation = USE wm.oD }",
  "  USE sliderframe",
  "  DEF slider6 SoGuiSlider1 { size = USE sliderframe.size }",

  "  SoGuiPosition { position = USE pane.objectSize }",
  "  SoGuiTranslation { translation -35 -25 0 }",
  "  DEF labelframe SoGuiFrame { size 25 15 0 width 2 design EMBOSS complement TRUE }",
  "  SoGuiLabel { text = SoGuiFormat { format \"%4.2f\" float1 = USE slider1.value } . output }",
  "  SoGuiTranslation { translation = USE wm.oD }",
  "  USE labelframe",
  "  SoGuiLabel { text = SoGuiFormat { format \"%4.2f\" float1 = USE slider2.value } . output }",
  "  SoGuiTranslation { translation = USE wm.oD }",
  "  USE labelframe",
  "  SoGuiLabel { text = SoGuiFormat { format \"%4.2f\" float1 = USE slider3.value } . output }",
  "  SoGuiTranslation { translation = USE wm.oD }",
  "  USE labelframe",
  "  SoGuiLabel { text = SoGuiFormat { format \"%4.2f\" float1 = USE slider4.value } . output }",
  "  SoGuiTranslation { translation = USE wm.oD }",
  "  SoGuiTranslation { translation = USE wm.oD }",
  "  USE labelframe",
  "  SoGuiLabel { text = SoGuiFormat { format \"%4.2f\" float1 = USE slider5.value } . output }",
  "  SoGuiTranslation { translation = USE wm.oD }",
  "  USE labelframe",
  "  SoGuiLabel { text = SoGuiFormat { format \"%4.2f\" float1 = USE slider6.value } . output }",
  "}",
  NULL
};

MaterialEditor::MaterialEditor(void)
{
  this->api = NULL;
  this->material = NULL;
  this->button1 = NULL;
  this->button2 = NULL;
  this->button3 = NULL;
  this->button4 = NULL;
  this->radio1 = NULL;
  this->radio2 = NULL;
  this->radio3 = NULL;
  this->radio4 = NULL;
  this->slider1 = NULL;
  this->slider2 = NULL;
  this->slider3 = NULL;
  this->slider4 = NULL;
  this->slider5 = NULL;
  this->slider6 = NULL;
  this->ambient_slider_sensor = NULL;
  this->diffuse_slider_sensor = NULL;
  this->specular_slider_sensor = NULL;
  this->emissive_slider_sensor = NULL;
  this->shininess_slider_sensor = NULL;
  this->transparency_slider_sensor = NULL;
  this->coloreditoropen = FALSE;
  this->coloreditor = NULL;
  this->coloreditor_sensor = NULL;
}

MaterialEditor::~MaterialEditor(void)
{
  if ( this->coloreditor_sensor ) delete this->coloreditor_sensor;
}

void
MaterialEditor::ambient_slider_cb(void * closure, SoSensor * sensor)
{
  assert(closure);
  MaterialEditor * me = (MaterialEditor *) closure;
  assert(me->material);
  assert(me->slider1);
  SbColor ambient = me->material->ambientColor[0];
  float h, s, v;
  ambient.getHSVValue(h, s, v);
  v = me->slider1->value.getValue();
  ambient.setHSVValue(h, s, v);
  if ( ambient != me->material->ambientColor[0] )
    me->material->ambientColor.set1Value(0, ambient);
}

void
MaterialEditor::diffuse_slider_cb(void * closure, SoSensor * sensor)
{
  assert(closure);
  MaterialEditor * me = (MaterialEditor *) closure;
  assert(me->material);
  assert(me->slider2);
  SbColor diffuse = me->material->diffuseColor[0];
  float h, s, v;
  diffuse.getHSVValue(h, s, v);
  v = me->slider2->value.getValue();
  diffuse.setHSVValue(h, s, v);
  if ( diffuse != me->material->diffuseColor[0] )
    me->material->diffuseColor.set1Value(0, diffuse);
}

void
MaterialEditor::specular_slider_cb(void * closure, SoSensor * sensor)
{
  assert(closure);
  MaterialEditor * me = (MaterialEditor *) closure;
  assert(me->material);
  assert(me->slider3);
  SbColor specular = me->material->specularColor[0];
  float h, s, v;
  specular.getHSVValue(h, s, v);
  v = me->slider3->value.getValue();
  specular.setHSVValue(h, s, v);
  if ( specular != me->material->specularColor[0] )
    me->material->specularColor.set1Value(0, specular);
}

void
MaterialEditor::emissive_slider_cb(void * closure, SoSensor * sensor)
{
  assert(closure);
  MaterialEditor * me = (MaterialEditor *) closure;
  assert(me->material);
  assert(me->slider4);
  SbColor emissive = me->material->emissiveColor[0];
  float h, s, v;
  emissive.getHSVValue(h, s, v);
  v = me->slider4->value.getValue();
  emissive.setHSVValue(h, s, v);
  if ( emissive != me->material->emissiveColor[0] )
    me->material->emissiveColor.set1Value(0, emissive);
}

void
MaterialEditor::shininess_slider_cb(void * closure, SoSensor * sensor)
{
  assert(closure);
  MaterialEditor * me = (MaterialEditor *) closure;
  assert(me->material);
  assert(me->slider5);
  float shininess = me->slider5->value.getValue();
  if ( shininess != me->material->shininess[0] )
    me->material->shininess.set1Value(0, shininess);
}

void
MaterialEditor::transparency_slider_cb(void * closure, SoSensor * sensor)
{
  assert(closure);
  MaterialEditor * me = (MaterialEditor *) closure;
  assert(me->material);
  assert(me->slider6);
  float transparency = me->slider6->value.getValue();
  if ( transparency != me->material->transparency[0] )
    me->material->transparency.set1Value(0, transparency);
}

void
MaterialEditor::checkColorEditorOpenStatus(void)
{
  if ( this->button1->on.getValue() ||
       this->button2->on.getValue() ||
       this->button3->on.getValue() ||
       this->button4->on.getValue() ) {
    if ( !this->coloreditoropen && this->openeditorcb ) {
      this->coloreditor = this->openeditorcb(this->cbclosure);
      this->coloreditoropen = TRUE;
      if ( this->coloreditor_sensor == NULL )
        this->coloreditor_sensor = new SoFieldSensor(MaterialEditor::coloreditor_cb, this);
      if ( this->coloreditor )
        this->coloreditor_sensor->attach(&(this->coloreditor->color));
    }
  } else {
    if ( this->coloreditoropen && this->closeeditorcb ) {
      if ( this->coloreditor_sensor ) this->coloreditor_sensor->detach();
      this->closeeditorcb(this->cbclosure, this->coloreditor);
      this->coloreditor = NULL;
      this->coloreditoropen = FALSE;
    }
  }
}

void
MaterialEditor::button1_cb(void * closure, SoSensor * sensor)
{
  assert(closure);
  MaterialEditor * me = (MaterialEditor *) closure;
  SbBool down = me->button1->on.getValue();
  if ( !down ) {
    if ( me->radio1->on.getValue() ) me->radio1->on.setValue(FALSE);

    if (  me->button2->on.getValue() &&
         !me->button3->on.getValue() &&
         !me->button4->on.getValue() &&
         !me->radio2->on.getValue() ) me->radio2->on.setValue(TRUE);
    if ( !me->button2->on.getValue() &&
          me->button3->on.getValue() &&
         !me->button4->on.getValue() &&
         !me->radio3->on.getValue() ) me->radio3->on.setValue(TRUE);
    if ( !me->button2->on.getValue() &&
         !me->button3->on.getValue() &&
          me->button4->on.getValue() &&
         !me->radio4->on.getValue() ) me->radio4->on.setValue(TRUE);
  } else {
    if ( me->radio2->on.getValue() ) me->radio2->on.setValue(FALSE);
    if ( me->radio3->on.getValue() ) me->radio3->on.setValue(FALSE);
    if ( me->radio4->on.getValue() ) me->radio4->on.setValue(FALSE);
    if ( !me->button2->on.getValue() &&
         !me->button3->on.getValue() &&
         !me->button4->on.getValue() &&
         !me->radio1->on.getValue() ) me->radio1->on.setValue(TRUE);
  }
  me->checkColorEditorOpenStatus();
}

void
MaterialEditor::button2_cb(void * closure, SoSensor * sensor)
{
  assert(closure);
  MaterialEditor * me = (MaterialEditor *) closure;
  SbBool down = me->button2->on.getValue();
  if ( !down ) {
    if ( me->radio2->on.getValue() ) me->radio2->on.setValue(FALSE);
    if (  me->button1->on.getValue() &&
         !me->button3->on.getValue() &&
         !me->button4->on.getValue() &&
         !me->radio1->on.getValue() ) me->radio1->on.setValue(TRUE);
    if ( !me->button1->on.getValue() &&
          me->button3->on.getValue() &&
         !me->button4->on.getValue() &&
         !me->radio3->on.getValue() ) me->radio3->on.setValue(TRUE);
    if ( !me->button1->on.getValue() &&
         !me->button3->on.getValue() &&
          me->button4->on.getValue() &&
         !me->radio4->on.getValue() ) me->radio4->on.setValue(TRUE);
  } else {
    if ( me->radio1->on.getValue() ) me->radio1->on.setValue(FALSE);
    if ( me->radio3->on.getValue() ) me->radio3->on.setValue(FALSE);
    if ( me->radio4->on.getValue() ) me->radio4->on.setValue(FALSE);
    if ( !me->button1->on.getValue() &&
         !me->button3->on.getValue() &&
         !me->button4->on.getValue() &&
         !me->radio2->on.getValue() ) me->radio2->on.setValue(TRUE);
  }
  me->checkColorEditorOpenStatus();
}

void
MaterialEditor::button3_cb(void * closure, SoSensor * sensor)
{
  assert(closure);
  assert(closure);
  MaterialEditor * me = (MaterialEditor *) closure;
  SbBool down = me->button3->on.getValue();
  if ( !down ) {
    if ( me->radio3->on.getValue() ) me->radio3->on.setValue(FALSE);
    if (  me->button1->on.getValue() &&
         !me->button2->on.getValue() &&
         !me->button4->on.getValue() &&
         !me->radio1->on.getValue() ) me->radio1->on.setValue(TRUE);
    if ( !me->button1->on.getValue() &&
          me->button2->on.getValue() &&
         !me->button4->on.getValue() &&
         !me->radio2->on.getValue() ) me->radio2->on.setValue(TRUE);
    if ( !me->button1->on.getValue() &&
         !me->button2->on.getValue() &&
          me->button4->on.getValue() &&
         !me->radio4->on.getValue() ) me->radio4->on.setValue(TRUE);
  } else {
    if ( me->radio1->on.getValue() ) me->radio1->on.setValue(FALSE);
    if ( me->radio2->on.getValue() ) me->radio2->on.setValue(FALSE);
    if ( me->radio4->on.getValue() ) me->radio4->on.setValue(FALSE);
    if ( !me->button1->on.getValue() &&
         !me->button2->on.getValue() &&
         !me->button4->on.getValue() &&
         !me->radio3->on.getValue() ) me->radio3->on.setValue(TRUE);
  }
  me->checkColorEditorOpenStatus();
}

void
MaterialEditor::button4_cb(void * closure, SoSensor * sensor)
{
  assert(closure);
  MaterialEditor * me = (MaterialEditor *) closure;
  SbBool down = me->button4->on.getValue();
  if ( !down ) {
    if ( me->radio4->on.getValue() ) me->radio4->on.setValue(FALSE);
    if (  me->button1->on.getValue() &&
         !me->button2->on.getValue() &&
         !me->button3->on.getValue() &&
         !me->radio1->on.getValue() ) me->radio1->on.setValue(TRUE);
    if ( !me->button1->on.getValue() &&
          me->button2->on.getValue() &&
         !me->button3->on.getValue() &&
         !me->radio2->on.getValue()  ) me->radio2->on.setValue(TRUE);
    if ( !me->button1->on.getValue() &&
         !me->button2->on.getValue() &&
          me->button3->on.getValue() &&
         !me->radio3->on.getValue() ) me->radio3->on.setValue(TRUE);
  } else {
    if ( me->radio1->on.getValue() ) me->radio1->on.setValue(FALSE);
    if ( me->radio2->on.getValue() ) me->radio2->on.setValue(FALSE);
    if ( me->radio3->on.getValue() ) me->radio3->on.setValue(FALSE);
    if ( !me->button1->on.getValue() &&
         !me->button2->on.getValue() &&
         !me->button3->on.getValue() &&
         !me->radio4->on.getValue() ) me->radio4->on.setValue(TRUE);
  }
  me->checkColorEditorOpenStatus();
}

void
MaterialEditor::coloreditor_cb(void * closure, SoSensor * sensor)
{
  // coloreditor is being interacted with - so update material editor sliders
  assert(closure);
  MaterialEditor * me = (MaterialEditor *) closure;
  if ( me->button1->on.getValue() )
    me->material->ambientColor.setValue(me->coloreditor->color.getValue());
  if ( me->button2->on.getValue() )
    me->material->diffuseColor.setValue(me->coloreditor->color.getValue());
  if ( me->button3->on.getValue() )
    me->material->specularColor.setValue(me->coloreditor->color.getValue());
  if ( me->button4->on.getValue() )
    me->material->emissiveColor.setValue(me->coloreditor->color.getValue());
}
  
void
MaterialEditor::material_cb(void * closure, SoSensor * sensor)
{
  assert(closure);
  MaterialEditor * me = (MaterialEditor *) closure;
  SoMaterial * mat = me->material;
  if ( !mat ) return;
  float h, s, v;
  SbColor color = mat->ambientColor[0];
  color.getHSVValue(h, s, v);
  if ( v != me->slider1->value.getValue() ) me->slider1->value.setValue(v);
  color = mat->diffuseColor[0];
  color.getHSVValue(h, s, v);
  if ( v != me->slider2->value.getValue() ) me->slider2->value.setValue(v);
  color = mat->specularColor[0];
  color.getHSVValue(h, s, v);
  if ( v != me->slider3->value.getValue() ) me->slider3->value.setValue(v);
  color = mat->emissiveColor[0];
  color.getHSVValue(h, s, v);
  if ( v != me->slider4->value.getValue() ) me->slider4->value.setValue(v);
  v = mat->shininess[0];
  if ( v != me->slider5->value.getValue() ) me->slider5->value.setValue(v);
  v = mat->transparency[0];
  if ( v != me->slider6->value.getValue() ) me->slider6->value.setValue(v);
  SoNode * attachment = PUBLIC(me)->material.getValue();
  if ( attachment ) {
    if ( attachment->isOfType(SoMaterial::getClassTypeId()) ) {
      SoMaterial * material = (SoMaterial *) attachment;
      int index = PUBLIC(me)->index.getValue();
      material->ambientColor.set1Value(index, mat->ambientColor[0]);
      material->diffuseColor.set1Value(index, mat->diffuseColor[0]);
      material->specularColor.set1Value(index, mat->specularColor[0]);
      material->emissiveColor.set1Value(index, mat->emissiveColor[0]);
      material->shininess.set1Value(index, mat->shininess[0]);
      material->transparency.set1Value(index, mat->transparency[0]);
#ifdef HAVE_SOVRMLMATERIAL
    } else if ( attachment->isOfType(SoVRMLMaterial::getClassTypeId()) ) {
      SoVRMLMaterial * material = (SoVRMLMaterial *) attachment;
      SbColor ambient = mat->ambientColor[0];
      float h, s, v;
      ambient.getHSVValue(h, s, v);
      material->ambientIntensity.setValue(v);
      material->diffuseColor.setValue(mat->diffuseColor[0]);
      material->specularColor.setValue(mat->specularColor[0]);
      material->emissiveColor.setValue(mat->emissiveColor[0]);
      material->shininess.setValue(mat->shininess[0]);
      material->transparency.setValue(mat->transparency[0]);
#endif // HAVE_SOVRMLMATERIAL
    } else {
    }
  }
}

void
MaterialEditor::material_field_cb(void * closure, SoSensor * sensor)
{
  assert(closure);
  MaterialEditor * me = (MaterialEditor *) closure;
  SoNode * node = PUBLIC(me)->material.getValue();
  if ( !node ) return;
  SoMaterial * mat1 = (SoMaterial *) node;
  SoMaterial * mat2 = me->material;

  if ( mat1->ambientColor[0] != mat2->ambientColor[0] )
    mat2->ambientColor.set1Value(0, mat1->ambientColor[0]);
  if ( mat1->diffuseColor[0] != mat2->diffuseColor[0] )
    mat2->diffuseColor.set1Value(0, mat1->diffuseColor[0]);
  if ( mat1->specularColor[0] != mat2->specularColor[0] )
    mat2->specularColor.set1Value(0, mat1->specularColor[0]);
  if ( mat1->emissiveColor[0] != mat2->emissiveColor[0] )
    mat2->emissiveColor.set1Value(0, mat1->emissiveColor[0]);
  if ( mat1->shininess[0] != mat2->shininess[0] )
    mat2->shininess.set1Value(0, mat1->shininess[0]);
  if ( mat1->transparency[0] != mat2->transparency[0] )
    mat2->transparency.set1Value(0, mat1->transparency[0]);
}

SoGuiColorEditor *
MaterialEditor::coloreditoropen_cb(void * closure)
{
  SoDebugError::postInfo("MaterialEditor::coloreditoropen_cb", "default callback invoked - not implemented yet");
  return NULL;
}

void
MaterialEditor::coloreditorclose_cb(void * closure, SoGuiColorEditor * editor)
{
  SoDebugError::postInfo("MaterialEditor::coloreditorclose_cb", "default callback invoked - not implemented yet");
}

#undef PRIVATE
#undef PUBLIC

// *************************************************************************
