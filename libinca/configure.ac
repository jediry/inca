# Process this file with GNU autoconf to produce a configure script.


###############################################################################
##
## Build tools configuration
##
###############################################################################

# Autoconf/Automake configuration
AC_PREREQ(2.53)
AC_INIT(libinca, 0.5.0, rls@tamu.edu, libinca)
AC_CONFIG_SRCDIR(src/inca/inca-common.h)
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_MACRO_DIR(config)
AC_CONFIG_FILES(Makefile src/Makefile test_raster/Makefile)
AC_CONFIG_HEADERS(src/inca/config-ac.h)
AM_INIT_AUTOMAKE($PACKAGE_NAME, $PACKAGE_VERSION)
AC_CANONICAL_HOST


# Compilers and other tools
AC_PROG_CXX         # The C++ compiler
AC_PROG_INSTALL     # The 'install' script
AC_PROG_RANLIB      # Tool for building static libraries
#AC_PROG_LIBTOOL     # Tool for building dynamic libraries

# Compiler configuration
AC_LANG(C++)


###############################################################################
##
## Required headers and libraries
##
###############################################################################
# Check for some representative header files, and assume their usual
# companions are around as well

AC_HEADER_STDC
AC_HEADER_TIME
AC_CHECK_HEADERS(math.h sys/time.h sys/types.h sys/stat.h unistd.h)
ACT_REQUIRE_HEADER(vector,                  [C++ Standard Template Library])
#ACT_REQUIRE_HEADER(hash_map,                [C++ STL hash container extensions])
ACT_REQUIRE_HEADER(ext/hash_map,            [C++ STL hash container extensions])
#ACT_REQUIRE_HEADER(hash_map ext/hash_map,   [C++ STL hash container extensions])
ACT_REQUIRE_HEADER(sstream,                 [C++ IOstream library])
ACT_REQUIRE_HEADER(boost/shared_ptr.hpp,    [Boost smart pointer])
ACT_REQUIRE_HEADER(boost/multi_array.hpp,   [Boost multi-dim array])


###############################################################################
##
## Optional features and external library/package configuration
##
###############################################################################

#------------------------------------------------------------------------------
# For developer entertainment only :-)
#------------------------------------------------------------------------------
ACT_ENABLE_OPTION(zorbot-kill, "no", destruction of zorbots)


#------------------------------------------------------------------------------
# File I/O capability switches
#------------------------------------------------------------------------------
ACT_ENABLE_OPTION(config-io, yes, configuration file I/O (requires XML support))
ACT_ENABLE_OPTION(scene-io,  yes, scene file I/O)
ACT_ENABLE_OPTION(image-io,  yes, image/video file I/O)
ACT_ENABLE_OPTION(raytracing,  yes, raytracing)
ACT_ENABLE_OPTION(radiosity,  yes, radiosity)
ACT_ENABLE_OPTION(vertex-shaders,  yes, per-vertex programmable rendering)
ACT_ENABLE_OPTION(pixel-shaders,  yes, per-pixel programmable rendering)


#------------------------------------------------------------------------------
# Should we use the X-Window system?
#------------------------------------------------------------------------------
AC_PATH_XTRA
if test -z "$X_DISPLAY_MISSING"; then
    CXXFLAGS="$CXXFLAGS $X_CFLAGS"
    LDFLAGS="$LDFLAGS $X_LIBS"
fi


#------------------------------------------------------------------------------
# Which rendering interfaces should we use?
#------------------------------------------------------------------------------
ACT_WITH_PACKAGE(opengl, yes, OpenGL for 2D & 3D rendering, [
    ACT_REQUIRE_HEADER(GL/gl.h, OpenGL header file)
    ACT_REQUIRE_HEADER(GL/glu.h, OpenGL Utilities header file)
    ACT_REQUIRE_LIB(GL, glEnd, OpenGL library)
    ACT_REQUIRE_LIB(GLU, gluLookAt, OpenGL Utilities library)
])


#------------------------------------------------------------------------------
# Which (if any) XML parser should we use?
#------------------------------------------------------------------------------
ACT_CHOOSE_PACKAGE(xml-parser, [libxml xerces], [none], [XML parser], yes, yes, [
    ACT_CHOICE_VERIFY(xerces, [
        AC_CHECK_HEADERS(xercesc/util/PlatformUtils.hpp,
                         [], [act_package_verified="no"])
        ACT_CHECK_CXX_LIB(xerces-c, XMLPlatformUtils::Initialize(),
                         [#include <xercesc/util/PlatformUtils.hpp>],
                         [], [act_package_verified="no"])
    ])
    ACT_CHOICE_VERIFY(libxml, [
        echo "Just kidding...libxml isn't supported"
        act_package_verified=no
    ])
])


#------------------------------------------------------------------------------
# Should we use parsers built using Antlr?
#------------------------------------------------------------------------------
ACT_WITH_PACKAGE(antlr, no, file-format parsers generated using Antlr, [
    ACT_REQUIRE_CXX_LIB(antlr, [antlr::ANTLRException()],
                        [#include <antlr/ANTLRException.hpp>],
                        [Antlr support library])
])


#------------------------------------------------------------------------------
# What image/video packages should we use to load/save stuff?
#------------------------------------------------------------------------------
ACT_WITH_PACKAGE(image-magick, no, ImageMagick image/video codecs, [
    ACT_REQUIRE_HEADER(Magick++.h, ImageMagick C++ header)
    ACT_REQUIRE_CXX_LIB(Magick++, [Magick::Image()],
                        [#include <Magick++.h>],
                        [ImageMagick C++ library])
])
ACT_WITH_PACKAGE(free-image, no, FreeImage image/video codecs, [
    ACT_REQUIRE_HEADER(FreeImage.h, FreeImage header)
    ACT_REQUIRE_CXX_LIB(FreeImage, [FreeImage_Load(FIF_BMP, "", 0)],
                        [#include <FreeImage.h>],
                        [FreeImage library])
])


#------------------------------------------------------------------------------
# What user interface toolkits should we use?
#------------------------------------------------------------------------------
ACT_WITH_PACKAGE(glut,  yes, [GLUT as a user interface], [
    ACT_REQUIRE_HEADER(GL/glut.h, GLUT header)
    ACT_REQUIRE_LIB(glut, glutMainLoop, [GL Utilities Toolkit library])
])
ACT_WITH_PACKAGE(glui,  no,  [GLUI as a user interface])
ACT_WITH_PACKAGE(qt,    no,  [Trolltech's QT toolkit as a user interface])
ACT_WITH_PACKAGE(gtk,   no,  [the GTK+ toolkit as a user interface])
ACT_WITH_PACKAGE(fltk,  no,  [the Fast, Light Toolkit as a user interface])
ACT_WITH_PACKAGE(win32, no,  [Microsoft's Win32 GUI as a user interface])


# Output resulting files
AC_OUTPUT


# Print out a description of our current configuration
echo
echo "    $PACKAGE_NAME v$PACKAGE_VERSION Compile-time Configuration"
echo
echo "    Feature                      Status"
echo "    ---------------------------- --------------------"
echo "    Rendering support"
echo "      OpenGL.....................$with_opengl"
echo "        Vertex shaders...........$enable_vertex_shaders"
echo "        Pixel shaders............$enable_pixel_shaders"
echo "      Raytracing.................$enable_raytracing"
echo "      Radiosity..................$enable_radiosity"
echo
echo "    I/O support"
echo "      Scene file I/O.............$enable_scene_io"
echo "        Antlr parsers............$with_antlr"
echo "      Config file I/O............$enable_config_io"
echo "        XML parser...............$with_xml_parser"
echo "      Image/video file I/O.......$enable_image_io"
echo "        ImageMagick codecs.......$with_image_magick"
echo "        FreeImage codecs.........$with_free_image"
echo
echo "    User interface toolkit support"
echo "      GLUT.......................$with_glut"
echo "      GLUI.......................$with_glui"
echo "      QT.........................$with_qt"
echo "      GTK+.......................$with_gtk"
echo "      FLTK.......................$with_fltk"
echo "      Win32......................$with_win32"
echo
echo "    Environment variables:"
echo "    ----------------------"
echo "    CPPFLAGS = $CPPFLAGS"
echo "    CFLAGS   = $CFLAGS"
echo "    CXXFLAGS = $CXXFLAGS"
echo "    LDFLAGS  = $LDFLAGS"
echo "    LIBS     = $LIBS"
echo
echo "Now type 'make' to build $PACKAGE_NAME"

