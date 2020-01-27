#! /usr/bin/env python

#######################################################################################################################
# SConstruct -- build script for doing standalone build of inca
#
# Inca supports being embedded as a submodule of another project (`git help submodule`), in which case the parent project
# can invoke inca/SConscript directly, and bypass this script entirely. See comments at the top of the SConscript.
#######################################################################################################################


"""
help       -> scons -h
compile    -> scons
clean      -> scons -c
install    -> scons install
uninstall  -> scons -c install
configure  -> scons configure prefix=/tmp/ita debug=full extraincludes=/usr/local/include:/tmp/include prefix=/usr/local

Run from a subdirectory -> scons -u
The variables are saved automatically after the first run (look at cache/kde.cache.py, ..)
"""


###################################################################
# Create and configure the Environment
###################################################################

DefaultEnvironment(tools = [])
env = Environment(tools = ['default', 'doxygen', 'c++17', 'build.flavor'],
                  toolpath = ['#/external/scons-tools', '#/tools'])

# Determine debug vs. ship build, and use this to determine the variant directory
flavor = GetOption('flavor')
variantDir = '#/build/' + flavor

# Here's where VCPkg is located
env['VCPKGROOT'] = '#/external/vcpkg'

# Delegate to the SConscript file for the core build logic
Export('env')
env.SConscript('#/SConscript', variant_dir = variantDir, duplicate = 0)
