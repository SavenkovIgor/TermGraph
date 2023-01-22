SELECT uuid,
       name,
       comment
  FROM groups
 WHERE name = :name;
