<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:mods="http://www.loc.gov/mods/v3"
    exclude-result-prefixes="xs" version="1.0">
    <xsl:template match="/">
        <mods:mods>
            <xsl:apply-templates select="entry"/>
        </mods:mods>
    </xsl:template>
    <xsl:template match="entry">
        <xsl:apply-templates/>
    </xsl:template>
    <xsl:template match="itemType">
        <mods:typeOfResource>text</mods:typeOfResource>
        <mods:genre authority="local">
            <xsl:value-of select="text()"/>
        </mods:genre>
        <mods:genre authority="marcgt">
            <xsl:value-of select="text()"/>
        </mods:genre>
    </xsl:template>
    <xsl:template match="title">
        <mods:titleInfo>
            <mods:title>
                <xsl:value-of select="text()"/>
            </mods:title>
        </mods:titleInfo>
    </xsl:template>
    <xsl:template match="creators">
        <xsl:apply-templates select="item/creator"/>
    </xsl:template>
    <xsl:template match="creator">
        <mods:name type="personal">
            <mods:namePart type="family">
                <xsl:value-of select="lastName"/>
            </mods:namePart>
            <mods:namePart type="given">
                <xsl:value-of select="firstName"/>
            </mods:namePart>
            <mods:role>
                <mods:roleTerm type="code" authority="marcrelator">
                    <xsl:choose>
                        <xsl:when test="creatorType='author'">aut</xsl:when>
                        <xsl:when test="creatorType='editor'">edt</xsl:when>
                    </xsl:choose>
                </mods:roleTerm>
            </mods:role>
        </mods:name>
    </xsl:template>
    <xsl:template match="abstractNote">
        <mods:abstract>
            <xsl:value-of select="text()"/>
        </mods:abstract>
    </xsl:template>
    <xsl:template match="seriesTitle">
        <mods:relatedItem type="series">
            <mods:titleInfo>
                <mods:title><xsl:value-of select="text()" /></mods:title>
            </mods:titleInfo>
        </mods:relatedItem>
    </xsl:template>
    <xsl:template match="volume">
        <mods:part>
            <mods:detail type="volume">
                <mods:number>
                    <xsl:value-of select="text()"/>
                </mods:number>
            </mods:detail>
        </mods:part>
    </xsl:template>
    <xsl:template match="issue">
        <mods:originInfo>
            <mods:edition>
                <xsl:value-of select="text()"/>
            </mods:edition>
        </mods:originInfo>
    </xsl:template>
    <xsl:template match="publicationTitle">
        <mods:relatedItem type="host">
            <mods:titleInfo>
                <mods:title>
                    <xsl:value-of select="text()"/>
                </mods:title>
            </mods:titleInfo>
        </mods:relatedItem>
    </xsl:template>
    <xsl:template match="date">
        <mods:originInfo>
            <mods:copyrightDate>
                <xsl:value-of select="text()" />
            </mods:copyrightDate>
        </mods:originInfo>
    </xsl:template>
    <xsl:template match="pages">
        <mods:part>
            <mods:extent unit="pages">
                <mods:total><xsl:value-of select="text()" /></mods:total>
            </mods:extent>
        </mods:part>
    </xsl:template>
    <xsl:template match="url">
        <mods:location>
            <mods:url>
                <xsl:value-of select="text()"/>
            </mods:url>
        </mods:location>
    </xsl:template>
    <xsl:template match="language">
        <mods:language>
            <xsl:value-of select="text()"></xsl:value-of>
        </mods:language>
    </xsl:template>
    <xsl:template match="DOI">
        <mods:identifier type="doi">
            <xsl:value-of select="text()"></xsl:value-of>
        </mods:identifier>
    </xsl:template>
    <xsl:template match="ISSN">
        <mods:identifier type="issn">
            <xsl:value-of select="text()"></xsl:value-of>
        </mods:identifier>
    </xsl:template>
    <xsl:template match="rights">
        <mods:accessCondition>
            <xsl:value-of select="text()" />
        </mods:accessCondition>
    </xsl:template>
    <xsl:template match="callNumber">
        <mods:classification>
            <xsl:value-of select="text" />
        </mods:classification>
    </xsl:template>
    <xsl:template match="accessDate">
        <mods:originInfo>
            <mods:dateCreated>
                <xsl:value-of select="text()" />
            </mods:dateCreated>
        </mods:originInfo>
    </xsl:template>
    <xsl:template match="shortTitle">
        <mods:titleInfo>
            <mods:title><xsl:value-of select="text()" /></mods:title>
        </mods:titleInfo>
    </xsl:template>
    <xsl:template match="archiveLocation">
        <mods:location>
            <mods:physicalLocation><xsl:value-of select="text()" /></mods:physicalLocation>
        </mods:location>
    </xsl:template>
    <xsl:template match="url">
        <mods:relatedItem type="original">
          <mods:location>
            <mods:url><xsl:value-of select="text()" /></mods:url>
          </mods:location>
        </mods:relatedItem>
    </xsl:template>
    <xsl:template match="node()"><!-- Ignore everything else --></xsl:template>
</xsl:stylesheet>
