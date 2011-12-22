<?php

class IslandoraFeedsZoteroFetcher extends FeedsZoteroFetcher {
  
   public function sourceFormValidate(&$values) {
     parent::sourceFormValidate($values);
     $values['source'] .= '&itemType=journalArticle || attachment&order=dateModified&sort=asc';
     // TODO store the most recently fetched offset in a cache table
   }
   
}