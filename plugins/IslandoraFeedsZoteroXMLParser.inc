<?php

class IslandoraFeedsZoteroXMLParser extends FeedsZoteroParser {
  
  function parse(FeedsImportBatch $batch, FeedsSource $source) {

    feeds_include_library('common_syndication_parser.inc', 'common_syndication_parser');

    require_once 'XML/Serializer.php';
  
    $serializer_options = array( 
    'addDecl' => TRUE, 
    'encoding' => 'UTF-8', 
    'indent' => '', 
    'rootName' => 'entry',
    'defaultTagName' => 'item',
    );

    $Serializer = &new XML_Serializer($serializer_options);
  
    $raw = $batch->getRaw();
    $refined = common_syndication_parser_parse($raw);

    $batch->title = $refined['title'];
    $batch->description = $refined['description'];
    $batch->link = $refined['link'];
    
    $xml = new SimpleXMLElement($raw);
    $children = $xml->children('http://www.w3.org/2005/Atom');
    
    foreach ($children->entry as $item) {
      $result = array();
      $result['xml'] = preg_replace(
        '/<entry>/',
        '<entry xmlns:zapi="http://zotero.org/ns/api">',
        $item->asXML()
      );
      watchdog('ifzxp',print_r($result,TRUE));
      $batch->items[] = $result;
    }
    
  }
  
}
