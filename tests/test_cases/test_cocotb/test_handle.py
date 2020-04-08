# Copyright cocotb contributors
# Licensed under the Revised BSD License, see LICENSE for details.
# SPDX-License-Identifier: BSD-3-Clause
"""
Tests for handles
"""
import cocotb
from cocotb.result import TestFailure


@cocotb.test()
def test_lessthan_raises_error(dut):
    """
    Test that trying to use <= as if it were a comparison produces an error
    """
    ret = dut.stream_in_data <= 0x12
    try:
        bool(ret)
    except TypeError:
        pass
    else:
        raise TestFailure(
            "No exception was raised when confusing comparison with assignment"
        )

    # to make this a generator
    if False: yield


@cocotb.test()
async def test_extended_identifiers(dut):
    if cocotb.LANGUAGE == 'vhdl':
        # sample_module.vhdl
        names = [
            "\\weird.signal(1)\\",
            "\\weird.signal(2)\\",
            "\\(.*|this looks like a regex)\\"
        ]
    else:
        # sample_module.sv
        names = [
            "weird.signal[1]",
            "weird.signal[2]",
            "(.*|this_looks_like_a_regex)",
        ]

    # icarus can't find the signals by name unless we scan all signals
    if cocotb.SIM_NAME.lower().startswith("icarus"):
        dut._discover_all()

    try:
        for name in names:
            # extended does not do the right thing for verilog, so just put
            # \\ in the vhdl strings above ourselves.
            hdl = dut._id(name, extended=False)
            assert hdl._name == name

            # gh-1548
            assert dut._sub_handles[name] is hdl

    except AttributeError:
        # if something goes wrong, make sure we know which signals were found
        from pprint import pformat
        dut._discover_all()
        cocotb.log.warning(
            "Signals in the design were:\n%s", pformat(dut._sub_handles))
        raise


@cocotb.test()
def test_bad_attr(dut):
    yield cocotb.triggers.NullTrigger()
    try:
        _ = dut.stream_in_data.whoops
    except AttributeError as e:
        assert 'whoops' in str(e)
    else:
        assert False, "Expected AttributeError"


# strings are not supported on Icarus
@cocotb.test(skip=cocotb.SIM_NAME.lower().startswith("icarus"))
async def test_string_handle_takes_bytes(dut):
    dut.string_input_port.value = b"bytes"
    await cocotb.triggers.Timer(10, 'ns')
    val = dut.string_input_port.value
    assert isinstance(val, bytes)
    assert val == b"bytes"
