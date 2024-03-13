#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include "vector.h" 

// Function to parse a Python list into a vector array
Vector* parsePyListToVectorArray(PyObject* py_list, int size, int dimension) {
    Vector* vec_array = (Vector*)malloc(size * sizeof(Vector));
    if (!vec_array) {
        PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory for vector array");
        return NULL;
    }

    for (int i = 0; i < size; i++) {
        PyObject* py_item = PyList_GetItem(py_list, i);
        if (!PyList_Check(py_item) || PyList_Size(py_item) != dimension) {
            PyErr_SetString(PyExc_ValueError, "Invalid vector format in input list");
            free(vec_array);
            return NULL;
        }

        double* values = (double*)malloc(dimension * sizeof(double));
        if (!values) {
            PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory for vector components");
            free(vec_array);
            return NULL;
        }

        for (int j = 0; j < dimension; j++) {
            values[j] = PyFloat_AsDouble(PyList_GetItem(py_item, j));
        }

        vec_array[i] = createVector(dimension, values);
        free(values);
    }

    return vec_array;
}

static PyObject* my_c_function(PyObject* self, PyObject* args) {
    int N, K, D;
    PyObject *py_array1, *py_array2;

    /* Parse the arguments from Python */
    if (!PyArg_ParseTuple(args, "O!O!iii", &PyList_Type, &py_array1, &PyList_Type, &py_array2, &N, &K, &D)) {
        return NULL;
    }

    /* Check if the lengths of the Python lists match N and K */
    if (PyList_Size(py_array1) != N || PyList_Size(py_array2) != K) {
        PyErr_SetString(PyExc_ValueError, "Input list sizes do not match the specified dimensions");
        return NULL;
    }

    // Parse Python lists into vector arrays
    Vector* array1 = parsePyListToVectorArray(py_array1, N, D);
    if (!array1) {
        return NULL;
    }

    Vector* array2 = parsePyListToVectorArray(py_array2, K, D);
    if (!array2) {
        free(array1);
        return NULL;
    }

    /* Your further processing with the arrays goes here... */

    /* Clean up allocated memory */
    for (int i = 0; i < N; i++) {
        free(array1[i].components);
    }
    free(array1);
    for (int i = 0; i < K; i++) {
        free(array2[i].components);
    }
    free(array2);

    /* Return the arrays */
    return Py_BuildValue("(OO)", array1, array2);
}

/* Method definitions */
static PyMethodDef myModule_methods[] = {
    {"my_c_function", my_c_function, METH_VARARGS, "Receives two lists of vectors and dimensions N, K, D."},
    {NULL, NULL, 0, NULL}   /* Sentinel */
};

/* Module initialization */
static struct PyModuleDef myModule_definition = {
    PyModuleDef_HEAD_INIT,
    "myModule",
    "A Python module that receives two lists of vectors and dimensions N, K, D.",
    -1,
    myModule_methods
};

PyMODINIT_FUNC PyInit_myModule(void) {
    return PyModule_Create(&myModule_definition);
}
