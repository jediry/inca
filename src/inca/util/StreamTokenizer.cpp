/*
 * Filename:        StreamTokenizer.cpp
 *
 * Author:          Adapted by Ryan L. Saunders <rls@tamu.edu>
 *                  from vvtokenizer, developed by
 *                  Juergen Schulze <schulze@hlrs.de>
 *                  of the University of Stuttgart, Supercomputing Center
 *
 * Comments:        Define ST_STANDALONE for demonstration mode
 */

// Include precompiled header
#include <inca/precomp.h>

// We need this for case conversion
#include <algorithm>
#include <iostream>
using namespace std;
struct upCase {
    char operator()(char c) {
        if (c >= 'a' && c <= 'z')
            return c - 'a' + 'A';
        else
            return c;
    }
};

struct downCase {
    char operator()(char c) {
        if (c >= 'A' && c <= 'Z')
            return c - 'A' + 'a';
        else
            return c;
    }
};

#include "StreamTokenizer.hpp"

const int StreamTokenizer::BLOCK_SIZE       = 4096;
const int StreamTokenizer::MAX_TOKEN_LENGTH = 1024;
const int StreamTokenizer::EOS              = -1;

//#define ST_STANDALONE      // uncomment for demonstration

//----------------------------------------------------------------------------
/**
 * Constructor for reading from a C-style FILE *.
 *
 * @param file pointer to a source file, which must have been opened for reading
 */
StreamTokenizer::StreamTokenizer(FILE *f) : file(f), stream(NULL) {
    // Initialize the rest of the tokenizer
    initialize();
}

// We're reading from a C++-style input file stream
/**
 * Constructor for reading from a C++ style istream.
 *
 * @param is pointer to an input stream, which must be open
 */
StreamTokenizer::StreamTokenizer(istream *is) : stream(is), file(NULL) {
    initialize();
}


/**
 * This method is called by the constructors, and is used to initialize the bulk
 * of the tokenizer's starting state.
 * The default state is:
 * <UL>
 *   <LI> Case-conversion is disabled </LI>
 *   <LI> C and C++ style comments are not treated specially </LI>
 *   <LI> EOL is not significant </LI>
 *   <LI> Numbers are not parsed </LI>
 * </UL>
 */
void StreamTokenizer::initialize() {
    // Memory allocation
    wordToken = new char[MAX_TOKEN_LENGTH + 1];
    data = new char[BLOCK_SIZE];

    // Attribute initialization
    ttype       = TOKEN_TYPE_NONE;
    numberToken = 0.0f;
    wordToken   = "";
    pushedBack  = false;
    line        = 1;
    caseConversion              = CASE_CONVERT_NONE;
    eolIsSignificant            = false;
    ignoringSlashStarComments   = false;
    ignoringSlashSlashComments  = false;
    blockUsed = 0;
    cursor = 0;

    setDefault();  // disable number parsing
}


/** Destructor */
StreamTokenizer::~StreamTokenizer() {
    delete[] data;
}


//----------------------------------------------------------------------------
/**
 * Read a character from the file buffer. '\n\r' sequences will be converted
 * to single newline characters, '\n'.
 *
 * @return the character that was read, or EOS if there are no more characters
 */
int StreamTokenizer::readChar() {
    static unsigned char previousChar = '\0';
    if (cursor >= blockUsed) {     // end of buffer reached?
        if (file != NULL) {       // ...then we're using a FILE *
            if (feof(file)) {
                return EOS;
            } else {
                // Read next block:
                blockUsed = fread(data, 1, BLOCK_SIZE, file);
                cursor = 0;
            }
        } else {                    // ...then we're using an istream
            if (stream->eof()) {
                return EOS;
            } else {
                // Read next block:
                stream->read(data, BLOCK_SIZE);
                blockUsed = stream->gcount();
                cursor = 0;
            }
        }
    }

    unsigned char c = data[cursor++];
//    cerr << "Char is '" << int(c) << "'\n";
    if (c == '\r') {                            // Return '\n' in place of '\r'
//        cerr << "Converting \\r to \\n\n";
        previousChar = c;
        return '\n';
    } else if (c == '\n' && previousChar == '\r') { // Skip a redundant '\r\n'
//        cerr << "Skipping redundant \\n\n";
        return readChar();  // Get another one...this wasn't good enough!
    } else {                                        // Normal character
        previousChar = c;
        return c;
    }
}


/**
 * Read a character from the file buffer, but filter out any comments.
 * If C/C++ style comments are being ignored, the next character to be 
 * returned will be the first character following the termination of the
 * comment (which includes the EOL that terminates the '//' style comment.
 * Also, any user-defined comment characters will be similarly ignored,
 * up to (but not including) EOL.
 *
 * @return the character that was read, or EOS if there are no more characters
 */
int StreamTokenizer::readFilteredChar() {
    char c = readChar();

    // Ignore C++ '//' comments
    if (ignoringSlashSlashComments && c == '/') {
        // We need to check the next char for another '/'
        char nextChar = readChar();
        if (nextChar == '/') {
            c = skipToEOL();
        } else {
            // Push back the character...it wasn't a comment
            cursor--;
        }
    }

    // Ignore C '/* */ comments
    if (ignoringSlashStarComments && c == '/') {
        // We need to check the next char for '*'
        char nextChar = readChar();
        if (nextChar == '*') {
            // Ignore to '*/'
            while (!(c == '*' && nextChar == '/') && c >= 0) {
                c = nextChar;
                nextChar = readChar();
            }
            // If we found the ending '*/', read another character
            if (c >= 0)
                c = readChar();
        } else {
            // Push back the character...it wasn't a comment
            cursor--;
        }
    }

    // Ignore any other comment characters
    if (charTypes[c] == CHAR_TYPE_COMMENT)
        c = skipToEOL();

    return c;
}

int StreamTokenizer::skipToEOL() {
    int c;
    // Ignore to EOL
    while ((c = readChar()) != '\n' && c >= 0) /* skip */;
    return c;
}


//----------------------------------------------------------------------------
/**
 * Interpret all displayable characters as alpha characters and the rest
 * of ASCII codes as whitespace characters. Displayable characters are ASCII
 * codes 33..126 and 192..255. 
 * This method reverses the effects of setCommentCharacter, 
 * setWhitespaceCharacter, setParseNumbers and setAlphaCharacters.
 */
void StreamTokenizer::setDefault() {
    int i;

    for (i=0; i<33; ++i)
        charTypes[i] = CHAR_TYPE_WHITESPACE;
    for (i=33; i<127; ++i)
        charTypes[i] = CHAR_TYPE_ALPHA;
    for (i=127; i<192; ++i)
        charTypes[i] = CHAR_TYPE_WHITESPACE;
    for (i=192; i<256; ++i)
        charTypes[i] = CHAR_TYPE_ALPHA;
}


//----------------------------------------------------------------------------
/**
 * Set the line number.
 * The tokenizer uses line number 1 as default for the first line when 
 * instantiated, so this function only needs to be called when the file
 * pointer was not set to the beginning of the file.
 *
 * @param lineNumber new line number to be used as current line
 */
void StreamTokenizer::setLineNumber(int lineNumber) {
    line = lineNumber;
}


/**
 * Get the line number. The first line of the file is line 1.
 *
 * @return the current line number [1..numLines]
 */
int StreamTokenizer::getLineNumber() {
    return line;
}


//----------------------------------------------------------------------------
/**
 * Specifies that a character is an alphabetic character.
 * Multiple characters can be defined as alpha characters.
 * This operation can only be reversed by @see setAllAlpha.
 *
 * @param cc alphabetic character (e.g. 'A')
 */
void StreamTokenizer::setAlphaCharacter(char cc) {
    charTypes[cc] = CHAR_TYPE_ALPHA;
}


/**
 * Set a comment character. 
 * Multiple characters can be defined as comment characters.
 * This operation can only be reversed by @see setDefault.
 *
 * @param cc comment character (e.g. '#' for UNIX style comments)
 */
void StreamTokenizer::setCommentCharacter(char cc) {
    charTypes[cc] = CHAR_TYPE_COMMENT;
}


/** 
 * Set a delimiter character, which will break up word and number tokens, but
 * unlike whitespace characters, will be returned as a token itself.
 * Multiple characters can be defined as delimiter characters.
 * This operation can only be reversed by @see setDefault.
 *
 * @param dc delimiter character (e.g. '=' to split around equal signs)
 */
void StreamTokenizer::setDelimiterCharacter(char dc) {
    charTypes[dc] = CHAR_TYPE_DELIMITER;
}


/** 
 * Set a whitespace character, which will be ignored in the parsing process. 
 * Multiple characters can be defined as whitespace characters.
 * This operation can only be reversed by @see setDefault.
 *
 * @param wc whitespace character (e.g. '=' to ignore all equal signs)
 */
void StreamTokenizer::setWhitespaceCharacter(char wc) {
    charTypes[wc] = CHAR_TYPE_WHITESPACE;
}


//----------------------------------------------------------------------------
/**
 * Specifies whether numbers should be parsed by this tokenizer.
 * When true and parser encounters a word token that has the format of an
 * integer or floating-point number, it treats the token as a 
 * number rather than a word, by setting the the <CODE>ttype</CODE> 
 * field to the value <CODE>TT_NUMBER</CODE> and putting the numeric 
 * value of the token into the <CODE>number</CODE> field.
 *
 * @param pn if true, '0'..'9', '.' and '-' are considered digits, 
 *          if false, the above are considered alphas
 */
void StreamTokenizer::setParseNumbers(bool pn) {
    for (int i='0'; i<='9'; ++i)
        charTypes[i]    = pn ? CHAR_TYPE_DIGIT : CHAR_TYPE_ALPHA;
    charTypes['.']      = pn ? CHAR_TYPE_DIGIT : CHAR_TYPE_ALPHA;
    charTypes['-']      = pn ? CHAR_TYPE_DIGIT : CHAR_TYPE_ALPHA;
}


//----------------------------------------------------------------------------
/**
 * Determines whether or not ends of line are treated as tokens.
 * If the flag argument is true, this tokenizer treats end of lines 
 * as tokens; the <CODE>nextToken</CODE> method returns 
 * <CODE>TT_EOL</CODE> and also sets the <CODE>ttype</CODE> field to 
 * this value when an end of line is read.<P>
 * A line is a sequence of characters ending with either a 
 * carriage-return character (\r) or a newline character ('\n'). 
 * In addition, a carriage-return character followed immediately 
 * by a newline character is treated as a single end-of-line token.<P>
 * If the flag is false, end-of-line characters are 
 * treated as white space and serve only to separate tokens. 
 *
 * @param eol true indicates that end-of-line characters
 *            are separate tokens, false indicates that
 *            end-of-line characters are white space.
 */
void StreamTokenizer::setEOLisSignificant(bool eol) {
    eolIsSignificant = eol;
}


//----------------------------------------------------------------------------
/**
 * Determines if C style comments will be ignored. If set to true, all
 * characters following the character combination '/*' will be ignored, up to
 * the character combination '* /' (ignore the space).
 * @param ssc whether or not to use // comments
 */
void StreamTokenizer::setIgnoreSlashStarComments(bool ssc) {
    ignoringSlashStarComments = ssc;
}


//----------------------------------------------------------------------------
/**
 * Determines if C++ style comments will be ignored. If set to true, any
 * portion of a line following the characters '//' will be ignored, up to EOL
 * @param ssc whether or not to use // comments
 */
void StreamTokenizer::setIgnoreSlashSlashComments(bool ssc) {
    ignoringSlashSlashComments = ssc;
}


//----------------------------------------------------------------------------
/**
 * Determines if word tokens are converted to upper or lowercase.
 * @param cc force word tokens to be converted to given case
 * @see CaseType
 */
void StreamTokenizer::setCaseConversion(CaseType cc) {
    caseConversion = cc;
}


//----------------------------------------------------------------------------
/**
 * Causes the next call to the <CODE>nextToken</CODE> method of this 
 * tokenizer to return the current value in the <CODE>ttype</CODE> 
 * field, and not to modify the value in the <CODE>number</CODE> or 
 * <CODE>word</CODE> field.
 */
void StreamTokenizer::pushBack() {
    if (ttype != TOKEN_TYPE_NONE)   // no-op if nextToken() not called
        pushedBack = true;
}


//----------------------------------------------------------------------------
/**
 * Parses the next token from the input file of this tokenizer. 
 * The type of the next token is returned in the <CODE>ttype</CODE> 
 * field. Additional information about the token may be in the 
 * <CODE>number</CODE> field or the <CODE>word</CODE> field of this 
 * tokenizer.<P>
 * Typical clients of this class first set up the syntax tables 
 * and then sit in a loop calling nextToken to parse successive 
 * tokens until TT_EOF is returned. 
 *
 * @return the value of the <CODE>ttype</CODE> field
 */
StreamTokenizer::TokenType StreamTokenizer::nextToken() {
    int   c;            // read character, or -1 for EOF
    int   charType;

    if (pushedBack) {   // if token was pushed back, re-use the last read token
        pushedBack = false;
        return ttype;
    }

    // Initialization:
    wordToken = "";
    numberToken = 0.0f;

    if (ttype == TOKEN_TYPE_NONE) {    // is this the first pass?
        c = readFilteredChar();
    } else {
        c = peekChar;
    }

    if (c < 0 || c > 255)
        return ttype = TOKEN_TYPE_EOF;

    charType = charTypes[c];      // look up character properties
    
    // Parse whitespace:
    while (charType == CHAR_TYPE_WHITESPACE) {
        if (c == '\n') {
            line++;
            if (eolIsSignificant) {
                peekChar = readFilteredChar();
                return ttype = TOKEN_TYPE_EOL;
            }
        }
        c = readFilteredChar();

        if (c<0 || c>255)
            return ttype = TOKEN_TYPE_EOF;
        charType = charTypes[c];
    }

    // If this is a delimiter, we can return it right now
    if (charType == CHAR_TYPE_DELIMITER) {
        delimiterToken = c;
        peekChar = readFilteredChar();
        return ttype = TOKEN_TYPE_DELIMITER;
    }

    // Parse out a word token:
    while ((charType == CHAR_TYPE_DIGIT || charType == CHAR_TYPE_ALPHA)) {
        wordToken.append(1, (char)c);
        c = readFilteredChar();
        charType = charTypes[c];
    }
    peekChar = c;
    if ((charTypes[wordToken[0]] == CHAR_TYPE_DIGIT) != 0) { // is token a number?
        numberToken = (float)atof(wordToken.c_str());
        return ttype = TOKEN_TYPE_NUMBER;
    } else {    // word token
        switch (caseConversion) {
            case CASE_CONVERT_TO_UPPER:
                transform(wordToken.begin(), wordToken.end(), wordToken.begin(), upCase());
                break;
            case CASE_CONVERT_TO_LOWER:
                transform(wordToken.begin(), wordToken.end(), wordToken.begin(), downCase());
                break;
        }
        return ttype = TOKEN_TYPE_WORD;
    }
}


//============================================================================
// Functions for STANDALONE mode
//============================================================================

#ifdef ST_STANDALONE

#include <iostream>
#include <fstream>

int main(int argc, char** argv) {
    StreamTokenizer* tokenizer;         // ASCII file tokenizer
    StreamTokenizer::TokenType ttype;   // currently processed token type
    ifstream instream;
    bool done;          
   
    if (argc!=2) {
        cout << "Tokenizer Test. Syntax: tokentext <filename.txt>" << endl;
        return(0);
    }

    //if ( (infile = fopen(argv[1], "rb")) == NULL) {
    instream.open(argv[1]);
    if (!instream.is_open()) {
        cout << "Error: Cannot open input file." << endl;
        return(0);
    }
    
    // Read file data:
    tokenizer = new StreamTokenizer(&instream);
    tokenizer->setCommentCharacter('#');
    tokenizer->setDelimiterCharacter('/');
    tokenizer->setEOLisSignificant(false);
    tokenizer->setCaseConversion(StreamTokenizer::CASE_CONVERT_TO_UPPER);
    tokenizer->setIgnoreSlashSlashComments(true);
    tokenizer->setIgnoreSlashStarComments(true);
    tokenizer->setParseNumbers(true);
    tokenizer->setWhitespaceCharacter('=');
    done = false;
    while (!done) {
        // Read a token:
        ttype = tokenizer->nextToken();
        switch (ttype) {
        case StreamTokenizer::TOKEN_TYPE_WORD:
            cout << "Line " << tokenizer->getLineNumber() << 
                ": Found word token: " << tokenizer->wordToken << endl;      
            break;
        case StreamTokenizer::TOKEN_TYPE_NUMBER:
            cout << "Line " << tokenizer->getLineNumber() << 
                ": Found number token: " << tokenizer->numberToken << endl;
            break;
        case StreamTokenizer::TOKEN_TYPE_DELIMITER:
            cout << "Line " << tokenizer->getLineNumber() << 
                ": Found delimiter token: " << tokenizer->delimiterToken << endl;
            break;
        default:
            done = true;
            break;
        }
    }

    // Clean up:
    delete tokenizer;
    instream.close();
//    fclose(infile);
    return 0;
}

#endif
