<?xml version="1.0" encoding="UTF-8"?>

<!-- This XSL transform stylesheet translates the Schema definition for
     Inca data files into C++ serialization/deserialization code for
     converting between the in-memory DOM tree and the Inca object model.
     In order to do this, it demands that declarations in the Schema document 
     contain some additional attributes (belonging to the Inca scene namespace:
     http://halogyn.com/inca/scene):
     
     1) Each <xsd:attribute> must have the "name" attribute, as well 
        as the following additional attributes:

            cpp:identifier : a string giving the name by which to #define the
                attribute's name for C++

            cpp:property : a string giving the method to invoke on the parent
                object in order to add this value

            cpp:type : the C++ type to which the value should be cast before
                it is set on the parent

      2) Each <xsd:element> that is not a reference element (containing the
         "ref" attribute) must have the following additional attributes:

            cpp:identifier : similar to that of <xsd:attribute> (above)

            type : the C++ object type that this element deserializes as

      3) Each <xsd:element> that is a reference element must additionally
         have the following element:
XXX This discussion is a bit wrong.
            property : the name of the parent object's method that
                corresponds to this element. How this is handled depends on
                whether the element is unique or not (for the purpose of the
                following examples, the value of method is taken to be
                "Object"). In the case of a unique element, a call is made to
                "getObject()". In the case of a recurring element, calls are
                made to "createObject()".

      4) The values of the "identifier" attributes must be unique throughout
         the schema. If elements share child elements, these children should
         be defined once and subsequently ref'd. If elements share attributes,
         these should be declared once as an attributeGroup, which should
         be subsequently ref'd.
     
    Possible points of confusion:
        Admittedly, this XSLT stylesheet reads almost as easily as a Yagarian
        personal ad. This is somewhat due to the verbosity and event-driven
        nature of the XSLT language, but is further complicated by the
        inclusion of several concepts common to both the XML Schema and the
        Inca XML documents and corresponding C++ code. These are:

        1) References: the Schema langauge supports the idea of "references",
           indicated by the 'ref' attribute. The value of the 'ref' attribute
           supplies the 'name' of the element that should be inserted at that
           place in the Schema.

           The Inca XML documents also contain the concept of "references", 
           also specified with the 'ref' attribute. In this case, references 
           are dual-valued, the general form of which is:

              filename#id

           "filename" gives the URI of the file containing the referenced
           object, and "id" gives the unique id of the exact object within
           that file. If "filename" is absent, the current file is assumed.
           If "id" is absent, the object to be read is presumed to be the
           entire file (this is only valid with objects that can constitute
           an entire file (a PolygonMesh from an OBJ file, for example)).
           Note that a reference that is internal to the current file should
           look like this:

              #id

           which will be treated identically to this:

              thisfile#id

            but does not require the current filename to be hard-coded within
            the file (a bad idea, in most cases).
-->
<stylesheet xmlns="http://www.w3.org/1999/XSL/Transform"
            xmlns:xsd="http://www.w3.org/2001/XMLSchema"
            xmlns:cpp="http://halogyn.com/inca/c++"
            xmlns:redirect="http://xml.apache.org/xalan/redirect"
            xmlns:lxslt="http://xml.apache.org/xslt"
            xmlns:js="http://www.halogyn.com/XSL/JSExtension"
            extension-element-prefixes="redirect js"
            exclude-result-prefixes="js"
            version="1.0">
  <output method="text"/>


<!-- ====================================================================== -->
<!-- ===================== Parameters ===================================== -->
<!-- ====================================================================== -->
  <!-- These parameters should be set from the command-line, and tell us
       what our output filenames should be. -->
  <param name="ident-decl"/>
  <param name="ident-impl"/>
  <param name="ser-decl"/>
  <param name="ser-impl"/>
  <param name="source"/>


<!-- ====================================================================== -->
<!-- ===================== Configuration ================================== -->
<!-- ====================================================================== -->
  <!-- These variables are used to create the header comments in the
       generated files. The format is the same as used by other code in the
       Inca project. -->
  <variable name="transformer"  select="'schema-to-c++.xsl'"/>
  <variable name="author"       select="'Ryan L. Saunders'"/>
  <variable name="year"         select="'2003'"/>
  

<!-- ====================================================================== -->
<!-- ===================== Function Definitions =========================== -->
<!-- ====================================================================== -->
  <!-- These are the extension functions (implemented in JavaScript) that
       are needed in order to properly handle multiple occurances of
       C++ data-types. As some data-types (e.g., Point3D) correspond to
       more than one XML element, we need to be able to ensure that

          1) only one (de)serializer is created
      and
          2) it is created with tag-name checking disabled (as elements
             with different names must all use the same deserializer).
  -->
  <lxslt:component prefix="js" functions="wroteIdentDecl wroteIdentImpl
                                          wroteResolvProto wroteResolvImpl
                                          wroteSerProto wroteSerImpl
                                          wroteDeserProto wroteDeserImpl">
    <lxslt:script lang="javascript">
      // This functions prints a message to stdout. It is preferable to the
      // "message" facility provided by XSLT, because it doesn't barf up all
      // that unnecessary text.
      function printMessage(str) {
          java.lang.System.out.println(str);
          return false;
      }


      // These functions construct the unique identifiers used as protection
      // against multiple #inclusion of the resulting .hpp files by the
      // C preprocessor. The string argument 'source' is expected to be of the
      // form "path/to/schema/XXX-schema.xsd", where XXX is what we want to
      // use to make our identifiers.
      function makeIdentifiersIncludeProtection(source) {
          var start = source.lastIndexOf('/') + 1;
          var end = source.lastIndexOf('-') - start;
          var tmp = source.substr(start, end);
          return "IO_" + tmp.toUpperCase() + "_IDENTIFIERS";
      }

      function makeSerializationIncludeProtection(source) {
          var start = source.lastIndexOf('/') + 1;
          var end = source.lastIndexOf('-') - start;
          var tmp = source.substr(start, end);
          return "IO_" + tmp.toUpperCase() + "_SERIALIZATION";
      }


      // These functions are used to record in how many "contexts" a C++ "type"
      // may occur. The "context" is the XML element name, and the "type" is
      // the value of the "cpp:type" attribute attached to it. If a type occurs
      // in multiple contexts (e.g., Point3D in "locationPoint", then again in
      // "lookAtPoint"). If a type occurs in multiple contexts, the
      // 'isMultiContext' return true.
      function sawTypeInContext(type, context) {
          var lastContext = typeContext.get(type);
          if (lastContext == null) {              // This is new
              typeContext.put(type, context);
              java.lang.System.out.println("Saw " + type + " as " + context);
          } else if (lastContext != context) {    // Multi-context
              typeContext.put(type, "#multiple");
              java.lang.System.out.println(type + " is multi-context");
          }
          return false;
      }

      function isMultiContext(type) {
          return typeContext.get(type) == "#multiple";
      }

      // The hashtable that stores context information
      var typeContext = new java.util.Hashtable();
      

      // These functions are used to check whether or not a given string value
      // has already been processed in a certain way. The testAndSet function
      // does most of the work on behalf of the other functions. After
      // incrementing the hash's count of that string, it returns true if this
      // string's been seen before, and false if this is its first time.
      function testAndSet(str, hash) {
          var count = hash.get(str);

          if (count == null || count == 0)    // The first one!
              count = 0;

          count++;    // Increment the count
          hash.put(str, count);

          return (count > 1);
      }

      function wroteIdentDecl(id) { return testAndSet(id, idDecl); }
      function wroteIdentImpl(id) { return testAndSet(id, idImpl); }
      function wroteResolvProto(type) { return testAndSet(type, resolvProto); }
      function wroteResolvImpl(type) { return testAndSet(type, resolvImpl); }
      function wroteSerProto(type) { return testAndSet(type, serProto); }
      function wroteSerImpl(type) { return testAndSet(type, serImpl); }
      function wroteDeserProto(type) { return testAndSet(type, deserProto); }
      function wroteDeserImpl(type) { return testAndSet(type, deserImpl); }

      // Hashtables for the above functions
      var idDecl = new java.util.Hashtable();
      var idImpl = new java.util.Hashtable();
      var resolvProto = new java.util.Hashtable();
      var resolvImpl = new java.util.Hashtable();
      var serProto = new java.util.Hashtable();
      var serImpl = new java.util.Hashtable();
      var deserProto = new java.util.Hashtable();
      var deserImpl = new java.util.Hashtable();

    </lxslt:script>
  </lxslt:component>


<!-- ====================================================================== -->
<!-- ===================== Template Definitions =========================== -->
<!-- ====================================================================== -->
  <!-- The schema element is the root element of the document. We will use
       it as our hook to attach our XML identifier declarations. -->
  <template match="xsd:schema">

    <!-- Create the output files we'll use. -->
    <redirect:open select="$ident-decl"/>
    <redirect:open select="$ident-impl"/>
    <redirect:open select="$ser-decl"/>
    <redirect:open select="$ser-impl"/>


    <if test="js:printMessage('    Writing boilerplate text')"/>
    <!-- ================================================================ -->
    <!-- Create boilerplate content in the $ident-decl file               -->
    <!-- ================================================================ -->
    <redirect:write select="$ident-decl">
      <!-- Write headers comments -->
      <call-template name="write-header">
        <with-param name="filename"     select="$ident-decl"/>
        <with-param name="author"       select="$author"/>
        <with-param name="year"         select="$year"/>
        <with-param name="description"
          select="concat('This file was automatically generated from ',
            $source, ' and ', $transformer, '. It contains identifier ',
            'declarations for the various XML tags that may occur in an ',
            'XML document conforming to ', $source, '.')"/>
      </call-template>
      <variable name="identifiers-include-protection"
                select="js:makeIdentifiersIncludeProtection($source)"/>
#ifndef <value-of select="$identifiers-include-protection"/>
#define <value-of select="$identifiers-include-protection"/>

// Import system configuration
#include &lt;config.h&gt;

// Import our XML utilities
#include &lt;io/XMLUtilities.hpp&gt;

// Import our Unicode string definition
#include &lt;util/ustring.hpp&gt;
    </redirect:write>

    <!-- ================================================================ -->
    <!-- Create boilerplate content in the $ser-decl file                 -->
    <!-- ================================================================ -->
    <redirect:write select="$ser-decl">
      <!-- Write headers comments -->
      <call-template name="write-header">
        <with-param name="filename"     select="$ser-decl"/>
        <with-param name="author"       select="$author"/>
        <with-param name="year"         select="$year"/>
        <with-param name="description"
          select="concat('This file was automatically generated from ',
            $source, ' and ', $transformer, '. It contains function ',
            'prototypes for converting between the XML-DOM representation ',
            'of objects and the corresponding Inca runtime object model.')"/>
      </call-template>
      <variable name="serialization-include-protection"
                select="js:makeSerializationIncludeProtection($source)"/>
#ifndef <value-of select="$serialization-include-protection"/>
#define <value-of select="$serialization-include-protection"/>

// Include scene object model definitions
#include &lt;scene/Scene.hpp&gt;

// Include our XML utilities
#include &lt;io/XMLUtilities.hpp&gt;

// Include the file-related exceptions
#include &lt;io/FileExceptions.hpp&gt;
    </redirect:write>

    <!-- ================================================================ -->
    <!-- Create boilerplate content in the $ident-impl file               -->
    <!-- ================================================================ -->
    <redirect:write select="$ident-impl">
      <call-template name="write-header">
        <with-param name="filename"     select="$ident-impl"/>
        <with-param name="author"       select="$author"/>
        <with-param name="year"         select="$year"/>
        <with-param name="description"
          select="concat('This file was automatically generated from ',
            $source, ' and ', $transformer, '. It contains definitions of the ',
            'identifiers declared in ', $ident-decl, '.')"/>
      </call-template>

// Include identifier declarations
#include &lt;<value-of select="$ident-decl"/>&gt;
    </redirect:write>

    <!-- ================================================================ -->
    <!-- Create boilerplate content in the $ser-impl file                 -->
    <!-- ================================================================ -->
    <redirect:write select="$ser-impl">
      <call-template name="write-header">
        <with-param name="filename"     select="$ser-impl"/>
        <with-param name="author"       select="$author"/>
        <with-param name="year"         select="$year"/>
        <with-param name="description"
          select="concat('This file was automatically generated from ',
            $source, ' and ', $transformer, '. It contains implementations ',
            'of the serialization functions prototyped in ', $ser-decl, '.')"/>
      </call-template>

// Include operator prototypes
#include "<value-of select="$ser-decl"/>"
    </redirect:write>

    <!-- Catalog (and validate) our non-reference element nodes -->
    <if test="js:printMessage('    Examining elements')"/>
    <for-each select="//xsd:element">
      <call-template name="validate-element"/>
      <redirect:write select="$ident-decl">
        <call-template name="write-identifier-declaration"/>
      </redirect:write>
      <redirect:write select="$ident-impl">
        <call-template name="write-identifier-implementation"/>
      </redirect:write>
    </for-each>

    <!-- Catalog (and validate) our attribute nodes -->
    <if test="js:printMessage('    Examining attributes')"/>
    <for-each select="//xsd:attribute">
      <call-template name="validate-attribute"/>
      <redirect:write select="$ident-decl">
        <call-template name="write-identifier-declaration"/>
      </redirect:write>
      <redirect:write select="$ident-impl">
        <call-template name="write-identifier-implementation"/>
      </redirect:write>
    </for-each>

    <!-- ================================================================ -->
    <!-- Referenceable elements need a reference cache and a resolver     -->
    <!-- ================================================================ -->
    <!--if test="js:printMessage('    Creating reference resolvers')"/>
    <for-each select="//xsd:element">
      <if test="xsd:complexType/xsd:attribute[@name='ref']">
        <variable name="type" select="xsd:complexType/@cpp:type"/>
        <redirect:write select="$h-file">
          <call-template name="write-resolver-prototype"/>
        </redirect:write>
        <redirect:write select="$c-file">
          <call-template name="write-resolver-implementation"/>
        </redirect:write>
      </if>
    </for-each-->

    <!-- ================================================================ -->
    <!-- Each element type needs a serializer and deserializer            -->
    <!-- ================================================================ -->
    <if test="js:printMessage('    Creating (de)serializers')"/>
    <for-each select="//xsd:element[@name]">
      <redirect:write select="$ser-decl">
        <call-template name="write-serializer-prototype"/>
        <call-template name="write-deserializer-prototype"/>
      </redirect:write>

      <redirect:write select="$ser-impl">
        <call-template name="write-serializer-implementation"/>
        <call-template name="write-deserializer-implementation"/>
      </redirect:write>
    </for-each>


    <!-- Write footer information to the files -->
    <redirect:write select="$ident-decl">
#endif
    </redirect:write>

    <redirect:write select="$ser-decl">
#endif
    </redirect:write>

    <!-- Close the output files -->
    <redirect:close select="$ident-decl"/>
    <redirect:close select="$ident-impl"/>
    <redirect:close select="$ser-decl"/>
    <redirect:close select="$ser-impl"/>

  </template>


  <!-- ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ -->
  <!-- This named template is called to format the header comments at   -->
  <!-- the beginning of a C++ source file                               -->
  <!-- ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ -->
  <template name="write-header">
    <param name="filename"/>
    <param name="author"/>
    <param name="year"/>
    <param name="description"/>
/*
 * File: <value-of select="$filename"/>
 *
 * Author: <value-of select="$author"/>
 *
 * Copyright <value-of select="concat($year, ', ', $author)"/>. All rights reserved.
 *
 * Description:
 * <value-of select="$description"/>
 */

  </template>


  <!-- ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ -->
  <!-- This named template is called to ensure that the specified       -->
  <!-- element has the additional attributes required for generating    -->
  <!-- C++ code to handle it.                                           -->
  <!-- ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ -->
  <template name="validate-element">
    <variable name="name" select="@name"/>
    <variable name="ref" select="@ref"/>
    <variable name="identifier" select="@cpp:identifier"/>
    <variable name="property" select="@cpp:property"/>
    <variable name="type" select="*/@cpp:type"/>

    <!-- We require ref's to have "property", non-ref's to have others -->
    <choose>
      <when test="$ref">
        <if test="not($property)">
          <message terminate="yes">
Expected "property" attribute in schema reference: &lt;<value-of select="$ref"/>&gt;
          </message>
        </if>
      </when>
      <otherwise>
        <if test="not($identifier)">
          <message terminate="yes">
Expected "identifier" attribute in schema element: &lt;<value-of select="$name"/>&gt;
          </message>
        </if>
        <if test="not($type)">
          <message terminate="yes">
Expected "type" attribute in schema element: &lt;<value-of select="$name"/>&gt;
          </message>
        </if>
      </otherwise>
    </choose>

    <!-- Record that we saw this type -->
    <if test="js:sawTypeInContext(string($type), string($name))"/>
  </template>


  <!-- ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ -->
  <!-- This named template is called to ensure that the specified       -->
  <!-- attribute has the additional attributes required for generating  -->
  <!-- C++ code to handle it.                                           -->
  <!-- ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ -->
  <template name="validate-attribute">
    <variable name="name" select="@name"/>
    <variable name="identifier" select="@cpp:identifier"/>
    <variable name="property" select="@cpp:property"/>
    <if test="not($identifier)">
      <message terminate="yes">
Expected "identifier" attribute in schema attribute: <value-of select="$name"/>
      </message>
    </if>
    <if test="not($property)">
      <message terminate="yes">
Expected "property" attribute in schema attribute: <value-of select="$name"/>
      </message>
    </if>
  </template>


<!-- ====================================================================== -->
<!-- ===================== XML Constant Definitions ======================= -->
<!-- ====================================================================== -->
  <!-- ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ -->
  <!-- This named template is called to write the declaration for an    -->
  <!-- XML identifier (element or attribute tag name).                  -->
  <!-- ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ -->
  <template name="write-identifier-declaration">
    <variable name="name" select="@name"/>
    <variable name="identifier" select="@cpp:identifier"/>
    <if test="not(js:wroteIdentDecl(string($identifier)))">
extern const ustring <value-of select="$identifier"/>;
    </if>
  </template>


  <!-- ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ -->
  <!-- This named template is called to write the definition for an     -->
  <!-- XML identifier (element or attribute tag name).                  -->
  <!-- ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ -->
  <template name="write-identifier-implementation">
    <variable name="name" select="@name"/>
    <variable name="identifier" select="@cpp:identifier"/>
    <if test="not(js:wroteIdentImpl(string($identifier)))">
const ustring <value-of select="$identifier"/>("<value-of select="$name"/>");
    </if>
  </template>


<!-- ====================================================================== -->
<!-- ===================== Function Prototypes ============================ -->
<!-- ====================================================================== -->
  <!-- This named template is called to write the reference resolver function
       prototype for an element type. -->
  <!--template name="write-resolver-prototype">
    <variable name="name" select="@name"/>
    <variable name="type" select="*/@cpp:type"/>
    <if test="not(js:wroteResolvProto(string($type)))">
// Reference resolver for object type "<value-of select="$type"/>"
<value-of select="$type"/>Ptr resolve<value-of select="$type"/>Reference(DOMDocumentPtr fromDoc, const ustring &amp;ref);
    </if>
  </template-->

  <!-- ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ -->
  <!-- This named template is called to write the operator prototype    --> 
  <!-- for serializing an object as an XML element.                     -->
  <!-- ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ -->
  <template name="write-serializer-prototype">
    <variable name="name" select="@name"/>
    <variable name="type" select="*/@cpp:type"/>
    <if test="not(js:wroteSerProto(string($type)))">
// Serializer for converting a(n) a <value-of select="$type"/> object to a(n) &lt;<value-of select="$name"/>&gt; element
void operator&lt;&lt;(DOMElementPtr e, const <value-of select="$type"/>Ptr n);
    </if>
  </template>


  <!-- ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ -->
  <!-- This named template is called to write the operator prototype    --> 
  <!-- for deserializing an XML element into an object.                 -->
  <!-- ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ -->
  <template name="write-deserializer-prototype">
    <variable name="name" select="@name"/>
    <variable name="type" select="*/@cpp:type"/>
    <if test="not(js:wroteDeserProto(string($type)))">
// Deserializer for converting a(n) &lt;<value-of select="$name"/>&gt; element to a(n) <value-of select="$type"/> object
void operator&gt;&gt;(DOMElementPtr e, <value-of select="$type"/>Ptr n);
    </if>
  </template>


<!-- ====================================================================== -->
<!-- ===================== Function Definitions =========================== -->
<!-- ====================================================================== -->
  <!-- This named template is called to write the reference resolver function
       prototype for an element type. -->
  <template name="write-resolver-implementation">
    <variable name="name" select="@name"/>
    <variable name="type" select="*/@cpp:type"/>
    <if test="not(js:wroteResolvImpl(string($type)))">
// Reference resolver for object type "<value-of select="$type"/>"
hash_map&lt;string, <value-of select="$type"/>Ptr, hashString, equalsString&gt; <value-of select="$type"/>Table;

<value-of select="$type"/>Ptr resolve<value-of select="$type"/>Reference(DOMDocumentPtr fromDoc, const ustring &amp;ref) {

    // Normalize the reference 
    string file, id, refString = (string)ref;
    int hashIndex = refString.find('#', 0);
    if (refString == "") {                    // Empty reference!!!
        throw InvalidReferenceException("Expected reference string");
    } else if (hashIndex == 0) {              // Ref in current file
        file = ustring(fromDoc->getDocumentURI());
        id = refString.substr(1, refString.size() - 1);
        refString = file + '#' + id;    // Rebuild ref string
    } else if (hashIndex == string::npos) {   // Ref'ing an entire file
        file = refString;
        id = "";
    } else {                                  // Reference into other file
        file = refString.substr(0, hashIndex);
        id = refString.substr(hashIndex + 1, refString.size() - hashIndex - 1);
    }

    logger &lt;&lt; DESERIALIZATION
           &lt;&lt; "Resolving reference \"" &lt;&lt; refString &lt;&lt; "\"";

    <value-of select="$type"/>Ptr instance;

    instance = <value-of select="$type"/>Table[refString];

    if (instance != NULL) {   // We've got it!
        logger &lt;&lt; ": Found in <value-of select="$type"/> cache";
        logger.info();
    } else {                  // We'll have to go hunt it down

        logger &lt;&lt; ": Looking in " &lt;&lt; file;
        if (id != "")
            logger &lt;&lt; " for " &lt;&lt; id;
        logger.info();

        // Ensure that the DOM tree for this file is loaded
        DOMDocumentPtr dom = IOUtilities::loadDOM(file);
        DOMElementPtr e;

        if (id == "")     // The ref is to the entire file
            e = dom->getDocumentElement();
        else              // The ref is a particular item in the file
            e = dom->getElementById(ustring(id));

        if (e == NULL)
            throw InvalidReferenceException("Unresolved reference", file, id);
        
        // Deserialize the sucker
        instance = new <value-of select="$type"/>();
        e >> instance;
    }
    return instance;
}
    </if>
  </template>


  <!-- ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ -->
  <!-- This named template is called to write the operator function     --> 
  <!-- for serializing an object as an XML element.                     -->
  <!-- ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ -->
  <template name="write-serializer-implementation">
    <variable name="name" select="@name"/>
    <variable name="identifier" select="@cpp:identifier"/>
    <variable name="type" select="*/@cpp:type"/>
    <if test="not(js:wroteSerImpl(string($type)))">
// Serializer for converting a(n) a <value-of select="$type"/> object to a(n) &lt;<value-of select="$name"/>&gt; element
void operator&lt;&lt;(DOMElementPtr e, <value-of select="$type"/>Ptr n) {
    logger &lt;&lt; "Serializer for &lt;<value-of select="$name"/>&gt; "
                    "NOT IMPLEMENTED";
    logger.warning();
}
    </if>
  </template>


  <!-- ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ -->
  <!-- This named template is called to write the operator prototype    --> 
  <!-- for deserializing an XML element into an object.                 -->
  <!-- ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ -->
  <template name="write-deserializer-implementation">
    <variable name="name" select="@name"/>
    <variable name="identifier" select="@cpp:identifier"/>
    <variable name="type" select="*/@cpp:type"/>
    <variable name="multi-context" select="js:isMultiContext(string($type))"/>
    <if test="not(js:wroteDeserImpl(string($type)))">
// Deserializer for converting a(n) &lt;<value-of select="$name"/>&gt; element to a(n) <value-of select="$type"/> object
void operator&gt;&gt;(DOMElementPtr e, <value-of select="$type"/>Ptr n) {
    // Prepare for deserialization
    <if test="not($multi-context)">
    SINGLE_CONTEXT_ELEMENT(<value-of select="$name"/>);
    </if>
    <if test="$multi-context">
    MULTI_CONTEXT_ELEMENT;
    </if>

    // Deserialize each expected attribute
    FOREACH_ATTRIBUTE {
      <for-each select="xsd:complexType/xsd:attribute">
          
        <variable name="args" select="concat(@name, ', ', @cpp:property)"/>
        <choose>
          <when test="@cpp:type = 'id'">
        DESERIALIZE_ID_ATTRIBUTE( <value-of select="$args"/> );
          </when>
          <when test="@cpp:type = 'reference'">
        DESERIALIZE_REFERENCE_ATTRIBUTE( <value-of select="$args"/> );
          </when>
          <when test="@cpp:type = 'boolean'">
        DESERIALIZE_BOOLEAN_ATTRIBUTE( <value-of select="$args"/> );
          </when>
          <when test="@cpp:type = 'integer'">
        DESERIALIZE_INTEGER_ATTRIBUTE( <value-of select="$args"/> );
          </when>
          <when test="@cpp:type = 'scalar'">
        DESERIALIZE_SCALAR_ATTRIBUTE( <value-of select="$args"/> );
          </when>
          <when test="@cpp:type = 'string'">
        DESERIALIZE_STRING_ATTRIBUTE( <value-of select="$args"/> );
          </when>
          <otherwise>
            <message terminate="yes">
Unrecognized @cpp:type of "<value-of select="@cpp:type"/>"
            </message>
          </otherwise>
        </choose>
      </for-each>
        HANDLE_UNRECOGNIZED_ATTRIBUTE( <value-of select="$name"/> );
    }

    // Deserialize each expected element
    FOREACH_ELEMENT {
      <for-each select="xsd:complexType/*/xsd:element">
        <variable name="args" select="concat(@name, ', ', @cpp:property)"/>
        <choose>
          <when test="@cpp:type = 'string'">
        DESERIALIZE_STRING_ELEMENT( <value-of select="$args"/> );
          </when>
          <otherwise>
        DESERIALIZE_OBJECT_ELEMENT( <value-of select="$args"/> );
          </otherwise>
        </choose>
      </for-each>
        HANDLE_UNRECOGNIZED_ELEMENT( <value-of select="$name"/> );
    }

    CLEAN_UP;
}
    </if>
  </template>


  <!-- This named template is called to create the attribute-handling code 
       for a particular attribute type. -->
  <template name="write-attribute-handler">
    <param name="owner-type"/>

    <!-- Match the 
    <!-- Retrieve/create the data thingy we want to set -->
    <choose>
      <when test="@cpp:acquire-by = 'creation' and @cpp:pass-by = 'pointer'">
        ACQUIRE_CREATE_POINTER( <value-of select="@cpp:type"/> );
      </when>
      <when test="@cpp:acquire-by = 'lookup' and @cpp:pass-by = 'pointer'">
        ACQUIRE_LOOKUP_POINTER( XXX );
      </when>
      <when test="@cpp:acquire-by = 'lookup' and @cpp:pass-by = 'reference'">
        ACQUIRE_LOOKUP_REFERENCE( XXX );
      </when>
      <when test="@cpp:acquire-by = 'lookup' and @cpp:pass-by = 'value'">
        ACQUIRE_LOOKUP_VALUE( XXX );
      </when>
      <when test="@cpp:acquire-by = 'access' and @cpp:pass-by = 'pointer'">
        ACQUIRE_ACCESS_POINTER( XXX );
      </when>
      <otherwise>
        <if test="js:die(concat('Unsupported attribute creation mode: ',
                                'acquire-by: ', @cpp:acquire-by,
                                'pass-by:', @cpp:pass-by))"/>
      </otherwise>
    </choose>

  </template>


  <!-- This named template is called to create the element-handling code 
       for an element type. The C++ methods called differ depending on whether
       the element is declared to be unique or not. -->
  <template name="write-element-handler">
    <variable name="name" select="@name"/>
    <variable name="type" select="*/@cpp:type"/>
    <variable name="super-type" select="parent::*/@cpp:type"/>
    <variable name="identifier" select="@cpp:identifier"/>
    <variable name="property" select="@cpp:property"/>
    <variable name="can-be-ref" select="xsd:complexType/xsd:attribute[@name='ref']"/>
    <variable name="argument">
      <choose>
        <when test="$super-type">superclass</when>
        <otherwise>instance</otherwise>
      </choose>
    </variable>
        // Handle unique child &lt;<value-of select="$name"/>&gt; element
        if (tagName == <value-of select="$identifier"/>) {
            // This will hold our object (whether new or 'used')
        <!-- If this element should be assigned to a pointer of a superclass
             type, we do that here. Else, assign to same type pointer. -->
          <if test="$super-type">
            <value-of select="$super-type"/>Ptr superclass;
          </if>
            <value-of select="$type"/>Ptr instance;

          <!-- First, see if this is a reference, instead of a new element -->
          <if test="$can-be-ref">
            // See if this is a reference to an existing object
            ustring ref = child->getAttribute(XML_REFERENCE);
            if (ref != "") {
                logger.increaseIndentLevel();
                instance = resolve<value-of select="$type"/>Reference(e->getOwnerDocument(), ref);
                logger.decreaseIndentLevel();
            } else {        // Nope. This is a new object
          </if>

                // Create an instance of this object type
                instance = new <value-of select="$type"/>();

          <!-- We treat 'string's as a special case -->
          <if test="$type = 'string'">
                // Get the text of the node
                ustring text = child->getFirstChild()->getNodeValue();
                (*instance) = text;
          </if>
          <!-- Otherwise, we do the default deserialization -->
          <if test="$type != 'string'">
                // Deserialize the child object
                child &gt;&gt; instance;
          </if>

          <if test="$can-be-ref">
            }
          </if>

          <!-- If the actual pointer type is to a superclass, we must handle -->
          <if test="$super-type">
            superclass = instance;
          </if>

            // Give this new object to its owner
            n-><value-of select="$property"/>(<value-of select="$argument"/>);

            // Next child, please.
            continue;
        }
  </template>

</stylesheet>
