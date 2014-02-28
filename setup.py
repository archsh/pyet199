import setuptools
import os
import sys
import platform
import distutils.spawn
from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
from distutils.errors import *
import pickle

__version__ = "0.2"

# Disable hard links, otherwise building distributions fails on OS X
try:
    del os.link
except:
    pass

ARCH,OS = platform.architecture()
if OS.startswith('Windows'):
    library_dirs=['lib\\windows\\%s'%ARCH]
    libraries   =['ET199_S']
    def_macros  =[('WIN32',1)]
    extra_compile_args = []
    extra_link_args =['/NODEFAULTLIB:LIBCMT']
    #ET199_LIB.append('ET199_S.lib')
elif OS.startswith('ELF'):
    library_dirs=['lib/linux/%s'%ARCH]
    libraries   =['ET199']
    def_macros  =[('LINUX',1)]
    extra_compile_args = []
    extra_link_args =[]
    #ET199_LIB.append('libET199.a')


def_macros.append(("PYET199_VERSION", '"%s"'%__version__))

pyet199_mod = Extension('pyet199',
                         sources=['pyet199.c'],
                         include_dirs=['include/'],
                         library_dirs=library_dirs,
                         libraries=libraries,
                         extra_link_args=extra_link_args,
                         extra_compile_args=extra_compile_args,
                         define_macros=def_macros)

setup (name='pyet199',
       version=__version__,
       description="A Python interface to ET199 USB Dongle.",
       license="MIT",
       long_description="""\
	   
""",
       author='Mingcai SHEN',
       author_email='archsh@gmail.com',
       url='http://github.com/archsh/pyet199',
       classifiers=[
    'Development Status :: 4 - Beta',
    'Intended Audience :: Developers',
    'License :: OSI Approved :: MIT',
    'Topic :: System :: License',
    ],
       ext_modules=[pyet199_mod])
