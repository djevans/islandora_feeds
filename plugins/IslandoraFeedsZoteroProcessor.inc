<?php

// for update mode for existing items
define('FEEDS_CREATE_SEPARATE',3);

class IslandoraFeedsZoteroProcessor extends FeedsProcessor {
  
 
  function map(FeedsImportBatch $batch, $target_item = NULL) {
    try {
      $proc = new XsltProcessor();
    } catch (Exception $e) {
      drupal_set_message(t('@e', array('@e' => check_plain($e->getMessage()))), 'error');
      return " ";
    }
    $path = drupal_get_path('module','islandora_feeds');
    $xsl = new DomDocument();
    $xsl->load($path . '/xslt/zotero_to_mods.xsl');
    $input = new DomDocument();
    $current_item = $batch->currentItem();
    $input->loadXML(trim($current_item['xml'])); // ends up as null
    $xsl = $proc->importStylesheet($xsl);
    $output = $proc->transformToDoc($input);
    return $output;

  }
  
  function process(FeedsImportBatch $batch, FeedsSource $source) {
        
    module_load_include('inc', 'fedora_repository', 'api/fedora_utils');
    module_load_include('inc', 'fedora_repository', 'api/fedora_item');
    module_load_include('inc', 'fedora_repository', 'BatchIngest');
    
    // Keep track of processed items in this pass, set total number of items.
    $processed = 0;
    if (!$batch->getTotal(FEEDS_PROCESSING)) {
      $batch->setTotal(FEEDS_PROCESSING, count($batch->items));
    }
    
    $collection_pid = 'islandora:articleCollection';
    $content_model = 'islandora:articleCModel';
    $thumbnail_file = '';
    
    while($item = $batch->shiftItem()) {
      
      // Create/update if item does not exist or update existing is enabled.
      if (!$this->existingItemId($batch,$source) || ($this->config['update_existing'] != FEEDS_SKIP_EXISTING))
      
      try {
        $pid = Fedora_Item::get_next_PID_in_namespace($namespace = 'article');
        $fedora_object = Fedora_Item::ingest_new_item($pid, 'A', $item->title, 'Owner');

        // create MODS based on the Zotero data
        $modsDOM = $this->map($batch,$fedora_object);
        $modsStream = $modsDOM->saveXML();
        $title = $dcStream->dc['dc:title'];
        if (is_array($title)) {
          $title = $title[0];
        }
        $fedora_object->add_datastream_from_string($modsStream, 'MODS', 'MODS Metadata','text/xml');
        $dcStream = batch_create_dc_from_mods($modsStream);
        watchdog('mods_stream',$modsStream);
        watchdog('dc_stream',$dcStream);
        $fedora_object->modify_datastream_by_value($dcStream, 'DC', "Dublin Core", 'text/xml');
        $fedora_object->add_relationship('isMemberOfCollection', $collection_pid);
        $fedora_object->add_relationship('hasModel', $content_model, 'info:fedora/fedora-system:def/model#');
        // $fedora_object->add_datastream_from_file($thumbnail_file, 'TN');
        $batch->created++;
        
      } catch (Exception $e) {
        
      }
    }
    
    // Set messages.
    if ($batch->created) {
      drupal_set_message(
        format_plural($batch->created, 'Created @number object.', 'Created @number objects.', array('@number' => $batch->created))
      );
    }
    elseif ($batch->updated) {
      drupal_set_message(
        format_plural($batch->updated, 'Created @number object.', 'Created @number objects.', array('@number' => $batch->updated))
      );
    }
    else {
      drupal_set_message(t('There is no new content.'));
    }
    $batch->setProgress(FEEDS_PROCESSING, FEEDS_BATCH_COMPLETE);

  }
  
  function clear(FeedsBatch $batch, FeedsSource $source) {}
  
  function setTargetElement(&$target_item, $target_element, $value) {
    $target_item->dc->set_element($target_element,$value);
  }
  
  function configForm(&$form_state) {
    $form = array();
    $form['update_existing'] = array(
      '#type' => 'radios',
      '#title' => t('Update existing objects'),
      '#description' => t('Select how existing objects should be updated. Existing nodes will be determined using mappings that are a "unique target".'),
      '#options' => array(
        FEEDS_SKIP_EXISTING => 'Do not update existing objects (Do nothing)',
        FEEDS_REPLACE_EXISTING => 'Replace existing nodes (Overwrite records)',
        FEEDS_UPDATE_EXISTING => 'Update existing objects (Merge records) (slower than replacing them)',
        FEEDS_CREATE_SEPARATE => 'Create objects with different PIDs (will duplicate data)',
      ),
      '#default_value' => $this->config['update_existing'],
    );
    return $form;
  }
  
  function configDefaults() {
    return array(
      'update_existing' => FEEDS_UPDATE_EXISTING,
      'mappings' => array(),
    );
  }
  
  function existingItemId(FeedsImportBatch $batch, FeedsSource $source) {
    // Iterate through all unique targets and test whether they do already
    // exist in the database.
    foreach ($this->uniqueTargets($batch) as $target => $value) {
      switch ($target) {
        case 'url':
        case 'guid':
          $pids = Fedora_Item::find_objects_by_pattern($value,'source'); 
          break;
      }
      if (!empty($pids)) {
        // Return with the first nid found.
        $keys = array_keys($pids);
        return $keys[0];
      }
    }
    return FALSE;
  }
  
  function uniqueTargets(FeedsImportBatch $batch) {
    $item = $batch->currentItem();
    $targets = array(
      'guid' => $item['guid'],
      'url' => $item['url'],
    );
    return $targets;
  }
  
}