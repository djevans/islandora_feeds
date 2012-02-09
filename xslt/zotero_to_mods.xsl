<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:atom="http://www.w3.org/2005/Atom" xmlns:zapi="http://zotero.org/ns/api" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:html="http://www.w3.org/1999/xhtml"
    xmlns:mods="http://www.loc.gov/mods/v3"
    exclude-result-prefixes="atom html zapi" 
    version="1.0">
    <xsl:output method="xml" indent="yes"/>
    <xsl:strip-space elements="*"/>
    <xsl:template match="/">
        <xsl:apply-templates select="//entry" />
    </xsl:template>
    <xsl:template match="entry">
        <mods:mods>
            <xsl:apply-templates select="title" />
            <xsl:apply-templates select="id" />
            <xsl:apply-templates select="descendant::html:div/html:table" />
        </mods:mods>
    </xsl:template>
    <xsl:template match="title">
        <mods:titleInfo>
            <mods:title>
                <xsl:value-of select="." />
            </mods:title>
        </mods:titleInfo>
    </xsl:template>
    <xsl:template match="id">
        <mods:relatedItem type="original">
            <mods:location>
                <mods:url>
                    <xsl:value-of select="." />
                </mods:url>
            </mods:location>
        </mods:relatedItem>
    </xsl:template>
    <xsl:template match="html:table">
        <xsl:apply-templates select="html:tr[@class='itemType' and html:td[text()]]"/>
        <xsl:apply-templates select="html:tr[@class='title' and html:td[text()]]"/>
        <xsl:apply-templates select="html:tr[@class='shortTitle' and html:td[text()]]"/>
        <mods:relatedItem type="host">
            <xsl:apply-templates select="html:tr[@class='publicationTitle' and html:td[text()]]"/>
            <xsl:apply-templates select="html:tr[@class='ISSN' and html:td[text()]]"/>
            <xsl:apply-templates select="html:tr[@class='volume' and html:td[text()]]"/>
            <xsl:apply-templates select="html:tr[@class='issue' and html:td[text()]]"/>
        </mods:relatedItem>
        <xsl:apply-templates select="html:tr[@class='creators' and html:td[text()]]"/>
        <xsl:apply-templates select="html:tr[@class='abstractNote' and html:td[text()]]"/>
        <xsl:apply-templates select="html:tr[@class='seriesTitle' and html:td[text()]]"/>
        <xsl:apply-templates select="html:tr[@class='accessDate' and html:td[text()]]"/>
        <xsl:apply-templates select="html:tr[@class='date' and html:td[text()]]"/>
        <xsl:apply-templates select="html:tr[@class='url' and html:td[text()]]"/>
        <xsl:apply-templates select="html:tr[@class='DOI' and html:td[text()]]"/>
        <xsl:apply-templates select="html:tr[@class='rights' and html:td[text()]]"/>
        <xsl:apply-templates select="html:tr[@class='callNumber' and html:td[text()]]"/>
        <xsl:apply-templates select="html:tr[@class='archiveLocation' and html:td[text()]]"/>
    </xsl:template>
    <xsl:template match="html:tr[@class='itemType']">
        <mods:typeOfResource>text</mods:typeOfResource>
        <mods:genre authority="local">
            <xsl:value-of select="html:td"/>
        </mods:genre>
        <mods:genre authority="marcgt">
            <xsl:value-of select="html:td"/>
        </mods:genre>
    </xsl:template>
    <xsl:template match="html:tr[@class='creator']">
        <mods:name type="personal">
            <mods:namePart>
                <xsl:value-of select="html:td"/>
            </mods:namePart>
            <mods:role>
                <mods:roleTerm type="code" authority="marcrelator">
                    <xsl:choose>
                        <xsl:when test="html:th='Author'">aut</xsl:when>
                        <xsl:when test="html:th='Editor'">edt</xsl:when>
                    </xsl:choose>
                </mods:roleTerm>
                <mods:roleTerm type="text">
                    <xsl:value-of select="html:th"/>
                </mods:roleTerm>
            </mods:role>
        </mods:name>
    </xsl:template>
    <xsl:template match="html:tr[@class='url']">
        <mods:location>
            <mods:url>
                <xsl:value-of select="html:td"/>
            </mods:url>
        </mods:location>
    </xsl:template>
    <xsl:template match="html:tr[@class='rights']">
        <mods:accessCondition>
            <xsl:value-of select="html:td"/>
        </mods:accessCondition>
    </xsl:template>
    <xsl:template match="html:tr[@class='series']">
        <xsl:element name="{@class}">
            <xsl:value-of select="html:td"/>
        </xsl:element>
    </xsl:template>
    <xsl:template match="html:tr[@class='volume']">
        <mods:part>
            <mods:detail type="volume">
                <mods:number>
                    <xsl:value-of select="html:td"/>
                </mods:number>
            </mods:detail>
        </mods:part>
    </xsl:template>
    <xsl:template match="html:tr[@class='issue']">
        <mods:part>
            <mods:detail type="issue">
                <mods:number>
                    <xsl:value-of select="html:td"/>
                </mods:number>
            </mods:detail>
        </mods:part>
    </xsl:template>
    <xsl:template match="html:tr[@class='pages']">
        <mods:part>
            <mods:extent unit="pages">
                <mods:total>
                    <xsl:value-of select="html:td"/>
                </mods:total>
            </mods:extent>
        </mods:part>
    </xsl:template>
    <xsl:template match="html:tr[@class='publicationTitle']">
        <mods:titleInfo>
            <mods:title>
                <xsl:value-of select="html:td"/>
            </mods:title>
        </mods:titleInfo>
    </xsl:template>
    <xsl:template match="html:tr[@class='ISSN']">
        <mods:identifier type="issn">
            <xsl:value-of select="html:td"/>
        </mods:identifier>
    </xsl:template>
    <xsl:template match="html:tr[@class='date']">
        <mods:originInfo>
            <mods:copyrightDate>
                <xsl:value-of select="html:td"/>
            </mods:copyrightDate>
        </mods:originInfo>
    </xsl:template>
    <xsl:template match="html:tr[@class='callNumber']">
        <mods:classification>
            <xsl:value-of select="html:td"/>
        </mods:classification>
    </xsl:template>
    <xsl:template match="html:tr[@class='archiveLocation']">
        <mods:location>
            <mods:physicalLocation>
                <xsl:value-of select="html:td"/>
            </mods:physicalLocation>
        </mods:location>
    </xsl:template>
    <xsl:template match="html:tr[@class='extra']" />
    <xsl:template match="html:tr[@class='journalAbbreviation']" />
    <xsl:template match="html:tr[@class='DOI']">
        <mods:identifier type="doi">
            <xsl:value-of select="html:td"/>
        </mods:identifier>
    </xsl:template>
    <xsl:template match="html:tr[@class='accessDate']">
        <mods:originInfo>
            <mods:dateCreated>
                <xsl:value-of select="html:td"/>
            </mods:dateCreated>
        </mods:originInfo>
    </xsl:template>
    <xsl:template match="html:tr[@class='seriesTitle']">
        <mods:relatedItem type="series">
            <mods:titleInfo>
                <mods:title>
                    <xsl:value-of select="html:td"/>
                </mods:title>
            </mods:titleInfo>
        </mods:relatedItem>
    </xsl:template>
    <xsl:template match="html:tr[@class='seriesText']" />
    <xsl:template match="html:tr[@class='libraryCatalog']" />
    <xsl:template match="html:tr[@class='language']">
        <mods:language>
            <xsl:value-of select="html:td"/>
        </mods:language>
    </xsl:template>
    <xsl:template match="html:tr[@class='abstractNote']">
        <mods:abstract>
            <xsl:value-of select="html:td"/>
        </mods:abstract>
    </xsl:template>
    <xsl:template match="html:tr[@class='shortTitle']">
        <mods:titleInfo type="alternative">
            <mods:title>
                <xsl:value-of select="html:td"/>
            </mods:title>
        </mods:titleInfo>
    </xsl:template>
    <xsl:template match="html:tr[@class='archive']" />
</xsl:stylesheet>