#define PY_SSIZE_T_CLEAN
#include <Python.h>

static PyObject* fit(PyObject* self, PyObject* args) {
    PyObject* dataframe;
    PyObject* values;
    PyObject* iterator;
    PyObject* item;
    PyObject* row;
    PyObject* value;
    Py_ssize_t num_cols;
    double val;
    Py_ssize_t j;

    /* Parse the arguments from Python */
    if (!PyArg_ParseTuple(args, "O", &dataframe)) {
        return NULL;
    }

    /* Check if it's a DataFrame */
    if (!PyObject_HasAttrString(dataframe, "values")) {
        PyErr_SetString(PyExc_TypeError, "Expected a DataFrame");
        return NULL;
    }

    /* Get the 'values' attribute of the DataFrame */
    values = PyObject_GetAttrString(dataframe, "values");

    /* Check if it's iterable */
    if (!PyIter_Check(values)) {
        PyErr_SetString(PyExc_TypeError, "Expected an iterable object");
        Py_DECREF(values);
        return NULL;
    }

    iterator = PyObject_GetIter(values);

    /* Iterate over the DataFrame */
    while ((item = PyIter_Next(iterator))) {
        row = PySequence_Fast(item, "expected a sequence");
        Py_DECREF(item);
        if (!row) {
            PyErr_SetString(PyExc_TypeError, "Expected a sequence of sequences");
            Py_DECREF(iterator);
            Py_DECREF(values);
            return NULL;
        }
        num_cols = PySequence_Fast_GET_SIZE(row);
        for (j = 0; j < num_cols; j++) {
            value = PySequence_Fast_GET_ITEM(row, j);
            val = PyFloat_AsDouble(value);
            if (val == -1.0 && PyErr_Occurred()) {
                PyErr_SetString(PyExc_TypeError, "Expected a DataFrame of floats");
                Py_DECREF(row);
                Py_DECREF(iterator);
                Py_DECREF(values);
                return NULL;
            }
            printf("Value at (%zd, %zd): %f\n", j, j, val);
        }
        Py_DECREF(row);
    }

    Py_DECREF(iterator);
    Py_DECREF(values);

    /* Return None */
    Py_RETURN_NONE;
}

/* Method definitions */
static PyMethodDef myModule_methods[] = {
    {"my_c_function", my_c_function, METH_VARARGS, "Receives a DataFrame."},
    {NULL, NULL, 0, NULL}   /* Sentinel */
};

/* Module initialization */
static struct PyModuleDef myModule_definition = {
    PyModuleDef_HEAD_INIT,
    "myModule",
    "A Python module that receives a DataFrame.",
    -1,
    myModule_methods
};

PyMODINIT_FUNC PyInit_myModule(void) {
    return PyModule_Create(&myModule_definition);
}
