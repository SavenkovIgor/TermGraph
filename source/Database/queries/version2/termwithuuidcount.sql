SELECT COUNT( * )
  FROM terms
 WHERE uuid = :uuid
 LIMIT 1;
