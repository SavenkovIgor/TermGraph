SELECT uuid
  FROM terms
 WHERE term = :term AND 
       groupUuid = :groupUuid
 LIMIT 1;
