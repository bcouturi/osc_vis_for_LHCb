%{
static void
convert_SbDPMat_array(PyObject *input, SbDPMat temp)
{
  if (PySequence_Check(input) && (PySequence_Size(input) == 4) &&
      (PySequence_Size(PySequence_GetItem(input, 0)) == 4) &&
      (PySequence_Size(PySequence_GetItem(input, 1)) == 4) &&
      (PySequence_Size(PySequence_GetItem(input, 2)) == 4) &&
      (PySequence_Size(PySequence_GetItem(input, 3)) == 4)) {
    int i,j;
    for (i=0; i < 4; i++) {
      for (j=0; j < 4; j++) {
        PyObject *oij = PySequence_GetItem(PySequence_GetItem(input, i), j);
        if (!PyNumber_Check(oij)) {
          PyErr_SetString(PyExc_TypeError,
                          "sequence must contain 4 sequences where every sequence contains 4 floats");
          PyErr_Print();
          return;
        }
        temp[i][j] = PyFloat_AsDouble(oij);
      }
    }
  } else {
    PyErr_SetString(PyExc_TypeError,
                    "sequence must contain 4 sequences where every sequence contains 4 floats");
    PyErr_Print();
  }
}
%}

%typemap(in) SbDPMat * (SbDPMat temp) {
  convert_SbDPMat_array($input, temp);
  $1 = &temp;
}

%typemap(typecheck) SbDPMat * {
  $1 = PySequence_Check($input) ? 1 : 0;
}

%typemap(out) SbDPMat & {
  int i,j;
  $result = PyTuple_New(4);
  
  for (i=0; i<4; i++) {
    PyObject *oi = PyList_New(4);
    for (j=0; j<4; j++) {
      PyObject *oj = PyFloat_FromDouble((double)(*$1)[i][j]);
      PyList_SetItem(oi, j, oj);
    }
    PyTuple_SetItem($result, i, oi);    
  }
}

/* add operator overloading methods instead of the global functions */
%extend SbDPMatrix {
  SbDPMatrix __mul__(const SbDPMatrix & u) { return *self * u; }
  SbVec3d __mul__(const SbVec3d & u) { SbVec3d res; self->multMatrixVec(u, res); return res; }
  SbVec3d __rmul__(const SbVec3d & u) { SbVec3d res; self->multVecMatrix(u, res); return res; }
  int __eq__(const SbDPMatrix & u) { return *self == u; }
  int __ne__(const SbDPMatrix & u) { return *self != u; }
}

%ignore SbDPMatrix::SbDPMatrix(const SbDPMat & matrix);
