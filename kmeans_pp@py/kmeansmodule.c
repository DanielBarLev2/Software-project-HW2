#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include "vector.h" 
#include "kmeans.c"


Vector* parsePyListToVectorArray(PyObject* pyList, int size, int dimension) {
    Vector *vectorList;
    int i, j;
    double* values;
    vectorList = (Vector*)malloc(size * sizeof(Vector));

    if (!vectorList) {
        PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory for vector array");
        return NULL;
    }

    for (i = 0; i < size; i++) {
        PyObject* py_item = PyList_GetItem(pyList, i);
        if (!PyList_Check(py_item) || PyList_Size(py_item) != dimension) {
            PyErr_SetString(PyExc_ValueError, "Invalid vector format in input list");
            free(vectorList);
            return NULL;
        }

        values = (double*)malloc(dimension * sizeof(double));

        if (!values) {
            PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory for vector components");
            free(vectorList);
            return NULL;
        }

        for (j = 0; j < dimension; j++) {
            values[j] = PyFloat_AsDouble(PyList_GetItem(py_item, j));
        }

        vectorList[i] = createVector(dimension, values);
        free(values);
    }

    return vectorList;
}

static PyObject* fit_c(PyObject* self, PyObject* args) {
    float eps;
    int k, n, d, iter, i, j;
    PyObject *pyVectorList, *pyCentroidList, *pyCentroidListObj, *pyVectorObj, *pyListObj;
    Vector *vectorList, *centroidList;

    /* type test */
    if (!PyArg_ParseTuple(args, "O!O!iiii", &PyList_Type, &pyVectorList, &PyList_Type, &pyCentroidList, &k, &n, &d, &iter, &eps)) {
        return NULL;
    }

    /* Check if the lengths of the Python lists match N and K */
    if (PyList_Size(pyVectorList) != n || PyList_Size(pyCentroidList) != k) {
        PyErr_SetString(PyExc_ValueError, "Input list sizes do not match the specified dimensions");
        return NULL;
    }

    /* Parse Python lists into vector arrays */
    vectorList = parsePyListToVectorArray(pyVectorList, n, d);
    centroidList = parsePyListToVectorArray(pyCentroidList, k, d);

    centroidList = Kmeans(vectorList, centroidList, k, n, d, iter, eps);

    pyCentroidListObj = PyList_New(k);
    for (i = 0; i < k; i++) {
        pyVectorObj = PyList_New(d);
        for (j = 0; j < d; j++) {
            PyList_SetItem(pyVectorObj, j, PyFloat_FromDouble(centroidList[i].components[j]));
        }
        
        pyListObj = PyList_New(1);
        PyList_SetItem(pyListObj, 0, pyVectorObj);
        PyList_SetItem(pyCentroidListObj, i, pyListObj);
    }

    return pyCentroidListObj;
}

/* Method definitions */
static PyMethodDef myModule_methods[] = {
    {"fit",
      (PyCFunction) fit_c,
      METH_VARARGS,
    PyDoc_STR("Receives two lists of vectors and dimensions k, n, d and iter.")},
    {NULL, NULL, 0, NULL}
};

/* Module initialization */
static struct PyModuleDef myModule_definition = {
    PyModuleDef_HEAD_INIT,
    "mykmeanssp",
    "A Python module that receives two lists of vectors and dimensions k, n, d, iter.",
    -1,
    myModule_methods
};

PyMODINIT_FUNC PyInit_mykmeanssp(void) {
    return PyModule_Create(&myModule_definition);
    PyObject *m;
    m = PyModule_Create(&myModule_definition);
    if (!m){
        return NULL;
    }
    return m;
}
