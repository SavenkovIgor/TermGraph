SELECT uuid,
       term,
       definition,
       description,
       examples,
       wikiUrl,
       wikiImage,
       groupUuid,
       lastEdit
  FROM terms
 WHERE term = :term AND 
       groupUuid = :groupUuid
 LIMIT 1;
