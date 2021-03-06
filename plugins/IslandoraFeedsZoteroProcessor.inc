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
    
    // Keep track of skipped items
    $skipped = 0;
    
    // Keep track of processed items in this pass, set total number of items.
    $processed = 0;
    if (!$batch->getTotal(FEEDS_PROCESSING)) {
      $batch->setTotal(FEEDS_PROCESSING, count($batch->items));
    }
    
    $collection_pid = 'islandora:articleCollection';
    $content_model = 'islandora:articleCModel';
    $thumbnail_file = '';
    
    while($item = $batch->shiftItem()) {
      
      if ($item['itemType'] == 'attachment') {
        // TODO insert it as a datastream in the existing object.
        // FORNOW skip it!
        watchdog('islandora_feeds','Skipped over attachent record <%guid>',array(
          '%guid' => $item['guid'],
        ));
        $skipped++;
        continue;
      }
      
      $existing_id = $this->existingItemId($batch,$source);
      if ($existing_id && $this->config['update_existing'] == FEEDS_SKIP_EXISTING) {
        // do nowt
        watchdog('islandora_feeds','Skipped duplicate record <%guid> which has existing ID %xid', 
          array('%guid' => $item['guid'], '%xid' => $existing_id)
        );
        $skipped++;
        
      }
      else {
        // create MODS and DC metadata
        $modsDOM = $this->map($batch,$fedora_object);
        $modsStream = $modsDOM->saveXML();
        $dcStream = batch_create_dc_from_mods($modsStream);
        
        if ($existing_id && $this->config['update_existing'] == FEEDS_UPDATE_EXISTING) {
          $fedora_object = new Fedora_Item($existing_id);
          // we assume a MODS datastream already exists
          $fedora_object->modify_datastream_by_value($modsStream, 'MODS', "MODS Metadata", 'text/xml');
          $batch->updated++;
        }
        else {
          if ($existing_id && $this->config['update_existing'] == FEEDS_REPLACE_EXISTING) {
            $fedora_object = new Fedora_Item($existing_id);
            $fedora_object->purge();
          }
          $pid = Fedora_Item::get_next_PID_in_namespace($namespace = 'article');
          $fedora_object = Fedora_Item::ingest_new_item($pid,'A',$item['title'],'Owner');
          $fedora_object->add_datastream_from_string($modsStream, 'MODS', 'MODS Metadata','text/xml');
          $fedora_object->add_relationship('isMemberOfCollection', $collection_pid);
          $fedora_object->add_relationship('hasModel', $content_model, 'info:fedora/fedora-system:def/model#');
          
          // invoke hooks with the 
          
          watchdog('islandora_feeds','Created object with PID %pid',array('%pid' => $pid));
          $batch->created++;
        }
        $fedora_object->modify_datastream_by_value($dcStream, 'DC', "Dublin Core", 'text/xml');
        
      }

    }
    
    // Set messages.
    if ($batch->created) {
      drupal_set_message(
        format_plural($batch->created, 'Created @number object.', 'Created @number objects.', array('@number' => $batch->created))
      );
    }
    if ($batch->updated) {
      drupal_set_message(
        format_plural($batch->updated, 'Updated @number object.', 'Updated @number objects.', array('@number' => $batch->updated))
      );
    }
    if ($skipped) {
      drupal_set_message(
        format_plural($skipped, 'Skipped @number record.', 'Skipped @number records.', array('@number' => $skipped))
      );      
    }
    if(!($batch->created || $batch->updated)) {
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
      'update_existing' => FEEDS_SKIP_EXISTING,
      'mappings' => array(),
    );
  }
  
  function existingItemId(FeedsImportBatch $batch, FeedsSource $source) {
    // Iterate through all unique targets and test whether they do already
    // exist in the database.
    foreach ($this->uniqueTargets($batch) as $target => $value) {
      switch ($target) {
        case 'guid':
          preg_match('/\w+$/',$value,$preg_value);
          watchdog('islandora_feeds','Searching for object with dc.source <%source>',
            array('%source' => $preg_value[0])
          );
          $pids = Fedora_Item::find_objects_by_pattern('*'. $preg_value[0],'source'); 
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
    );
    return $targets;
  }
  
}