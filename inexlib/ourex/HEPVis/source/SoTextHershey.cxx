//  This file is part of the HEPVis library.
//  Copyright (C) 2004 by the HEPVis team.  All rights reserved.
//  This library is free software and under the terms of the
//  GNU General Public License described in the License file
//  coming with the HEPVis distribution. 

// this :
#include <HEPVis/nodes/SoTextHershey.h>

//////////////////////////////////////////////////////////////////////////////
//
//  Specify 3D text.
//
//  20/08/1998 : G.Barrand : creation.
//
//////////////////////////////////////////////////////////////////////////////

#include <Inventor/SbBox.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/details/SoPointDetail.h>
#include <Inventor/elements/SoLazyElement.h>
#ifdef SOTEXT_HERSHEY_CHILD_LIST
#include <Inventor/misc/SoChildList.h>
#endif

#include <HEPVis/actions/SoAlternateRepAction.h>

#include <HEPVis/SbText.h>

static void SpecialCharPoints(char,float,int*,int*,float*,float*,float&);
static void GreekCharPoints(char,float,int*,int*,float*,float*,float&);
static void LatinCharPoints(char,float,int*,int*,float*,float*,float&);

#define MINIMUM(a,b) ((a)<(b)?(a):(b))
#define MAXIMUM(a,b) ((a)>(b)?(a):(b))

#define HERSHEY__MAX_POLY   4
#define HERSHEY__MAX_POINT  160 /* 40 * 4 */

SO_NODE_SOURCE(SoTextHershey)

void SoTextHershey::initClass() {
  SO_NODE_INIT_CLASS(SoTextHershey,SoShape,"Shape");
}

SoTextHershey::SoTextHershey ()
#ifdef SOTEXT_HERSHEY_CHILD_LIST
:fChildren(0)
,fOld_font(-1)
,fOld_horizontalJustification(-1)
,fOld_verticalJustification(-1)
,fOld_height(0)
#endif
{
  SO_NODE_CONSTRUCTOR(SoTextHershey);

  SO_NODE_ADD_FIELD(string,(""));
  SO_NODE_ADD_FIELD(encoding,("none")); //PAW
  SO_NODE_ADD_FIELD(font,(LATIN));
  SO_NODE_ADD_FIELD(horizontalJustification,(LEFT));
  SO_NODE_ADD_FIELD(verticalJustification,(BOTTOM));
  SO_NODE_ADD_FIELD(height,(1.0F));

  SO_NODE_ADD_FIELD(alternateRep,(NULL));

  SO_NODE_DEFINE_ENUM_VALUE(Font,LATIN);
  SO_NODE_DEFINE_ENUM_VALUE(Font,GREEK);
  SO_NODE_DEFINE_ENUM_VALUE(Font,SPECIAL);

  SO_NODE_SET_SF_ENUM_TYPE(font,Font);

  SO_NODE_DEFINE_ENUM_VALUE(HorizontalJustification,LEFT);
  SO_NODE_DEFINE_ENUM_VALUE(HorizontalJustification,CENTER);
  SO_NODE_DEFINE_ENUM_VALUE(HorizontalJustification,RIGHT);

  SO_NODE_SET_SF_ENUM_TYPE(horizontalJustification,HorizontalJustification);

  SO_NODE_DEFINE_ENUM_VALUE(VerticalJustification,BOTTOM);
  SO_NODE_DEFINE_ENUM_VALUE(VerticalJustification,MIDDLE);
  SO_NODE_DEFINE_ENUM_VALUE(VerticalJustification,TOP);

  SO_NODE_SET_SF_ENUM_TYPE(verticalJustification,VerticalJustification);

#ifdef SOTEXT_HERSHEY_CHILD_LIST
  fChildren = new SoChildList(this);
#endif
}

SoTextHershey::~SoTextHershey() {
#ifdef SOTEXT_HERSHEY_CHILD_LIST
  delete fChildren;
#endif
}

void SoTextHershey::generatePrimitives(SoAction* aAction) {
#ifdef SOTEXT_HERSHEY_CHILD_LIST
  updateChildren();
  fChildren->traverse(aAction);  
#else

  std::vector<SbVec3f> pts;
  generateSegments(pts);
  unsigned int number = pts.size();
  if(number==0) return;

  SbVec3f normal(0,0,1);

  SoPrimitiveVertex pvb;
  pvb.setNormal(normal);
 {SbVec4f texCoord(0,1,0,1); //FIXME
  pvb.setTextureCoords(texCoord);}

  SoPrimitiveVertex pve;
  pve.setNormal(normal);
 {SbVec4f texCoord(0,1,0,1); //FIXME
  pve.setTextureCoords(texCoord);}

#ifdef __COIN__ 
  SoPointDetail pointDetail;
  pve.setDetail(&pointDetail);
  pvb.setDetail(&pointDetail);

  //SoLineDetail lineDetail;
  //beginShape(aAction, SoShape::LINES, &lineDetail);
  beginShape(aAction,SoShape::LINES);
  for(unsigned int count=0;count<number;count+=2) {
    pvb.setPoint(pts[count]);
    shapeVertex(&pvb);
    pve.setPoint(pts[count+1]);
    shapeVertex(&pve);
  }
  endShape();
#else
  for(unsigned int count=0;count<number;count+=2) {
    pvb.setPoint(pts[count]);
    pve.setPoint(pts[count+1]);
    invokeLineSegmentCallbacks(aAction,&pvb,&pve);
  }
#endif

#endif
}

void SoTextHershey::GLRender(SoGLRenderAction* aAction) {
  if(!shouldGLRender(aAction)) return;
#ifdef SOTEXT_HERSHEY_CHILD_LIST
  updateChildren();
  fChildren->traverse(aAction);  
#else
  SoState* state = aAction->getState();
  state->push();

  SoLazyElement::setLightModel(state,SoLazyElement::BASE_COLOR);

  SoShape::GLRender(aAction);

  state->pop();
#endif
}
#ifdef SOTEXT_HERSHEY_CHILD_LIST
#else

void SoTextHershey::rayPick(SoRayPickAction* aAction) {
  SoShape::rayPick(aAction);
}

void SoTextHershey::pick(SoPickAction* aAction) {
  SoShape::pick(aAction);
}
#endif

void SoTextHershey::computeBBox(SoAction*,SbBox3f& aBox,SbVec3f& aCenter){
  std::vector<SbVec3f> pts;
  generateSegments(pts);
  unsigned int number = pts.size();
  if(number==0) {
    aBox.makeEmpty();
  } else {
    float xmn = pts[0][0];
    float xmx = xmn;
    float ymn = pts[0][1];
    float ymx = ymn;
    float zmn = pts[0][2];
    float zmx = zmn;
    for(unsigned int count=1;count<number;count++) {
     {float x = pts[count][0];
      xmn = MINIMUM(xmn,x);
      xmx = MAXIMUM(xmx,x);}
     {float y = pts[count][1];
      ymn = MINIMUM(ymn,y);
      ymx = MAXIMUM(ymx,y);}
     {float z = pts[count][2];
      zmn = MINIMUM(zmn,z);
      zmx = MAXIMUM(zmx,z);}
    }
    aBox.setBounds(SbVec3f(xmn,ymn,zmn),SbVec3f(xmx,ymx,zmx));
  }
  aCenter = aBox.getCenter();
}

void SoTextHershey::getTextBounds(SbBox3f& aBox) {
  getTextBounds(string.getNum(),string.getValues(0),aBox);
}

void SoTextHershey::getTextBounds(int aNumber,const SbString* aLines,SbBox3f& aBox){
  float HEIGHT = height.getValue();
  float Y_ADVANCE = 2 * HEIGHT;
  float width = 0;
  float Y = 0;
  std::vector<SbVec3f> dummy;
  for(int count=0;count<aNumber;count++) {
    float XL = 0;
    renderString(FALSE,aLines[count].getString(),XL,Y,dummy);
    Y -= Y_ADVANCE;
    width = MAXIMUM(width,XL);
  }
  if(aNumber) {
    SbVec3f mn(0,-(aNumber-1) * Y_ADVANCE,0);
    //FIXME : should be the true height of the text !
    SbVec3f mx(width,HEIGHT,0.01F); //Give some depth
    aBox.setBounds(mn,mx);
  } else {
    aBox.makeEmpty();
  }
}

float SoTextHershey::getAscent() const {
  SbBox3f box;
  getCharBound('/',LATIN,height.getValue(),FALSE,box);
  return box.getMax()[1];
}

float SoTextHershey::getDescent() const {
  SbBox3f box;
  getCharBound('_',LATIN,height.getValue(),FALSE,box);
  return -box.getMin()[1]; //positive.
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void SoTextHershey::generateSegments(std::vector<SbVec3f>& aPoints) {
  SbBox3f box;
  float szx,szy,szz;
  if( (horizontalJustification.getValue()==CENTER) ||
      (horizontalJustification.getValue()==RIGHT) ||
      (verticalJustification.getValue()==MIDDLE) ||
      (verticalJustification.getValue()==TOP) ){
    getTextBounds(box);
    box.getSize(szx,szy,szz);
  }

  // Justification :
  float X = 0;
  float Y = 0;
  if(horizontalJustification.getValue()==LEFT) {
  } else if(horizontalJustification.getValue()==CENTER) {
    X -= 0.5F * szx;
  } else if(horizontalJustification.getValue()==RIGHT) {
    X -= szx;
  }

  if(verticalJustification.getValue()==BOTTOM) {
  } else if(verticalJustification.getValue()==MIDDLE) {
    Y -= 0.5F * szy;
  } else if(verticalJustification.getValue()==TOP) {
    Y -= szy;
  }

  float HEIGHT = height.getValue();
  float Y_ADVANCE = 2 * HEIGHT;

  int linen = string.getNum();
  const SbString* lines = string.getValues(0);
  for(int count=0;count<linen;count++) {
    float XL = X;
    renderString(TRUE,lines[count].getString(),XL,Y,aPoints);
    Y -= Y_ADVANCE;
  }
}

void SoTextHershey::renderString (
 SbBool aGEN_POINTS // FALSE = advance only.
,const char* aString
,float& aX
,float& aY
,std::vector<SbVec3f>& aPoints
){
  float oldX = 0;
  float HEIGHT = height.getValue();

  SbBool encod_PAW = (encoding.getValue()=="PAW"?TRUE:FALSE);

  SbEncodedString sbs = encod_PAW ? 
    SbText::decodePAW(aString) : SbText::decodePlain(aString);

  unsigned int n = sbs.size();
  for(unsigned int index=0;index<n;index++) {
    const SbChar& sbChar = sbs[index];

    SoTextHershey::Font hershey_font = (SoTextHershey::Font)sbChar.fFont;
    if(encod_PAW) {
      hershey_font = (SoTextHershey::Font)sbChar.fFont;
    } else {
      hershey_font = (SoTextHershey::Font)font.getValue();
    }

    float scale = HEIGHT;
    float ymove = 0;
    if(sbChar.fYMove==SbChar::UP) {
      scale = HEIGHT*0.6F;
      ymove = HEIGHT*0.6F;
    } else if(sbChar.fYMove==SbChar::DOWN) {
      scale = HEIGHT*0.6F;
      ymove = -HEIGHT*0.6F;
    }
    if(sbChar.fBack==TRUE) aX = oldX;
    oldX = aX;
    //FIXME : bar
    aY += ymove;
    aX += renderCharacter
            (aGEN_POINTS, 
             sbChar.fChar,hershey_font,scale,sbChar.fBar,aX,aY,
             aPoints);
    aX += HEIGHT * 0.01F;
    aY -= ymove;
  }
}

float SoTextHershey::renderCharacter (
 SbBool aGEN_POINTS // FALSE = advance only.
,char aChar 
,SoTextHershey::Font aFont
,float aScale
,SbBool aBar
,float aX
,float aY
,std::vector<SbVec3f>& aPoints
){
  int max_point[HERSHEY__MAX_POLY];
  float xp[HERSHEY__MAX_POINT],yp[HERSHEY__MAX_POINT];
  int number;
  float width;
  if (aFont==SoTextHershey::GREEK) {
    GreekCharPoints(aChar,aScale,&number,max_point,xp,yp,width);
  } else if (aFont==SoTextHershey::SPECIAL) {
    SpecialCharPoints(aChar,aScale,&number,max_point,xp,yp,width);
  } else {
    LatinCharPoints(aChar,aScale,&number,max_point,xp,yp,width);
  }
  if(aGEN_POINTS==FALSE) return width;

  float ymax = 0;

  SbVec3f point;
  int ipoint = 0;
  int pointn;
  for (int ipoly=0;ipoly<number;ipoly++) { 
    pointn  = max_point[ipoly];
    if(pointn>0) {
      for(int count=0;count<pointn-1;count++) {
        point.setValue(aX+xp[ipoint],aY+yp[ipoint],0.);
        ymax = MAXIMUM(ymax,yp[ipoint]);
        aPoints.push_back(point);

        point.setValue(aX+xp[ipoint+1],aY+yp[ipoint+1],0.);
        ymax = MAXIMUM(ymax,yp[ipoint+1]);
        aPoints.push_back(point);

        ipoint ++;
      }
      ipoint ++;
    }
  }
  
  if(aBar) { //Draw a bar on top of the character.
    float xbar = 0;
    float ybar = ymax * 1.3F;

    point.setValue(aX+xbar,aY+ybar,0.);
    aPoints.push_back(point);

    point.setValue(aX+xbar+width,aY+ybar,0.);
    aPoints.push_back(point);
  }

  return width;
}

void SoTextHershey::getCharBound (
 char aChar 
,SoTextHershey::Font aFont
,float aScale
,SbBool aBar
,SbBox3f& aBox
) const
{
  aBox.makeEmpty();

  int max_point[HERSHEY__MAX_POLY];
  float xp[HERSHEY__MAX_POINT],yp[HERSHEY__MAX_POINT];
  int number;
  float width;
  if (aFont==SoTextHershey::GREEK) {
    GreekCharPoints(aChar,aScale,&number,max_point,xp,yp,width);
  } else if (aFont==SoTextHershey::SPECIAL) {
    SpecialCharPoints(aChar,aScale,&number,max_point,xp,yp,width);
  } else {
    LatinCharPoints(aChar,aScale,&number,max_point,xp,yp,width);
  }

  float ymax = 0;

  SbVec3f point;
  int ipoint = 0;
  for (int ipoly=0;ipoly<number;ipoly++) { 
    int pointn  = max_point[ipoly];
    if(pointn>0) {
      for(int count=0;count<pointn-1;count++) {
        point.setValue(xp[ipoint],yp[ipoint],0.);
        ymax = MAXIMUM(ymax,yp[ipoint]);
        aBox.extendBy(point);
        point.setValue(xp[ipoint+1],yp[ipoint+1],0.);
        ymax = MAXIMUM(ymax,yp[ipoint+1]);
        aBox.extendBy(point);
        ipoint ++;
      }
      ipoint ++;
    }
  }

  if(aBar) { //Draw a bar on top of the character.
    float xbar = 0;
    float ybar = ymax * 1.3F;
    point.setValue(xbar,ybar,0.);
    aBox.extendBy(point);
    point.setValue(xbar+width,ybar,0.);
    aBox.extendBy(point);
  }
}

//////////////////////////////////////////////////////////////////////////////
/*** Hershey routines ******************************************************/
//////////////////////////////////////////////////////////////////////////////
static void Extract(int,int,char,float,int*,int*,float*,float*,float&);
static void Rescale(int,int*,int*);

static struct {
  int x[751];
  int y[751];
  int start[210];
} Class = {
    {0x1969C7D6,0x217DC699,0x269E79A4,0x00FDF864,0x2185E716,0x0000003F,
     0x1C6DA696,0x2696489E,0x27663966,0x0000003F,0x238266D6,0x269E79A5,
     0x1A6DE864,0x00000FD9,0x3FA198D6,0x0003F8E3,0x1B69B956,0x279A485E,
     0x1E8649A7,0x00FD969B,0x208A5996,0x1B69A6DD,0x2692181D,0x219269E7,
     0x3F69B760,0x19FDD9D6,0x00000FE7,0x1A69B796,0x2692175B,0x229669E7,
     0x1965A6DE,0x258DF71A,0x1E8A59A6,0x0000003F,0x208E5996,0x1965A71F,
     0x2381F71A,0x239669A5,0x3F69B7A0,0x20FD8817,0x3F95BFE8,0x19FD9655,
     0x279E6962,0x19FE2966,0x279E6962,0x3F662966,0x23967A16,0x1969B75F,
     0x1F75B699,0x3FA27963,0x19FD9655,0x279A58E0,0x208E59A7,0x00000FD9,
     0x1AFDA696,0x3F89AFE7,0x0003F9DA,0x1AFDA696,0x3F89AFE7,0x23967A16,
     0x1969B75F,0x1F75B699,0x28A27963,0x00FE88FF,0x27FD9655,0x3F9D9FE7,
     0x00FE081C,0x228E4918,0x1A6DC7A0,0x00000FDA,0x27FD9655,0x3F9DEFD9,
     0x1AFDA696,0x00000FE6,0x18FD8614,0x3F828FE0,0x0003FA28,0x19FD9655,
     0x3F9E7FE7,0x1969C795,0x1C699618,0x279A489E,0x249A7A28,0x0003F7A2,
     0x19FD9655,0x279E6962,0x3F662966,0x1969C795,0x1C699618,0x279A489E,
     0x249A7A28,0x2787F7A2,0x0000003F,0x19FD9655,0x279E6962,0x3F662966,
     0x0003F9E0,0x1E8A59D6,0x1B69965B,0x279A58DD,0x1B7A2967,0x00000FD9,
     0x19FE0818,0x00000FE7,0x1C699655,0x279A485F,0x00000FE7,0x28FE0617,
     0x00000FE0,0x20FDB594,0x3F960FDB,0x0003F96A,0x27FE7656,0x00000FD9,
     0x3F820617,0x0003F828,0x19FD99D6,0x3F9D9FE7,0x20FD8817,0x3F95BFE8,
     0x19FD9655,0x279E6962,0x19FE2966,0x279E6962,0x3F662966,0x1AFDA696,
     0x00000FE6,0x20FD8817,0x3FA18FE8,0x1AFDA696,0x3F89AFE7,0x0003F9DA,
     0x19FD99D6,0x3F9D9FE7,0x27FD9655,0x3F9D9FE7,0x1969C795,0x1C699618,
     0x279A489E,0x249A7A28,0x2377F7A2,0x0000003F,0x00FE081C,0x27FD9655,
     0x3F9DEFD9,0x20FD8817,0x00000FE8,0x18FD8614,0x3F828FE0,0x0003FA28,
     0x19FD9655,0x3F9E7FE7,0x1DFE7657,0x3F9D9FE3,0x1969C795,0x1C699618,
     0x279A489E,0x249A7A28,0x0003F7A2,0x27FD9655,0x3F9D9FE7,0x19FD9655,
     0x279E6962,0x3F662966,0x3F660657,0x2767F9D9,0x0000003F,0x19FE0818,
     0x00000FE7,0x1B699657,0x2081F79D,0x259A79FF,0x3F8218A3,0x1EFE0816,
     0x1A65969B,0x2696279B,0x229669E7,0x00000FDE,0x19FE7656,0x00000FE7,
     0x17FE0815,0x1C6DA658,0x2696485F,0x00FE9A27,0x1969D656,0x217DC699,
     0x269E79A4,0x0003F9E3,0x18FE0814,0x3FA18FE8,0x00FE081C,0x1A69E79A,
     0x268A279E,0x3F7A28A6,0x22FDE796,0x26966FE2,0x1E8A59E7,0x1B69965B,
     0x3F9E58DD,0x258DD6D9,0x229679E6,0x1A6596DE,0x0003F69B,0x20FE081B,
     0x3F82181F,0x27FDA856,0x3FA1AFE0,0x0003F9D9,0x3F628614,0x1C6DA697,
     0x2696489E,0x20824966,0x2181F83F,0x00000FE0,0x2AA68A54,0x23967A6A,
     0x1D7A0862,0x228E4FDC,0x1B75E7E0,0x175965D9,0x0003F5D8,0x2181F81B,
     0x207E0FE0,0x0003F821,0x2181F81B,0x1F821FE0,0x1F821860,0x0000003F,
     0x3FA18A14,0x1EFDD759,0x3F91DFDE,0x0003F91D,0x23FE2899,0x3F8DCFE3,
     0x0003F8DC,0x3F6E56D8,0x256FF95B,0x0000003F,0x1E7E0899,0x218607DE,
     0x1F7E0FDF,0x218A2860,0x218A285D,0x3F81F7E0,0x1F82185F,0x2281F79E,
     0x0000003F,0x22860799,0x1F7E0862,0x21860FE1,0x1F79E7E0,0x1F79E7E3,
     0x3F821860,0x2181F7E1,0x1E8218A2,0x0000003F,0x3F81748F,0x20C3F816,
     0x0000003F,0x1865C814,0x1F75B658,0x27920FE0,0x23967A28,0x1F83F821,
     0x22860FDE,0x00FE27BF,0x1C79F814,0x1961865B,0x20FE071A,0x279648A1,
     0x249A7A28,0x00000FE0,0x1769E814,0x299A283F,0x2079A5FF,0x208A6A7F,
     0x0000003F,0x26922814,0x26A29A68,0x2387F864,0x1F863924,0x1F75C71D,
     0x1869C7FF,0x1C6985D7,0x1F83F81E,0x22860FDE,0x00FE27BF,0x28A6AA93,
     0x208A49A7,0x165D869E,0x207D85D6,0x1C7A0861,0x2379C6DB,0x2AAA99E5,
     0x0000003F,0x27FE7655,0x00000FD9,0x1DFDA998,0x1A6DD79E,0x2485D6DA,
     0x228A3FE6,0x259A6963,0x00000FE3,0x258E2815,0x2BAEAA67,0x23967A6A,
     0x196DD7A2,0x16555597,0x1E75B657,0x00000FE0,0x1C71D7DC,0x248E17DD,
     0x3F7E18E4,0x1E7BF75D,0x20FDF7FF,0x3F861FE0,0x238FF8A2,0x0000003F,
     0x2491C71C,0x3F75DFDC,0x1F7FF79E,0x21FE083F,0x3F8A2FE1,0x0003F8E3,
     0x2095B81B,0x20FDD83F,0x3F7E0FE3,0x0003F860,0x206E581B,0x20FE383F,
     0x3F860FDD,0x0003F7E0,0x1A99C81A,0x2083F824,0x20FDA83F,0x3F920FDC,
     0x0003F9A0,0x3F9A9993,0x17FE3A23,0x00000FE8,0x3F8A0798,0x20FE581B,
     0x00000FE0,0x3F697693,0x18FDD61D,0x00000FE9,0x3F8A0798,0x20FE581B,
     0x00000FE0,0x2181F81B,0x00FDF821,0x2181F81B,0x00000FE0,0x207E085B,
     0x3F7E0861,0x00FD7A55,0x1E822919,0x2281E75D,0x00000FE4,0x2281E719,
     0x1E8228E3,0x00000FDC,0x00FE95D3,0x17FE0813,0x00000FE9,0x17FE95D3,
     0x00000FE9,0x1BFE0818,0x3F6E5FE5,0x1D75E81B,0x238E281E,0x0003F822,
     0x3F82079B,0x2385F75B,0x3F8DD8A3,0x2385F75B,0x2183F863,0x1D7E18E3,
     0x0000003F,0x21FE185B,0x0003F91C,0x207DD79B,0x1F8228E2,0x3F89EFDD,
     0x1D7A089B,0x238A079D,0x3F75E822,0x1DFDF8DB,0x00000FE3,0x1F75D7DB,
     0x1F8638E1,0x1F75D7FF,0x3F8638E1,0x1E8228DB,0x238A079D,0x3F7A08A3,
     0x26FE6997,0x1B75F8A4,0x1F75B69A,0x00FE6922,0x1AFDA696,0x258E179C,
     0x218E59A6,0x00FDA71E,0x1F8A4997,0x1B69A6DD,0x269227DD,0x0000003F,
     0x26FE6997,0x1B75F8A4,0x1F75B69A,0x00FE6922,0x259A6697,0x1B75F8A4,
     0x1F75B69A,0x00FE6922,0x2086395B,0x3F91DFE0,0x24966997,0x26FDD7E2,
     0x1B75F8A4,0x1F75B69A,0x00FE6922,0x1BFDB6D7,0x2696381E,0x00000FE6,
     0x208607DC,0x3F820FDF,0x218A181B,0x20861FE0,0x0003F71E,0x25FDB6D7,
     0x3F99FFDB,0x00FE081C,0x15FD5551,0x207DD698,0x258E0FE0,0x3FAEBAA8,
     0x1BFDB6D7,0x2696381E,0x00000FE6,0x1A6DD7D7,0x227DD6DA,0x269E79A4,
     0x00FDF8A4,0x1AFDA696,0x258E179C,0x218E59A6,0x00FDA71E,0x26FE6997,
     0x1B75F8A4,0x1F75B69A,0x00FE6922,0x1DFDD759,0x3F96281E,0x1F8A5998,
     0x2379C6DC,0x229669A5,0x00FDB71F,0x2386081B,0x3F91DFE5,0x1E71B6D7,
     0x26FE68E1,0x00000FE6,0x26FE0698,0x00000FE0,0x20FDC615,0x3F920FDC,
     0x0003F928,0x26FE66D8,0x00000FDB,0x26FE0698,0x1969C7A0,0x0000003F,
     0x1BFDB998,0x3F99BFE6,0x1A6DD7D6,0x1E71A659,0x289E58E0,0x238A17FF,
     0x3FA279A5,0x1D7E18D7,0x3F65A6DC,0x269E7963,0x20FE08E5,0x24965922,
     0x1C75F863,0x00000FDB,0x1D71A617,0x2086181F,0x209A7A3F,0x0003F75E,
     0x1B75F897,0x1E71B69A,0x259648A0,0x1F7E08A4,0x3F9A48A0,0x1F8A4958,
     0x3F85E75D,0x1C6DB761,0x3F96385E,0x1F7E0898,0x26FE68E0,0x1B6DC7E2,
     0x228A179C,0x00FDE7E1,0x1C6985D6,0x3F69C75D,0x248A079C,0x3F8A59A6,
     0x1B657595,0x1C6DB71C,0x248E17DD,0x239669A5,0x23860821,0x0003FA25,
     0x1D75E81A,0x3F8E281E,0x27FD9757,0x1D7E3966,0x1F79CFDC,0x3F9238A1,
     0x1E75B658,0x3F6A0FE6,0x1CFD7756,0x217DD6DB,0x259FF963,0x299E5924,
     0x00000FEA,0x1B71D697,0x259A7FDA,0x3F69D823,0x1F7E0898,0x23FE68E0,
     0x1F75D7A0,0x1E8BF922,0x2175B6DC,0x3F7A08A2,0x1B71E818,0x217DD71B,
     0x259A6963,0x00FE08A4,0x23FDA795,0x17FE6964,0x00FE9719,0x1D71B6D7,
     0x2696385F,0x208A4966,0x3F5DB71E,0x1B75FA57,0x1E71B69A,0x259648A0,
     0x00FE18E4,0x18FDE856,0x00FE875A,0x1C6985D6,0x1D6DB75D,0x279A489F,
     0x00000FE7,0x1865B755,0x1F71A658,0x28A27962,0x1E822926,0x00000FDB,
     0x2375B657,0x27A3F9E5,0x3F6196E5,0x15FDC914,0x1B6DA616,0x1F75B69A,
     0x29A26922,0x0000003F,0x1761A714,0x1D6D9617,0x207E0FDF,0x289E58E1,
     0x00FE69E8,0x1BFE0818,0x3F95BFE5,0x00FE081E,0x209D9819,0x2066783F,
     0x0000003F,0x22FDF7D9,0x24925FE2,0x1C7A3965,0x3F96475C,0x2596475C,
     0x1C75F8A4,0x3F71D75C,0x21FE081E,0x21820FE1,0x0003F821,0x23FDE81B,
     0x3F91DFE1,0x0003F91D,0x3F6E56D8,0x2386079C,0x21860823,0x21820FE3,
     0x0003F821,0x279E6997,0x20862926,0x23FDD79F,0x1E7E0862,0x1A65969C,
     0x00000FDA,0x2186081E,0x21820FE0,0x0003F821,0x2186081E,0x20821FE0,
     0x00FE0861,0x3F95B958,0x1FFDE79B,0x3F8DEFDF,0x0003F8DE,0x22FE185B,
     0x3F89DFE2,0x0003F89D,0x3F95B958,0x256FF95B,0x0000003F,0x1E79F85A,
     0x1E7FF860,0x1EFE087F,0x1E82185F,0x1E7E1860,0x1EFE183F,0x1E821FDF,
     0x00FE17DE,0x228A17DB,0x2287F7E0,0x22FE07FF,0x2281F7E1,0x2285F7E0,
     0x22FDF83F,0x2281FFE1,0x00FDF862,0x3F81A5D5,0x20A7F819,0x0000003F,
     0x217DE795,0x1657F8A2,0x1B69A658,0x269A58DD,0x3FAEAA27,0x1E7FFAD5,
     0x3F86285F,0x20FE0818,0x3F860FDF,0x1F83F85F,0x1C7FF71E,0x248A183F,
     0x20FE487F,0x1EFD869C,0x2483F61A,0x268BFA26,0x1D7A0FE8,0x1D7A0FDD,
     0x238E283F,0x3F8E283F,0x20FE0818,0x3F820FE0,0x2183F7E0,0x20FE17FF,
     0x2183F79F,0x2281EFE2,0x1B71E83F,0x2592283F,0x2281E73F,0x1C7A0FE4,
     0x3F69965A,0x279A48A0,0x1E73F9A7,0x20FE48A0,0x1969B75E,0x20FDA618,
     0x279A58E2,0x1BFE6A28,0x3F96381D,0x20FDF818,0x3F85FFE1,0x28A268E0,
     0x27A27967,0x218E48E5,0x1B75C75F,0x196D9619,0x2075A618,0x0000003F,
     0x218A391A,0x1D71C760,0x1E821860,0x2385F75D,0x00000FE4,0x23FE375B,
     0x00000FDD,0x29AABAD7,0x1A761926,0x1A6175D8,0x299A485D,0x00FEBAEA,
     0x24967A16,0x1C79F862,0x1961865B,0x217DE71B,0x289E5922,0x00000FE8,
     0x1969C7D9,0x217DC699,0x269E79A4,0x00FDF864,0x2699A69A,0x00000FDA,
     0x209D9819,0x0000003F,0x2681A81A,0x00000FE0,0x1D61E818,0x288E581B,
     0x0003F822,0x239A3897,0x23963FE2,0x00FE56BF,0x1DFE081B,0x3F8E281E,
     0x00FE281E,0x1D69D797,0x1D6DDFDE,0x00FE66FF,0x1DFE081B,0x3F8E281E,
     0x00FE281E,0x2182085E,0x0003F821,0x2186081E,0x00000FE0,0x2182085E,
     0x0003F821,0x00FDC91B,0x1F7E089D,0x0003F8A0,0x2186079C,0x0003F7A0,
     0x00FE471A,0x1CFE081A,0x00000FE4,0x1CFE471A,0x00000FE4,0x1DFE081B,
     0x3F763FE3},
    {0x1D61552A,0x29A68960,0x18760968,0x00FD4515,0x2951762A,0x0000003F,
     0x1559866A,0x18595514,0x29A5F71A,0x0000003F,0x1C71452A,0x268E179D,
     0x27A29A68,0x00000FE5,0x208A252A,0x0003FA54,0x1C75452A,0x2179C6DB,
     0x29A689A3,0x00FE59E8,0x145155EA,0x2689D615,0x26A29A68,0x1C75F8A3,
     0x3F89F75C,0x1482952A,0x00000FD4,0x195D552A,0x2079D71B,0x29A27962,
     0x22967A29,0x1B71D7A0,0x145155D9,0x0000003F,0x2181E6EA,0x1A6DE821,
     0x15514557,0x289606D7,0x3F9A8A69,0x14829529,0x3F8A2829,0x1482952A,
     0x1A616554,0x1E81E75C,0x258A07DE,0x3FA69A27,0x1455766B,0x1C657554,
     0x29A26921,0x3F926A29,0x1482952A,0x1C657554,0x29A26921,0x00000FE9,
     0x14829529,0x2079E814,0x0003FA69,0x14829528,0x3F79E814,0x1455766B,
     0x1C657554,0x29A26921,0x21926A29,0x00FE1860,0x1482952B,0x3F79E829,
     0x00FE9524,0x289E4528,0x249E8A69,0x00000FE2,0x1482952A,0x3FA5D822,
     0x29829527,0x00000FE9,0x1482952C,0x20A54829,0x0003FA54,0x1482952B,
     0x3FA54829,0x195D552B,0x289A485C,0x249A8A69,0x155D9721,0x0003F514,
     0x1482952A,0x1B616554,0x3F7DF79D,0x195D552B,0x289A485C,0x249A8A69,
     0x155D9721,0x2B960514,0x0000003F,0x1482952A,0x1A616554,0x2079E75C,
     0x0003FA5E,0x145155EA,0x1C6D95D5,0x238607DD,0x28A69A26,0x00000FE6,
     0x14829528,0x00000FD4,0x289A352B,0x239A8A69,0x00000FD4,0x14829529,
     0x00000FE9,0x1482952C,0x20A54829,0x0003FA54,0x1482952A,0x00000FE9,
     0x20A5E529,0x0003F794,0x1482952A,0x3FA69814,0x14829529,0x3F8A2829,
     0x1482952A,0x1A616554,0x1E81E75C,0x258A07DE,0x3FA69A27,0x14829527,
     0x00000FD4,0x14829529,0x3FA69829,0x14829529,0x2079E814,0x0003FA69,
     0x1482952A,0x3FA69814,0x1482952B,0x3F79E829,0x195D552B,0x289A485C,
     0x249A8A69,0x155D9721,0x1E7A0514,0x0000003F,0x00FE9524,0x1482952A,
     0x3FA5D822,0x14829529,0x00000FE9,0x1482952C,0x20A54829,0x0003FA54,
     0x1482952B,0x3FA54829,0x1E814529,0x3FA6981E,0x195D552B,0x289A485C,
     0x249A8A69,0x155D9721,0x0003F514,0x1482952B,0x3F514829,0x1482952A,
     0x1B616554,0x3F7DF79D,0x20A5E529,0x29A60514,0x0000003F,0x14829528,
     0x00000FD4,0x14557669,0x296D7554,0x14557660,0x3F6D7554,0x1982952A,
     0x2281D6DA,0x228E4923,0x1969B760,0x00000FD9,0x2982952A,0x00000FD4,
     0x1A82952B,0x2285F6DA,0x1F8628E3,0x00FDA69B,0x1E8A9A6A,0x145155DA,
     0x2279A5D5,0x0003FA69,0x2082962C,0x3FA69820,0x00FF0424,0x2279E6A6,
     0x228A69A2,0x3F69A79E,0x1082D42A,0x1961782D,0x145155D8,0x1C6D95D5,
     0x208A07DD,0x1F79C6D9,0x29A268A0,0x24966A29,0x0003F9A5,0x27822525,
     0x3F9E8A68,0x1083042B,0x2075D830,0x0003F8E3,0x3FA605EC,0x15598669,
     0x18595514,0x227DD71A,0x28A689E0,0x00000FE7,0x124D246C,0x13450411,
     0x2CA1C615,0x1851282E,0x2FB6BA24,0x2DBAFC30,0x0003FBEE,0x1C75C6E5,
     0x29A2781B,0x0003F9E8,0x1C75C6E5,0x28A6881B,0x2DB2AA27,0x0000003F,
     0x3FA605EC,0x10830427,0x20410830,0x0003FC30,0x10830427,0x20410830,
     0x0003FC30,0x2085D668,0x269A08A2,0x0000003F,0x14491427,0x1D6D9616,
     0x154D181F,0x1E71A617,0x289A48A0,0x20BEDAE9,0x289E58E1,0x30BEEB2A,
     0x0000003F,0x14491427,0x1D6D9616,0x154D181F,0x1E71A617,0x289A48A0,
     0x20BEDAE9,0x289E58E1,0x30BEEB2A,0x0000003F,0x209DB6F0,0x29220A5C,
     0x0000003F,0x2179A5AC,0x259A6963,0x1E696823,0x269A58E1,0x278E08E5,
     0x2A9E382A,0x00FEAAA0,0x165D972C,0x207595D6,0x1C82A962,0x175965D9,
     0x258A0759,0x00000FEA,0x2075856C,0x20758560,0x2BA23820,0x2BA23820,
     0x0000003F,0x269A58AC,0x1E7E18E5,0x1D7E07DE,0x1659765B,0x1F75B657,
     0x1F79E7E0,0x269A58E1,0x278A08A5,0x2A9E282A,0x00FEAAA0,0x1B6DC76D,
     0x289A379C,0x259E8A69,0x1B720863,0x155155D9,0x267DC657,0x27A29A68,
     0x0000003F,0x1982766B,0x00000FE7,0x19826668,0x1C75D71A,0x1A69965A,
     0x258E2819,0x228E59A6,0x00000FE2,0x2592382B,0x1F863925,0x1C6DB71D,
     0x259648DD,0x1D7E18E4,0x1D71B6DC,0x00000FE0,0x217DD724,0x218E4923,
     0x2071C75F,0x227A085F,0x1D823760,0x208DD823,0x217E089E,0x0000003F,
     0x1C924724,0x208DD81C,0x237608DD,0x1D823760,0x208DD823,0x0003F8DD,
     0x1A8E36A5,0x1D822760,0x208A0822,0x0003F8A0,0x2675D9A5,0x2381E8E0,
     0x207A081E,0x0003F7A0,0x1E7A56A6,0x1A8206A5,0x2081E820,0x20960825,
     0x0003F7A0,0x208A07AD,0x2082581B,0x00000FE0,0x206976A8,0x1881D61D,
     0x00000FE9,0x208A07AD,0x2082581B,0x00000FE0,0x209A99A8,0x17823A23,
     0x00000FE8,0x155155A5,0x00FDA657,0x28A689E5,0x00000FE7,0x27A29A25,
     0x3FB6CAA8,0x00FF042B,0x19552427,0x2EAE789E,0x00000FF0,0x19552427,
     0x2EAE789E,0x00000FF0,0x00FE082D,0x208295ED,0x00000FE0,0x2381D76D,
     0x00000FE3,0x1D8266A8,0x3F8DD823,0x2179C6E5,0x1E863923,0x0003F6DC,
     0x3F91B765,0x1C6DB725,0x3F92481E,0x1C6DB725,0x1F7E07DE,0x239248E0,
     0x0000003F,0x1B8246E5,0x0003F861,0x1E79F6E5,0x2492385F,0x3F6DB823,
     0x1E71B6E5,0x218E48E1,0x3F85F79F,0x1B8246E5,0x00000FDB,0x1F79C6E5,
     0x1B6DC79F,0x248E07E0,0x3F7E08E4,0x208607A5,0x1E71B71E,0x3F9248E1,
     0x1E8296EA,0x1E71B6DC,0x29A268E1,0x00FE6A29,0x1E829529,0x1E71B6DC,
     0x29A268E1,0x00FE6A29,0x1B6DC7A9,0x268E179C,0x26A29A68,0x0000003F,
     0x1E82952A,0x1E71B6DC,0x29A268E1,0x00FE6A29,0x1D7E1869,0x1E71B6DC,
     0x29A268E1,0x00FE6A29,0x18554527,0x3F6DB829,0x2FBAB6EA,0x1E82FC30,
     0x1E71B6DC,0x29A268E1,0x00FE6A29,0x1F82952A,0x1F71B6DC,0x00000FE9,
     0x13515524,0x3FA5B814,0x13515525,0x2FB1B814,0x0003FC30,0x1B829528,
     0x3FA61825,0x00FE9524,0x1F8296EF,0x1F71B6DC,0x1B71F829,0x3FA5F71B,
     0x1F8296EA,0x1F71B6DC,0x00000FE9,0x2179C6EA,0x29A689A3,0x1E8639A8,
     0x00FDB6DC,0x1E8306E9,0x1E71B6DC,0x29A268E1,0x00FE6A29,0x1E8306EA,
     0x1E71B6DC,0x29A268E1,0x00FE6A29,0x218296E6,0x3F6DB71E,0x1B6DC7A9,
     0x2286079C,0x29A26963,0x00FE6A29,0x29A25527,0x3F6DB829,0x29A256EA,
     0x1B825A29,0x00000FE9,0x1B8296E8,0x00000FE9,0x1B8296EB,0x20A5B829,
     0x0003FA5B,0x1B8296E9,0x00000FE9,0x1B8296E8,0x30C2FB69,0x0000003F,
     0x1B8296E9,0x3FA6981B,0x2079C6EB,0x29A689A3,0x1B7A2968,0x1E71B6E0,
     0x3FA69A26,0x1B5D552A,0x20C2889E,0x1B656514,0x1D81D75C,0x2796281E,
     0x27A29A68,0x00000FE4,0x1B6DC7AA,0x2992075C,0x2981E6E0,0x0003FC2D,
     0x1E71B6E9,0x29A27921,0x208E6A29,0x155D96DD,0x3F5D5514,0x1B6DC768,
     0x2086079C,0x289A48A1,0x3F9A8A69,0x17595527,0x19819658,0x258E075B,
     0x2FB6BA67,0x00FEEC30,0x1B6DD7EA,0x20A6279C,0x1B6DC7A2,0x3FC2581D,
     0x1B6DD7EA,0x289A379C,0x23968A69,0x145586E0,0x1E6D8594,0x0003F8A0,
     0x289A26E5,0x3F967A69,0x1C8296E9,0x2181C6DB,0x238A1821,0x3FA29A68,
     0x16554528,0x3FA5B829,0x1F8306EB,0x28A699E4,0x226E08A6,0x27A69A26,
     0x00000FE5,0x2685B6E9,0x2079B829,0x3FA689A3,0x17595528,0x19819658,
     0x217DD6DA,0x238A08A2,0x2CAA89A4,0x3FC30BED,0x2179C6E9,0x29A689E4,
     0x1D8239A8,0x00FDB6DC,0x1B8296EB,0x1E8299A1,0x00FDB6DC,0x289E4869,
     0x239A8A69,0x1B6DC760,0x3FC2179C,0x1E71B6EB,0x29A27921,0x208E6A29,
     0x00FDB71D,0x1E8296EA,0x00FDB6DC,0x1B6DD7EA,0x299E479C,0x1E8A6A29,
     0x00000FDB,0x227DD72B,0x29A689E5,0x1D8239A8,0x2685D6DB,0x00000FF0,
     0x2E75B6E9,0x1D6E0C30,0x3FC2EAE0,0x1F83052B,0x1E71B6DD,0x29A689A3,
     0x1B7A39A8,0x0000003F,0x227DC6EB,0x28A69A25,0x28961825,0x22968A69,
     0x00FDB71F,0x208266E8,0x3F9A6820,0x00FE56A2,0x18924627,0x2871CA20,
     0x0000003F,0x1682A5A8,0x1B69A82A,0x1A65969B,0x208A179C,0x248A075C,
     0x249669A5,0x3F9248E3,0x1B8206E3,0x24923820,0x0003F8E3,0x1A8256A6,
     0x2079E825,0x0003F861,0x3F9A06A8,0x1C6DB725,0x1F8207DE,0x2492381E,
     0x0003F8E3,0x176185E9,0x1D698596,0x178299E3,0x289A3759,0x28A29AAA,
     0x00000FE9,0x1E7DF7A3,0x2492381E,0x0003F8E3,0x1E7DF7A3,0x2392481E,
     0x00FE7963,0x3F9A06A8,0x1682A5A5,0x2059682A,0x0003FAAA,0x1682A5A5,
     0x2059682A,0x0003FAAA,0x2085D668,0x269A08A2,0x0000003F,0x1A6175A5,
     0x1A5E079C,0x1881F720,0x207DE71A,0x289A48A1,0x26824860,0x26922829,
     0x00FEAA68,0x1A6175A6,0x1A5E079C,0x1881F720,0x207DE71A,0x289A48A1,
     0x26824860,0x26922829,0x00FEAA68,0x2095D769,0x2652099D,0x0000003F,
     0x145155EB,0x26A205D5,0x1865E8A4,0x1E6585D7,0x20A26922,0x29A20A28,
     0x3FA29AAA,0x218216E8,0x20AA182A,0x186E0AAA,0x15620556,0x155986E0,
     0x1B815620,0x1A81B659,0x196E06DA,0x1A6A06D9,0x1D71B81B,0x2075B820,
     0x2075C6E0,0x3F81D6E0,0x1C8195E8,0x208E181E,0x2A9E0AA7,0x1582AAA0,
     0x17560617,0x185D8818,0x1C75C660,0x1C75C660,0x1D71D760,0x2285E81D,
     0x208A1822,0x208A285E,0x228A08A1,0x238228A1,0x27A289E6,0x238289E5,
     0x27A289E6,0x288289E5,0x3FA289E8,0x2782A9E8,0x20AAA82A,0x239A8A27,
     0x1B7608A2,0x155986DA,0x1A6D8595,0x228A075B,0x27A289A3,0x0000003F,
     0x23860826,0x20863924,0x1B6DC79F,0x248E179C,0x00000FE4,0x1D823765,
     0x00000FE3,0x218E6A2B,0x1D71C75E,0x238A17DE,0x1F8A3924,0x00FD869D,
     0x238E282A,0x1C75E862,0x2281F75C,0x1E8628E3,0x1F75C71D,0x00000FE0,
     0x1F71A667,0x279E6921,0x1C7E1926,0x00FD965A,0x1A9A66A6,0x00000FDA,
     0x18924627,0x0000003F,0x20AA05A6,0x00000FD6,0x2175D5E8,0x1D8678E7,
     0x0003F5DD,0x2281E769,0x2281E823,0x00FE0820,0x1E8266E5,0x2079D69D,
     0x00FDD6DD,0x2281E769,0x2281E823,0x00FE0820,0x228256A5,0x208A39A3,
     0x00FE3963,0x1B6DC723,0x0003F7DD,0x239248E3,0x00000FE3,0x238E4923,
     0x0003F9E5,0x00FE56A5,0x2179C6A4,0x0003F963,0x2179C6A3,0x0003F963,
     0x00FE0826,0x20824726,0x00000FE0,0x2281E7A6,0x00000FE2,0x1E823765,
     0x3F89E822},
    {  1,  5,  7, 11, 15, 17, 21, 26, 28, 35, 40, 42, 47, 51, 55,
      58, 60, 65, 67, 68, 71, 73, 75, 78, 80, 85, 88, 94, 98,103,
     105,108,110,113,115,117,119,121,126,128,130,133,135,137,143,
     144,146,148,151,153,155,160,162,165,168,170,174,179,181,185,
     189,191,192,195,204,206,209,210,215,222,225,229,230,233,236,
     239,248,257,260,267,273,278,288,296,298,304,311,319,325,329,
     333,338,341,344,347,350,352,354,356,357,360,363,364,366,368,
     370,373,374,376,380,382,385,388,390,394,397,401,405,409,413,
     417,419,424,427,429,432,434,435,439,442,446,450,454,456,460,
     462,465,467,470,472,475,477,482,489,493,498,502,507,511,517,
     519,523,525,530,533,539,543,546,550,554,556,560,565,568,573,
     578,580,581,584,591,594,597,598,602,608,611,614,615,618,621,
     624,632,640,643,650,663,682,691,696,698,703,709,713,715,717,
     719,722,725,728,731,734,736,738,740,741,743,745,746,748,750}
};
#define HERSHEY__NBITMX 30
#define HERSHEY__ROMAN   1
#define HERSHEY__DIGIT   2
#define HERSHEY__MATH    3
#define HERSHEY__SPECIAL 4
#define HERSHEY__GREEK   5
#define HERSHEY__UPPER   1
#define HERSHEY__LOWER   2

void SpecialCharPoints (
 char This 
,float aScale
,int* aNumber 
,int* aMax_point 
,float* aX 
,float* aY
,float& aWidth
)
{
  // PAW special characters.
  Extract(HERSHEY__SPECIAL,
          HERSHEY__UPPER,This,aScale,aNumber,aMax_point,aX,aY,aWidth);
}

void GreekCharPoints (
 char This 
,float aScale
,int* aNumber 
,int* aMax_point 
,float* aX 
,float* aY
,float& aWidth
)
{
  switch(This) {
  case 'A':Extract(HERSHEY__GREEK,HERSHEY__UPPER,'A',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'B':Extract(HERSHEY__GREEK,HERSHEY__UPPER,'B',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'C':Extract(HERSHEY__GREEK,HERSHEY__UPPER,'C',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'D':Extract(HERSHEY__GREEK,HERSHEY__UPPER,'D',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'E':Extract(HERSHEY__GREEK,HERSHEY__UPPER,'E',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'F':Extract(HERSHEY__GREEK,HERSHEY__UPPER,'F',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'G':Extract(HERSHEY__GREEK,HERSHEY__UPPER,'G',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'H':Extract(HERSHEY__GREEK,HERSHEY__UPPER,'H',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'I':Extract(HERSHEY__GREEK,HERSHEY__UPPER,'I',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'J':Extract(HERSHEY__GREEK,HERSHEY__UPPER,'J',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'K':Extract(HERSHEY__GREEK,HERSHEY__UPPER,'K',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'L':Extract(HERSHEY__GREEK,HERSHEY__UPPER,'L',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'M':Extract(HERSHEY__GREEK,HERSHEY__UPPER,'M',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'N':Extract(HERSHEY__GREEK,HERSHEY__UPPER,'N',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'O':Extract(HERSHEY__GREEK,HERSHEY__UPPER,'O',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'P':Extract(HERSHEY__GREEK,HERSHEY__UPPER,'P',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'Q':Extract(HERSHEY__GREEK,HERSHEY__UPPER,'Q',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'R':Extract(HERSHEY__GREEK,HERSHEY__UPPER,'R',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'S':Extract(HERSHEY__GREEK,HERSHEY__UPPER,'S',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'T':Extract(HERSHEY__GREEK,HERSHEY__UPPER,'T',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'U':Extract(HERSHEY__GREEK,HERSHEY__UPPER,'U',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'V':Extract(HERSHEY__GREEK,HERSHEY__UPPER,'V',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'W':Extract(HERSHEY__GREEK,HERSHEY__UPPER,'W',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'X':Extract(HERSHEY__GREEK,HERSHEY__UPPER,'X',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'Y':Extract(HERSHEY__GREEK,HERSHEY__UPPER,'Y',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'Z':Extract(HERSHEY__GREEK,HERSHEY__UPPER,'Z',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
    
  case 'a':Extract(HERSHEY__GREEK,HERSHEY__LOWER,'A',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'b':Extract(HERSHEY__GREEK,HERSHEY__LOWER,'B',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'c':Extract(HERSHEY__GREEK,HERSHEY__LOWER,'C',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'd':Extract(HERSHEY__GREEK,HERSHEY__LOWER,'D',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'e':Extract(HERSHEY__GREEK,HERSHEY__LOWER,'E',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'f':Extract(HERSHEY__GREEK,HERSHEY__LOWER,'F',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'g':Extract(HERSHEY__GREEK,HERSHEY__LOWER,'G',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'h':Extract(HERSHEY__GREEK,HERSHEY__LOWER,'H',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'i':Extract(HERSHEY__GREEK,HERSHEY__LOWER,'I',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'j':Extract(HERSHEY__GREEK,HERSHEY__LOWER,'J',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'k':Extract(HERSHEY__GREEK,HERSHEY__LOWER,'K',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'l':Extract(HERSHEY__GREEK,HERSHEY__LOWER,'L',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'm':Extract(HERSHEY__GREEK,HERSHEY__LOWER,'M',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'n':Extract(HERSHEY__GREEK,HERSHEY__LOWER,'N',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'o':Extract(HERSHEY__GREEK,HERSHEY__LOWER,'O',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'p':Extract(HERSHEY__GREEK,HERSHEY__LOWER,'P',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'q':Extract(HERSHEY__GREEK,HERSHEY__LOWER,'Q',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'r':Extract(HERSHEY__GREEK,HERSHEY__LOWER,'R',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 's':Extract(HERSHEY__GREEK,HERSHEY__LOWER,'S',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 't':Extract(HERSHEY__GREEK,HERSHEY__LOWER,'T',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'u':Extract(HERSHEY__GREEK,HERSHEY__LOWER,'U',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'v':Extract(HERSHEY__GREEK,HERSHEY__LOWER,'V',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'w':Extract(HERSHEY__GREEK,HERSHEY__LOWER,'W',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'x':Extract(HERSHEY__GREEK,HERSHEY__LOWER,'X',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'y':Extract(HERSHEY__GREEK,HERSHEY__LOWER,'Y',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'z':Extract(HERSHEY__GREEK,HERSHEY__LOWER,'Z',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
    
  default : LatinCharPoints (This,aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  }
}

void LatinCharPoints (
 char This 
,float aScale
,int* aNumber 
,int* aMax_point 
,float* aX 
,float* aY 
,float& aWidth
){

  // Latin correspondance with HPLSOF                                         
  // ABCDEFGHIJKLMNOPQRSTUVWXYZ  done with HERSHEY__ROMAN UPPER
  // abcdefghijklmnopqrstuvwxyz  done with HERSHEY__ROMAN LOWER
  // 0123456789                  done with HERSHEY__DIGIT
  // .,/()-+=*                   done with HERSHEY__MATH
  // $!#>?:;<[]{}%&              done with HERSHEY__SPECIAL

  switch(This) {
  case 'A':Extract(HERSHEY__ROMAN,HERSHEY__UPPER,'A',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'B':Extract(HERSHEY__ROMAN,HERSHEY__UPPER,'B',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'C':Extract(HERSHEY__ROMAN,HERSHEY__UPPER,'C',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'D':Extract(HERSHEY__ROMAN,HERSHEY__UPPER,'D',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'E':Extract(HERSHEY__ROMAN,HERSHEY__UPPER,'E',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'F':Extract(HERSHEY__ROMAN,HERSHEY__UPPER,'F',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'G':Extract(HERSHEY__ROMAN,HERSHEY__UPPER,'G',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'H':Extract(HERSHEY__ROMAN,HERSHEY__UPPER,'H',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'I':Extract(HERSHEY__ROMAN,HERSHEY__UPPER,'I',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'J':Extract(HERSHEY__ROMAN,HERSHEY__UPPER,'J',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'K':Extract(HERSHEY__ROMAN,HERSHEY__UPPER,'K',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'L':Extract(HERSHEY__ROMAN,HERSHEY__UPPER,'L',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'M':Extract(HERSHEY__ROMAN,HERSHEY__UPPER,'M',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'N':Extract(HERSHEY__ROMAN,HERSHEY__UPPER,'N',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'O':Extract(HERSHEY__ROMAN,HERSHEY__UPPER,'O',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'P':Extract(HERSHEY__ROMAN,HERSHEY__UPPER,'P',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'Q':Extract(HERSHEY__ROMAN,HERSHEY__UPPER,'Q',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'R':Extract(HERSHEY__ROMAN,HERSHEY__UPPER,'R',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'S':Extract(HERSHEY__ROMAN,HERSHEY__UPPER,'S',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'T':Extract(HERSHEY__ROMAN,HERSHEY__UPPER,'T',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'U':Extract(HERSHEY__ROMAN,HERSHEY__UPPER,'U',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'V':Extract(HERSHEY__ROMAN,HERSHEY__UPPER,'V',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'W':Extract(HERSHEY__ROMAN,HERSHEY__UPPER,'W',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'X':Extract(HERSHEY__ROMAN,HERSHEY__UPPER,'X',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'Y':Extract(HERSHEY__ROMAN,HERSHEY__UPPER,'Y',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'Z':Extract(HERSHEY__ROMAN,HERSHEY__UPPER,'Z',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
    
  case 'a':Extract(HERSHEY__ROMAN,HERSHEY__LOWER,'A',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'b':Extract(HERSHEY__ROMAN,HERSHEY__LOWER,'B',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'c':Extract(HERSHEY__ROMAN,HERSHEY__LOWER,'C',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'd':Extract(HERSHEY__ROMAN,HERSHEY__LOWER,'D',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'e':Extract(HERSHEY__ROMAN,HERSHEY__LOWER,'E',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'f':Extract(HERSHEY__ROMAN,HERSHEY__LOWER,'F',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'g':Extract(HERSHEY__ROMAN,HERSHEY__LOWER,'G',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'h':Extract(HERSHEY__ROMAN,HERSHEY__LOWER,'H',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'i':Extract(HERSHEY__ROMAN,HERSHEY__LOWER,'I',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'j':Extract(HERSHEY__ROMAN,HERSHEY__LOWER,'J',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'k':Extract(HERSHEY__ROMAN,HERSHEY__LOWER,'K',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'l':Extract(HERSHEY__ROMAN,HERSHEY__LOWER,'L',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'm':Extract(HERSHEY__ROMAN,HERSHEY__LOWER,'M',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'n':Extract(HERSHEY__ROMAN,HERSHEY__LOWER,'N',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'o':Extract(HERSHEY__ROMAN,HERSHEY__LOWER,'O',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'p':Extract(HERSHEY__ROMAN,HERSHEY__LOWER,'P',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'q':Extract(HERSHEY__ROMAN,HERSHEY__LOWER,'Q',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'r':Extract(HERSHEY__ROMAN,HERSHEY__LOWER,'R',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 's':Extract(HERSHEY__ROMAN,HERSHEY__LOWER,'S',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 't':Extract(HERSHEY__ROMAN,HERSHEY__LOWER,'T',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'u':Extract(HERSHEY__ROMAN,HERSHEY__LOWER,'U',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'v':Extract(HERSHEY__ROMAN,HERSHEY__LOWER,'V',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'w':Extract(HERSHEY__ROMAN,HERSHEY__LOWER,'W',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'x':Extract(HERSHEY__ROMAN,HERSHEY__LOWER,'X',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'y':Extract(HERSHEY__ROMAN,HERSHEY__LOWER,'Y',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case 'z':Extract(HERSHEY__ROMAN,HERSHEY__LOWER,'Z',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
    
  case '0':Extract(HERSHEY__DIGIT,HERSHEY__UPPER,'0',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case '1':Extract(HERSHEY__DIGIT,HERSHEY__UPPER,'1',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case '2':Extract(HERSHEY__DIGIT,HERSHEY__UPPER,'2',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case '3':Extract(HERSHEY__DIGIT,HERSHEY__UPPER,'3',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case '4':Extract(HERSHEY__DIGIT,HERSHEY__UPPER,'4',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case '5':Extract(HERSHEY__DIGIT,HERSHEY__UPPER,'5',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case '6':Extract(HERSHEY__DIGIT,HERSHEY__UPPER,'6',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case '7':Extract(HERSHEY__DIGIT,HERSHEY__UPPER,'7',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case '8':Extract(HERSHEY__DIGIT,HERSHEY__UPPER,'8',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case '9':Extract(HERSHEY__DIGIT,HERSHEY__UPPER,'9',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
    
  case '.':Extract(HERSHEY__MATH,HERSHEY__UPPER,'.',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case ',':Extract(HERSHEY__MATH,HERSHEY__UPPER,',',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case '/':Extract(HERSHEY__MATH,HERSHEY__UPPER,'/',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case '(':Extract(HERSHEY__MATH,HERSHEY__UPPER,'(',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case ')':Extract(HERSHEY__MATH,HERSHEY__UPPER,')',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case '-':Extract(HERSHEY__MATH,HERSHEY__UPPER,'-',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case '+':Extract(HERSHEY__MATH,HERSHEY__UPPER,'+',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case '=':Extract(HERSHEY__MATH,HERSHEY__UPPER,'=',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case '*':Extract(HERSHEY__MATH,HERSHEY__UPPER,'*',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
    
  case '$':Extract(HERSHEY__SPECIAL,HERSHEY__UPPER,'D',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case '!':Extract(HERSHEY__SPECIAL,HERSHEY__UPPER,'E',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case '#':Extract(HERSHEY__SPECIAL,HERSHEY__UPPER,'F',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case '>':Extract(HERSHEY__SPECIAL,HERSHEY__UPPER,'G',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case '?':Extract(HERSHEY__SPECIAL,HERSHEY__UPPER,'H',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case ':':Extract(HERSHEY__SPECIAL,HERSHEY__UPPER,'J',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case ';':Extract(HERSHEY__SPECIAL,HERSHEY__UPPER,'K',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case '<':Extract(HERSHEY__SPECIAL,HERSHEY__UPPER,'L',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case '[':Extract(HERSHEY__SPECIAL,HERSHEY__UPPER,'M',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case ']':Extract(HERSHEY__SPECIAL,HERSHEY__UPPER,'N',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case '{':Extract(HERSHEY__SPECIAL,HERSHEY__UPPER,'P',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case '}':Extract(HERSHEY__SPECIAL,HERSHEY__UPPER,'Q',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case '%':Extract(HERSHEY__SPECIAL,HERSHEY__UPPER,'Y',aScale,aNumber,aMax_point,aX,aY,aWidth);break;
  case '&':Extract(HERSHEY__SPECIAL,HERSHEY__UPPER,'W',aScale,aNumber,aMax_point,aX,aY,aWidth);break;

  case '_':if(aNumber!=NULL) *aNumber      = 1;
    aMax_point[0] = 2;
    aX[0] = 0;
    aY[0] = 0;
    aX[1] = aScale;
    aY[1] = 0;
    aWidth = aScale;
    break;

  case ' ':if(aNumber!=NULL) *aNumber = 0;
    aWidth = aScale/2;
    break;

  default :if(aNumber!=NULL) *aNumber = 0;
    aWidth = aScale;
    break;
  }
}

void Extract (
 int aType 
,int aCase 
,char aC 
,float aScale 
,int* aN
,int* aMax_point 
,float* aX 
,float* aY 
,float& aWidth 
){
  int   count;
  int   npoly,ichr,iw,idif,ibit,kx,ky,iy0,ix0,dky,ipoint;
  float scale,size;
  float max_x;

  ichr = 1;
  if    (aType==HERSHEY__DIGIT) {
    switch(aC) {
    case '0':ichr= 1;break;
    case '1':ichr= 2;break;
    case '2':ichr= 3;break;
    case '3':ichr= 4;break;
    case '4':ichr= 5;break;
    case '5':ichr= 6;break;
    case '6':ichr= 7;break;
    case '7':ichr= 8;break;
    case '8':ichr= 9;break;
    case '9':ichr=10;break;
    }
  } else if (aType==HERSHEY__ROMAN) {
    switch(aC) {
    case 'A':ichr=11;break;
    case 'B':ichr=12;break;
    case 'C':ichr=13;break;
    case 'D':ichr=14;break;
    case 'E':ichr=15;break;
    case 'F':ichr=16;break;
    case 'G':ichr=17;break;
    case 'H':ichr=18;break;
    case 'I':ichr=19;break;
    case 'J':ichr=20;break;
    case 'K':ichr=21;break;
    case 'L':ichr=22;break;
    case 'M':ichr=23;break;
    case 'N':ichr=24;break;
    case 'O':ichr=25;break;
    case 'P':ichr=26;break;
    case 'Q':ichr=27;break;
    case 'R':ichr=28;break;
    case 'S':ichr=29;break;
    case 'T':ichr=30;break;
    case 'U':ichr=31;break;
    case 'V':ichr=32;break;
    case 'W':ichr=33;break;
    case 'X':ichr=34;break;
    case 'Y':ichr=35;break;
    case 'Z':ichr=36;break;
    }
  } else if (aType==HERSHEY__GREEK) {
    switch(aC) {
    case 'A':ichr=1 ;break;
    case 'B':ichr=2 ;break;
    case 'C':ichr=7 ;break;
    case 'D':ichr=4 ;break;
    case 'E':ichr=5 ;break;
    case 'F':ichr=21;break;
    case 'G':ichr=3 ;break;
    case 'H':ichr=22;break;
    case 'I':ichr=9 ;break;
    case 'J':ichr=9 ;break;
    case 'K':ichr=10;break;
    case 'L':ichr=11;break;
    case 'M':ichr=12;break;
    case 'N':ichr=13;break;
    case 'O':ichr=15;break;
    case 'P':ichr=16;break;
    case 'Q':ichr=8 ;break;
    case 'R':ichr=17;break;
    case 'S':ichr=18;break;
    case 'T':ichr=19;break;
    case 'U':ichr=20;break;
    case 'V':ichr=22;break;
    case 'W':ichr=24;break;
    case 'X':ichr=14;break;
    case 'Y':ichr=23;break;
    case 'Z':ichr=6 ;break;
    }
    ichr += 36;
  } else if (aType==HERSHEY__SPECIAL) {
    switch(aC) {
    case 'A':ichr=61;break;
    case 'B':ichr=62;break;
    case 'C':ichr=63;break;
    case 'D':ichr=64;break;
    case 'E':ichr=65;break;
    case 'F':ichr=66;break;
    case 'G':ichr=67;break;
    case 'H':ichr=68;break;
    case 'I':ichr=69;break;
    case 'J':ichr=70;break;
    case 'K':ichr=71;break;
    case 'L':ichr=72;break;
    case 'M':ichr=73;break;
    case 'N':ichr=74;break;
    case 'O':ichr=75;break;
    case 'P':ichr=76;break;
    case 'Q':ichr=77;break;
    case 'R':ichr=78;break;
    case 'S':ichr=79;break;
    case 'T':ichr=80;break;
    case 'U':ichr=81;break;
    case 'V':ichr=82;break;
    case 'W':ichr=83;break;
    case 'X':ichr=84;break;
    case 'Y':ichr=85;break;
    case 'Z':ichr=86;break;
    case '0':ichr=87;break;
    case '1':ichr=88;break;
    case '2':ichr=89;break;
    case '3':ichr=90;break;
    case '4':ichr=91;break;
    case '5':ichr=92;break;
    case '6':ichr=93;break;
    case '7':ichr=94;break;
    case '8':ichr=95;break;
    case '9':ichr=96;break;
    }
  } else if (aType==HERSHEY__MATH) {
    switch(aC) {
    case '.':ichr= 97;break;
    case ',':ichr= 98;break;
    case '/':ichr= 99;break;
    case '(':ichr=100;break;
    case ')':ichr=101;break;
    case '-':ichr=102;break;
    case '+':ichr=103;break;
    case '=':ichr=104;break;
    case '*':ichr=105;break;
    }
  }
  
  if (aCase==HERSHEY__LOWER) ichr  += 105;

/* scale */
  size = aScale;
  if(size<0.001) size = 0.25F;
  iy0   = 9;
  dky   = 21;
  if (aType==HERSHEY__SPECIAL) Rescale(ichr,&iy0,&dky);
  scale = size/dky;

  iw    = Class.start[ichr-1] - 1;
  kx    = (Class.x[iw] % 64 ) - 32;
  ky    = (Class.y[iw] % 64 ) - 32;
  ibit  = 1;
  idif  = 1;
  ix0   = kx;
/* a character may be composed of many polylines */
  npoly  = 0;
  ipoint = 0;
  max_x  = 0;
  while(1) {
    count = 0;
    while(1) {
      ibit += 6;
      idif *= 64;
      if(ibit>=HERSHEY__NBITMX) {
        ibit  = 1;
        idif  = 1;
        iw    ++;
      }
      kx    = ((Class.x[iw]/idif) % 64 ) - 32;
      ky    = ((Class.y[iw]/idif) % 64 ) - 32;
      if(kx==31) break;
      aX[ipoint] = scale * (kx - ix0);
      aY[ipoint] = scale * (iy0 - ky);
      if (aX[ipoint] > max_x) max_x = aX[ipoint];
      ipoint ++;
      count  ++;
    }
    /* got one polyline */
    aMax_point[npoly] = count;
    npoly ++;
    if(ky==31) break;
  }
  if(aN!=NULL) *aN = npoly;
  aWidth = max_x;
}

void Rescale(int aIchr,int* aIy0,int* aDky){
  int iw,ibit,idif,kx,ky,kymin,kymax;

  iw    = Class.start[aIchr-1] - 1;
  kymin = 100;
  kymax = -100;
  ibit  = 1;
  idif  = 1;
  while(1) {
    while(1) {
      ibit += 6;
      idif *= 64;
      if(ibit>=HERSHEY__NBITMX)        {
        ibit  = 1;
        idif  = 1;
        iw    ++;
      }
      kx    = ((Class.x[iw]/idif) % 64 ) - 32;
      ky    = ((Class.y[iw]/idif) % 64 ) - 32;
      if(kx==31) break;
      if(ky>kymax) kymax = ky;
      if(ky<kymin) kymin = ky;
    }
    if(ky==31) break;
  }
  if(aIy0!=NULL) *aIy0 = kymax;
  if(aDky!=NULL) *aDky = 21;
  if(kymin<kymax)  {
    if(aDky!=NULL) *aDky = kymax-kymin;
  }
}
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoLineSet.h>

#ifdef SOTEXT_HERSHEY_CHILD_LIST

void SoTextHershey::updateChildren() {
  SbBool update = FALSE;

  if(fOld_string != string) {
    fOld_string = string;
    update = TRUE;
  }

  if(fOld_encoding != encoding.getValue()) {
    fOld_encoding = encoding.getValue();
    update = TRUE;
  }
  if(fOld_font != (Font)font.getValue()) {
    fOld_font = font.getValue();
    update = TRUE;
  } 
  if(fOld_horizontalJustification != horizontalJustification.getValue()) {
    fOld_horizontalJustification = horizontalJustification.getValue();
    update = TRUE;
  }
  if(fOld_verticalJustification != verticalJustification.getValue()) {
    fOld_verticalJustification = verticalJustification.getValue();
    update = TRUE;
  }
  if(fOld_height != height.getValue()) {
    fOld_height = height.getValue();
    update = TRUE;
  }

  if(update==FALSE) return;

  //printf("debug : SoTextHershey::updateChildren : %lu |%s|\n",
  //  this,string.getValues(0)[0].getString());

  fChildren->truncate(0);

  std::vector<SbVec3f> pts;
  generateSegments(pts);
  int32_t pointn = (int32_t)pts.size();
  if(pointn<=1) return;

  SbVec3f* points = new SbVec3f[pointn];
  int segmentn = pointn/2;
  int32_t* vertices = new int32_t[segmentn];

  int index;
  for(index=0;index<pointn;index++) points[index] = pts[index];
  for(index=0;index<segmentn;index++) vertices[index] = 2;

  SoCoordinate3* coordinate3 = new SoCoordinate3;
  coordinate3->point.setValues(0,pointn,points);
  fChildren->append(coordinate3);

  SoLineSet* lineSet = new SoLineSet;
  lineSet->numVertices.setValues(0,segmentn,vertices);
  fChildren->append(lineSet);

  delete [] vertices;
  delete [] points;

}
#endif

void SoTextHershey::generateAlternateRep(SoAction*) {
#ifdef SOTEXT_HERSHEY_CHILD_LIST
  updateChildren();
  alternateRep.setValue((SoSeparator*)(*fChildren)[0]);
#else
  std::vector<SbVec3f> pts;
  generateSegments(pts);
  int32_t pointn = (int32_t)pts.size();
  if(pointn<=1) return;

  SbVec3f* points = new SbVec3f[pointn];
  int segmentn = pointn/2;
  int32_t* vertices = new int32_t[segmentn];

  int index;
  for(index=0;index<pointn;index++) points[index] = pts[index];
  for(index=0;index<segmentn;index++) vertices[index] = 2;

  SoSeparator* separator = new SoSeparator;

  SoCoordinate3* coordinate3 = new SoCoordinate3;
  coordinate3->point.setValues(0,pointn,points);
  separator->addChild(coordinate3);

  SoLineSet* lineSet = new SoLineSet;
  lineSet->numVertices.setValues(0,segmentn,vertices);
  separator->addChild(lineSet);

  delete [] vertices;
  delete [] points;

  alternateRep.setValue(separator);
#endif
}

void SoTextHershey::clearAlternateRep() {alternateRep.setValue(NULL);}

void SoTextHershey::doAction(SoAction* aAction) {
  SO_ALTERNATEREP_DO_ACTION(aAction)
#ifdef SOTEXT_HERSHEY_CHILD_LIST
  updateChildren();
#endif
  SoShape::doAction(aAction);
}

void SoTextHershey::write(SoWriteAction* aAction){
  SoNode* altRep = alternateRep.getValue();
  if(altRep) {
    //NOTE : the below logic loose the fact
    //       that we had a SoTextHershey. 
    //       We may also introduce a coworking SoAltTextHershey
    //       that has no problematic fields (like the SoSFEnum), 
    //       and arrange that the SoAlternateRepAction swaps 
    //       from SoPolyhedron to SoAltTextHershey.
    altRep->write(aAction);
  } else {
    SoShape::write(aAction);
  }
}

void SoTextHershey::callback(SoCallbackAction* aAction){
  SoNode* altRep = alternateRep.getValue();
  if(altRep) {
    //NOTE :
    // To handle SoToVRML2Action that internally uses a SoCallbackAction.
    // With Coin-2.5.0, the SoToVRML2Action does not handle the 
    // generatePrimitives beginShape(LINES). Then we can't use
    // the default SoShape::callback behaviour to produce VRML.
    // (See the SoToVRML2Action cstor that set only triangle callback
    //  on the internal SoCallbackAction)
    //FIXME : we should find a way to check that the action 
    //  is a SoToVRML2Action.
    altRep->callback(aAction);
  } else {
    SoShape::callback(aAction);
  }
}
