from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import numpy

class my_build_ext(build_ext):
    def build_extensions(self):
        if self.compiler.compiler_type == 'unix':
            for e in self.extensions:
                e.extra_compile_args = ['-std=c++11', '-msse4.2', '-mbmi', '-mbmi2', '-mavx2']
        elif self.compiler.compiler_type == 'msvc':
            for e in self.extensions:
                e.extra_compile_args = ['/arch:AVX2']

        build_ext.build_extensions(self)

ext_modules = [
    Extension('creversi.creversi',
        ['creversi/creversi.pyx',
         'creversi_cpp/bit_manipulations.cpp', 'creversi_cpp/hand.cpp', 'creversi_cpp/movable_generator.cpp', 'creversi_cpp/move_generator.cpp', 'creversi_cpp/state.cpp', 'creversi_cpp/utils.cpp', 'creversi_cpp/value.cpp'],
        language='c++',
        include_dirs = ['creversi_cpp', numpy.get_include()]),
    Extension('creversi.gym_reversi.envs.reversi_env',
        ['creversi/gym_reversi/envs/reversi_env.pyx'],
        language='c++'),
    Extension('creversi.gym_reversi.envs.reversi_vec_env',
        ['creversi/gym_reversi/envs/reversi_vec_env.pyx'],
        language='c++'),
]

setup(
    name='creversi',
    version='0.0.0',
    packages=['creversi', 'creversi.gym_reversi', 'creversi.gym_reversi.envs'],
    ext_modules=ext_modules,
    cmdclass={'build_ext': my_build_ext}
)
