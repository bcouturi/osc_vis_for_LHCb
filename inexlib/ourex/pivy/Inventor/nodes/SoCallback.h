#ifndef COIN_SOCALLBACK_H
#define COIN_SOCALLBACK_H

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

#ifdef __PIVY__
%include Inventor/nodes/SoCallback.i
#endif

#include <Inventor/nodes/SoSubNode.h>

typedef void SoCallbackCB(void * userdata, SoAction * action);

class COIN_DLL_API SoCallback : public SoNode {
    typedef SoNode inherited;

  SO_NODE_HEADER(SoCallback);

public:
  static void initClass(void);
  SoCallback(void);

  void setCallback(SoCallbackCB * function, void * userdata = NULL);

  virtual void doAction(SoAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void getMatrix(SoGetMatrixAction * action);
  virtual void handleEvent(SoHandleEventAction * action);
  virtual void pick(SoPickAction * action);
  virtual void search(SoSearchAction * action);
  virtual void write(SoWriteAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);

protected:
  virtual ~SoCallback();

  virtual void copyContents(const SoFieldContainer * from,
                            SbBool copyconnections);

private:
  SoCallbackCB * cbfunc;
  void * cbdata;
};

#endif // !COIN_SOCALLBACK_H
