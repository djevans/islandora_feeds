<?php

class IslandoraFeedsZoteroXMLParser extends FeedsZoteroParser {
  
  function parse(FeedsImportBatch $batch, FeedsSource $source) {

    require_once 'XML/Serializer.php';
  
    $serializer_options = array( 
    'addDecl' => TRUE, 
    'encoding' => 'UTF-8', 
    'indent' => '', 
    'rootName' => 'entry',
    'defaultTagName' => 'item',
    );

    $Serializer = &new XML_Serializer($serializer_options);
  
    parent::parse($batch,$source);
      
    foreach($batch->items as &$item) {

      $status = $Serializer->serialize($item);
      $output = $Serializer->getSerializedData();
      $item['xml'] = $output; 
      
    }
    
  }
  
}
