//
//WARNING : do not put the below in the SWIG.i file
//  in some %{... %}. Else it is going to be after Python.h
//  and then compilation problem may happen on some platform.
//  (For example clash with Coin-2.x code on a slc3_gcc323).
//

#include <CoinPython/Inventor.h>

#include <HEPVis/SbAll.h>
#include <HEPVis/Utils.h>
#include <HEPVis/Detector.h>
#include <HEPVis/Geometry.h>
#include <HEPVis/Plotter.h>
#include <HEPVis/GUI.h>

#include <HEPVis/misc/SoTools.h>
#include <HEPVis/misc/SoStyleCache.h>

extern "C" {
  void HEPVisUtils_initClasses();
  void HEPVisGeometry_initClasses();
  void HEPVisDetector_initClasses();
  void HEPVisPlotter_initClasses();
  void HEPVisGUI_initClasses();

  void SoPlotter_initClasses();
}

#include <HEPVis/inlib_plot>

#if defined(__linux)
// Clash between os_defines.h (coming from <string>) and pyconfig.h
#if defined(_POSIX_C_SOURCE)
#undef _POSIX_C_SOURCE
#endif
#if defined(_XOPEN_SOURCE)
#undef _XOPEN_SOURCE
#endif
#endif

#include <Python.h>

#ifdef WIN32
#include <windows.h>
#undef max
#undef ERROR
#undef DELETE
#endif

//#undef ANY

//If wrapping full Inventor too.
typedef SoGLLazyElement::GLState GLState;

#if PY_VERSION_HEX >= 0x03000000
//FIXME : what is PyFile_Check and PyFile_AsFile for Python3 ?
#define PyFile_Check(op) false
#define PyFile_AsFile(op) 0
#endif

static void
convert_SbVec3d_array(PyObject *input, double temp[3])
{
  if (PySequence_Check(input) && (PySequence_Size(input) == 3) &&
      PyNumber_Check(PySequence_GetItem(input, 0)) && 
      PyNumber_Check(PySequence_GetItem(input, 1)) && 
      PyNumber_Check(PySequence_GetItem(input, 2))) {
    temp[0] = PyFloat_AsDouble(PySequence_GetItem(input, 0));
    temp[1] = PyFloat_AsDouble(PySequence_GetItem(input, 1));
    temp[2] = PyFloat_AsDouble(PySequence_GetItem(input, 2));
  } else {
    PyErr_SetString(PyExc_TypeError, "expected a sequence with 3 doubles");
    PyErr_Print();
  } 
}

#include "HEPVis_SWIG_Python.ic"
