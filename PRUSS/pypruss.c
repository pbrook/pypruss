/*
PyPRUSS - a Python binding for the PRU driver on BeagleBone 

Author: Elias Bakken
email: elias.bakken@gmail.com
Website: http://www.hipstercircuits.com
License: BSD

You can use and change this, but keep this heading :)
*/

#include <Python.h>
#include <prussdrv.h>
#include <pruss_intc_mapping.h>

// Modprobe 
static PyObject *pypruss_modprobe(PyObject *self, PyObject *args){
	system("modprobe uio_pruss");									
    Py_INCREF(Py_None);
    return Py_None;
}


// Init
static PyObject *pypruss_init(PyObject *self, PyObject *args){
    prussdrv_init ();	    
    Py_INCREF(Py_None);
    return Py_None;
}

// Open 
static PyObject *pypruss_open(PyObject *self, PyObject *args){
    int ret;
	int evtout;

	if (!PyArg_ParseTuple(args, "i", &evtout)){
     	return NULL;
    }
    ret = prussdrv_open(evtout);
    if (ret){
        printf("prussdrv_open open failed\n");
        return NULL;
    }
    Py_INCREF(Py_None);
    return Py_None;
}

// Reset a PRU
static PyObject *pypruss_pru_reset(PyObject *self, PyObject *args){
	int pru_num;
	if (!PyArg_ParseTuple(args, "i", &pru_num))
        return NULL;
	prussdrv_pru_reset(pru_num); 										// Enable the PRU
    Py_INCREF(Py_None);						
    return Py_None;	
}

// Disable a PRU
static PyObject *pypruss_pru_disable(PyObject *self, PyObject *args){
	int pru_num;
	if (!PyArg_ParseTuple(args, "i", &pru_num))
        return NULL;
	prussdrv_pru_disable(pru_num); 										// Enable the PRU
    Py_INCREF(Py_None);						
    return Py_None;	
}

// Enable a PRU
static PyObject *pypruss_pru_enable(PyObject *self, PyObject *args){
	int pru_num;
	if (!PyArg_ParseTuple(args, "i", &pru_num))
        return NULL;
	prussdrv_pru_enable(pru_num); 											// Start the PRU
    Py_INCREF(Py_None);													// Return None to indicate Ok
    return Py_None;	
}

// Write data to the memory
static PyObject *pypruss_pru_write_memory(PyObject *self, PyObject *args){
	int mem_type;		// PRUSS0_PRU0_DATARAM or PRUSS0_PRU1_DATARAM
    int i;    
    int len;	
	int offset;
	PyObject* data_obj;
	PyObject* int_obj;
	PyObject* data_seq;
	unsigned int *data;

    if (!PyArg_ParseTuple(args, "iiO", &mem_type, &offset, &data_obj))
        return NULL;

    data_seq = PySequence_Fast(data_obj, "expected a sequence");
    len = PySequence_Size(data_obj);
	data = (unsigned int *)calloc(len, 4);

    for (i = 0; i < len; i++) {
        int_obj	= PySequence_Fast_GET_ITEM(data_seq, i);
		data[i] = PyInt_AsUnsignedLongMask(int_obj);		
    }
    Py_DECREF(data_seq);


    prussdrv_pru_write_memory(mem_type, offset,  data, len*4);

    Py_INCREF(Py_None);													
    return Py_None;	
}

// Interrupt init
static PyObject *pypruss_pruintc_init(PyObject *self, PyObject *args){
    tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;	
    prussdrv_pruintc_init(&pruss_intc_initdata);					// Get the interrupt initialized

    Py_INCREF(Py_None);
    return Py_None;
}


// Execute a program 
static PyObject *pypruss_exec_program(PyObject *self, PyObject *args){
    int pru_num;
	char* filename;

    if (!PyArg_ParseTuple(args, "is", &pru_num, &filename)){		// Parse the PRU number
     	return NULL;
    }

    prussdrv_exec_program (pru_num, filename);						// Load and execute the program 
	
	Py_INCREF(Py_None);
    return Py_None;
}
 

// Wait for an event from a PRU
static PyObject *pypruss_wait_for_event(PyObject *self, PyObject *args){
	int evtout; // PRU_EVTOUT_0 or PRU_EVTOUT_1
    if (!PyArg_ParseTuple(args, "i", &evtout))
     	return NULL;	
	prussdrv_pru_wait_event (evtout);			// Wait for the event. This blocks the thread. 
    Py_INCREF(Py_None);
    return Py_None;
}

// Clear an event 
static PyObject *pypruss_clear_event(PyObject *self, PyObject *args){
	int event; // PRU0_ARM_INTERRUPT or PRU1_ARM_INTERRUPT
    if (!PyArg_ParseTuple(args, "i", &event))
     	return NULL;
	prussdrv_pru_clear_event (event); 	// Clear the event 
    Py_INCREF(Py_None);
    return Py_None;
}

// Exit the PRU driver
static PyObject *pypruss_exit(PyObject *self, PyObject *args){	
    prussdrv_exit();
    Py_INCREF(Py_None);
    return Py_None;
}

// Declare the methods to export
static PyMethodDef pypruss_methods[] = {
        { "modprobe", (PyCFunction)pypruss_modprobe, METH_VARARGS, NULL },
        { "init", (PyCFunction)pypruss_init, METH_VARARGS, NULL },
        { "open", (PyCFunction)pypruss_open, METH_VARARGS, NULL },
		{ "pru_reset", (PyCFunction)pypruss_pru_reset, METH_VARARGS, NULL},
		{ "pru_disable", (PyCFunction)pypruss_pru_disable, METH_VARARGS, NULL},
		{ "pru_enable", (PyCFunction)pypruss_pru_enable, METH_VARARGS, NULL},		
		{ "pru_write_memory", (PyCFunction)pypruss_pru_write_memory, METH_VARARGS, NULL},
        { "pruintc_init", (PyCFunction)pypruss_pruintc_init, METH_VARARGS, NULL },
        { "exec_program", (PyCFunction)pypruss_exec_program, METH_VARARGS, NULL },
		{ "wait_for_event", (PyCFunction)pypruss_wait_for_event, METH_VARARGS, NULL},
		{ "clear_event", (PyCFunction)pypruss_clear_event, METH_VARARGS, NULL},		
		{ "exit", (PyCFunction)pypruss_exit, METH_VARARGS, NULL},
        { NULL, NULL, 0, NULL }
};
 
// Some sort of init stuff.         
PyMODINIT_FUNC initpypruss()
{
        Py_InitModule3("pypruss", pypruss_methods, "Extenstion lib for PRUSS");
}
