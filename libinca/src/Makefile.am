## Makefile.am -- Process this file with automake to generate Makefile.in

###############################################################################
##
## Build configuration variables
##
###############################################################################
# Final output binaries go here
distdir=$(top_srcdir)/dist

# Intermediate generated source files go here (and make sure we 'clean' them)
gen_srcdir = $(top_srcdir)/src/generated
CLEANFILES = $(gen_srcdir)/*


###############################################################################
##
## Source file definitions
##
###############################################################################
## Include the source file catalog
include $(top_srcdir)/src_catalog


dist_LIBRARIES = libinca.a

libinca_a_SOURCES = $(src_inca_util) $(src_inca_math) $(src_inca_imaging)   \
		    $(src_inca_world) $(src_inca_rendering)		    \
                    $(src_inca_ui)					    \
		    $(src_inca_dynamics)				    \
		    $(src_inca_integration_glut)			    \
		    $(src_inca_integration_opengl)			    \
		    $(src_inca_integration_posix)			    \
		    inca/globals.cpp
#libinca_la_LDFLAGS    = -version-info $(PACKAGE_VERSION)


## Unit tests
check_PROGRAMS = run_unit_tests
run_unit_tests_SOURCES = inca/test/unit_test_main.cpp
run_unit_tests_LDADD    = -L. -lcppunit
#run_unit_tests_LDADD    = -L. -lcppunit -linca


###############################################################################
##
## Special build target definitions
##
###############################################################################
# Make the Makefile depend on the src catalog
Makefile.in : $(top_srcdir)/src_catalog

# Rebuild the src catalog
catalog :
	( cd $(top_srcdir) ; make catalog)

# Reset the dependency information that make uses
clean-deps :
	for file in .deps/*; do echo > $$file ; done

# Generate serialization code from XML
XSLT = java org.apache.xalan.xslt.Process
xslt_src = io/normalize-schema.xsl io/schema-to-c++.xsl
$(gen_srcdir)/SceneIdentifiers.h++ : io/scene/scene-schema.xsd $(xslt_src)
	$(XSLT) -IN io/scene/scene-schema.xsd                \
		-XSL io/normalize-schema.xsl                \
		-OUT $(gen_srcdir)/scene-normalized.xsd &&        \
	$(XSLT) -IN $(gen_srcdir)/scene-normalized.xsd            \
		-XSL io/schema-to-c++.xsl                \
		-OUT $(gen_srcdir)/dummy.xml                \
		-PARAM source        io/scene/scene-schema.xsd        \
		-PARAM ident-decl   SceneIdentifiers.h++        \
		-PARAM ident-impl   SceneIdentifiers.c++        \
		-PARAM ser-decl        SceneSerialization.h++        \
		-PARAM ser-impl        SceneSerialization.c++ &&        \
	echo >> $(gen_srcdir)/SceneIdentifiers.h++ &&            \
	echo >> $(gen_srcdir)/SceneIdentifiers.c++ &&            \
	echo >> $(gen_srcdir)/SceneSerialization.h++ &&            \
	echo >> $(gen_srcdir)/SceneSerialization.c++
#	rm -f $(gen_srcdir)/dummy.xml $(gen_srcdir)/normalized.xsd

# Generate parser/lexer code from Antlr grammar files
$(gen_srcdir)/%Lexer.hpp : io/%.g
	java antlr.Tool -o $(gen_srcdir) $<
$(gen_srcdir)/%Parser.hpp : io/%.g
	java antlr.Tool -o $(gen_srcdir) $<
%Lexer.cpp : %Lexer.hpp
%Parser.cpp : %Parser.hpp
