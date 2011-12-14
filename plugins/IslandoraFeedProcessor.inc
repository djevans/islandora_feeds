<?php

class IslandoraFeedProcessor extends FeedsProcessor {
  
  /**
   * Implementation of FeedsProcessor::process().
   */
  function process(FeedsImportBatch $batch, FeedsSource $source) {
    
    module_load_include('inc','islandora_harvester','islandora_harvester');
    watchdog('islandora_feeds','Starting process');
    $namespace = 'article';
    $collection_pid = 'islandora:articleCollection';
    $content_model = 'islandora:articleCModel';
    while ($batch->shiftItem()) {
      $record = array('dc' => $this->map($batch));
      build_oai_objects($record,$namespace,$collection_pid,$content_model);
    }
    
  }
  
  /**
   * Implementation of FeedsProcessor::clear().
   */
  function clear(FeedsBatch $batch, FeedsSource $source) {
    drupal_set_message("The 'Clear' operation is not supported yet.");
  }
  
  function map(FeedsImportBatch $batch) {
    module_load_include('inc','fedora_repository','api/dublin_core');
    $item = $batch->currentItem();
    // watchdog('islandora_feeds',print_r($item,TRUE));
    $dc_object = new Dublin_Core();
    foreach ($item as $key => $value) {
      if (array_key_exists("dc:{$key}",$dc_object->dc)) {
        /*
        watchdog('islandora_feeds',"Setting %field to %value", array(
          '%field' => "dc:{$key}",
          '%value' => $value,
        ));
        */
        $dc_object->set_element("dc:{$key}",$value);        
      }
    }
    // Schematron for FOXML 1.1 won't accept labels of over 255 characters
    $dc_object->set_element("dc:title",truncate_utf8(
      $title,
      250,
      $wordsafe = FALSE,
      $dots = TRUE
    ));
    // watchdog('islandora_feeds',$dc_object->as_xml());
    return $dc_object->as_xml();
  }
  
}