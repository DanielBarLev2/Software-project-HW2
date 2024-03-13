from setuptools import Extension, setup

module = Extension("mykmeanssp", sources=['/a/home/cc/students/cs/danielbarlev/testrun/Software-project-HW2/kmean_pp@C/pd.c'])
setup(name='mykmeanssp',
     version='1.0',
     description='Python wrapper for custom C extension',
     ext_modules=[module])