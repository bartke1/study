#include <Python.h>
#include "chi2.hpp"
#include "npywrap.hpp"
#include "parse_tuple.hpp"

extern "C"
{

static char module_docstring[] =
    "This module provides an interface for calculating chi-squared using C.";
static char chi2_docstring[] =
    "Calculate the chi-squared of some data given a model";

static PyObject* chi2_chi2(PyObject* self, PyObject* args);

static PyMethodDef module_methods[] = {
    { "chi2", chi2_chi2, METH_VARARGS, chi2_docstring },
    { NULL, NULL, 0, NULL }
};

PyMODINIT_FUNC init_chi2(void)
{
    PyObject* m = Py_InitModule3("_chi2", module_methods, module_docstring);
    if (m == NULL)
    {
        return;
    }

    /* Load 'numpy' functionality */
    import_array();
}

static PyObject* chi2_chi2(PyObject* self, PyObject* args)
{
    double m, b;
    npyarray<double> x, y, yerr;
    if (!parse_tuple(args, m, b, x, y, yerr))
    {
        return NULL;
    }

    double value = chi2(m, b, x.data(), y.data(), yerr.data(), x.size());

    if (value < 0.0)
    {
        PyErr_SetString(PyExc_RuntimeError, "Chi-squared returned an impossible value.");
        return NULL;
    }

    return Py_BuildValue("d", value);
}

}
