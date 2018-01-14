import re
import SCons.Node.FS

def CatalogFiles(self, path, ignorePatterns = [], verbose = False):
    catalog = { 'headers':  [], \
                'sources':  [], \
                'grammars': [],\
              }
    self.CatalogSubtree(path, catalog, ignorePatterns, verbose)
    return catalog


def CatalogSubtree(self, path, catalog, ignorePatterns, verbose):
    files = self.Glob(path + '/*')
    for f in files:
        # Recurse directories
        if isinstance(f, SCons.Node.FS.Dir):
            self.CatalogSubtree(str(f), catalog, ignorePatterns, verbose)
            continue

        fn = str(f)

        # Skip files that we don't want right now
        ignore = False
        for pattern in ignorePatterns:
            if re.match(pattern, fn):
                if verbose:
                    print "  Manually ignored:   " + str(f)
                ignore = True
                break
        if ignore:
            continue

        # Build files
        if fn == 'sconscript' \
        or fn.startswith('makefile') \
        or re.search(r'expanded.*\.g$', fn) \
        or fn.endswith('.o'):
            if verbose:
                print "  Build-related file: " + str(f)
            pass

        # C/C++ header files
        elif fn.endswith('.h') \
        or fn.endswith('.hpp') \
        or fn.endswith('.h++') \
        or fn.endswith('.hxx') \
        or fn.endswith('.hh'):
            if verbose:
                print "  C/C++ header file:  " + str(f)
            catalog['headers'].append(f)

        # C/C++ source files
        elif fn.endswith('.c') \
        or fn.endswith('.cpp') \
        or fn.endswith('.c++') \
        or fn.endswith('.cxx') \
        or fn.endswith('.cc'):
            if verbose:
                print "  C/C++ source file:  " + str(f)
            catalog['sources'].append(f)

        # Antlr grammar files
        elif fn.endswith('.g'):
            if verbose:
                print "  Antlr grammar file: " + str(f)
            catalog['grammars'].append(f)

        # Editor files
        elif fn.endswith('~') \
        or fn.startswith('#') \
        or fn.endswith('.swp'):
            if verbose:
                print "  Editor temp file:   " + str(f)
            pass

        # Files without extensions are probably C++ templates
        elif fn.find('.') == -1:
            if verbose:
                print "  C++ header file:    " + str(f)
            catalog['headers'].append(f)

        # Uhh...what's this?
        else:
            print "Ignoring unrecognized file:", fn

    return catalog


# Call this on your Environment to make 'env.CatalogFiles(srcdir)' work
def EnableSourceCatalog(env):
    env.AddMethod(CatalogSubtree)
    env.AddMethod(CatalogFiles)
