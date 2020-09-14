UPDATE terms
   SET term = :term,
       definition = :definition,
       description = :description,
       examples = :examples,
       wikiUrl = :wikiUrl,
       wikiImage = :wikiImage,
       groupUuid = :groupUuid,
       lastEdit = :lastEdit
 WHERE uuid = :uuid;
