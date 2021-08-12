SELECT lastEdit
  FROM terms
 WHERE uuid = :uuid
 LIMIT 1;
