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
 WHERE groupUuid = :groupUuid;
