SELECT uuid,
       name,
       comment
  FROM groups
 WHERE uuid = :uuid;
