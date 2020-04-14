from setuptools import setup, Extension
from distutils.sysconfig import get_config_var


setup(
    name="cocotb_examples_endian_swapper",
    ext_modules=[
        Extension(
            'io_module',
            sources=[
                'io.c',
            ],
            extra_link_args=['-Wl,-soname,io_module' + get_config_var('EXT_SUFFIX')]
        ),
        Extension(
            'hal',
            sources=[
                '../hal/endian_swapper_hal.c',
                '../hal/endian_swapper_hal.i',
            ],
            include_dirs=['.'],
            libraries=['io_module' + get_config_var('EXT_SUFFIX')],
            library_dirs=['.'],
        ),
    ]
)
