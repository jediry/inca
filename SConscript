#######################################################################################################################
# SConscript -- core build script for building inca
#
# Inca supports being embedded as a submodule of another project (`git help submodule`), in which case the parent
# project can invoke inca/SConscript directly, bypassing inca/SConstruct entirely. This allows Inca to be built into
# a build tree of the parent project's chosing, and to share its VCPkg instance and other configuration (e.g., compiler
# options). This "embedded" mode is useful when you want to hack on both Inca and the subproject at the same time.
#
# Inca also supports building in isolation, as its own top-level project. If you run "scons" (or build.bat) from the
# Inca root directory, this will cause inca/SConstruct to be read, which will set up a build environment appropriate
# for a standalone build. This "standalone" mode is useful when you're hacking on Inca alone (e.g., with unit tests)
# or when building distribution packages for Inca.
#
# This dual-mode behavior implies the following dichotomy: SConstruct is responsible for creating the Environment,
# setting VCPKGROOT
#######################################################################################################################

# Get the construction environment from the top-level script
Import('env')


###################################################################
# Locate and install build-time dependencies
###################################################################

env.VCPkg('boost-static-assert')
env.VCPkg('boost-smart-ptr')
env.VCPkg('boost-mpl')
env.VCPkg('boost-utility')
env.VCPkg('boost-iterator')
env.VCPkg('opengl')
env.VCPkg('freeglut')
env.VCPkg('fftw3')


###################################################################
# Build it!
###################################################################

env.SConscript('src/inca/SConscript')
