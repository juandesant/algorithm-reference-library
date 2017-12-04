#include <Python.h>
#include <stdio.h>

#include "arlwrap.h"


/* Simple exit-on-error */
void pycheck(PyObject *obj)
{
	if (!obj) {
		PyErr_Print();
		exit(1);
	}
}

/* In: module name, function name
 * Out: function address */
void *get_ffi_fn_addr(const char* module, const char* fn_name)
{
	PyObject *mod, *fn, *fn_addr;

	pycheck(mod = PyImport_ImportModule(module));
	pycheck(fn = PyObject_GetAttrString(mod, fn_name));
	pycheck(fn_addr = PyObject_GetAttrString(fn, "address"));

	return (void*)PyNumber_AsSsize_t(fn_addr, NULL);
}

/* DO NOT USE - we do not want PyObjects */
/* Leaving for reference only */
PyObject *get_plain_fn_addr(const char* module, const char* fn_name)
{
	PyObject *mod, *fn;

	pycheck(mod = PyImport_ImportModule(module));
	pycheck(fn = PyObject_GetAttrString(mod, fn_name));

	return fn;
}


int main(int argc, char **argv)
{
	Py_Initialize();

	/* Old demo code using above routines */
	/*
	int i, count;
	
	int (*pfun)(char *) = get_ffi_fn_addr("ffi_pyroutines", "ffi_phfp");
  int len = pfun("C-based interface user");
  printf("Name length was: %d\n", len);

  int (*cfun)(void) = get_ffi_fn_addr("ffi_pyroutines", "ffi_gcfp");
  for(i = 0; i < 10; i++) {
    count = cfun();
    printf("\t%d\n", count);
  }

  PyObject *arl_fun = get_plain_fn_addr("arl.util.testing_support", "create_named_configuration");
  PyObject *config = NULL;
  PyObject *arg_tup = PyTuple_New(1);

  printf("Set tuple result: %d\n", PyTuple_SetItem(arg_tup, 0, PyUnicode_DecodeFSDefault("LOWBD2-CORE")));
  pycheck(config = PyObject_CallObject(arl_fun, arg_tup));
  printf("config pointer: %p\n", config);
	*/

	ARLVis *vin = malloc(104 * sizeof(char));
	ARLVis *vout = malloc(104 * sizeof(char));

	vin->nvis = 1;
	vin->npol = 4;

	// malloc to ARLDataVisSize
	vin->data = malloc(72+(32*vin->npol*vin->nvis) * sizeof(char));
	vout->data = malloc(72+(32*vin->npol*vin->nvis) * sizeof(char));

	if (!vin->data || !vout->data) {
		fprintf(stderr, "Malloc error\n");
		exit(1);
	}

	arl_copy_visibility(vin, vout, false);

	return 0;
}