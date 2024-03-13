from setuptools import Extension, setup

module = Extension("geo_capi", sources=['geomodule.c'])
setup(name='geo_capi',
     version='1.0',
     description='Python wrapper for custom C extension',
     ext_modules=[module])