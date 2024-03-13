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
    int k, n, d, iter;
    PyObject *pyVectorList, *pyCentroidList;
    Vector *vectorList, *centroidList;

    /* type test */
    if (!PyArg_ParseTuple(args, "O!O!iiii", &PyList_Type, &pyVectorList, &PyList_Type, &pyCentroidList, &k, &n, &d, &iter)) {
        return NULL;
    }

    /* Check if the lengths of the Python lists match N and K */
    if (PyList_Size(pyVectorList) != n || PyList_Size(pyCentroidList) != k) {
        PyErr_SetString(PyExc_ValueError, "Input list sizes do not match the specified dimensions");
        return NULL;
    }

    // Parse Python lists into vector arrays
    vectorList = parsePyListToVectorArray(pyVectorList, n, d);
    centroidList = parsePyListToVectorArray(pyCentroidList, k, d);

    Kmeans(vectorList, centroidList, k, n, d, iter);

    /* Clean up allocated memory */
    for (int i = 0; i < n; i++) {
        free(vectorList[i].components);
    }
    free(vectorList);

    return Py_BuildValue("(O)", centroidList);
}

/* Method definitions */
static PyMethodDef myModule_methods[] = {
    {"fit",
     fit_c,
     METH_VARARGS,
     PyDoc_STR("Receives two lists of vectors and dimensions k, n, d, iter.")},
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
    PyObject *m;
    m = PyModule_Create(&myModule_definition);
    if (!m){
        return NULL;
    }
    return m;
}
