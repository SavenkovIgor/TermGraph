SELECT COUNT( * ) 
  FROM groups
 WHERE uuid = :uuid
 LIMIT 1;
