<?php

class FeedsIslandoraZoteroHTTPBatch extends FeedsImportBatch {
 
  protected $zotero_feed_url;
  protected $first_pass;
  protected $records_fetched;
  protected $most_recent_fetched;
 
  // what do we want to do?
 
  public function __construct($zotero_feed_url, $first_pass, $records_fetched, $most_recent_fetched, $feed_nid) {
    $this->zotero_feed_url = $zotero_feed_url;
    $this->setFirstPass($first_pass);
    $this->setRecordsFetched($records_fetched);
    $this->setMostRecentFetched($most_recent_fetched);
    parent::__construct($zotero_feed_url,$feed_nid);
  }

  public function getRaw() {
    
    $url = $this->zotero_feed_url;
    $url .= '&itemType=journalArticle%20||%20attachment&order=dateAdded';
    // first pass: sort by dateCreated asc
    // second pass and beyond: sort by dateCreated desc
    $url .= '&sort=';
    $url .= ($this->first_pass)? 'asc': 'desc';
    $url .= '&limit=2&start='. $this->records_fetched;
            
    $result = drupal_http_request($url);
    if ($result->code == 200) {

      $result_xml = simplexml_load_string($result->data);
      $entry_count = count($result_xml->entry);
      $feed_zapi = $result_xml->children('http://zotero.org/ns/api');
      $total_results = $feed_zapi->totalResults;
      
      if ($this->first_pass) {
        if ($this->records_fetched == 0) {
          $latest_update = $result_xml->updated; // returned in YYYY-MM-DDThh:mm:ssZ format
          $this->setMostRecentFetched($latest_update);
        }
        $this->setRecordsFetched($this->records_fetched + $entry_count);
        if ($this->records_fetched == $total_results) {
          $this->setFirstPass(FALSE);
          $this->setRecordsFetched(0);
        }        
      }
      else {
        // if $first_pass == FALSE,
        // restart the pass if we have a record in the set that is older than mostRecentFetched
        // to do this, we first need the oldest record in the set
        $oldest_entry = $result_xml->entry[$entry_count - 1];
        $oldest_timestamp = $oldest_entry->published;
        if ($oldest_timestamp < $this->most_recent_fetched) {
          // restart the pass
          $this->setRecordsFetched(0);
        } 
        else {
          $this->setRecordsFetched($this->records_fetched + $entry_count);
          if ($this->records_fetched == $total_results) {
            $this->setRecordsFetched(0);
          }
        }
      }
    }
    else {
      // TODO log
    }
    return $result->data;
    
  }
  
  public function setRecordsFetched($count) {
    $this->records_fetched = $count;
    variable_set('islandora_feeds:'. $this->zotero_feed_url .':records_fetched',$count);
  }
  
  public function setMostRecentFetched($timestamp) {
    $this->most_recent_fetched = $timestamp;
    variable_set('islandora_feeds:'. $this->zotero_feed_url .':most_recent_fetched',$timestamp);
  }
  
  public function setFirstPass($is_first) {
    $this->first_pass = (bool)$is_first;
    variable_set('islandora_feeds:'. $this->zotero_feed_url .':first_pass',(bool)$is_first);
  }
  
  protected function formatDate($timestamp) {
    $date_format = 'Y-m-d\TH:m:s\Z';
    return date($date_format, $timestamp);
  }
  
}

class IslandoraFeedsZoteroFetcher extends FeedsZoteroFetcher {
  
  /**
  * Implements FeedsFetcher::fetch().
  */  
  public function fetch(FeedsSource $source) {
    
    $source_config = $source->getConfigFor($this);
    
    // get the variables from the 'variable' table
    // TODO: What happens when the feed gets deleted?
    $first_pass = variable_get('islandora_feeds:'. $source_config['source'] .':first_pass',TRUE);
    $records_fetched = variable_get('islandora_feeds:'. $source_config['source'] .':records_fetched',0);
    $most_recent_fetched = variable_get('islandora_feeds:'. $source_config['source'] .':most_recent_fetched',0);
  
    return new FeedsIslandoraZoteroHTTPBatch(
      $source_config['source'], 
      $first_pass,
      $records_fetched,
      $most_recent_fetched,
      $source->feed_nid // unnecessary?
    );
  
  }

}