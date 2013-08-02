#!/usr/bin/env python
# PyPRUSS setup script
# Note that there is a hack here for removing the default sysroot directive
# that is standard for python installers. 
 
print "Installing PyPRUSS"

try:
    from distutils.core import setup, Extension
    from distutils import sysconfig
    import re
    
    print "Removing --sysroot directive from Link command"
    vars  = sysconfig.get_config_vars()
    vars['LDSHARED'] =  re.sub("--sysroot=.* ", " ", vars['LDSHARED'])

    print "Running setup"

    setup(name='PyPRUSS',
        version='0.1',
        description='A Python binding for prussdrv - for controlling the PRUs on BeagleBone',
        author='Elias Bakken',
        author_email='elias.bakken@gmail.com',
        license='BSD',
        url='http://hipstercirtuits.com',
        ext_modules=[Extension('pypruss', 
                            ['pypruss/pypruss.c', 'pypruss/prussdrv.c'], 
                            include_dirs=['pypruss/include', 
                                '/usr/include/python2.7',
                                '/usr/include'],
                            define_macros=[('__DEBUG', None)],
                            libraries=['pthread'], 
                            extra_link_args=["-shared"]

                    )],
        )
    print "Finished installing, Great!"
except Exception, e:
    print "Install failed with exception:\n%s" % e

