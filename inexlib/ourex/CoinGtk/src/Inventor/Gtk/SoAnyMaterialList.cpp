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

#if SOGTK_DEBUG
static const char rcsid[] =
  "";
#endif // SOGTK_DEBUG

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifdef HAVE_DIRENT_H
#include <dirent.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <stdlib.h>

#include <Inventor/SbPList.h>
#include <Inventor/errors/SoDebugError.h>

#include <sogtkdefs.h>
#include <Inventor/Gtk/SoAnyMaterialList.h>
#include <Inventor/Gtk/SoAny.h>

struct SoGtkMaterialListCallbackInfo {
  SoGtkMaterialListCB * callback;
  void * closure;
};

/*!
  \class SoAnyMaterialList Inventor/Gtk/SoAnyMaterialList.h
  \brief The SoAnyMaterialList class is the common code for the MaterialList
  component classes.
*/

// *************************************************************************

/*!
*/

SoAnyMaterialList::SoAnyMaterialList(
  const char * const dir)
{
  this->callbacks = NULL;
  this->dirpath = NULL;
  this->directory = NULL;
  if (dir != NULL)
    this->dirpath = strcpy(new char [strlen(dir)+1], dir);
}

/*!
*/

SoAnyMaterialList::~SoAnyMaterialList(
  void)
{
  if (this->callbacks != NULL) {
    const int num = this->callbacks->getLength();
    for (int i = 0; i < num; i++)
      delete (SoGtkMaterialListCallbackInfo *) (*this->callbacks)[i];
    delete this->callbacks;
  }
  if (this->dirpath)
    delete [] this->dirpath;
  if (this->directory != NULL)
    this->freeMaterialDirectory();
}

// *************************************************************************

/*!
*/

void
SoAnyMaterialList::addCallback(
  SoGtkMaterialListCB * const callback,
  void * const closure)
{
  if (this->callbacks == NULL)
    this->callbacks = new SbPList;
  SoGtkMaterialListCallbackInfo * info =
    new SoGtkMaterialListCallbackInfo;
  info->callback = callback;
  info->closure = closure;
  this->callbacks->append(info);
} // addCallback()

/*!
*/

void
SoAnyMaterialList::removeCallback(
  SoGtkMaterialListCB * const callback,
  void * const closure)
{
  if (! this->callbacks) {
#if SOGTK_DEBUG
    SoDebugError::postInfo("SoAnyMaterialList::removeCallback",
      "component has zero callbacks set.");
#endif // SOGTK_DEBUG
    return;
  }

  const int numcallbacks = this->callbacks->getLength();
  for (int i = 0; i < numcallbacks; i++) {
    SoGtkMaterialListCallbackInfo * info =
      (SoGtkMaterialListCallbackInfo *) (*this->callbacks)[i];
    if (info->callback == callback && info->closure == closure) {
      this->callbacks->remove(i);
      delete info;
      return;
    }
  }

#if SOGTK_DEBUG
  SoDebugError::postInfo("SoAnyMaterialList::removeCallback",
    "callback was not set for component.");
#endif // SOGTK_DEBUG
} // removeCallback()

/*!
*/

void
SoAnyMaterialList::invokeCallbacks(
  SoMaterial * material)
{
  if (this->callbacks) {
    const int numCallbacks = this->callbacks->getLength();
    for (int i = 0; i < numCallbacks; i++) {
      SoGtkMaterialListCallbackInfo * info =
        (SoGtkMaterialListCallbackInfo *) (*this->callbacks)[i];
      info->callback(info->closure, (SoMaterial *) material);
    }
  }
} // invokeCallbacks()

// *************************************************************************

/*!
  \internal

  This method frees up the memory used by the material-index data strucure.
*/

void
SoAnyMaterialList::freeMaterialDirectory(
  void)
{
  if (this->directory == NULL)
    return;
  int i, j;
  if ((this->directory->flags & SOGTK_BUILTIN_MATERIALS) == 0) {
    // all data is allocated
    for (i = 0; i < this->directory->numGroups; i++) {
      for (j = 0; j < this->directory->groups[i]->numMaterials; j++) {
        delete [] (char *) this->directory->groups[i]->materials[j]->data;
        delete [] (char *) this->directory->groups[i]->materials[j]->name;
        delete this->directory->groups[i]->materials[j];
      }
      delete [] (char *) this->directory->groups[i]->name;
      delete [] this->directory->groups[i]->materials;
      delete this->directory->groups[i];
    }
    delete [] this->directory->groups;
  } else {
    // lots of data is static and should therefore not be freed
    for (i = 0; i < this->directory->numGroups; i++) {
      for (j = 0; j < this->directory->groups[i]->numMaterials; j++)
        delete this->directory->groups[i]->materials[j];
      delete [] this->directory->groups[i]->materials;
      delete this->directory->groups[i];
    }
    delete [] this->directory->groups;
  }
  delete this->directory;
  this->directory = NULL;
} // freeMaterialDirectory()

// *************************************************************************

const char *
SoAnyMaterialList::getMaterialDirectoryPath(
  void) const
{
  return this->dirpath;
} // getMaterialDirectoryPath()

// *************************************************************************

/*!
  \internal
*/

static
int
containsFiles(
  const char * const path)
{
#ifdef HAVE_DIRENT_H
  DIR * directory = opendir(path);
  struct dirent * entry;
  struct stat entrystats;
  if (directory != NULL) {
    const int pathnamelen = strlen(path);
    int foundfile = 0;
    while (! foundfile && (entry = readdir(directory)) != NULL) {
      if (entry->d_name[0] == '.') continue;
      char * entrypath = new char [pathnamelen + strlen(entry->d_name) + 2];
      sprintf(entrypath, "%s/%s", path, entry->d_name);
      if (stat(entrypath, &entrystats) == 0 &&
           S_ISREG(entrystats.st_mode)) {
        delete [] entrypath;
        closedir(directory);
        return 1;
      }
      delete [] entrypath;
    }
    closedir(directory);
  }
#endif
  return 0;
} // containsFiles()

/*!
  \internal
*/

char **
SoAnyMaterialList::getNonemptySubdirs(// static, private
  const char * const path)
{
#ifdef HAVE_DIRENT_H
  DIR * dir = opendir(path);
  if (! dir) return NULL;

  SbPList subdirs;
  DIR * subdir;
  const int pathlen = strlen(path) + 2;
  struct dirent * entry, * subentry;
  struct stat statbuf, substatbuf;
  while ((entry = readdir(dir)) != NULL) {
    if (entry->d_name[0] == '.') continue;
    int pathnamelen = pathlen + strlen(entry->d_name);
    char * pathname = new char [ pathnamelen ];
    sprintf(pathname, "%s/%s", path, entry->d_name);
    if ((stat(pathname, &statbuf) == 0) && S_ISDIR(statbuf.st_mode)) {
      if ((subdir = opendir(pathname)) != NULL) {
        int foundfile = 0;
        while (! foundfile && (subentry = readdir(subdir)) != NULL) {
          if (subentry->d_name[0] == '.') continue;
          char * entrypathname =
            new char [pathnamelen + strlen(subentry->d_name) + 1];
          sprintf(entrypathname, "%s/%s", pathname, subentry->d_name);
          if (stat(entrypathname, &substatbuf) == 0 &&
               S_ISREG(substatbuf.st_mode)) {
            foundfile = 1;
          }
          delete [] entrypathname;
        }
        if (foundfile)
          subdirs.append(strcpy(new char [strlen(entry->d_name)+1],
                                  entry->d_name));
        closedir(subdir);
      }
    }
    delete [] pathname;
  }
  closedir(dir);

  const int num = subdirs.getLength();
  char ** subdirarray = new char * [ num + 1 ];
  for (int i = 0; i < num; i++)
    subdirarray[i] = (char *) subdirs[i];
  subdirarray[num] = NULL;
  return subdirarray;
#endif
  return NULL;
} // numNonemptySubdirs()


/*!
  \internal
*/

char **
SoAnyMaterialList::getRegularFiles(// static, private
  const char * const path)
{
#ifdef HAVE_DIRENT_H
  DIR * dir = opendir(path);
  if (! dir) return NULL;

  const int pathlen = strlen(path);
  SbPList files;
  struct dirent * entry;
  while ((entry = readdir(dir)) != NULL) {
    if (entry->d_name[0] == '.') continue;
    char * pathname = new char [pathlen + 2 + strlen(entry->d_name)];
    sprintf(pathname, "%s/%s", path, entry->d_name);
    struct stat statbuf;
    if ((stat(pathname, &statbuf) == 0) && S_ISREG(statbuf.st_mode))
      files.append(strcpy(new char [strlen(entry->d_name)+1],
                            entry->d_name));
    delete [] pathname;
  }
  closedir(dir);

  const int num = files.getLength();
  char ** filearray = new char * [ num + 1 ];
  for (int i = 0; i < num; i++)
    filearray[i] = (char *) files[i];
  filearray[num] = NULL;
  return filearray;
#endif
  return NULL;
} // getRegularFiles()

/*!
  \internal

  Used for qsort().
*/

int
SoAnyMaterialList::qsort_comparator(// static, private
  const void * itemA,
  const void * itemB)
{
  SOGTK_STUB();
  return 0;
} // qsort_comparator()

/*!
*/

SoGtkMaterialDirectory *
SoAnyMaterialList::getMaterialDirectory(
  void)
{
  if (this->directory != NULL) // already created
    return this->directory;

  // get the path to search for materials in
  SbString path;
  if (this->dirpath) { path = this->dirpath; }

  const char * envvars[] = {
    // FIXME: where the hell does the "WALLET" env-var come from?
    // 20020109 mortene.
    "SO_MATERIAL_DIR", "WALLET", "COIN_HOME"
  };

  for (unsigned int i=0; i < (sizeof(envvars) / sizeof(char *)); i++) {
    if (path.getLength() == 0) {
      const char * p = SoAny::si()->getenv(envvars[i]);
      if (p) { path = p; }
    }
  }

  if (path.getLength() > 0) { path += "/materials"; }
  else {
    const char * p = SoAny::si()->getenv("OIVHOME");
    if (p) {
      path = p;
      // FIXME: use DIRSEP string? ????-??-?? larsa.
      path += "/data/materials";
    }
  }

  this->directory = new SoGtkMaterialDirectory;
  this->directory->flags = 0;
  this->directory->current = 0;

  SbBool founddiskmaterials = FALSE;

  if (path.getLength() > 0) {
#if SOGTK_DEBUG && 0 // debug
    SoDebugError::postInfo("SoAnyMaterialList::getMaterialDirectory",
                           "scanning '%s'", path.getString());
#endif // debug
    char ** subdirs = SoAnyMaterialList::getNonemptySubdirs(path.getString());
    if (subdirs) {
      founddiskmaterials = TRUE;
      int numsubdirs;
      for (numsubdirs = 0; subdirs[numsubdirs] != NULL; numsubdirs++) { }
      this->directory->numGroups = numsubdirs;
      this->directory->groups = new SoGtkMaterialGroup * [ numsubdirs ];
      for (int subdir = 0; subdir < numsubdirs; subdir++) {
        this->directory->groups[subdir] = new SoGtkMaterialGroup;
        this->directory->groups[subdir]->name = subdirs[subdir];
        char * subdirname =
          new char [strlen(path.getString()) + strlen(subdirs[subdir]) + 2];
        sprintf(subdirname, "%s/%s", path.getString(), subdirs[subdir]);
        char ** files = SoAnyMaterialList::getRegularFiles(subdirname);
        assert(files != NULL);
        int numfiles;
        for (numfiles = 0; files[numfiles] != NULL; numfiles++) { }
        this->directory->groups[subdir]->numMaterials = numfiles;
        this->directory->groups[subdir]->materials =
          new SoGtkMaterial * [ numfiles ];
        for (int file = 0; file < numfiles; file++) {
          this->directory->groups[subdir]->materials[file] =
            new SoGtkMaterial;
          this->directory->groups[subdir]->materials[file]->name =
            files[file];
          char * buf = new char [strlen(subdirname) + strlen(files[file]) + 2];
          sprintf(buf, "%s/%s", subdirname, files[file]);
          this->directory->groups[subdir]->materials[file]->data = buf;
        }
        delete [] files; // actual strings are transfered, don't delete them!
        delete [] subdirname;
      }
      delete [] subdirs; // actual strings are transfered, don't delete them!
    }
  }

  // fallback on builtins
  if (!founddiskmaterials) { this->setupBuiltinMaterials(this->directory); }
  return this->directory;
} // getMaterialDirectory()

// *************************************************************************

#if SOGTK_DEBUG
static const char * getSoAnyMaterialListRCSId(void) { return rcsid; }
#endif // SOGTK_DEBUG

