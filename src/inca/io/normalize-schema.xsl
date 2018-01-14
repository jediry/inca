<?xml version="1.0" encoding="UTF-8"?>

<!-- This XSL transform stylesheet translates the Schema definition for
     Inca data files into a normalized form, replacing all ref'd elements,
     attributes and types with copies of the referred-to node. This is an
     intermediate step between the Schema that the developer (me :-) writes and
     what the schema -> c++ transformer sees. Having this middle step allows
     the second XSL sheet to be much simpler.
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
  <output method="xml" indent="yes"/>


<!-- ====================================================================== -->
<!-- ===================== Template Definitions =========================== -->
<!-- ====================================================================== -->
  
  <!-- This is the entry point of the stylesheet. It processes recursively
       xsd:elements contained by the xsd:schema element. -->
  <template match="xsd:schema">
    <copy>
      <call-template name="copy-attributes"/> <!-- Clone attributes -->

      <!-- Normalize each top-level element -->
      <for-each select="xsd:element">
        <call-template name="normalize-element"/>
      </for-each>
    </copy>
  </template>


  <!-- This named template clones (almost) all of the attributes of the current
       element. We can't just use copy-of, since we may want to alter the 
       element or its children. We don't copy 'ref', 'type', or 'default'
       attributes. -->
  <template name="copy-attributes">
    <for-each select="attribute::*">
      <if test="name(.)!='ref' and name(.)!='type' and name(.)!='default'">
        <copy/>
      </if>
    </for-each>
  </template>


  <!-- This named template allows us to trim out some unwanted relics of the
       original schema, such as 'attributeGroup's. -->
  <template name="handle-element">
    <choose>
      <when test="name() = 'xsd:attributeGroup'">
        <variable name="ref" select="@ref"/>
        <for-each select="//xsd:attributeGroup[@name=$ref]/*">
          <!-- Just process the kids -->
          <call-template name="normalize-element"/>
        </for-each>
      </when>
      <otherwise>
        <!-- Process the entire element -->
        <call-template name="normalize-element"/>
      </otherwise>
    </choose>
  </template>


  <!-- This named template clones the child elements of this element,
       replacing any 'ref's, uses of 'complexType's or 'simpleType's,
       and 'attributeGroup's with a copy of the original. -->
  <template name="normalize-element">

    <!-- Clone this element, replacing ref's with the real thing -->
    <copy>
      <call-template name="copy-attributes"/> <!-- Clone attributes -->

      <variable name="ref" select="@ref"/>
      <variable name="type">
        <choose>
          <when test="not( $ref )"> <value-of select="@type"/> </when>
          <otherwise> <value-of select="//*[@name=$ref]/@type"/> </otherwise>
        </choose>
      </variable>

      <!--=====================================================-->
      <!-- Case 1: This is a ref to another element:           -->
      <!--=====================================================-->
      <if test="$ref">
        <!-- Copy the name attribute -->
        <attribute name="name">
          <value-of select="$ref"/>
        </attribute>

        <!-- Copy the C++ identifier attribute, if needed... -->
        <if test="//*[@name=$ref]/@cpp:identifier">
          <attribute name="cpp:identifier">
            <value-of select="//*[@name=$ref]/@cpp:identifier"/>
          </attribute>
        </if>

        <!-- ...and all the children of the ref'd original -->
        <for-each select="//*[@name=$ref]/*">
          <call-template name="handle-element"/>
        </for-each>
      </if>

      <!--=====================================================-->
      <!-- Case 2: This is a instantiation of a named type:    -->
      <!--=====================================================-->
      <if test="$type and //xsd:simpleType[@name=$type]">
        <for-each select="//xsd:simpleType[@name=$type]">
          <call-template name="handle-element"/>
        </for-each>
      </if>
      <if test="$type and //xsd:complexType[@name=$type]">
        <for-each select="//xsd:complexType[@name=$type]">
          <call-template name="handle-element"/>
        </for-each>
      </if>

      <!--=====================================================-->
      <!-- Case 3: This is a normal element                    -->
      <!--=====================================================-->
      <for-each select="*">
        <call-template name="handle-element"/>
      </for-each>
    </copy>

  </template>

</stylesheet>
