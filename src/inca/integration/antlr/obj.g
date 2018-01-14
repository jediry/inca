/*
 * File: obj.g
 * 
 * Author: Ryan L. Saunders
 *
 * Copyright 2003, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      This file specifies an ANTLR grammar for the Alias/Wavefront OBJ
 *      file format. The parser actions delegate to a model handler, which
 *      does whatever it wants with the data as it is parsed from the file.
 */


// C++ Header section
header "pre_include_hpp" {
    // Forward declarations
    namespace Inca {
        namespace IO {
            class OBJLexer;
            class OBJParser;
        };
    };
}


// Global options section
options {
    language = cpp;             // Create C++ output
    namespace = "Inca::IO";     // Put classes into the Inca::IO subsystem
    namespaceStd = "std";       // Cosmetic option to make ANTLR-generated
    namespaceAntlr = "antlr";   // code more readable
    genHashLines = true;        // Generate #line thingies
}


/**
 * The OBJParser class processes tokens from an OBJ file.
 */
class OBJParser extends Parser;
options {
    exportVocab = OBJ;
    caseSensitive = false;
}

// Match any valid statement
statement: vertex3 | vertex4 ;

// Vertex data statements
vertex3: VERTEX x:SCALAR y:SCALAR z:SCALAR          EOL;
vertex4: VERTEX x:SCALAR y:SCALAR z:SCALAR w:SCALAR EOL;
normal3: NORMAL i:SCALAR j:SCALAR k:SCALAR          EOL;
normal4: NORMAL i:SCALAR j:SCALAR k:SCALAR w:SCALAR EOL;
texcoords2: TEXCOORDS u:SCALAR v:SCALAR                     EOL;
texcoords3: TEXCOORDS u:SCALAR v:SCALAR w:SCALAR            EOL;
texcoords4: TEXCOORDS u:SCALAR v:SCALAR w:SCALAR r:SCALAR   EOL;


/**
 * The OBJLexer class creates tokens from the characters in the OBJ file.
 */
class OBJLexer extends Lexer;
options {
    exportVocab = OBJ;
    caseSensitive = false;
    charVocabulary = '\3'..'\377';
}

// Primitive character classes
protected DIGIT options { paraphrase = "digit"; }            : ( '0'..'9' ) ;
protected UPPER options { paraphrase = "uppercase letter"; } : ( 'A'..'Z' ) ;
protected LOWER options { paraphrase = "uppercase letter"; } : ( 'a'..'z' ) ;
protected LETTER options { paraphrase = "letter"; } : ( UPPER | LOWER ) ;
EOL     : ( ( '\r' )? '\n' ( EOF )? ) | EOF ) { newline(); } ;

// Comments and whitespace
COMMENT : '#' ( ~EOL )* EOL
            { setType(Token.SKIP); } ;
WS      : ( ' ' | '\t' )
            { setType(Token.SKIP); } ;

// Data types
INTEGER options { paraphrase = "integer"; } : DIGIT+ ; 
SCALAR  options { paraphrase = "scalar"; }  : INTEGER ( '.' INTEGER )? ;
NAME    options { paraphrase = "name"; }
        : ( LETTER | '_' ) ( LETTER | DIGIT | '_' )* ;

// Statement declarators
VERTEX          : "v" ;
NORMAL          : "vn" ;
TEXTURE_COORDS  : "vt" ;
PS_VERTEX       : "vp" ;
CS_TYPE         : "cstype" ;
DEGREE          : "deg" ;
BASIS_MATRIX    : "bmat" ;
STEP_SIZE       : "step" ;
POINT           : "p" ;
LINE            : "l" ;
FACE            : "f" ;
CURVE2          : "curv2" ;
CURVE3          : "curv" ;
SURFACE         : "surf" ;
PARAM_VALUE     : "param" ;
OUTER_TRIM_LOOP : "trim" ;
INNER_TRIM_LOOP : "hole" ;
SPECIAL_CURVE   : "scrv" ;
SPECIAL_POINT   : "sp" ;
END             : "end" ;
CONNECTIVITY    : "con" ;
GROUP           : "g" ;
SMOOTHING_GROUP : "s" ;
MERGING_GROUP   : "mg" ;
OBJECT_NAME     : "o" ;
BEVEL_INTERP    : "bevel" ;
COLOR_INTERP    : "c_interp" ;
DISSOLVE_INTERP : "d_interp" ;
LOD             : "lod" ;
MATERIAL        : "usemtl" ;
MATERIAL_LIBRARY: "mtllib" ;
RAYTRACE_OBJECT : "trace_obj" ;
SHADOW_OBJECT   : "shadow_obj" ;
CURVE_APPROX    : "ctech" ;
SURFACE_APPROX  : "stech" ;
CALL_FILE       : "call" ;
SUBSTITUTE      : "scmp" ;
EXEC            : "csh" ;
