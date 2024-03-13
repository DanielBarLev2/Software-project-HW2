# define PY_SSIZE_T_CLEAN
# include <Python.h>

static PyObject* geo_sum(PyObject *self, PyObject *args)
{
    double z;
    int n;
    /* This parses the Python arguments into a double (d)  variable named z and int (i) variable named n*/
    if(!PyArg_ParseTuple(args, "di", &z, &n)) {
        return NULL; /* In the CPython API, a NULL value is never valid for a
                        PyObject* so it is used to signal that an error has occurred. */
    }

/* This builds the answer ("d" = Convert a C double to a Python floating point number) back into a python object */
    return Py_BuildValue("d", geo_c(z, n)); /*  Py_BuildValue(...) returns a PyObject*  */
}

typedef struct _object {
    Py_ssize_t ob_refcnt;
    struct _typeobject *ob_type;
} PyObject;



static PyMethodDef geoMethods[] = {
    {"geo_sum",                   /* the Python method name that will be used */
      (PyCFunction) geo_sum, /* the C-function that implements the Python function and returns static PyObject*  */
      METH_VARARGS,           /* flags indicating parameters
accepted for this function */
      PyDoc_STR("A geometric series up to n. sum_up_to_n(z^n)")}, /*  The docstring for the function */
    {NULL, NULL, 0, NULL}     /* The last entry must be all NULL as shown to act as a
                                 sentinel. Python looks for this entry to know that all
                                 of the functions for the module have been defined. */
};

static PyMethodDef geomodule[] = {
    {"geo_sum",                   /* the Python method name that will be used */
      (PyCFunction) geo_sum, /* the C-function that implements the Python function and returns static PyObject*  */
      METH_VARARGS,           /* flags indicating parameters
accepted for this function */
      PyDoc_STR("A geometric series up to n. sum_up_to_n(z^n)")}, /*  The docstring for the function */
    {NULL, NULL, 0, NULL}     /* The last entry must be all NULL as shown to act as a
                                 sentinel. Python looks for this entry to know that all
                                 of the functions for the module have been defined. */
};

PyMODINIT_FUNC PyInit_geo_capi(void)
{
    PyObject *m;
    m = PyModule_Create(&geomodule);
    if (!m) {
        return NULL;
    }
    return m;
}

