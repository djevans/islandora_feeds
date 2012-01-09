<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:mods="http://www.loc.gov/mods/v3" version="1.0">
    <xsl:strip-space elements="*"/>
    <xsl:template match="/">
        <mods:mods>
            <xsl:apply-templates select="entry"/>
        </mods:mods>
    </xsl:template>
    <xsl:template match="entry">
        <xsl:apply-templates select="itemType"/>
        <xsl:apply-templates select="title"/>
        <xsl:apply-templates select="shortTitle"/>
        <mods:relatedItem type="host">
            <xsl:apply-templates select="publicationTitle"/>
            <xsl:apply-templates select="ISSN"/>
            <xsl:apply-templates select="volume"/>
            <xsl:apply-templates select="issue"/>
        </mods:relatedItem>
        <xsl:apply-templates select="creators"/>
        <xsl:apply-templates select="abstractNote"/>
        <xsl:apply-templates select="seriesTitle"/>
        <xsl:apply-templates select="accessDate"/>
        <xsl:apply-templates select="date"/>
        <xsl:apply-templates select="url"/>
        <xsl:apply-templates select="DOI"/>
        <xsl:apply-templates select="rights"/>
        <xsl:apply-templates select="callNumber"/>
        <xsl:apply-templates select="archiveLocation"/>
    </xsl:template>
    <xsl:template match="itemType">
        <mods:typeOfResource>text</mods:typeOfResource>
        <mods:genre authority="local">
            <xsl:value-of select="."/>
        </mods:genre>
        <mods:genre authority="marcgt">
            <xsl:value-of select="."/>
        </mods:genre>
    </xsl:template>
    <xsl:template match="title">
        <mods:titleInfo>
            <mods:title>
                <xsl:value-of select="."/>
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
    <xsl:template match="abstractNote[text()]">
        <mods:abstract>
            <xsl:value-of select="."/>
        </mods:abstract>
    </xsl:template>
    <xsl:template match="seriesTitle[text()]">
        <mods:relatedItem type="series">
            <mods:titleInfo>
                <mods:title>
                    <xsl:value-of select="."/>
                </mods:title>
            </mods:titleInfo>
        </mods:relatedItem>
    </xsl:template>
    <xsl:template match="date[text()]">
        <mods:originInfo>
            <mods:copyrightDate>
                <xsl:value-of select="."/>
            </mods:copyrightDate>
        </mods:originInfo>
    </xsl:template>
    <xsl:template match="url[text()]">
        <mods:location>
            <mods:url>
                <xsl:value-of select="."/>
            </mods:url>
        </mods:location>
    </xsl:template>
    <xsl:template match="language[text()]">
        <mods:language>
            <xsl:value-of select="."/>
        </mods:language>
    </xsl:template>
    <xsl:template match="DOI[text()]">
        <mods:identifier type="doi">
            <xsl:value-of select="."/>
        </mods:identifier>
    </xsl:template>
    <xsl:template match="rights[text()]">
        <mods:accessCondition>
            <xsl:value-of select="."/>
        </mods:accessCondition>
    </xsl:template>
    <xsl:template match="callNumber[text()]">
        <mods:classification>
            <xsl:value-of select="."/>
        </mods:classification>
    </xsl:template>
    <xsl:template match="accessDate[text()]">
        <mods:originInfo>
            <mods:dateCreated>
                <xsl:value-of select="."/>
            </mods:dateCreated>
        </mods:originInfo>
    </xsl:template>
    <xsl:template match="shortTitle[text()]">
        <mods:titleInfo type="alternative">
            <mods:title>
                <xsl:value-of select="."/>
            </mods:title>
        </mods:titleInfo>
    </xsl:template>
    <xsl:template match="archiveLocation[text()]">
        <mods:location>
            <mods:physicalLocation>
                <xsl:value-of select="."/>
            </mods:physicalLocation>
        </mods:location>
    </xsl:template>
    <!-- start journal templates -->
    <xsl:template match="publicationTitle[text()]">
        <mods:titleInfo>
            <mods:title>
                <xsl:value-of select="."/>
            </mods:title>
        </mods:titleInfo>
    </xsl:template>
    <xsl:template match="ISSN[text()]">
        <mods:identifier type="issn">
            <xsl:value-of select="."/>
        </mods:identifier>
    </xsl:template>
    <xsl:template match="volume[text()]">
        <mods:part>
            <mods:detail type="volume">
                <mods:number>
                    <xsl:value-of select="."/>
                </mods:number>
            </mods:detail>
        </mods:part>
    </xsl:template>
    <xsl:template match="issue[text()]">
        <mods:part>
            <mods:detail type="issue">
                <mods:number>
                    <xsl:value-of select="."/>
                </mods:number>
            </mods:detail>
        </mods:part>
    </xsl:template>
    <xsl:template match="pages[text()]">
        <mods:part>
            <mods:extent unit="pages">
                <mods:total>
                    <xsl:value-of select="."/>
                </mods:total>
            </mods:extent>
        </mods:part>
    </xsl:template>
</xsl:stylesheet>
