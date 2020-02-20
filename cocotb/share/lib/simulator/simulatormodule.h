/******************************************************************************
* Copyright (c) 2013 Potential Ventures Ltd
* Copyright (c) 2013 SolarFlare Communications Inc
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*    * Redistributions of source code must retain the above copyright
*      notice, this list of conditions and the following disclaimer.
*    * Redistributions in binary form must reproduce the above copyright
*      notice, this list of conditions and the following disclaimer in the
*      documentation and/or other materials provided with the distribution.
*    * Neither the name of Potential Ventures Ltd,
*       SolarFlare Communications Inc nor the
*      names of its contributors may be used to endorse or promote products
*      derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL POTENTIAL VENTURES LTD BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
******************************************************************************/

#ifndef _SIMULATOR_MODULE_H
#define _SIMULATOR_MODULE_H

#include <Python.h>
#include "gpi_logging.h"
#include "gpi.h"

// This file defines the routines available to Python

#define COCOTB_ACTIVE_ID        0xC0C07B        // User data flag to indicate callback is active
#define COCOTB_INACTIVE_ID      0xDEADB175      // User data flag set when callback has been de-registered

#define MODULE_NAME "simulator"

// callback user data
typedef struct t_callback_data {
    PyThreadState *_saved_thread_state; // Thread state of the calling thread FIXME is this required?
    uint32_t id_value;                  // COCOTB_ACTIVE_ID or COCOTB_INACTIVE_ID
    PyObject *function;                 // Function to call when the callback fires
    PyObject *args;                     // The arguments to call the function with
    PyObject *kwargs;                   // Keyword arguments to call the function with
    gpi_sim_hdl cb_hdl;
} s_callback_data, *p_callback_data;


static PyTypeObject gpi_sim_hdl_Type;

typedef struct {
    PyObject_HEAD
    gpi_sim_hdl hdl;
} gpi_sim_hdl_Object;

static PyTypeObject gpi_cb_hdl_Type;

typedef struct {
    PyObject_HEAD
    gpi_cb_hdl hdl;
} gpi_cb_hdl_Object;

static PyTypeObject gpi_iterator_hdl_Type;

typedef struct {
    PyObject_HEAD
    gpi_iterator_hdl hdl;
} gpi_iterator_hdl_Object;

static PyObject *error_out(PyObject *m, PyObject *args);
static PyObject *log_msg(PyObject *self, PyObject *args);

// Raise an exception on failure
// Return None if for example get bin_string on enum?
static PyObject *get_signal_val_long(gpi_sim_hdl_Object *self, PyObject *args);
static PyObject *get_signal_val_real(gpi_sim_hdl_Object *self, PyObject *args);
static PyObject *get_signal_val_str(gpi_sim_hdl_Object *self, PyObject *args);
static PyObject *get_signal_val_binstr(gpi_sim_hdl_Object *self, PyObject *args);
static PyObject *set_signal_val_long(gpi_sim_hdl_Object *self, PyObject *args);
static PyObject *set_signal_val_real(gpi_sim_hdl_Object *self, PyObject *args);
static PyObject *set_signal_val_str(gpi_sim_hdl_Object *self, PyObject *args);
static PyObject *set_signal_val_binstr(gpi_sim_hdl_Object *self, PyObject *args);
static PyObject *get_definition_name(gpi_sim_hdl_Object *self, PyObject *args);
static PyObject *get_definition_file(gpi_sim_hdl_Object *self, PyObject *args);
static PyObject *get_handle_by_name(gpi_sim_hdl_Object *self, PyObject *args);
static PyObject *get_handle_by_index(gpi_sim_hdl_Object *self, PyObject *args);
static PyObject *get_name_string(gpi_sim_hdl_Object *self, PyObject *args);
static PyObject *get_type(gpi_sim_hdl_Object *self, PyObject *args);
static PyObject *get_const(gpi_sim_hdl_Object *self, PyObject *args);
static PyObject *get_type_string(gpi_sim_hdl_Object *self, PyObject *args);
static PyObject *get_num_elems(gpi_sim_hdl_Object *self, PyObject *args);
static PyObject *get_range(gpi_sim_hdl_Object *self, PyObject *args);

static PyObject *register_timed_callback(PyObject *self, PyObject *args);
static PyObject *register_value_change_callback(PyObject *self, PyObject *args);
static PyObject *register_readonly_callback(PyObject *self, PyObject *args);
static PyObject *register_nextstep_callback(PyObject *self, PyObject *args);
static PyObject *register_rwsynch_callback(PyObject *self, PyObject *args);

static PyObject *get_root_handle(PyObject *self, PyObject *args);
static PyObject *stop_simulator(PyObject *self, PyObject *args);

static PyObject *iterate(gpi_sim_hdl_Object *self, PyObject *args);

static PyObject *get_sim_time(PyObject *self, PyObject *args);
static PyObject *get_precision(PyObject *self, PyObject *args);

static PyObject *log_level(PyObject *self, PyObject *args);

static PyMethodDef SimulatorMethods[] = {
    {"log_msg", log_msg, METH_VARARGS, "Log a message"},
    {"get_root_handle", get_root_handle, METH_VARARGS, "Get the root handle"},
    {"register_timed_callback", register_timed_callback, METH_VARARGS, "Register a timed callback"},
    {"register_value_change_callback", register_value_change_callback, METH_VARARGS, "Register a signal change callback"},
    {"register_readonly_callback", register_readonly_callback, METH_VARARGS, "Register a callback for the read-only section"},
    {"register_nextstep_callback", register_nextstep_callback, METH_VARARGS, "Register a callback for the NextSimTime callback"},
    {"register_rwsynch_callback", register_rwsynch_callback, METH_VARARGS, "Register a callback for the read-write section"},
    {"stop_simulator", stop_simulator, METH_VARARGS, "Instruct the attached simulator to stop"},
    {"log_level", log_level, METH_VARARGS, "Set the log level for GPI"},

    // FIXME METH_NOARGS => initialization from incompatible pointer type
    {"get_sim_time", get_sim_time, METH_VARARGS, "Get the current simulation time as an int tuple"},
    {"get_precision", get_precision, METH_VARARGS, "Get the precision of the simulator"},
    {"error_out", error_out, METH_NOARGS, NULL},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};


static PyObject *deregister(gpi_cb_hdl_Object *hdl_obj, PyObject *args);

static PyMethodDef gpi_cb_hdl_methods[] = {
    {"deregister", (PyCFunction)deregister, METH_NOARGS, "De-register this callback"},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

static PyMethodDef gpi_sim_hdl_methods[] = {
    {"get_signal_val_long", (PyCFunction)get_signal_val_long, METH_NOARGS, "Get the value of a signal as a long"},
    {"get_signal_val_str", (PyCFunction)get_signal_val_str, METH_NOARGS, "Get the value of a signal as an ASCII string"},
    {"get_signal_val_binstr", (PyCFunction)get_signal_val_binstr, METH_NOARGS, "Get the value of a signal as a binary string"},
    {"get_signal_val_real", (PyCFunction)get_signal_val_real, METH_NOARGS, "Get the value of a signal as a double precision float"},
    {"set_signal_val_long", (PyCFunction)set_signal_val_long, METH_VARARGS, "Set the value of a signal using a long"},
    {"set_signal_val_str", (PyCFunction)set_signal_val_str, METH_VARARGS, "Set the value of a signal using an NUL-terminated 8-bit string"},
    {"set_signal_val_binstr", (PyCFunction)set_signal_val_binstr, METH_VARARGS, "Set the value of a signal using a string with a character per bit"},
    {"set_signal_val_real", (PyCFunction)set_signal_val_real, METH_VARARGS, "Set the value of a signal using a double precision float"},
    {"get_definition_name", (PyCFunction)get_definition_name, METH_NOARGS, "Get the name of a GPI object's definition"},
    {"get_definition_file", (PyCFunction)get_definition_file, METH_NOARGS, "Get the file that sources the object's definition"},
    {"get_handle_by_name", (PyCFunction)get_handle_by_name, METH_VARARGS, "Get handle of a named object"},
    {"get_handle_by_index", (PyCFunction)get_handle_by_index, METH_VARARGS, "Get handle of a object at an index in a parent"},
    {"get_name_string", (PyCFunction)get_name_string, METH_NOARGS, "Get the name of an object as a string"},
    {"get_type_string", (PyCFunction)get_type_string, METH_NOARGS, "Get the type of an object as a string"},
    {"get_type", (PyCFunction)get_type, METH_NOARGS, "Get the type of an object, mapped to a GPI enumeration"},
    {"get_const", (PyCFunction)get_const, METH_NOARGS, "Get a flag indicating whether the object is a constant"},
    {"get_num_elems", (PyCFunction)get_num_elems, METH_NOARGS, "Get the number of elements contained in the handle"},
    {"get_range", (PyCFunction)get_range, METH_NOARGS, "Get the range of elements (tuple) contained in the handle, returns None if not indexable"},
    {"iterate", (PyCFunction)iterate, METH_VARARGS, "Get an iterator handle to loop over all members in an object"},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

#endif
