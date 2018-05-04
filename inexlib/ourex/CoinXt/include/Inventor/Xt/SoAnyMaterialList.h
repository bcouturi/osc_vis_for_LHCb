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

#ifndef SOANY_MATERIALLIST_H
#define SOANY_MATERIALLIST_H

#include <Inventor/Xt/SoXtMaterialList.h>

class SbPList;
class SoMaterial;

// *************************************************************************

#define SOXT_BUILTIN_MATERIALS 0x0001

struct SoXtMaterial {
  const char * name;
  const char * data;
};

struct SoXtMaterialGroup {
  const char * name;
  short numMaterials;
  SoXtMaterial ** materials;
  Widget menuitem;
};

struct SoXtMaterialDirectory {
  short flags;
  short numGroups;
  SoXtMaterialGroup ** groups;
  short current;
};

// *************************************************************************

class SOXT_DLL_API SoAnyMaterialList {
public:
  SoAnyMaterialList(const char * const dir);
  ~SoAnyMaterialList(void);

  void addCallback(
    SoXtMaterialListCB * const callback, void * const closure);
  void removeCallback(
    SoXtMaterialListCB * const callback, void * const closure);
  void invokeCallbacks(SoMaterial * material);

  const char * getMaterialDirectoryPath(void) const;
  SoXtMaterialDirectory * getMaterialDirectory(void);

private:
  void setupBuiltinMaterials(SoXtMaterialDirectory * const index) const;
  void freeMaterialDirectory(void);

  static char ** getNonemptySubdirs(const char * const path);
  static char ** getRegularFiles(const char * const path);

  static int qsort_comparator(const void * itemA, const void * itemB);

private:
  SbPList * callbacks;
  char * dirpath;
  SoXtMaterialDirectory * directory;

}; // class SoAnyMaterialList

// *************************************************************************

#endif // ! SOANY_MATERIALLIST_H
