## Makefile.am -- Process this file with automake to generate Makefile.in

## Incorporate automatic source file tracking mechanism
include $(top_srcdir)/ac_tools/automake/src-catalog.rules

## Incorporate manually-invoked rules for regenerating and cleaning things
include $(top_srcdir)/ac_tools/automake/manual.rules


# Import the auto-generated source catalog
include $(top_srcdir)/config/src-catalog

# Library definitions
lib_LIBRARIES = libinca.a
libinca_a_SOURCES = $(inca_util_src) $(inca_math_src) $(inca_imaging_src)   \
		    $(inca_world_src) $(inca_rendering_src)	    	    \
		    $(inca_ui_src)					    \
		    $(inca_dynamics_src)				    \
		    $(inca_integration_hardware_math_src)		    \
		    $(inca_integration_glut_src)			    \
		    $(inca_integration_opengl_src)			    \
		    $(inca_integration_fftw_src)				    \
		    $(inca_integration_posix_src)			    \
		    inca/globals.cpp
#libinca_la_LDFLAGS    = -version-info $(PACKAGE_VERSION)

# Distributed include files
nobase_include_HEADERS = $(inca_tpl) $(inca_hdr) inca/config-ac.h


## Unit tests
#check_PROGRAMS = run_unit_tests
check_PROGRAMS = simple_raster_op_test run_unit_tests
simple_raster_op_test_SOURCES = test/simple_raster_op_test.cpp
simple_raster_op_test_LDADD   = -L. -linca
run_unit_tests_SOURCES = test/unit_test_main.cpp
run_unit_tests_LDADD   = -L. -lcppunit -linca
