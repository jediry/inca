/*
 * Filename:        StreamTokenizer.hpp
 *
 * Author:          Adapted by Ryan L. Saunders <rls@tamu.edu>
 *                  from StreamTokenizer, developed by
 *                  Juergen Schulze <schulze@hlrs.de>
 *                  of the University of Stuttgart, Supercomputing Center
 */

#ifndef INCA_STREAM_TOKENIZER
#define INCA_STREAM_TOKENIZER

#include <stdio.h>
#include <istream>
#include <string>
using namespace std;

/**
 * The StreamTokenizer class takes an input file and parses it into "tokens"
 * allowing the tokens to be read one at a time. It is very similar to the
 * Java class of the same name. The parsing process is controlled by a
 * number of flags that can be set to various states. The 
 * tokenizer can recognize identifiers, numbers, and comments.<P>
 *
 * In addition, an instance has several flags. These flags indicate: 
 * <UL>
 * <LI>Whether line terminators are to be returned as tokens or treated 
 *      as white space that merely separates tokens. </LI>
 * <LI>Whether comments are to be recognized and skipped. </LI>
 * <LI>Whether the characters of identifiers are to be converted to upper
 *      or lowercase. </LI>
 * <LI>Whether numbers are to be parsed or returned as strings.
 * </UL><P>
 * 
 * A typical application first constructs an instance of this class, 
 * sets up the flags, and then repeatedly loops calling the 
 * <CODE>nextToken</CODE> method in each iteration of the loop until 
 * it returns the value <CODE>TT_EOF</CODE>. <P>
 *
 * The default character properties are:<UL>
 * <LI>ASCII codes 0 to 32 and 127 to 191 are whitespace </LI>
 * <LI>All other ASCII codes are alpha characters </LI></UL>
 *
 * Here is an example code snippet which uses the StreamTokenizer class
 * to search for the word "FOUND" in the file "testfile.txt" ignoring
 * comments starting with '#':
 * <PRE>
 * StreamTokenizer::TokenType ttype;
 * FILE* fp = fopen("testfile.txt", "rb");
 * StreamTokenizer* tokenizer = new StreamTokenizer(fp);
 * tokenizer->setCommentCharacter('#');
 * tokenizer->setEOLisSignificant(false);
 * tokenizer->setCaseConversion(StreamTokenizer::CASE_CONVERT_UPPER);
 * while ((ttype = tokenizer->nextToken()) != StreamTokenizer::TOKEN_TYPE_EOF)
 *   if (strcmp(tokenizer->sval, "FOUND")==0)
 *     break;    
 * delete tokenizer;
 * fclose(fp);
 * </PRE>
 *
 * @author Juergen Schulze
 * @author Ryan L. Saunders
 */
class StreamTokenizer {
public:
    enum TokenType {        // Enumeration of token types, returned from nextToken()
        TOKEN_TYPE_EOF,         // end of file has been read
        TOKEN_TYPE_EOL,         // end of line has been read
        TOKEN_TYPE_DELIMITER,   // a delimiter token has been read
        TOKEN_TYPE_NUMBER,      // a number token has been read
        TOKEN_TYPE_WORD,        // a word token has been read
        TOKEN_TYPE_NONE,        // no token has been read (used for initialization)
    };

    enum CaseType {         // attributes for setCaseConversion()
        CASE_CONVERT_NONE,      // no case conversion
        CASE_CONVERT_TO_UPPER,  // force uppercase letters
        CASE_CONVERT_TO_LOWER,  // force lowercase letters
    };

private:
    enum CharacterType {    // Character classes (any char can be assigned to any class)
        CHAR_TYPE_ALPHA,        // The character is an ordinary char
        CHAR_TYPE_DIGIT,        // The character is a numeric value
        CHAR_TYPE_DELIMITER,    // The character is a non-whitespace delimiter
        CHAR_TYPE_WHITESPACE,   // The character is a whitespace character
        CHAR_TYPE_COMMENT,      // The character starts a comment (until EOL)
    };

    static const int BLOCK_SIZE;        // data block size to be read at once [bytes]
    static const int MAX_TOKEN_LENGTH;  // maximum token length [characters]
    static const int EOS;               // End-of-stream sentinel value

    istream *stream;                    // Input stream
    FILE *file;                         // Input file
    int   blockUsed;                    // number of data block bytes used
    char *data;                         // raw data read from input file
    int   cursor;                       // index of current data byte
    int   peekChar;                     // next character in input file
    bool  pushedBack;                   // true to not read the same token again
    int   line;                         // line number of last token read
    CaseType caseConversion;            // case type to convert alpha values to
    bool  eolIsSignificant;             // true if EOL is returned as a token
    bool  ignoringSlashStarComments;    // true if C++ '//' comments are skipped
    bool  ignoringSlashSlashComments;   // true if C '/* */' comments are skipped
    CharacterType charTypes[256];       // character type list

    int readChar();         // Reads the next character from the input
    int readFilteredChar(); // Reads the next non-comment character from the input
    void initialize();      // Initializes the starting state of the tokenizer

  public:
    TokenType ttype;  ///< After a call to the <CODE>nextToken</CODE> method, this field 
                      ///< contains the type of the token just read. 
                      ///< Its value is one of the following: 
                      ///< <UL>
                      ///< <LI><CODE>TT_WORD</CODE> indicates that the token is a word.
                      ///< <LI><CODE>TT_NUMBER</CODE> indicates that the token is a number.
                      ///< <LI><CODE>TT_EOL</CODE> indicates that the end of line has been read. 
                      ///<     The field can only have this value if the 
                      ///<     <CODE>eolIsSignificant</CODE> method has been called with the 
                      ///<     argument <CODE>true</CODE>. 
                      ///< <LI><CODE>TT_EOF</CODE> indicates that the end of the input file 
                      ///<     has been reached. 
                      ///< </UL>

    string wordToken;      ///< If the current token is a word or number token token, 
                      ///< this field contains a string giving the characters of 
                      ///< the word or number token.
                      ///< The current token is a word when the value of the 
                      ///< <CODE>ttype</CODE> field is <CODE>TT_WORD</CODE>,
                      ///< it is a number if the value is <CODE>TT_NUMBER</CODE>,

    float numberToken;     ///< If the current token is a number, this field contains the value 
                      ///< of that number. The current token is a number when the value of 
                      ///< the <CODE>ttype</CODE> field is <CODE>TT_NUMBER</CODE>. 

    char delimiterToken;

    StreamTokenizer(FILE *);
    StreamTokenizer(istream *);
    ~StreamTokenizer();
    void setDefault();
    void setLineNumber(int);
    int  getLineNumber();
    void setAlphaCharacter(char);
    void setCommentCharacter(char);
    void setDelimiterCharacter(char);
    void setWhitespaceCharacter(char);
    void setParseNumbers(bool);
    void setEOLisSignificant(bool);
    void setIgnoreSlashSlashComments(bool);
    void setIgnoreSlashStarComments(bool);
    void setCaseConversion(CaseType);
    void pushBack();
    TokenType nextToken();
    int skipToEOL();       // Skips everything to (not including) EOL or EOF
};

#endif
